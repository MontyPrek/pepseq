#include "psFiltering.h"

void type_FilteringUnit::initData() {
	this->thresholdFilter = type_ThresholdFilter::New();
	this->roiFilter = type_ROIFilter::New();
	this->inputImageReader = type_InputFloatImageReader::New();
	//this->filteredImage = type_OutputFloatImage::New();
}

void type_FilteringUnit::thresholdBelow( int cutoff, int value ) {
	this->thresholdFilter->SetInput( this->inputImage );
	this->thresholdFilter->SetOutsideValue( value );
	this->thresholdFilter->ThresholdBelow( cutoff );
	this->thresholdFilter->Update();
	this->filteredImage = this->thresholdFilter->GetOutput();
}

type_OutputFloatImage::Pointer type_FilteringUnit::getOutput() {
	return this->filteredImage;
}

void type_FilteringUnit::setImage( const char* file ) {
	this->inputImageReader->SetFileName( file );
	this->inputImageReader->Update();
	this->inputImage = this->inputImageReader->GetOutput();
}

void type_FilteringUnit::roiCrop( int xStartCoord, int yStartCoord,
				  int xDimension, int yDimension ) {
	this->roiStart[0] = xStartCoord;
	this->roiStart[1] = yStartCoord;
	this->roiSize[0] = xDimension;
	this->roiSize[1] = yDimension;

	this->roiRegion.SetSize( roiSize );
	this->roiRegion.SetIndex( roiStart );

	this->roiFilter->SetInput( this->inputImage );
	this->roiFilter->SetRegionOfInterest( roiRegion );
	this->roiFilter->Update();
	this->filteredImage = this->roiFilter->GetOutput();
}

void type_FilteringUnit::setupReFilter() {
	this->inputImage = this->filteredImage;
	std::cout << this->inputImage->GetRequestedRegion() << std::endl;
	std::cout << this->filteredImage->GetRequestedRegion() << std::endl;
}
