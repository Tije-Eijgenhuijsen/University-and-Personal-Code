#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

class Prison{
	public:
		//Cell = 1 indicates unlocked cell
		//Cell = 2 indicates locked cell
		std::vector<int> cells{1, 1, 0, 0, 0, 1, 0};
		int lastCellOpened;
		int prisonersFreed;
};

void swapLocks(Prison& currPrison){
	for(int i = 0; i < currPrison.cells.size(); i++){
		currPrison.cells[i] = 1 - currPrison.cells[i];
	}
}

bool freePrisoner(Prison& currPrison){
	for(int i = currPrison.lastCellOpened; i < currPrison.cells.size(); i++){
		if(currPrison.cells[i] == 1){
			currPrison.prisonersFreed += 1;
			currPrison.lastCellOpened = i;
			swapLocks(currPrison);
			return 1;
		}
	}
	return 0;
}

int freedPrisoners(Prison& currPrison){
	//Check if we can get out of the cell, if not return
	//Else initialize the prison escape
	if(!currPrison.cells[0]){
		return 0;
	}else{
		currPrison.lastCellOpened = 0;
		currPrison.prisonersFreed = 0;
	}

	while(freePrisoner(currPrison));
	
	return currPrison.prisonersFreed;
}

//Opening cells from left to right, starting with our own at the left
//If we open a cell all locks swap from locked to unlocked, or vise versa
//How many prisoners can be freed by the time we reach the end
//If the first (our own cell) starts out locked, we can't free anyone and escape

int main(){

	Prison test;
	int nrFreed = freedPrisoners(test);
	std::cout << "We managed to free " << nrFreed << " prisoners!" << std::endl;
	
	return 0;
}