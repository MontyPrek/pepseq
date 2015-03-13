#include "psImageAnalysis.h"
#include "../Filtering/psFiltering.h"

type_SpotData::type_SpotData( int givenSpotID, int givenArea, int givenNetBrightness, 
			      int givenMaxBrightness, int givenMaxX, int givenMaxY ) {
	this->spotID = givenSpotID;
	this->area = givenArea;
	this->netBrightness = givenNetBrightness;	
	this->maxBrightness = givenMaxBrightness;
	this->maxX = givenMaxX;
	this->maxY = givenMaxY;
}

type_PixelData::type_PixelData( int givenIntensity, int givenX, int givenY, int givenSpotParent ) {
	this->intensity = givenIntensity;
	this->index[0] = givenX;
	this->index[1] = givenY;
	this->spotParent = givenSpotParent;
}

type_PrevRowTransition::type_PrevRowTransition( int givenTransXCoord, int givenSpotIndex ) {
	this->transXCoord = givenTransXCoord;
	this->spotIndex = givenSpotIndex;
}

//type_ImageAnalysisUnit::type_ImageAnalysisUnit() {
//	this->inputImageReader = type_InputFloatImageReader::New();
//}

//type_ImageAnalysisUnit::~type_ImageAnalysisUnit() {
//	delete this->inputImageReader;
	
	// This shouldn't be necessary since this pointer should always end up looking at the same image
	//  as the reader. Keeping as safeguard.
//	delete this->inputImageReader;
//}

std::vector<type_SpotData> type_ImageAnalysisUnit::getSpotData() {
	return this->spotVector;
}

void type_ImageAnalysisUnit::initData() {
	// TODO: *Feature*
	// 	 -Set other stuff to a null type
	// 	 -If this is not the first invocation of this function on object, destroy previous sub-objects
	
	this->inputImageReader = type_InputFloatImageReader::New();
	// Pretty sure this will cause memory leak, keeping for now in case I'm wrong
	//this->inputImage = type_InputFloatImage::New();	
}

void type_ImageAnalysisUnit::setImage( char* file ) {
	this->inputImageReader->SetFileName( file );
	this->inputImageReader->Update();
	this->inputImage = this->inputImageReader->GetOutput();
}

void type_ImageAnalysisUnit::setInputToFiltered( type_FilteringUnit* filteringUnit ) {
	this->inputImage = filteringUnit->getOutput();
}

int type_ImageAnalysisUnit::getTotalNumberOfSpots() {
	return this->spotVector.size();
}

int type_ImageAnalysisUnit::getSpotNetBrightness( int index ) {
	return this->spotVector[index].netBrightness;
}

int type_ImageAnalysisUnit::getSpotArea( int index ) {
	return this->spotVector[index].area;
}

int type_ImageAnalysisUnit::getAverageBackground() {
	return this->averageBackground;
}

