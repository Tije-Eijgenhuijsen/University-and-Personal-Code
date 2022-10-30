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

void printMaze(std::vector<char> maze, int height, int width){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            std::cout << maze.at((i * width) + j);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void placeCoordinate(std::vector<char>& maze, int width, Location coordinate, char symbol){
    maze.at((coordinate.at(0) * width) + coordinate.at(1)) = symbol;
}

void playerMoves(std::vector<char> maze, std::vector<int>& player, std::vector<char> moves, int width, int height, int i){
    switch(moves.at(i)){
        case 'u' :
            if(player.at(0) - 1 < 0){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.at(0) = player.at(0) - 1;
            break;
        case 'd' :
            if(player.at(0) + 1 >= height){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.at(0) = player.at(0) + 1;
            break;
        case 'l' :
            if(player.at(1) - 1 < 0){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.at(1) = player.at(1) - 1;
            break;
        case 'r' :
            if(player.at(1) + 1 >= width){
                throw std::runtime_error("player outside maze or off the path");
            }
            player.at(1) = player.at(1) + 1;
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
            std::cout << "an error occurred: could not open file " << argv[1] << std::endl;
            return 1;
        }  
    
        int height, width;
        inFS >> height >> width;
        if(inFS.fail()){
            throw std::runtime_error("could not read height and width of the maze");
        }

        char temp;
        std::vector<char> maze(height * width);
        for(int i = 0; i < maze.size(); i++){
            inFS >> temp;
            maze.at(i) = temp;   
        }

        Location exit;
        
        inFS >> temp;
        exit.setY(temp);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }
        inFS >> temp;
        exit.setX(temp);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }
        std::vector<int> start(2);
        inFS >> start.at(0) >> start.at(1);
        if(inFS.fail()){
            throw std::runtime_error("could not read coordinates");
        }


        temp = maze.at((exit.getY() * width) + exit.getX());
        if(temp == 'l'){
            throw std::runtime_error("exit outside maze or off the path");
        }

        std::vector<char> moves;
        while(!inFS.eof()){
            inFS >> temp;
            moves.push_back(temp);

        }
        moves.resize(moves.size() - 1);

        std::vector<char> mazeProgression;
        mazeProgression = maze;

        placeCoordinate(mazeProgression, width, exit, 'x');
        placeCoordinate(mazeProgression, width, start, 'P');

        printMaze(mazeProgression, height, width);

        mazeProgression = maze;
        std::vector<int> player = start;

        for(int i = 0; i < moves.size(); i++){
            playerMoves(mazeProgression, player, moves, width, height, i);
            if(maze.at((player.at(0) * width) + player.at(1)) == '1'){
                throw std::runtime_error("player outside maze or off the path");
            }
        }

        if(exit == player){
            placeCoordinate(mazeProgression, width, player, 'P');
            printMaze(mazeProgression, height, width);
            std::cout << "The player reached the exit!" << std::endl;
        }

    }
    catch(std::runtime_error& excpt){
        std::cout << "an error occurred: " << excpt.what() << std::endl;
    }

    return 0;
}
