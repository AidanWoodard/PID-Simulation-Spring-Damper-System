#include <iostream>
#include <fstream>
using namespace std;

int main() {
  // Create and open a text file
  fstream MyFile("testFile.txt");

  // Write to the file
  MyFile << "Hell world.";
  
  // Read from the file
  string myText;
  getline(MyFile, myText);
  cout << myText;

  // Close the file
  MyFile.close();
} 