void type_ImageAnalysisUnit::countSpotsGray( int xStartCoord, int yStartCoord, int xDimension, int yDimension ) {
	// Much slower implementation of the below counting function as a test of gray flooding spot detection.
	// If effective, can be reimplemented with iterators (more difficult)
	//
	// Sorting pixels into array is also down more slowly than it could be. Should be done with iterative divisions of array (later).
	//
	// Note that this is done with standard c x/y axes (y=0 is at the top of the image, x=0 is the right)
	std::vector<type_PixelData>::iterator it;
	int i; //This really should be an iterator, adjust when refactor
	int spotCount = 0;

	std::cout << "Org pixels" << std::endl;

	// Cluster compiler doesn't support extended initializer lists
	this->pixelQueryIndex[0] = xStartCoord; this->pixelQueryIndex[1] = yStartCoord;
	int current = NULL_INT;
	this->pixelVector.push_back( type_PixelData( this->inputImage->GetPixel( this->pixelQueryIndex ),
		xStartCoord, yStartCoord, NULL_INT ) );
	this->pixelVectorLin.push_back( type_PixelData( this->inputImage->GetPixel( this->pixelQueryIndex ),
		xStartCoord, yStartCoord, NULL_INT ) );

	/*while( this->pixelQueryIndex[0] != xStartCoord + xDimension && 
	       this->pixelQueryIndex[1] != yStartCoord + yDimension ) {
		this->pixelQueryIndex[0]++; this->pixelQueryIndex[1]++;
		current = this->inputImage->GetPixel( this->pixelQueryIndex );

		this->pixelVectorLin.push_back( type_PixelData ( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL ) );

	// Need iterator to do insert. For now, I'd say do naiive approach linearly and do divide later (may actually be slower due to iterator design)
		
		int previousSize = pixelVector.size();
		for( it = pixelVector.begin(); it < pixelVector.end(); it++ ) {
			if( current > it->intensity ) {
				pixelVector.insert( it, type_PixelData( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL ) );
				break;
			}
		}
		// Since there is no if-break in C++
		if( pixelVector.size() == previousSize ) {
			pixelVector.push_back( type_PixelData( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL ) );
		}*/

	for( this->pixelQueryIndex[0] = xStartCoord; this->pixelQueryIndex[0] < xStartCoord + xDimension; this->pixelQueryIndex[0]++ ) {
		for( this->pixelQueryIndex[1] = yStartCoord; this->pixelQueryIndex[1] < yStartCoord +yDimension; this->pixelQueryIndex[1]++ ) {
			//std::cout << "Organizing pixel: " << pixelQueryIndex[0] << "  " << pixelQueryIndex[1] << std::endl;
			if( this->pixelQueryIndex[0] == xStartCoord && this->pixelQueryIndex[1] == yStartCoord ) {
				this->pixelQueryIndex[1]++;
			}
			
			current = this->inputImage->GetPixel( this->pixelQueryIndex );

			this->pixelVectorLin.push_back( type_PixelData ( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL_INT ) );

			// Need iterator to do insert. For now, I'd say do naiive approach linearly and do divide later (may actually be slower due to iterator design)
		
			int previousSize = pixelVector.size();
			for( it = pixelVector.begin(); it != pixelVector.end(); ++it ) {
				if( current > it->intensity ) {
					pixelVector.insert( it, type_PixelData( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL_INT ) );
					break;
				}
			}
			// Since there is no if-break in C++
			if( pixelVector.size() == previousSize ) {
				pixelVector.push_back( type_PixelData( current, this->pixelQueryIndex[0], this->pixelQueryIndex[1], NULL_INT ) );
			
			}
		}
		
	}

	//Background average vars
	int total = 0;
	int bgPixels = 0;

	// Now that the array is sorted, proceed to gray flood starting with highest intensity pixels.
	// For now, gray flooding is with adjacent neighbors only, not diagonals. Test diagonals for better result?
	//  Diagonals should actually improve protectiopn from erroneous blobs since it is less likely for a 
	//  random spot to be higher than 8 neighbors vs 4. However net intensity will be less precise. Could do 2 passes!
	std::cout << "Gray flood step" << std::endl;
	for( it = pixelVector.begin(); it != pixelVector.end(); ++it ) {
		//This whole system is not ideal. Refactor.
		// Getting proper value to check from pixelVectorLin which will be relevant to both cases:
		//OLD 
		//int linElement = ((pixelQueryIndex[1] - yStartCoord) * xDimension) + (pixelQueryIndex[0] - xStartCoord);
		pixelQueryIndex = it->index;
		int linElement = ((pixelQueryIndex[0] - xStartCoord) * yDimension) + (pixelQueryIndex[1] - yStartCoord);
		
		//std::cout << "Determing spot for pixel: " << pixelQueryIndex[0] << " " << pixelQueryIndex[1] << std::endl;
		//Protection against blobs that go over edge. May need to increase to 2 pixels or more.
		// Creat functionality to do so easily if all goes well.  You're a retard Monty.
		// This 'if' excludes checking all edge pixels since they have no neighbor in at least one direction
		if( it->index[0] != xStartCoord && it->index[1] != yStartCoord && 
		    it->index[0] != (xStartCoord+xDimension-1) && it->index[1] != (yStartCoord+yDimension-1)) {
			//pixelQueryIndex = it->index;

			this->aboveQueryIndex = this->pixelQueryIndex;
			this->aboveQueryIndex[1]--;
			this->belowQueryIndex = this->pixelQueryIndex;
			this->belowQueryIndex[1]++;
			this->rightQueryIndex = this->pixelQueryIndex;
			this->rightQueryIndex[0]++;
			this->leftQueryIndex = this->pixelQueryIndex;
			this->leftQueryIndex[0]--;
			
			this->diagARQueryIndex = this->pixelQueryIndex;
			this->diagARQueryIndex[1]--;
			this->diagARQueryIndex[0]++;
			this->diagBRQueryIndex = this->pixelQueryIndex;
			this->diagBRQueryIndex[1]++;
			this->diagBRQueryIndex[0]++;
			this->diagBLQueryIndex = this->pixelQueryIndex;
			this->diagBLQueryIndex[1]++;
			this->diagBLQueryIndex[0]--;
			this->diagALQueryIndex = this->pixelQueryIndex;
			this->diagALQueryIndex[1]--;
			this->diagALQueryIndex[0]--;

			// OLD used to be xDimension
			std::vector<type_InputFloatImage::IndexType> neighbors = getHigherNeighbors( it->intensity, xStartCoord, yStartCoord, yDimension );	

			/*for( i = 0; i < neighbors.size(); i++ ) {
				std::cout << neighbors[i][0] << " " << neighbors[i][1] << std::endl;
			}*/

			//This whole system is not ideal. Refactor.
			//Getting proper value to check from pixelVectorLin which will be relevant to both cases:
			// OLD 
			//int linElement = ((pixelQueryIndex[1] - yStartCoord) * xDimension) + (pixelQueryIndex[0] - xStartCoord);
			//int linElement = ((pixelQueryIndex[0] - xStartCoord) * yDimension) + (pixelQueryIndex[1] - yStartCoord);

			//Note that all neighbors SHOULD have a spotParent of either a value or int_max - 1 indicating it is b/g
			if( neighbors.size() ) {
				// OLD
				//int neiLinElement = ((neighbors[0][1] - yStartCoord) * xDimension) + (neighbors[0][0] - xStartCoord);
				int neiLinElement = ((neighbors[0][0] - xStartCoord) * yDimension) + (neighbors[0][1] - yStartCoord);				

				if( pixelVectorLin[neiLinElement].spotParent == BACKGROUND ) {
					// OLD you're stupid
					//pixelVectorLin[neiLinElement].spotParent == BACKGROUND;
					pixelVectorLin[linElement].spotParent = BACKGROUND;
					it->spotParent = BACKGROUND;

					bgPixels++;
					total += it->intensity;
				}

				//Logic tree can obviously be reduced here
				else if( neighbors.size() == 1 ) {
					pixelVectorLin[linElement].spotParent = pixelVectorLin[neiLinElement].spotParent;
					it->spotParent = pixelVectorLin[neiLinElement].spotParent;
				
					spotVector[pixelVectorLin[neiLinElement].spotParent].area++;
					spotVector[pixelVectorLin[neiLinElement].spotParent].netBrightness += it->intensity;
					spotVector[pixelVectorLin[neiLinElement].spotParent].pixels.push_back( it->index );
				}
				else {
					// Finish implementing pixel splitting here when you get a chance and is safe to do so.
					//  Perhaps put it under a switch (compile or user, preferably user I think)
					//std::vector<int> parents;
					//parents.push_back( neiLinElement );
					for( i = 1; i < neighbors.size(); i++ ) {
						// OLD
						//int neiLinElementComp = ((neighbors[i][1] - yStartCoord) * xDimension) + (neighbors[i][0] - xStartCoord);
						int neiLinElementComp = ((neighbors[i][0] - xStartCoord) * yDimension) + (neighbors[i][1] - yStartCoord);
						if( pixelVectorLin[neiLinElement].spotParent != pixelVectorLin[neiLinElementComp].spotParent ) {
							//if( pixelVectorLin[neiLinElementComp].spotParent == BACKGROUND ) {
								pixelVectorLin[linElement].spotParent = BACKGROUND;
								it->spotParent = BACKGROUND;

								bgPixels++;
								total += it->intensity;
								break;
							//}
							//else {
							//	parents.push_back( neiLinElementComp );
							//}
						}
					}
					if( pixelVectorLin[linElement].spotParent == NULL_INT ) { //Add to parent spot

						pixelVectorLin[linElement].spotParent = pixelVectorLin[neiLinElement].spotParent;
						it->spotParent = pixelVectorLin[neiLinElement].spotParent;

						spotVector[pixelVectorLin[neiLinElement].spotParent].area++;
						spotVector[pixelVectorLin[neiLinElement].spotParent].netBrightness += it->intensity;
						spotVector[pixelVectorLin[neiLinElement].spotParent].pixels.push_back( it->index );
					}
				}
				if( pixelVectorLin[linElement].spotParent == NULL_INT ) {
					std::cout << "uhoh" << std::endl;
				}
			}
			else {  //It is the highest of its neighbors.
				spotVector.push_back( type_SpotData( spotCount, 1, it->intensity, it->intensity, pixelQueryIndex[0], pixelQueryIndex[1] ) );
				spotVector.back().pixels.push_back( it->index );
				it->spotParent = spotCount; //I think this is unnecessary since only the linear vector is looked at for parent
				pixelVectorLin[linElement].spotParent = spotCount;
				spotCount++;
			}
		}
		//If they are an edge, parent spot is automatically background
		else {
			pixelVectorLin[linElement].spotParent = BACKGROUND;
			it->spotParent = BACKGROUND;
			// Will not add these to BG calculation since they are BG regardless of their value
		}
	}
	// This should work now
	this->averageBackground = total / bgPixels;
	//this->averageBackground = 30;
}

