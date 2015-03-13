#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "itkObject.h"
#include "itkImage.h"
#include "itkImageFileReader.h"

#include "itkTIFFImageIO.h"

#define TWO_DIMENSIONAL 2
#define BACKGROUND 2147483646 // This is max_int - 1
#define NULL_INT 2147483647

typedef unsigned short					type_FloatPixel;
typedef itk::Image< type_FloatPixel, TWO_DIMENSIONAL >	type_InputFloatImage;
typedef itk::Image< type_FloatPixel, TWO_DIMENSIONAL >	type_OutputFloatImage;
typedef itk::ImageFileReader< type_InputFloatImage >	type_InputFloatImageReader;

#endif
