#include <iostream>

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

int main(){

Location end;

end.setX(2);
std::cout << end.getY() << std::endl;

return 0;
}
