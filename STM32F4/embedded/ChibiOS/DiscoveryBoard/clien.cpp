#include <fstream>
#include <iostream>


int main(){
  
  std::fstream file("/dev/ttyACM0");
  
  file << "print" << std::endl; // endl does flush, which may be important
  
  std::string response;
  
  file >> response;
  
  std::cout << response << "\n";
  
}
