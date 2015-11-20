#include <iostream>
#include <chrono>
#include <thread>

#include "board.hpp"

using namespace std;

void clearScreen() {
// 	printf("\e[1;1H\e[2J");
}



int main(int argc, char **argv) {
    
	srand(2);
	
	Board board(3,1);
	
	board.fillRandomly();
	board.uploadToDevice();

// 	board.print();
// 	board.debugPrintDeviceData();
// 	board.print();
	
	for(int i = 0; i < 6; i++){
		clearScreen();
	
		board.stepDeviceOptimized();
		
		board.updateFieldsHost();
		board.broadcastNeighboursHost();
// 		board.checkConsistency();
		
		board.print(false);
		board.debugPrintDeviceData(false);

		
// 

// 		board.print();
// 		board.debugPrintDeviceData();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	
    return EXIT_SUCCESS;
}