std::vector<type_InputFloatImage::IndexType> type_ImageAnalysisUnit::getHigherNeighbors( int centerIntensity, int xStartCoord, int yStartCoord, int yDimension ) {
	std::vector<type_InputFloatImage::IndexType> neighbors;

	//If there are two or more same intensity spots adjacent to each other, they will be counted as seperate entities!
	//Using greater or equal will result in no spots and accessing NULL parentSpot values.
	//
	//This should be easy to fix. In this function, simply check for equality. If equal, see if other pixel already has a parent spot.
	// If it does, treat it as a higher neighbor. If it doesn't, treat it as a lower neighbori.
	// FIXED 09/30/14, remove note when functional for sure

	if( inputImage->GetPixel( aboveQueryIndex ) > centerIntensity ) { neighbors.push_back( aboveQueryIndex ); }
	else if( inputImage->GetPixel( aboveQueryIndex ) == centerIntensity ) {
		// OLD
		//int neiLinElement = ((aboveQueryIndex[1] - yStartCoord) * xDimension) + (aboveQueryIndex[0] - xStartCoord);
		int neiLinElement = ((aboveQueryIndex[0] - xStartCoord) * yDimension) + (aboveQueryIndex[1] - yStartCoord);
		if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
			neighbors.push_back( aboveQueryIndex );
		}
	} 
	if( inputImage->GetPixel( belowQueryIndex ) > centerIntensity ) { neighbors.push_back( belowQueryIndex ); }
        else if( inputImage->GetPixel( belowQueryIndex ) == centerIntensity ) {
                // OLD
		//int neiLinElement = ((belowQueryIndex[1] - yStartCoord) * xDimension) + (belowQueryIndex[0] - xStartCoord);
		int neiLinElement = ((belowQueryIndex[0] - xStartCoord) * yDimension) + (belowQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( belowQueryIndex );
                }
	}
	if( inputImage->GetPixel( rightQueryIndex ) > centerIntensity ) { neighbors.push_back( rightQueryIndex ); }
        else if( inputImage->GetPixel( rightQueryIndex ) == centerIntensity ) {
                // OLD
		//int neiLinElement = ((rightQueryIndex[1] - yStartCoord) * xDimension) + (rightQueryIndex[0] - xStartCoord);
		int neiLinElement = ((rightQueryIndex[0] - xStartCoord) * yDimension) + (rightQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( rightQueryIndex );
                }
	}
	if( inputImage->GetPixel( leftQueryIndex ) > centerIntensity ) { neighbors.push_back( leftQueryIndex ); }
        else if( inputImage->GetPixel( leftQueryIndex ) == centerIntensity ) {
		int neiLinElement = ((leftQueryIndex[0] - xStartCoord) * yDimension) + (leftQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( leftQueryIndex );
                }
	}

        if( inputImage->GetPixel( diagARQueryIndex ) > centerIntensity ) { neighbors.push_back( diagARQueryIndex ); }
        else if( inputImage->GetPixel( diagARQueryIndex ) == centerIntensity ) {
                int neiLinElement = ((diagARQueryIndex[0] - xStartCoord) * yDimension) + (diagARQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( diagARQueryIndex );
                }
        }
        if( inputImage->GetPixel( diagBRQueryIndex ) > centerIntensity ) { neighbors.push_back( diagBRQueryIndex ); }
        else if( inputImage->GetPixel( diagBRQueryIndex ) == centerIntensity ) {
                int neiLinElement = ((diagBRQueryIndex[0] - xStartCoord) * yDimension) + (diagBRQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( diagBRQueryIndex );
                }
        }
        if( inputImage->GetPixel( diagBLQueryIndex ) > centerIntensity ) { neighbors.push_back( diagBLQueryIndex ); }
        else if( inputImage->GetPixel( diagBLQueryIndex ) == centerIntensity ) {
                int neiLinElement = ((diagBLQueryIndex[0] - xStartCoord) * yDimension) + (diagBLQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( diagBLQueryIndex );
                }
        }
        if( inputImage->GetPixel( diagALQueryIndex ) > centerIntensity ) { neighbors.push_back( diagALQueryIndex ); }
        else if( inputImage->GetPixel( diagALQueryIndex ) == centerIntensity ) {
                int neiLinElement = ((diagALQueryIndex[0] - xStartCoord) * yDimension) + (diagALQueryIndex[1] - yStartCoord);
                if( pixelVectorLin[neiLinElement].spotParent != NULL_INT ) {
                        neighbors.push_back( diagALQueryIndex );
                }
        }



	return neighbors;
}

