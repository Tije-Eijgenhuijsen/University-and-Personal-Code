#include <iostream>
#include <set>
#include <cmath>

int getInput(int& number){

    std::cout << "Which number do you want to test? ";
    std::cin >> number;

    return number;
}

int pdiFunction(int& number){
    const int base = 10;
    int total = 0;
    while(number > 0){
        total = total + pow(number % base, 2);
        number = number / base;
    }
    return total;
}

bool isHappy(int& number, std::set<int>& seenNumbers){

    while(number > 1 && seenNumbers.count(number) == 0){
        seenNumbers.insert(number);
        number = pdiFunction(number);
    }
    return number == 1;
}

void result(int initialNumber, int happyOrUnhappy){
    if(happyOrUnhappy == 1){
        std::cout << std::endl << initialNumber << " is happy!" << std::endl;
    } else if(happyOrUnhappy == 0){
        std::cout << std::endl << initialNumber << " is unhappy!" << std::endl;
    }
}

int main(){

    std::set<int> seenNumbers;
    int number;

    int initialNumber = getInput(number);

    bool happyOrUnhappy = isHappy(number, seenNumbers);
    
    result(initialNumber, happyOrUnhappy);

    return 0;
}
