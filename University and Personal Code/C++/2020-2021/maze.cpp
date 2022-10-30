#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>

class Location {
  public:
    Location(int tempX = 0, int tempY = 0);
    void setX(int tempX) { x = tempX; }
    int getX() { return x; }
    void setY(int tempY) { y = tempY; }
    int getY() { return y; }
  private:
    int x; 
    int y;
};

Location::Location(int tempX, int tempY) {
    x = tempX;
    y = tempY;
}

class Size {
  public:
    Size(int tempHeight = 0, int tempWidth = 0);
    void setHeight(int tempHeight) { Height = tempHeight; }
    int getHeight() { return Height; }
    void setWidth(int tempWidth) { Width = tempWidth; }
    int getWidth() { return Width; }
  private:
    int Height; 
    int Width;
};

Size::Size(int tempHeight, int tempWidth) {
    Height = tempHeight;
    Width = tempWidth;
}

void printMaze(std::vector<char> maze, Size mazeSize){
    for(int i = 0; i < mazeSize.getHeight(); i++){
        for(int j = 0; j < mazeSize.getWidth(); j++){
            std::cout << maze.at((i * mazeSize.getWidth()) + j);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void placeCoordinate(std::vector<char>& maze, Size mazeSize, Location coordinate, char symbol){
    maze.at((coordinate.getY() * mazeSize.getWidth()) + coordinate.getX()) = symbol;
}

void playerMoves(std::vector<char> maze, Location& player, std::vector<char> moves, Size mazeSize, int i){
    switch(moves.at(i)){
        case 'u' :
            if(player.getY() - 1 < 0){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.setY(player.getY() - 1);
            break;
        case 'd' :
            if(player.getY() + 1 >= mazeSize.getHeight()){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.setY(player.getY() + 1);
            break;
        case 'l' :
            if(player.getX() - 1 < 0){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.setX(player.getX() - 1);
            break;
        case 'r' :
            if(player.getX() + 1 >= mazeSize.getWidth()){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.setX(player.getX() + 1);
            break;
        default :
            break;
    }
}

int main(int argc, char *argv[]){

    try {
        if(argc == 1){
            throw std::runtime_error("no input file name given");
        }

        std::ifstream inFS;
        inFS.open(argv[1]);

        if(!inFS.is_open()){
            std::cout << "an error occurred: could not open input file " << argv[1] << std::endl;
            return 1;
        }  

        Size mazeSize;

        int tempInt;
        inFS >> tempInt;
        mazeSize.setHeight(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read height and width of the maze");
        }
        inFS >> tempInt;
        mazeSize.setWidth(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read height and width of the maze");
        }

        char tempChar;
        std::vector<char> maze(mazeSize.getHeight() * mazeSize.getWidth());
        for(int i = 0; i < maze.size(); i++){
            inFS >> tempChar;
            maze.at(i) = tempChar;
            if (tempChar != '.' && tempChar != 'l'){
                throw std::runtime_error("could not read maze layout");
            }
        }

        Location exit;
        
        inFS >> tempInt;
        exit.setY(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }
        inFS >> tempInt;
        exit.setX(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }

        Location start;

        inFS >> tempInt;
        start.setY(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }
        inFS >> tempInt;
        start.setX(tempInt);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }

        tempChar = maze.at((exit.getY() * mazeSize.getWidth()) + exit.getX());
        if(tempChar == 'l'){
            throw std::runtime_error("exit outside maze or off the path");
        }

        std::vector<char> moves;
        while(!inFS.eof()){
            inFS >> tempChar;
            moves.push_back(tempChar);
            if(tempChar != 'l' && tempChar != 'r' && tempChar != 'u' && tempChar != 'd'){
                throw std::runtime_error("invalid move");
            }
        }
        moves.resize(moves.size() - 1);

        std::vector<char> mazeProgression;
        mazeProgression = maze;

        placeCoordinate(mazeProgression, mazeSize, exit, 'x');
        placeCoordinate(mazeProgression, mazeSize, start, 'P');

        printMaze(mazeProgression, mazeSize);

        mazeProgression = maze;
        Location player;
        player.setY(start.getY());
        player.setX(start.getX());

        for(int i = 0; i < moves.size(); i++){
            playerMoves(mazeProgression, player, moves, mazeSize, i);
            if(maze.at((player.getY() * mazeSize.getWidth()) + player.getX()) == 'l'){
                throw std::runtime_error("player outside maze or off the path");
            }
        }

        if(exit.getY() == player.getY() && exit.getX() == exit.getX()){
            placeCoordinate(mazeProgression, mazeSize, player, 'P');
            printMaze(mazeProgression, mazeSize);
            std::cout << "The player reached the exit!" << std::endl;
        }else{
            placeCoordinate(mazeProgression, mazeSize, player, 'P');
            placeCoordinate(mazeProgression, mazeSize, exit, 'x');
            printMaze(mazeProgression, mazeSize);
            std::cout << "The player did not reach the exit." << std::endl;
        }

    }
    catch(std::runtime_error& excpt){
        std::cout << "an error occurred: " << excpt.what() << std::endl;
    }
    return 0;
}
