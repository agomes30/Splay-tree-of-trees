/************************************************************************
 * Author:    Alex Anthony Gomes
 * File:      BST.cpp
 * Project:   Project 3
 * Date:      04/10/2020
 * Section:   05, CMSC 341
 * Email:     mb38373@umbc.edu
 * Description: Definition file for the BST class
 ************************************************************************/

// CMSC 341 - Spring 2020 - Project 3

#include "BST.h"

// Constructor for the BNode nested class
BST::BNode::BNode(string ch, int rank, int height, int size) {
  _data = ch;
  _key = rank;
  _height = height;
  _size = size;
  _right = nullptr;
  _left = nullptr;
}

// Constructor for the BST class; requires a balance function
BST::BST(balfn_t imbalanced) {
  _root = nullptr;
  _imbalanced = imbalanced;
}

BST::~BST(){
  //freeing all memory used by the tree
  RemoveTree(_root); //removing the root with the given root
}

void BST::RemoveTree(BNode* ptr){

  //the tree is not empty
  if (ptr != nullptr){

    if (ptr -> _left != nullptr){
      RemoveTree(ptr -> _left);
    }
    if (ptr -> _right != nullptr){
      RemoveTree(ptr -> _right);
    }

    delete ptr;
    ptr = nullptr;
  }

}

//copy constructor
BST::BST(const BST& rhs) {
  
  _root = nullptr;
  _root = copy(rhs._root); //makes a deep copy
  _imbalanced = rhs._imbalanced;
}

BST::BNode* BST::copy(BNode *ptr){

  if (ptr == nullptr){
    return nullptr;
  }
  else{

    BNode* newNode = new BNode;

    //copying the data and the key
    newNode -> _data = ptr -> _data;
    newNode -> _key = ptr -> _key;

    //copying the right and left pointers
    newNode -> _left = copy(ptr -> _left);
    newNode -> _right = copy(ptr -> _right);

    //copying the height and size
    newNode -> _height = ptr -> _height;
    newNode -> _size = ptr -> _size;

    return newNode;
  }

}

BST& BST::operator=(const BST& rhs){

  //the objects are different
  if (this != &rhs){
    RemoveTree(_root); //deleting before assigning
    _root = copy(rhs._root); //copying the tree
    _imbalanced = rhs._imbalanced;
  }

  return *this; 
  
}



BST::BNode* BST::rInsert(BNode *p, string ch, int rank){

  BNode* newNode = nullptr;
  //if the tree is empty
  if (p == nullptr){
    newNode = new BNode(ch, rank); //dynamically creating a new node

    //setting the root to newNode when the tree is formed
    if (_root == nullptr){
      _root = newNode;
    }

    return newNode;
  }

  if (rank < p -> _key) {
    p -> _left = rInsert(p -> _left, ch, rank);
  } else if (rank > p -> _key){
    p -> _right = rInsert(p -> _right, ch, rank);
  }

  p -> _height = NodeHeight(p); // compares and returns the max. height of subtrees
  p -> _size = NodeSize(p); //size of the root

  return p;

}


// Insert a (character, rank) pair
bool BST::insert(string ch, int rank) {

  BNode* ptr;
  ptr = rInsert(_root, ch, rank);

  bool imbalance = false; //initially false
  int leftHeight;
  int rightHeight;
  int leftSize;
  int rightSize;

  //getting the heights of the left and right subtrees
  leftHeight = NodeHeight(ptr -> _left);
  rightHeight = NodeHeight(ptr -> _right);

  // //getting the sizes of the left and right subtrees
  leftSize = NodeSize(ptr -> _left);
  rightSize = NodeSize(ptr -> _right);

  //checking imbalance
  imbalance = _imbalanced(leftHeight, rightHeight, leftSize, rightSize);
  
  if (imbalance){
    _root = buildTree(ptr);
  }

  return true;

}

void BST::storeBSTNodes(BNode* root, BNode* nodes[], int &size){

  // Base case 
  if (root == nullptr){
    return; 
  }
  
  // Store nodes in Inorder (which is sorted 
  // order for BST) 
  
  storeBSTNodes(root -> _left, nodes, size);  
  nodes[size++] = root;
  storeBSTNodes(root -> _right, nodes, size); 

}

