#include <iostream>

int x = 33,
 y = 42;
 
int foo(){
 int x = 7;
 {
 int x = y;
 ++x;
 y = x;
 }
 ++x;
 return x;
}
int main(){
 x = foo();
 std::cout << x << ' ' << y;
 return 0;
}
