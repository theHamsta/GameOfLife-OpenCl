#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "board.hpp"

using namespace std;
using namespace std::chrono;

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
	bool bCheckCorrectness;
	bool bProfiling;
	
	if ( argc == 9 ) {
		try{
			numRounds = stoi(argv[1]);
			boardWidth = stoi(argv[2]);
			boardHeight = stoi(argv[3]);
			localWorkgroupSize = stoi(argv[4]);
			numLocalWorkgroups = stoi(argv[5]);
			bGraphicalOutput = stoi(argv[6]);
			bCheckCorrectness = stoi(argv[7]);
			bProfiling = stoi(argv[8]);
			cout << "boardWidth: " << boardWidth << endl;
			cout << "boardHeight: " << boardHeight << endl;
			cout << "localWorkgroupSize: " << localWorkgroupSize << endl;
			cout << "numLocalWorkgroups: " << numLocalWorkgroups << endl;
			cout << "bGraphicalOutput: " << bGraphicalOutput << endl;
			cout << "bCheckCorrectness: " << bGraphicalOutput << endl;
			cout << "bProfiling: " << bProfiling << endl;
		} catch (exception& err){
			cout << "Error parsing arguments" << endl;
			exit( EXIT_FAILURE );
		}
	} else {
		cout << "Usage: gameoflifecl NUM_ROUNDS BOARD_WIDTH*4 BOARD_HEIGHT*3 LOCAL_WORKGROUP_SIZE NUM_LOCAL_WORKGROUPS B_GRAPHICAL_OUTPUT[0 or 1] B_CHECK_CORRECTNESS[0 or 1] B_PROFILING[0 or 1]" << endl;
		exit( EXIT_SUCCESS );
	}
	Board board(boardWidth, boardHeight, localWorkgroupSize, numLocalWorkgroups, bProfiling);
	
	board.fillRandomly();
	board.uploadToDevice();

	if(bGraphicalOutput) {
		board.print(false);
		board.debugPrintDeviceData(false);
	}

	
	high_resolution_clock::time_point start = high_resolution_clock::now();


	for(uint i = 0; i < numRounds; i++){
		if (bGraphicalOutput){
			clearScreen();
			this_thread::sleep_for(chrono::milliseconds(100));
		}
		board.stepDeviceOptimized();
		if(bCheckCorrectness) {
			board.stepHost();
		}

		if(bGraphicalOutput){
			cout << "Round: " << (i+1) << " of " << numRounds << endl;
			if(bCheckCorrectness) {
				cout << "Host: " << endl;
				board.print(false);
			}
			cout << "Device: " << endl;
			board.debugPrintDeviceData(false);
		}
		if( bCheckCorrectness ){
			board.checkRelevantConsistency();
		}



		
// 

// 		board.print();
// 		board.debugPrintDeviceData();

	}
	
    high_resolution_clock::time_point end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( end - start ).count();
	if (bProfiling) {
		cout << "Total Step-Kernel time: " << (board.getTotalStepKernelTime() * 1e-6) << "ms" << endl;
		cout << "Total Fix-Kernel time: " << (board.getTotalFixKernelTime() * 1e-6) << "ms" << endl;
		cout << "Total device time: " << ((board.getTotalFixKernelTime() + board.getTotalStepKernelTime()) * 1e-6) << "ms" << endl;
	}
	
	

    cout << "Total time on host: " << duration << "ms" << endl;
    return EXIT_SUCCESS;
}
