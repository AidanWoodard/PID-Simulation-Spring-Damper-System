#include <iostream>
#include <fstream>
using namespace std;

int main() {
  // prviously created
  fstream TestFile("testFile.txt");
  TestFile << "Hell world.";
  
  // seekg takes (change, origin) such that if our file pointer is at 3, 
  // File.seekg(-1, fstream::curr) would move the pointer back one char.
  // note: 'beg' for origin, 'curr' for current pos, and 'end' for end.
  TestFile.seekg(0, fstream::beg);
  char contents[4];
  TestFile.read(contents, 3);
  contents[3] = '\0';
  cout << contents << '\n';

  // since we don't move the file pointer, we pick up where we left off.
  // in this case we start at the second 'l' of Hell world and print 'l w'.
  char contents2[4];
  TestFile.read(contents2, 3);
  contents2[3] = '\0';
  cout << contents2 << '\n';

  TestFile.close();
  return 0;
} 