/************************************************************************
 * Author:    Alex Anthony Gomes
 * File:      bstTest.cpp
 * Project:   Project 3
 * Date:      04/10/2020
 * Section:   05, CMSC 341
 * Email:     mb38373@umbc.edu
 * Description: Test for the BST
 ************************************************************************/


#include "BST.h"

bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize) {
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}


int main() {
  
  BST bst(imbalfn);

  bst.insert("A", 100);
  bst.insert("B", 110);
  bst.insert("A", 90);
  bst.insert("B", 80);
  bst.insert("B", 70);
  bst.insert("B", 60);
  bst.insert("A", 111);
  bst.insert("A", 112);
  bst.insert("B", 85);
  bst.insert("A", 86);
  bst.insert("B", 35);
  bst.insert("A", 45);
  bst.insert("B", 2);
  bst.insert("A", 1);

  bst.dump();
  cout << endl;
  bst.dump(true);

  

  
  cout << endl;
  cout << "size = " << bst.size() << endl;
  cout << "height = " << bst.height() << endl;

  cout << "Self Assignment Test: ";
  bst = bst;
  cout << "Successful!" << endl;

  //checks the copy constructor
  cout << "copy test: " << endl;
  BST bst2(bst);
  cout << "printing out the copy: " << endl;
  bst2.dump(true);
  cout << endl;

 
  cout << "adding 10 more nodes to the original: " << endl;
  //the change shouldn't reflect on bst2(deep copy)
  int size = 10; //number of pairs to be inserted
  string character[size] = {"A","B","C","D","E","F","G","H","I","J"};
  for (int i = 0; i < size; i++){
    bst.insert(character[i], i);
  }

  cout << "printing out the original: " << endl;
  bst.dump(true);
  cout << endl;

  cout << "printing out the copy: " << endl;
  bst2.dump(true);
  cout << endl;

  //checks the assignment
  cout << "assignment test: " << endl;
  BST bst3 = bst;
  cout << "Here's a copy of the original: " << endl;
  bst3.dump(true);
  cout << endl;

  return 0;
}
