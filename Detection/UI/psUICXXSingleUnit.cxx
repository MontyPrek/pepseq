#include "../Global/psGlobalDefinitions.h"
#include "../Filtering/psFiltering.h"
#include "../Registration/psRegistration.h"
#include "../ImageAnalysis/psImageAnalysis.h"
#include <iostream>
#include <fstream>

int main( int argc, char* argv[] ) {
	int i;
	char userEntry;
	std::ofstream outFile;
	std::vector<type_SpotData> result;

	outFile.open( argv[2], std::ios_base::app );

	if( argc < 3 ) { 
		std::cout << "Please specify an image file. Terminating." 
				<< std::endl;
		return 0;
	}
	//std::cout << "Enter 'c' for spot counting." << std::endl;
	//std::cin >> userEntry;// std::cout << std::endl;
	//switch( userEntry ) {
	  //  case 'c': {
		type_ImageAnalysisUnit imageAnalysisUnit;
		type_FilteringUnit filteringUnit;

		filteringUnit.initData();
		filteringUnit.setImage( argv[1] );
		filteringUnit.roiCrop(200, 300, 200, 150);
		imageAnalysisUnit.initData();
		imageAnalysisUnit.setInputToFiltered( &filteringUnit );
		imageAnalysisUnit.countSpotsGray( 0, 0, 200, 150 );

		std::cout << imageAnalysisUnit.getTotalNumberOfSpots() << std::endl;
		
		result = imageAnalysisUnit.getSpotData();
		for( i = 0; i < result.size(); i++ ) { 
			outFile << result[i].spotID << ",";
			outFile << result[i].area << ",";
			outFile << result[i].netBrightness << ",";
			outFile << result[i].maxBrightness << ",";
			outFile << result[i].maxX << ",";
			outFile << result[i].maxY << ",";
		}
		outFile << std::endl;

	std::cout << "BG: "  << imageAnalysisUnit.getAverageBackground() << std::endl;
	
	
	
	//int i, j;
	//int brightness[500];
	//std::fill_n( brightness, 500, 0 );
	//for( i = 0; i < imageAnalysisUnit.getTotalNumberOfSpots(); i++ ) {
	//	int intensity = imageAnalysisUnit.getSpotNetBrightness( i ) - 
	//		( imageAnalysisUnit.getSpotArea( i ) * 
	//		  imageAnalysisUnit.getAverageBackground() );
	//	if( intensity > 498 ) { brightness[499]++; }
	//	else { brightness[intensity]++; }
		/*if( imageAnalysisUnit.getSpotNetBrightness( i ) > 498 ) {
			brightness[499]++;
		}
		else {
			brightness[imageAnalysisUnit.getSpotNetBrightness( i )]++;
		}*/
	//}
	//for( i = 0; i < 500; i++ ) {
	//	for( j = 0; j < brightness[i]; j++ ) { 
	//		std::cout << "=";
	//	}
	//	std::cout << std::endl;
	//}
	//for( i = 0; i < 500; i++ ) {
	//	std::cout << brightness[i] << " ";
	//	outFile << brightness[i] << ",";
	//}
	//outFile << std::endl;

	//std::cout << std::endl <<imageAnalysisUnit.getAverageBackground() << std::endl;
	//for( i = 0; i < imageAnalysisUnit.getSpotData().size(); i++ ) {
		
	//	std::cout << imageAnalysisUnit.getSpotData()[i].area << std::endl;
	//	std::cout << imageAnalysisUnit.getSpotData()[i].maxX << std::endl;
	//	std::cout << imageAnalysisUnit.getSpotData()[i].maxY << std::endl << std::endl;
	//}
	//	break;
	  //  }
	   // default: {
	//	std::cout << "Not a valid option. Terminating." << std::endl;
	//	return 0;
	  //  }

	//}
}
