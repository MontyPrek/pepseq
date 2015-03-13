#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "../Global/psGlobalDefinitions.h"

//typedef float						type_FloatPixel;
typedef itk::TranslationTransform< 
	double, 
	TWO_DIMENSIONAL >				type_TransTransform;
typedef itk::RegularStepGradientDescentOptimizer	type_RSGDOptimizer;
typedef itk::MeanSquaresImageToImageMetric<
	type_InputFloatImage, 
	type_InputFloatImage >				type_MSITIMetric;
typedef itk::LinearInterpolateImageFunction<
	type_InputFloatImage, 
	double >					type_LinearInterpolator;
typedef itk::ImageRegistrationMethod<
	type_InputFloatImage,
	type_InputFloatImage >				type_Registration;

class type_RegistrationUnit {
	type_TransTransform::Pointer transform;
	type_RSGDOptimizer::Pointer optimizer;
	type_MSITIMetric::Pointer metric;
	type_LinearInterpolator::Pointer interpolator;
	type_Registration::Pointer registration;
	type_InputFloatImageReader::Pointer inputImageReader;
	type_InputFloatImage::Pointer inputFixedImage;
	type_InputFloatImage::Pointer inputMovingImage;
	type_OutputFloatImage::Pointer outputImage;

	float currentOptimizerMaxStep;
	float currentOptimizerMinStep;
	int currentOptimizerMaxIterations;

    public:
	void initData
		();
	void adjustImage
		();
	void setFixedImage
		( char* file );
	void setMovingImage
		( char* file );
	void setInitialParams
		( int xOffset, int yOffset );
	void setOptimizerParams
		( float optMaxStep, float optMinStep, int optIter);
};
	
