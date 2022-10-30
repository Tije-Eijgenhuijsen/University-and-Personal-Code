#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

bool isOutOfBounds(int width, int height, int x, int y){
	return (x < 0 || y < 0 || x >= width || y >= height);
}

int computeIslandSize(std::vector<std::vector<int>>& island, int x, int y){
	//check for absence of land or out of bounds on current cell
	if(isOutOfBounds(island[0].size(), island.size(), x, y) || island[y][x] == 0) {return 0;}
	
	//Else set island to 0 to make sure we don't count it again
	island[y][x] = 0;
	return (1 + 
			computeIslandSize(island, x + 1, y) +
			computeIslandSize(island, x - 1, y) +
			computeIslandSize(island, x, y + 1) +
			computeIslandSize(island, x, y - 1));
}

int findBiggestIslandSize(std::vector<std::vector<int>>& island){
	int biggestIslandSize = 0;

	for(int y = 0; y < island.size(); y++){
		for(int x = 0; x < island[0].size(); x++){
			biggestIslandSize = std::max(biggestIslandSize, computeIslandSize(island, x, y));
		}
	}

	return biggestIslandSize;
}

int main(){
	std::vector<std::vector<int>> island = {
		{1, 1, 1, 1, 1, 0, 0, 1},
		{1, 1, 1, 1, 1, 0, 0, 1},
		{1, 1, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 0, 0, 1},
		{1, 1, 1, 1, 1, 0, 0, 1}
	};

	int biggestIslandSize = findBiggestIslandSize(island);

	std::cout << "The biggest island is of size " << biggestIslandSize << "!" << std::endl;

	return 0;
}