#include <iostream>
#include <fstream>
#include <format>
//using namespace std;    // this is poor practice, use std:: for clarity, etc.

int main() {
  std::fstream fout;
  fout.open("test.csv", std::ios::out | std::ios::app);
  
  // redundant, but good practice
  if (!fout.is_open()) {
    std::cerr << "ERROR: could not find or open csv file 'test.csv'" << std::endl;
    return 1;
  }

  // add num1 and num2, multiply result by multFactor
  int num1, num2, multFactor;
  int totalSum, i;

  std::cout << "####### Write 3 different sums to add to the .csv #######"
            << '\n';
  std::cout << "Enter 2 numbers to add together."
       << "Then enter one more number to multiply the sum."
       << std::endl;
  
  for (i = 0; i < 3; i++) {
    std::cout << std::format("Current editing line: {}", i + 1) << '\n';
    std::cout << "Enter number 1, 2, and the multiplier:" << '\n';

    // save input
    std::cin >> num1
              >> num2
              >> multFactor;
    totalSum = (num1 + num2) * multFactor;

    // write to .csv
    fout << num1 << ","
        << num2 << ","
        << multFactor << ","
        << totalSum
        << '\n';
    }

  return 0;
} 