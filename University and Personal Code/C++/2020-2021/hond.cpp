#include <iostream>
#include <string>

class Dog {
    public:
    void setName(std::string tempName){ name = tempName; }
    void setEyeColour(std::string tempEyeColour){eyeColour = tempEyeColour; }
    void setFavToy(std::string tempFavToy){favToy = tempFavToy; }
    void setFavFood(std::string tempFavFood){favFood = tempFavFood; }
    void setHeight(int tempHeight){height = tempHeight; }
    void setWeight(int tempWeight){weight = tempWeight; }
    void print();

    private:
    std::string name;
    std::string eyeColour;
    std::string favToy;
    std::string favFood;
    int height;
    int weight;
};

void Dog::print(){
    std::cout << "Name: " << name << std::endl;
    std::cout << "Eye colour: " << eyeColour << std::endl;
    std::cout << "Favorite toy: " << favToy << std::endl;
    std::cout << "Favorite food: " << favFood << std::endl;
    std::cout << "Height (in cm) and Weight (in kg): " << height << ", " << weight << std::endl;
}



int main(){

    std::cout << "Hello world" << std::endl;

    return 0;
}