BST::BNode* BST::buildTreeUtil(BNode* nodes[], int start, int end){

  // base case 
  if (start > end) 
    return nullptr; 
  
  /* Get the middle element and make it root */
  int mid = (start + end)/2; 
  BNode* root = nodes[mid]; 
  
  /* Using index in Inorder traversal, construct 
  left and right subtress */
  root->_left  = buildTreeUtil(nodes, start, mid-1);  
  root->_right = buildTreeUtil(nodes, mid+1, end); 

  //updating the height and size
  root -> _height = NodeHeight(root);
  root -> _size = NodeSize(root);
  return root; 

}

//This functions converts an unbalanced BST to 
//a balanced BST 
BST::BNode* BST::buildTree(BNode* root)
{ 
  
  //creating the array of size root
  BNode** nodes = new BNode*[root -> _size];
  int* size = new int(0);

  storeBSTNodes(root, nodes, *size); 
  
  // Constucts balanced BST 
  int n = root -> _size; 

  BNode* node = buildTreeUtil(nodes, 0, n-1); 
  
  //clear the array.
  delete [] nodes;
  nodes = nullptr;

  //clearing the dynamically allocated size
  delete size;
  size = nullptr;

  return node;

} 


int BST::NodeHeight(BNode *itr){

  int leftHeight; //height of the left subtree
  int rightHeight; //height of the right subtree

  if (itr == nullptr){
    return -1;
  }

  leftHeight = NodeHeight(itr -> _left);
  rightHeight = NodeHeight(itr -> _right);

  //root height is the tallest child height + 1
  if (leftHeight > rightHeight){
    return leftHeight + 1;
  }
  else{
    return rightHeight + 1;
  }

}

int BST::NodeSize(BNode *itr){

  if (itr == nullptr){
    return 0;
  }
  
  else{
    return (NodeSize(itr -> _left) + 1 + NodeSize(itr -> _right));
  }

}

int BST::size() const {
  return ( _root == nullptr ? 0 : _root->_size );
}

int BST::height() const {
  return ( _root == nullptr ? -1 : _root->_height );
}

void BST::dump(bool verbose) const{
  inorder(_root, verbose);
}

BST::BNode* BST::inorder(BNode *itr, bool verbose) const{

  //if a tree exists
  if (_root != nullptr){

    //left child elements
    if (itr -> _left != nullptr){
      inorder(itr -> _left, verbose); //recursively call inorder with _left again
    }

    //root element
    if (verbose){
      cout << itr -> _key << "(" << itr -> _height << "," << itr -> _size << ") ";
    }
    else{
      cout << itr -> _data; //root data
    }

    //right child elements
    if (itr -> _right != nullptr){
      inorder(itr -> _right, verbose); //recursively call inorder with _right again
    } 
        
  }

  else{
    cout << "The tree is empty" << endl;
  }

  return itr;

}

//A sample balance function.  This implements the height-balanced
//property from AVL trees.
// bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize) {
//   return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
// }


//A sample main to exercise BST separately.  This is *NOT* a thorough
//test program

// int main() {
  
//   BST bst(imbalfn);

//   bst.insert("A", 100);
//   bst.insert("B", 110);
//   bst.insert("A", 90);
//   bst.insert("A", 80);
//   bst.insert("A", 70);
//   bst.insert("A", 60);
//   bst.insert("A", 111);
//   bst.insert("A", 112);
//   bst.insert("A", 85);
//   bst.insert("A", 86);
//   bst.insert("A", 35);
//   bst.insert("A", 45);

//   bst.insert("B", 2);
//   bst.insert("A", 1);

//   bst.insert("A", 5);
//   bst.insert("B", 10);
//   bst.insert("C", 15);
//   bst.insert("D", 20);
//   bst.insert("E", 30);
//   bst.insert("F", 40);
//   bst.insert("G", 55);
//   bst.insert("H", 60);
//   bst.insert("I", 65);
//   bst.insert("J", 70);
//   bst.insert("K", 75);
//   bst.insert("L", 80);

  



//   bst.dump();
//   cout << endl;
//   bst.dump(true);

  

  
//   cout << endl;
//   cout << "size = " << bst.size() << endl;
//   cout << "height = " << bst.height() << endl;

//   cout << "Self Assignment Test: " << endl;
//   bst = bst;

//   checks the copy constructor
//   cout << "copy test: " << endl;
//   BST bst2(bst);
//   bst.insert("3", 3);
//   bst.insert("13", 13);

//   bst2.dump(true);

//   checks the assignment
//   cout << "assignment test: " << endl;
//   BST bst3 = bst;
//   bst3.dump(true);

//   return 0;
// }
