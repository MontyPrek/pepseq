# Make sure CMake is up to date
cmake_minimum_required(VERSION 2.8)

# Establish project name
project(PeptideSeq)

# Set ITK_DIR, the variable that stores the location of the ITK binaries
#  Only use this if you did not "make install" or if installation location
#  cannot be in your path.
#set(ITK_DIR ${PROJECT_SOURCE_DIR}/../InsightToolkit-4.7.1/ITK_bin)

# Print out where CMake expects source and binary root directories to be
message("Project source directory: " ${PROJECT_SOURCE_DIR})
message("Project binary directory: " ${PROJECT_BINARY_DIR})
message("ITK binary directory: " ${ITK_DIR})

# Import all the ITK stuff
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Declare all additional libraries and their sources
add_library(Registration ${PROJECT_SOURCE_DIR}/Registration/psRegistration.cxx)
add_library(ImageAnalysis ${PROJECT_SOURCE_DIR}/ImageAnalysis/psImageAnalysis.cxx)
add_library(Filtering ${PROJECT_SOURCE_DIR}/Filtering/psFiltering.cxx)

# Declare the outputs and link requisite libraries to them
add_executable(psUICXXSingleUnit ${PROJECT_SOURCE_DIR}/UI/psUICXXSingleUnit.cxx)
add_executable(psUICXXMultiImageSingleThread ${PROJECT_SOURCE_DIR}/UI/psUICXXMultiImageSingleThread)

target_link_libraries(psUICXXSingleUnit 
		${ITK_LIBRARIES} 
		Registration
		ImageAnalysis
		Filtering)
target_link_libraries(psUICXXMultiImageSingleThread
		${ITK_LIBRARIES}
		Registration
		ImageAnalysis
		Filtering)
