#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "board.hpp"

using namespace std;

void clearScreen() {
 	printf("\e[1;1H\e[2J");
}

#define RANDOM_SEED 15

int main(int argc, char **argv) {
    
	srand(RANDOM_SEED);

	uint boardWidth;
	uint boardHeight;
	uint localWorkgroupSize;
	uint numLocalWorkgroups;
	uint numRounds;
	bool bGraphicalOutput;

	
	if ( argc == 7 ) {
		try{
			numRounds = stoi(argv[1]);
			boardWidth = stoi(argv[2]);
			boardHeight = stoi(argv[3]);
			localWorkgroupSize = stoi(argv[4]);
			numLocalWorkgroups = stoi(argv[5]);
			bGraphicalOutput = stoi(argv[6]);
			cout << "boardWidth: " << boardWidth << endl;
			cout << "boardHeight: " << boardHeight << endl;
			cout << "localWorkgroupSize: " << localWorkgroupSize << endl;
			cout << "numLocalWorkgroups: " << numLocalWorkgroups << endl;
			cout << "bGraphicalOutput: " << bGraphicalOutput << endl;
		} catch (exception& err){
			cout << "Error parsing arguments" << endl;
			exit( EXIT_FAILURE );
		}
	} else {
		cout << "Usage: gameoflifecl NUM_ROUNDS BOARD_WIDTH*4 BOARD_HEIGHT*3 LOCAL_WORKGROUP_SIZE NUM_LOCAL_WORKGROUPS B_GRAPHICAL_OUTPUT[0 or 1]" << endl;
		exit( EXIT_SUCCESS );
	}
	Board board(boardWidth, boardHeight, localWorkgroupSize, numLocalWorkgroups);
	
	board.fillRandomly();
	board.uploadToDevice();

	if(bGraphicalOutput) {
		board.print(false);
		board.debugPrintDeviceData(false);
	}
// 	board.print();
	
	for(uint i = 0; i < numRounds; i++){
		if (bGraphicalOutput){
			clearScreen();
			this_thread::sleep_for(chrono::milliseconds(100));
		}
		board.stepDeviceOptimized();
		
		board.updateFieldsHost();
		board.broadcastNeighboursHost();
		

		if(bGraphicalOutput){
			board.print(false);
			board.debugPrintDeviceData(false);
		}
		board.checkRelevantConsistency();




		
// 

// 		board.print();
// 		board.debugPrintDeviceData();

	}
	
    return EXIT_SUCCESS;
}
