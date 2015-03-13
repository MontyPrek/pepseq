#include "../Global/psGlobalDefinitions.h"
#include "../Filtering/psFiltering.h"
#include "../Registration/psRegistration.h"
#include "../ImageAnalysis/psImageAnalysis.h"
#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>

int main( int argc, char* argv[] ) {
	std::ofstream outFile;
	char userEntry;
	struct dirent* dirEntry;
	DIR* imgDirPtr;
	std::string currentImgName;
	std::vector<std::vector<type_SpotData> > allSpotData;
	int i, j;

	outFile.open(argv[2], std::ios_base::app);

	if( argc == 1 ) {
		std::cout << "Please specify an image directory. Terminating."
				<< std::endl;
		return 0;
	}

	imgDirPtr = opendir( argv[1] );
	if ( imgDirPtr == NULL ) {
		std::cout << "Invalid directory path. Terminating." 
				<< std::endl;
		return 0;
	}

	//std::cout << "Enter 'c' for spot counting." <<std::endl;
	//std::cin >> userEntry; std::cout << std::endl;

	//switch( userEntry ) {
	    //case 'c': {
		int userThresh;

		type_FilteringUnit filteringUnit;
		//type_ImageAnalysisUnit imageAnalysisUnit;

		//std::cout << "Enter thresholding level: ";
		//std::cin >> userThresh; std::cout << std::endl;
		userThresh = 1;

		dirEntry = readdir( imgDirPtr );
		while( dirEntry != NULL ) {

			while ( dirEntry -> d_name[0] == '.' ) {
				dirEntry = readdir( imgDirPtr );
			}
			
			type_ImageAnalysisUnit imageAnalysisUnit;

			currentImgName = argv[1];
			currentImgName += dirEntry -> d_name;
			std::cout << currentImgName << "   ";


			filteringUnit.initData();
			filteringUnit.setImage( currentImgName.c_str() );

			filteringUnit.roiCrop( 200, 300, 200, 150 );
			//filteringUnit.setupReFilter();

			//filteringUnit.thresholdBelow( userThresh, 0);

			imageAnalysisUnit.initData();
			imageAnalysisUnit.setInputToFiltered (&filteringUnit);
			imageAnalysisUnit.countSpotsGray( 0, 0, 200, 150 );

			std::cout << imageAnalysisUnit.getTotalNumberOfSpots()
				  << std:: endl;

			allSpotData.push_back( imageAnalysisUnit.getSpotData() );

			dirEntry = readdir( imgDirPtr );
		}

		for( i = 0; i < allSpotData.size(); i++ ) {
			for( j = 0; j < allSpotData[i].size(); j++ ) {
				outFile << allSpotData[i][j].spotID << ",";
				outFile << allSpotData[i][j].area << ",";
				outFile << allSpotData[i][j].netBrightness << ",";
				outFile << allSpotData[i][j].maxBrightness << ",";
				outFile << allSpotData[i][j].maxX << ",";
				outFile << allSpotData[i][j].maxY << ",";
			}
			outFile << std::endl;
		}
		/*
		// TODO: Make this not suck when there is time. Way too slow naive approach.
		int maxBright[500];
		int i, j, k, l;  
		std::fill_n(maxBright, 500, 0);
		for( i = 0; i < allSpotData.size(); i++ ) {
		  std::cout << "i " <<  i << std:: endl;
		  for( j = 0; j < allSpotData[i].size(); j++) {
		    int maxOfMaxes = 0;
		    //std::cout << "j " <<  j << std:: endl;
		    for( k = 0; k < allSpotData.size(); k++ ) {
		      //std::cout << "k " <<  k << std:: endl;
		      for( l = 0; l < allSpotData[k].size(); l++ ) {
		        //std::cout << "l " <<  l << std:: endl;
		        if( maxOfMaxes < allSpotData[k][j].netBrightness &&
			    allSpotData[i][j].maxX <= allSpotData[k][l].maxX + 2 &&
			    allSpotData[i][j].maxX >= allSpotData[k][l].maxX - 2 &&
			    allSpotData[i][j].maxY <= allSpotData[k][l].maxY + 2 &&
			    allSpotData[i][j].maxY >= allSpotData[k][l].maxY - 2 ) {
			  maxOfMaxes = allSpotData[k][j].netBrightness;
			  
			}
		      }
		    }	
		    if( maxOfMaxes < 499 ) {
		      maxBright[maxOfMaxes]++;
		    }
		    else {
		      //std::cout << maxOfMaxes << std::endl;
		      maxBright[499]++;
		    }			
		  }
		}*/
		
	    	/*
		int maxBright[100];
		int i, j;
		std::fill_n(maxBright, 100, 0);
		for( i = 0; i < allSpotData.size(); i++ ) {
			for( j=0; j < allSpotData[i].size(); j++) {
				if (allSpotData[i][j].maxBrightness > 99) {
					maxBright[99]++;
				}
				else {
					maxBright[allSpotData[i][j].maxBrightness]++;
				}
			}
		}*/

		/*
		for( i = 0; i < 499; i++ ) {
			for( j = 0; j < (maxBright[i]); j++ ) {
				std::cout << "=";
			}
			std::cout << std::endl;
		}
		for( i = 0; i < 499; i++ ) {
		 	std::cout << maxBright[i] << " ";
			outFile << maxBright[i] << ",";			
		}
		outFile << std::endl;
		*/
	   // }
	//}

}
