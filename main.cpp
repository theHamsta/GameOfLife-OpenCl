#include <iostream>
#include <chrono>
#include <thread>

#include "board.hpp"

using namespace std;

void clearScreen() {
// 	printf("\e[1;1H\e[2J");
}



int main(int argc, char **argv) {
    
	srand(3);
	
	Board board(5,1);
	
	board.fillRandomly();
	board.uploadToDevice();

// 	board.print(true);
// 	board.debugPrintDeviceData(true);
// 	board.print();
	
	for(int i = 0; i < 1; i++){
		clearScreen();
	
		board.stepDeviceOptimized();
		
		board.updateFieldsHost();
		board.broadcastNeighboursHost();
// 		board.checkConsistency();
		
		board.print(true);
		board.debugPrintDeviceData(true);

		
// 

// 		board.print();
// 		board.debugPrintDeviceData();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	
    return EXIT_SUCCESS;
}
