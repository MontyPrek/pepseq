#include "../Global/psGlobalDefinitions.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "../Filtering/psFiltering.h"
#include "itkObject.h"
#include "itkImage.h"
#include <fstream>
#include <vector>

typedef itk::ImageRegionConstIteratorWithIndex< 
		type_InputFloatImage >			type_ConstIndexedIterator;

struct type_PixelData {
	int intensity;
	int spotParent;
	type_InputFloatImage::IndexType	index;

	type_PixelData
		( int givenIntensity, int givenX, int givenY, int givenSpotParent);
};

struct type_SpotData {
	int spotID;
	int area;
	int netBrightness;
	int maxBrightness;
	int maxX;
	int maxY;

	std::vector<type_InputFloatImage::IndexType> pixels;

	type_SpotData
		( int givenSpotID, int givenArea, int givenNetBrightness, 
		  int givenMaxBrightness, int givenMaxX, int givenMaxY );
};

struct type_PrevRowTransition {
	int transXCoord;
	int spotIndex;

	type_PrevRowTransition
		( int givenTransXCoord, int givenSpotIndex );
};

class type_ImageAnalysisUnit {
	type_InputFloatImageReader::Pointer		inputImageReader;
	type_InputFloatImage::Pointer			inputImage;
	type_InputFloatImage::IndexType			pixelQueryIndex;
	type_InputFloatImage::IndexType			aboveQueryIndex;
	type_InputFloatImage::IndexType			belowQueryIndex;
	type_InputFloatImage::IndexType			rightQueryIndex;
	type_InputFloatImage::IndexType			leftQueryIndex;
	type_InputFloatImage::IndexType			diagARQueryIndex;
	type_InputFloatImage::IndexType			diagBRQueryIndex;
	type_InputFloatImage::IndexType			diagBLQueryIndex;
	type_InputFloatImage::IndexType			diagALQueryIndex;
	type_InputFloatImage::RegionType		iterRegion;
	type_InputFloatImage::RegionType::IndexType	iterStart;
	type_InputFloatImage::RegionType::SizeType	iterSize;
	std::vector<type_PixelData>			pixelVector;
	std::vector<type_PixelData>			pixelVectorLin;
	std::vector<type_SpotData>			spotVector;
	std::vector<type_PrevRowTransition>		prevRowVector;
	std::vector<type_PrevRowTransition>		prevRowTempVector;
	int 						averageBackground;

    public:
	//type_ImageAnalysisUnit
	//	();
	//~type_ImageAnalysisUnit
	//	();
	//
	//
	//
	void initData
		();
	void setImage
		( char* file );
	void countSpotsGray
		( int xStartCoord, int yStartCoord, int xDimension, int yDimension );
	void countSpots
		( int xStartCoord, int yStartCoord, int xDimension, int yDimension );
	void setInputToFiltered
		( type_FilteringUnit* filteringUnit);
	void setInput
		();
	int getTotalNumberOfSpots
		();
	int getSpotNetBrightness
		( int index );
	int getSpotArea
		( int index );
	std::vector<type_SpotData> getSpotData
		();
	std::vector<type_InputFloatImage::IndexType> getHigherNeighbors
		( int centerIntensity, int xStartCoord, int yStartCoord, int yDimension );
	int getAverageBackground
		();
	void outputToCSV
		();
};

