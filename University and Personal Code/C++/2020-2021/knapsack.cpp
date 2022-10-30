#include <iostream>
#include <vector>
#include <fstream>

std::vector<bool> inBackPack;
int biggestValue = 0;

int getTotal(std::vector<bool> temp, std::vector<int> vector){
    int total = 0;
    for(int i = 0; i < vector.size(); i++){
        total = total + (temp.at(i) * vector.at(i));
    }
    return total;
}

void printResult(int capacity, std::vector<int> weight, std::vector<int> value){

    std::cout << "With a capacity of " << capacity << " kg, the total value is " << biggestValue << " Eur, leading to a total weight of " << getTotal(inBackPack, weight) << " kg" << std::endl 
              << "The following items are included:";

    for(int i = 0; i < inBackPack.size(); i++){
        if (inBackPack.at(i) == 1){
            std::cout << " [" << weight.at(i) << "," << value.at(i) << "]";
        }
    }
    std::cout << std::endl;

}

void knapSack(int i, int capacity, std::vector<int> weight, std::vector<int> value, std::vector<bool> temp){
    if(i == weight.size()){
        return;
    }

    if(capacity >= weight.at(i)){
        knapSack(i + 1, capacity, weight, value, temp);
        temp.at(i) = 1;
        capacity -= weight.at(i);
        knapSack(i + 1, capacity, weight, value, temp);
    }else{
        knapSack(i + 1, capacity, weight, value, temp);
    }
    if(getTotal(temp, value) > biggestValue){
        biggestValue = getTotal(temp, value);
        inBackPack = temp;
    }
    temp.at(i) = 0;
    


}

int main(int argc, char *argv[]){
    if(argc == 1){
        std::cout << "No input filename given" << std::endl;
        return 1;
    }

    std::ifstream inFS;
    inFS.open(argv[1]);

    if(!inFS.is_open()){
        std::cout << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    unsigned int capacity;
    std::vector<int> weight;
    std::vector<int> value;
    int temp;

    inFS >> capacity;
    
    while(!inFS.eof()){
        inFS >> temp;
        weight.push_back(temp);
        inFS >> temp;
        value.push_back(temp);
    }
    weight.resize(weight.size() - 1);
    value.resize(value.size() - 1);

    std::vector<bool> temporary(weight.size(), 0);

    knapSack(0, capacity, weight, value, temporary);

    printResult(capacity, weight, value);

    return 0;  
}
