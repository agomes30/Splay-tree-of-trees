/************************************************************************
 * Author:    Alex Anthony Gomes
 * File:      scanTest.cpp
 * Project:   Project 3
 * Date:      04/10/2020
 * Section:   05, CMSC 341
 * Email:     mb38373@umbc.edu
 * Description: Test for the Scanner
 ************************************************************************/


#include <vector>
#include "Scanner.h"

using namespace std;

int main() {

  // Characters used in the ASCII art, in order.
  vector<string> chars = {"█","░"};
  
  // Scanner with 15 lines, 34 characters per line, 2 characters
  Scanner *s = new Scanner(15, 34, chars);

  // Read data files
  if (not s -> loadFiles("scrambled.txt", "weights.txt") ) {
    cerr << "Error reading data files." << endl;
  } else {
    s -> dump();
    s -> inorder();
  }

  //checking for self-assignment
  cout << endl;
  cout << "Checking the self-assignment: ";
  s = s;
  cout << "Successful!" << endl;

  //checks the copy constructor
  cout << endl;
  cout << "Copy test: " << endl;
  cout << "copying the object and deleting the original..." << endl;
  Scanner copy(*s);
  //deleting the original, and the copy prevails
  delete s;
  s = nullptr;
  cout << "Here's the deep copy: " << endl;
  copy.dump();
  cout << endl;

  //assignment test
  cout << "Assignment test: " << endl;
  Scanner copy2 = copy;
  cout << "printing out the copy..." << endl;
  copy2.dump();
  cout << endl;
  
  return 0;
}