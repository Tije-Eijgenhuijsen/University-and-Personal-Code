#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void fizzBuzz(int untilNumber){
	std::string output = "";
	for(int i = 0; i <= untilNumber; i++){
		if(i % 3 == 0){
			output += "Fizz";
		}
		if(i % 5 == 0){
			output += "Buzz";
		}
		if(output == ""){
			output += std::to_string(i);
		}
		std::cout << output << std::endl;
		output = "";
	}
}

int untilNumber(){
	int input;
	std::cout << "Until what number are we fizzbuzzing: ";
	std::cin >> input;
	std::cout << std::endl;
	return input;
}

int main(){
	fizzBuzz(untilNumber());
	return 0;
}