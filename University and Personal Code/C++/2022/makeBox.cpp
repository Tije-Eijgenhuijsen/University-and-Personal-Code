#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

int getBoxSize(){
	int boxSize;
	std::cout << "What box size do you want: ";
	std::cin >> boxSize;
	std::cout << std::endl;
	return boxSize;
}

void insertHorizontalLine(std::vector<std::vector<char>>& box, int y){
	for(int x = 0; x < box.size(); x++){
		box[x][y] = '#';
	}
}

void insertVerticalLine(std::vector<std::vector<char>>& box, int x){
	for(int y = 0; y < box.size(); y++){
		box[x][y] = '#';
	}
}

void printBox(std::vector<std::vector<char>> box){
	for(int y = 0; y < box.size(); y++){
		for(int x = 0; x < box.size(); x++){
			std::cout << box[x][y];
		}
		std::cout << std::endl;
	}
}

void makeBox(int boxSize){
	std::vector<std::vector<char>> box(boxSize, std::vector<char>(boxSize, ' '));
	insertHorizontalLine(box, 0);
	insertVerticalLine(box, 0);
	insertVerticalLine(box, boxSize - 1);
	insertHorizontalLine(box, boxSize - 1);
	printBox(box);
}

int main(){

	makeBox(getBoxSize());

	return 0;
}