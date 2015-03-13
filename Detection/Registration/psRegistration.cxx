#include "psRegistration.h"

void type_RegistrationUnit::initData() {
	this->transform = type_TransTransform::New();
	this->optimizer = type_RSGDOptimizer::New();
	this->metric = type_MSITIMetric::New();
	this->interpolator = type_LinearInterpolator::New();
	this->registration = type_Registration::New();
	this->inputImageReader = type_InputFloatImageReader::New();
	
	this->registration->SetTransform( this->transform );
	this->registration->SetOptimizer( this->optimizer );
	this->registration->SetMetric( this->metric );
	this->registration->SetInterpolator( this->interpolator );
}

void type_RegistrationUnit::setFixedImage( char* file ) {
	this->inputImageReader->SetFileName( file );
	this->inputImageReader->Update();
	this->inputFixedImage = this->inputImageReader->GetOutput();
}

void type_RegistrationUnit::setMovingImage( char* file ) {
	this->inputImageReader->SetFileName( file );
	this->inputImageReader->Update();
	this->inputMovingImage = this->inputImageReader->GetOutput();
}

void type_RegistrationUnit::setInitialParams( int xOffset, int yOffset ) {
	type_Registration::ParametersType initialParams( TWO_DIMENSIONAL );
	initialParams[0] = xOffset;
	initialParams[1] = yOffset;
	this->registration->SetInitialTransformParameters( initialParams );
}

void type_RegistrationUnit::setOptimizerParams( float optMaxStep, float optMinStep, int optIter ) {
	this->optimizer->SetMaximumStepLength( optMaxStep );
	this->optimizer->SetMinimumStepLength( optMinStep );
	this->optimizer->SetNumberOfIterations( optIter );
}

void type_RegistrationUnit::adjustImage() {
	this->registration->SetFixedImage( this->inputFixedImage );
	this->registration->SetMovingImage( this->inputMovingImage );

	this->registration->SetFixedImageRegion( inputFixedImage->GetBufferedRegion() );

	this->registration->Update();
	
	outputImage = type_OutputFloatImage::Pointer( inputMovingImage );
	//type_OutputFloatImage( inputMovingImage ) = outputImage;
	inputMovingImage = NULL;
}