void type_ImageAnalysisUnit::outputToCSV() {
	std::ofstream outFile;
	int i;	

	outFile.open("./outdata.txt", std::ios_base::app);

	for( i = 0; i < this->spotVector.size(); i++ ) {
		outFile << this->spotVector[i].spotID << ",";
		outFile << this->spotVector[i].area << ",";
		outFile << this->spotVector[i].netBrightness << ",";
		outFile << this->spotVector[i].maxBrightness << ",";
		outFile << this->spotVector[i].maxX << ",";
		outFile << this->spotVector[i].maxY << ",";
		outFile << std::endl;
	}
}

/*
void type_ImageAnalysisUnit::countSpots( int xStartCoord, int yStartCoord, int xDimension, int yDimension ) {
	//TODO: *Efficiency*
	//	-Implement timing framework to test all changes (FIRST PRIORITY)
	//	-Indexed vs. non-indexed iterators
	//	-Play with row and col iterators
	//	-Reduce all mem types to minimums, ints are way excessive
	//	-The -1 when initing a transition struct likely can be replaced with a  pre decrement
	//	-Mask xIndex so that compiler can do a 0/1 instead of an isequal
	//	-Look into ITK VNL offerings
	//	-Corner case step back solution seems bulky
	//	-Make sure no time critical variables/iterators/etc. are stored on the heap with the main
	//	 object.  Currently, the object is constantly accessed to update three different vectors
	//	 that are time critical. Instantiate temporary ones that will be on the stack and then
	//	 copy to the object (heap) once analysis is completed!
	//
	//TODO: *Critical Bug* [Likely fixed, run over corner cases carefully just in case]
	//	-Make sure you don't dereference null if the prev row had no spots
	//
	//TODO: *Bug*
	//	-"Requested region not buffered" error for some regions smaller than the entire image

	bool adjNewFlag = false;
	bool adjFlag = false;

	// Pixel iterators require a RegionType object which requires an object of size DIMENSION for
	//  both starting location and travel distance, so load in the requested values.
	this->iterStart[0] = xStartCoord;
	this->iterStart[1] = yStartCoord;
	this->iterSize[0] = xDimension;
	this->iterSize[1] = yDimension;

	// Now load index objects into region object to be used to initialize iterators.
	this->iterRegion.SetSize( this->iterSize );
	this->iterRegion.SetIndex( this->iterStart );

	// Initialize iterators!
        type_ConstIndexedIterator leadReadIter( this->inputImage, this->iterRegion );
        type_ConstIndexedIterator lagReadIter( this->inputImage, this->iterRegion );

	// The first row needs special treatment since there is no previous row to look at.
	for( leadReadIter.GoToBegin(); !leadReadIter.GetIndex()[1]; ++leadReadIter) {
//051314		
		int leadVal = leadReadIter.Get();
		int xIndex = leadReadIter.GetIndex()[0];
		int yIndex = leadReadIter.GetIndex()[1];
		if( leadVal && !adjFlag) {
			this->spotVector.push_back( type_SpotData( 1, leadVal, leadVal, xIndex, yIndex) );
			this->prevRowTempVector.push_back( type_PrevRowTransition( 
				xIndex, this->spotVector.size() - 1 ) );
			adjFlag = true;
		}
		else if( leadVal && adjFlag ) { 
			this->spotVector.back().area++; 
			this->spotVector.back().netBrightness += leadVal;
			if( leadVal > this->spotVector.back().maxBrightness ) {
				this->spotVector.back().maxBrightness = leadVal;
				this->spotVector.back().maxX = xIndex;
				this->spotVector.back().maxY = yIndex;
			}
		}
//\051314
		else { adjFlag = false; }
	}

	// This iterator NEEDS to be instantiated since compiler has an aneurism and segfaults if
	// 	instantiated in the if statement below even though it will always hit the first time.
	std::vector<type_PrevRowTransition>::iterator prevRowVectorIter = this->prevRowVector.begin();
	for( lagReadIter.GoToBegin(); !leadReadIter.IsAtEnd(); ++leadReadIter, ++lagReadIter ) {
		int leadVal = leadReadIter.Get();
		int lagVal = lagReadIter.Get();
		int xIndex = leadReadIter.GetIndex()[0];
		//std::cout << leadReadIter.Get() << std::endl;
//051314
		int yIndex = leadReadIter.GetIndex()[1];
//051314
		if( xIndex == xStartCoord ) {
			adjNewFlag = false;
			adjFlag = false;
			this->prevRowVector.clear();
			this->prevRowVector = this->prevRowTempVector;
			this->prevRowTempVector.clear();
			prevRowVectorIter = this->prevRowVector.begin();
		}
		//This if statement MUST be done in this order so that it does not seg fault
		if ( this->prevRowVector.size() && prevRowVectorIter < (this->prevRowVector.end() - 1) && 
		     xIndex == (*(prevRowVectorIter + 1)).transXCoord ) {
                        prevRowVectorIter++;
                }
		if( leadVal && adjNewFlag ) { 
			this->spotVector.back().area++;
			this->spotVector.back().netBrightness += leadVal;
//051314		
			if( leadVal > this->spotVector.back().maxBrightness ) {
                                this->spotVector.back().maxBrightness = leadVal;
                                this->spotVector.back().maxX = xIndex;
                                this->spotVector.back().maxY = yIndex;
			}
//\051314	
			// Corner case correction. Need to step back and add counts to existing spot and remove
			//  false spot. Does not seem like the optimal solution to the problem.
			if( lagVal ) {
				int numStepsBack;
				for(  numStepsBack = 0; leadReadIter.Get(); 
						numStepsBack++, --leadReadIter ) {
					this->spotVector[(*prevRowVectorIter).spotIndex].area++;
					this->spotVector[(*prevRowVectorIter).spotIndex].netBrightness 
						+= leadReadIter.Get();
//051314
					if( this->spotVector[(*prevRowVectorIter).spotIndex].maxBrightness <
					    leadReadIter.Get() ) {
							this->spotVector[(*prevRowVectorIter).spotIndex].maxBrightness = 
								leadReadIter.Get();
							this->spotVector[(*prevRowVectorIter).spotIndex].maxX = 
								leadReadIter.GetIndex()[0];
							this->spotVector[(*prevRowVectorIter).spotIndex].maxY = 
								leadReadIter.GetIndex()[1];
						}
//\051314
				}
				for( ; numStepsBack ; numStepsBack--, ++leadReadIter ) {}
				this->spotVector.pop_back();
				this->prevRowTempVector.back().spotIndex = (*prevRowVectorIter).spotIndex;
				adjNewFlag = false;
			}
		}
		else if( leadVal && !adjNewFlag ) {			
			if( lagVal && !adjFlag ) {
				this->spotVector[(*prevRowVectorIter).spotIndex].area++;
				//std::cout << this->spotVector[(*prevRowVectorIter).spotIndex].area << std::endl;
				this->spotVector[(*prevRowVectorIter).spotIndex].netBrightness += leadVal;
				//std::cout << this->spotVector[(*prevRowVectorIter).spotIndex].netBrightness << std::endl;
//051314
				if( leadVal > this->spotVector.back().maxBrightness ) {
                                	this->spotVector.back().maxBrightness = leadVal;
                                	this->spotVector.back().maxX = xIndex;
                                	this->spotVector.back().maxY = yIndex;
				}
//\051314
				this->prevRowTempVector.push_back( type_PrevRowTransition(
                                	xIndex, (*prevRowVectorIter).spotIndex ) );
			}
			else if( adjFlag ) {
				this->spotVector[(*prevRowVectorIter).spotIndex].area++;
				this->spotVector[(*prevRowVectorIter).spotIndex].netBrightness += leadVal;
//051314
				if( leadVal > this->spotVector.back().maxBrightness ) {
                                	this->spotVector.back().maxBrightness = leadVal;
                                	this->spotVector.back().maxX = xIndex;
                                	this->spotVector.back().maxY = yIndex;
				}
//\051314
			}
			else {
//051314
				this->spotVector.push_back( type_SpotData( 1, leadVal, leadVal, xIndex, yIndex ) );
//\051314
				this->prevRowTempVector.push_back( type_PrevRowTransition(
                                	xIndex, this->spotVector.size() - 1 ) );
				adjNewFlag = true;
			}
			adjFlag = true;
		}
		else {  adjNewFlag = false; adjFlag = false; }
	}

}
*/
