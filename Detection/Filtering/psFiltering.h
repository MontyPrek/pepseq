#ifndef H_FILTERING
#define H_FILTERING

#include "../Global/psGlobalDefinitions.h"
#include "itkThresholdImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

typedef itk::ThresholdImageFilter< type_InputFloatImage >	type_ThresholdFilter;
typedef itk::RegionOfInterestImageFilter< type_InputFloatImage, type_OutputFloatImage >	type_ROIFilter;

class type_FilteringUnit {
	type_InputFloatImage::Pointer			inputImage;
	type_ThresholdFilter::Pointer			thresholdFilter;
	type_ROIFilter::Pointer				roiFilter;
	type_OutputFloatImage::Pointer			filteredImage;
	type_InputFloatImageReader::Pointer		inputImageReader;
	type_InputFloatImage::RegionType::IndexType	roiStart;
	type_InputFloatImage::RegionType::SizeType	roiSize;
	type_InputFloatImage::RegionType		roiRegion;

    public:
	void initData
		();
	void thresholdBelow
		( int cutoff, int value );
	type_OutputFloatImage::Pointer getOutput
		();
	void setImage
		( const char* file );
	void roiCrop
		( int xStartCoord, int yStartCoord, int xDimension, int yDimension );
	void setupReFilter
		();
};

#endif
