// CSMC 341 - Spring 2020 - Project 3

// A balanced BST using array-based rebalancing and a user-provided
// imbalance function.

#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <string>
#include <vector>

// ********************************************
// Additional 'using' statements may be added.
// ********************************************

using std::string;
using std::cout;
using std::endl;

// Typedef for a balance function pointer.  A balance function returns
// true if the tree is **imbalanced** at a node.  This allows you to
// try different balance criteria.
typedef bool (*balfn_t)(int leftHeight, int rightHeight, int leftSize, int rightSize);

class BST { // Balanced BST class

public:
  
  class BNode{ // BST Node inner class
  public:
    BNode(string ch = "", int rank = -1, int height = 0, int size = 1);
    std::string _data;   // holds string data
    int _key;       // integer key value
    int _height;    // height of the node
    int _size;      // size of the subtree rooted at the node
    BNode* _right;  // right child pointer
    BNode* _left;   // left child pointer
  };

  //
  // Constructor, Copy Constructor, Assignment Operator, Destructor
  //

  // Constructor.  Must pass a pointer to a balanced function.
  BST(balfn_t imbalanced);
  
  // Copy constructor
  BST(const BST& rhs);

  // Assignment operator
  BST& operator=(const BST& rhs);

  // Destructor
  ~BST();                   

  //
  // Basic functions
  //
  
  // Insert (key, data) pair in the tree
  bool insert(string data, int key); 

  // Size of the tree (number of nodes)
  int size() ;

  // Height of the tree
  int height() const; 

  // Print the data using an inorder traversal; if verbose is 'true',
  // print the tree structure including sizes, heights, and
  // parentheses.
  void dump(bool verbose = false) const;

  
  
 private:
  BNode* _root;        // root of the tree
  balfn_t _imbalanced; // function to check for imbalance
  std::vector<BNode *> nodes;

  // ********************************************
  // Private function declarations go here!
  // ********************************************
  
  //helper function
  BNode* copy(BNode *ptr);
  void RemoveTree(BNode* ptr); 

  //calculating the root height and size
  int NodeHeight(BNode *itr); //checks the height of right and left subtrees
  int NodeSize(BNode *itr); //calculates the size of the tree

  //recursively inserts node
  BNode* rInsert(BNode *p, string ch, int rank);
  

  //rebalancing 
  std::vector<BNode*> nodes; //declaring a vector
  BNode* buildTree(BNode* root);
  void storeBSTNodes(BNode* root, std::vector<BNode*> &nodes);
  BNode* buildTreeUtil(std::vector<BNode*> &nodes, int start, int end);

  BNode* inorder(BNode *itr, bool verbose) const; //inorder traversal
  

};


#endif
