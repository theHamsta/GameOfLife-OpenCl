#include <iostream>
#include <chrono>
#include <thread>

#include "board.hpp"

using namespace std;

void clearScreen() {
	printf("\e[1;1H\e[2J");
}



int main(int argc, char **argv) {
    
	srand(2);
	
	Board board(6,6);
	
	board.fillRandomly();
	
	board.print();
	
	for(int i = 0; i < 100; i++){
		clearScreen();
		board.stepHost();
		board.print();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	
    return EXIT_SUCCESS;
}
