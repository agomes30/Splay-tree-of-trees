/************************************************************************
 * Author:    Alex Anthony Gomes
 * File:      Scanner.cpp
 * Project:   Project 3
 * Date:      04/10/2020
 * Section:   05, CMSC 341
 * Email:     mb38373@umbc.edu
 * Description: Definition file for the Scanner class
 ************************************************************************/

// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include "Scanner.h"

// Balance function for use with BSTs.  This implements the
// height-balance property for AVL trees.  Returns true if
// height-balance property is violated.
bool Scanner::heightImbalance(int leftHeight, int rightHeight, int leftSize, int rightSize) {
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

// SNode constructor.  bounds contains the upper and lower bounds on
// the weights for this BST.
Scanner::SNode::SNode(pair<int, int> bounds) {

  _bounds = bounds;
  _root = new BST(heightImbalance);
  _left = _right = nullptr;

}

Scanner::SNode::~SNode() {

  //deleting the root for the splay nodes
  if (_root != nullptr){
    delete _root;
    _root = nullptr;
  }

}

// Initialize a Scanner object to process an image with 'lines' lines
// of text, each of which is 'range' characters long.  The ASCII
// characters corresponding to the character indices are passed in
// 'chars'.
Scanner::Scanner(int lines, int range, vector<string> chars) {
  
  pair<int, int> temp; //range for each node of the splay tree
  //initially
  temp.first = 1;
  temp.second = range;

  SNode* itr; 
  SNode* trail = nullptr;
  for (int i = 0; i < lines - 1; i++){

    temp.first += range;
    temp.second += range;
  }
  for (int i = 0; i < lines; i++){
    itr = new SNode(temp); //creating a new node with bounds
    itr -> _right = trail; //storing the previous node
    trail = itr; //now the trail is catching up

    temp.first -= range;
    temp.second -= range;

  }

  //finally the root of the splay tree is set to already made tree
  _forest = itr;
  //setting the number of lines
  _lines = lines;
  //setting the characters
  _chars = chars;

}

Scanner::~Scanner() {
  if (_forest) {
    RemoveSplayTree(_forest); //removing the splay tree
    _forest = nullptr; 
  }
}

Scanner::SNode* Scanner::RemoveSplayTree(SNode* ptr){

  //if tree is not empty
  if (ptr != nullptr){

    if (ptr -> _left != nullptr){
      RemoveSplayTree(ptr -> _left);
    }
    if (ptr -> _right != nullptr){
      RemoveSplayTree(ptr -> _right);
    }

    delete ptr;
    ptr = nullptr;
  }

  return ptr;

}

Scanner::Scanner(const Scanner& rhs) {
  
  //before copying, setting the root to nullpointer
  _forest = nullptr;
  //copying the object
  _forest = copy(rhs._forest); //makes a deep copy

  _lines = rhs._lines; //copying the number of lines

  //deep copying the character vectors
  for (int i = 0; i < int(rhs._chars.size()); i++){
    _chars.push_back(rhs._chars[i]);
  }

}

Scanner::SNode* Scanner::copy(SNode *ptr){

  if (ptr == nullptr){
    return nullptr;
  }
  else{

    pair <int, int> temp;
    temp.first = ptr -> _bounds.first;
    temp.second = ptr -> _bounds.second;
    SNode* newNode = new SNode(temp);
    delete newNode->_root;
    newNode->_root = nullptr;
    newNode->_root = new BST(*ptr->_root);

    //copying the right and left pointers
    newNode -> _left = copy(ptr -> _left);
    newNode -> _right = copy(ptr -> _right);

    return newNode;
  }

}

Scanner& Scanner::operator=(const Scanner& rhs) {
  
  //the objects are different
  if (this != &rhs){

    if (_forest){
      RemoveSplayTree(_forest); //deleting before assigning
    }
    _forest = nullptr;
    _forest = copy(rhs._forest); //copying the tree

    _lines = rhs._lines; //copying the lines

    _chars = rhs._chars;
    // //deep copying the character vectors
    // for (int i = 0; i < int(rhs._chars.size()); i++){
    //   _chars.push_back(rhs._chars[i]);
    // }

  }
  return *this;
}

// Read the data files and insert into the datastructure.  File in
// 'ascii' contains the scrambled character indices; file 'weights'
// contains the corresponding weights.
bool Scanner::loadFiles(string ascii, string weights) {

  std::ifstream fileChar;
  std::ifstream fileWeight;

  fileChar.open(ascii);
  fileWeight.open(weights);

  int character;
  int weight;
  
  //file is not there
  if ((!fileChar.is_open()) or (!fileWeight.is_open())){
    return false;
  }

  while(!fileChar.eof() and !fileWeight.eof()){
    
    fileChar >> character;
    fileWeight >> weight;

    if ((fileChar.peek() == ',' and fileWeight.peek() == ',') or (fileChar.peek() == '\n' and fileWeight.peek() == '\n')) {
        fileChar.ignore();
        fileWeight.ignore();
      
    }

    insert(weight, character); //calling the insert function
  }

  //closing the files
  fileChar.close();
  fileWeight.close();
  
  return true;
}

Scanner::SNode* Scanner::splay(SNode *ptr, int weight){

  //if the root is nullptr or the weight is found in the root of the tree, just return the root
  if (ptr == nullptr or ((weight >= ptr -> _bounds.first) and (weight <= ptr -> _bounds.second))){
    return ptr;
  }

  //if the weight value is lesser than the checked weight range
  if (weight < ptr -> _bounds.first){
    //the weight is not in the tree
    if (ptr -> _left == nullptr){
      return ptr;
    }

    //zig-zig
    if (weight < ptr -> _left -> _bounds.first){
      ptr -> _left -> _left = splay(ptr -> _left -> _left, weight);
      ptr = rightRotate(ptr);
    }

    //zig-zag
    else if(weight > ptr -> _left -> _bounds.second){
      
      ptr -> _left -> _right = splay(ptr -> _left -> _right, weight);
      if (ptr -> _left -> _right != nullptr){
        ptr -> _left = leftRotate(ptr -> _left);
      }
    }
    
    return (ptr -> _left == nullptr) ? ptr: rightRotate(ptr);

  }

  else if (weight > ptr -> _bounds.second){ //key lies in right subtree
    //the weight is not in the tree
    if (ptr -> _right == nullptr){
      return ptr;
    }

    //zig-zig
    if (weight < ptr -> _right -> _bounds.first){
      ptr -> _right -> _left = splay(ptr -> _right -> _left, weight);
    
      if (ptr -> _right -> _left != nullptr){
        ptr -> _right = rightRotate(ptr -> _right);
      }
    }

    //zig-zag
    else if(weight > ptr -> _right -> _bounds.second){

      ptr -> _right -> _right = splay(ptr -> _right -> _right, weight);
      ptr = leftRotate(ptr);

    }

    return (ptr -> _right == nullptr) ? ptr: leftRotate(ptr);

  }
  return ptr;
}

Scanner::SNode* Scanner::rightRotate(SNode *ptr){

  //rotating right
  SNode* temp = ptr -> _left;
  ptr -> _left = temp -> _right;
  temp -> _right = ptr;
  return temp;

}
Scanner::SNode* Scanner::leftRotate(SNode *ptr){

  //rotating left
  SNode* temp = ptr -> _right;
  ptr -> _right = temp -> _left;
  temp -> _left = ptr;
  return temp;

}

// Insert a node; splay the node that is inserted.
bool Scanner::insert(int weight, int ch) {

  if (_forest == nullptr){
    return false;
  }

  //splaying the tree so the node where we're trying to insert becomes the root node
  _forest = splay(_forest, weight);
  if (_forest == nullptr){
    return false;
  }
  //inserting to the root node
  _forest -> _root -> insert(_chars[ch-1], weight);
  
  return true;
}

void Scanner::dump() const {

  //calling the recursive dumpHelp function
  dumpHelp(_forest);
  cout << endl;

}

void Scanner::dumpHelp(SNode* itr)const {

  if(itr == nullptr) {
    return;
  }

  //dumping the image
  dumpHelp(itr->_left);
  itr->_root->dump();
  cout << endl;
  dumpHelp(itr->_right);
 
}

void Scanner::inorder() const {

  //calling the recursive inOrder function
 if (_forest != nullptr) {
   inOrder(_forest);
   cout << endl;
 } 

}

void Scanner::inOrder(SNode* itr) const{

  if(itr == nullptr) 
    return;

  //inorder traversal of the tree
  cout << "(";
  inOrder(itr->_left);
  cout << itr->_bounds.first << ":" <<  itr->_bounds.second;
  inOrder(itr->_right);
  cout << ")";
  
}


