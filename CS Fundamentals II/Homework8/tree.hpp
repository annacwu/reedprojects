/*
 * Tree: A simple recursive data structure to hold keys and values.
 */

#pragma once

#include <string>
#include <memory>

using key_type = int;
using value_type = uint64_t;



class TreeNode {
protected:
  key_type key_;
  value_type value_;
  TreeNode* left_;
  TreeNode* right_;

public:

// Constructor for a TreeNode.
// By default, the tree is a leaf, so its children point to
// nullptr, but you may override with actual pointers to other nodes.
  TreeNode(key_type k, value_type v, TreeNode* l=nullptr, TreeNode* r=nullptr);

// Destructor for a TreeNode. 
  ~TreeNode(); 

// deletes a heap-allocated tree
// and ALL of its children, children's children etc
  void destroy_tree();

  key_type get_key() const; // return key in current node
  value_type get_value() const; // return value in current node

  // get_child(dir):
  // if dir='L', return left child
  // if dir='R', return right child
  // note that dir needs to be a CHAR and not a STRING
  // if the indicated child is nullptr, return nullptr
  // if dir= any other char, return nullptr
  TreeNode* get_child(char dir);

  //////////////////////////////////////////////////////////////////////////////
// path_to: return a string representing the path taken from a given tree root
// to a given key in the tree. For each left child taken along the path, the
// string contains an 'L' character, and 'R' for a right child, in order.
// So for example for the following tree (only keys depicted, values omitted):
/*
            126
           /   \
          /     \
        -5       12
        / \     /
      12   6   3
          /
         9
*/
// path_to(tree, 9) will return "LRL", path_to(tree, 3) will return "RL", and
// path_to(126) will return "".
//
// If the key isn't found in the tree, path_to should return the string "-".
//
// If multiple matches to key exist in the tree, return the path to the
// leftmost match. For the tree above, path_to(tree, 12) returns "LL".
  std::string path_to(key_type k);

  // partial credit version of path_to:
  // if you're having trouble with path_to,
  // try writing find_ptr, which returns a pointer
  // to a node with the key k
  // if no node in the tree has key k, return nullptr
  TreeNode* find_ptr(key_type k);

//////////////////////////////////////////////////////////////////////////////
// node_at: Follow a path from the current TreeNode and return the node that is
// at the end of the path. For example, for the root of the tree above,
// root.node_at("LR") will return a pointer to the TreeNode whose key is 6.
// If the path leads to an invalid or empty child, or contains any character
// other than 'L' or 'R', return nullptr (don't crash)
  TreeNode* node_at(std::string path);
};


// IMPORTANT NOTE ABOUT THE REMAINING PROBLEMS:
// This material is important to learn, but it is not worth an excessive amount of suffering.
// I am making these problems worth only 10% of the grade of this homework in total,
// with the intention that if you are not a CS major and you are short for time, you will consider skipping them.
// Even if you ARE a CS major, if this part of the assignment is feeling overwhelming,
// PLEASE get help, or just skip it and just be sure to learn it after the class concludes.
class BSTNode : public TreeNode {
private:
  BSTNode* parent_;
  BSTNode(key_type k, value_type v);
  
  // optional: consider using a recursive helper function with this
  // type signature to help implement insert().
  void insert_help(key_type k, value_type v, BSTNode* new_node);

public:
  static BSTNode* create_root(key_type k, value_type v);
  
  // add a node to the BST while preserving the ordering property:
  // For a given node n, all nodes in n's left subtree have key_ < n.key_
  // All nodes in n's right subtree have key_ > n.key_
  // You don't need to worry about cases where more than one node has the same key.
  void insert(key_type k, value_type v);
  
  // return a pointer to the node with the minimum key
  // in this node's subtree
  BSTNode* minimum(); 

  // OPTIONAL (but encouraged, especially for CS majors):
  // return a pointer to the node with the smallest key that's greater
  // than the current node. If no node in the BST is greater than the current node,
  // return nullptr.
  BSTNode* successor();
};
