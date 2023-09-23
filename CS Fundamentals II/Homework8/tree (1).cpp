#include "hw8.hpp"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Constructor for a TreeNode.
// By default, the tree is a leaf, so its children point to
// nullptr, but you may override with actual pointers to other nodes.
TreeNode::TreeNode(key_type k, value_type v, TreeNode* l, TreeNode* r){
    key_ = k;
    value_ = v;
    left_ = l;
    right_ = r;
}

// return key in current node
key_type TreeNode::get_key() const{
    return this -> key_;
} 

// return value in current node
value_type TreeNode::get_value() const{
    return this -> value_; // REPLACE WITH YOUR CODE
} 

  // get_child(dir):
  // if dir='L', return left child
  // if dir='R', return right child
  // note that dir needs to be a CHAR and not a STRING
  // if the indicated child is nullptr, return nullptr
  // if dir= any other char, return nullptr
TreeNode* TreeNode::get_child(char dir){
    
    // return left_ or right_ if R or L is input and the node exists, otherwise
    // return nullptr
    if (dir == 'L'){
        if (left_ != nullptr){
            return left_;
        }
        return nullptr;
        
    }
    else if (dir == 'R'){
        if (right_ != nullptr){
            return right_;
        }
        return nullptr;
    }
    
    // if the input char isn't R or L
    else {
        return nullptr;
    }
}
// Destructor for a TreeNode. 
// LEAVE AS IS
TreeNode::~TreeNode(){
    // std::cout << "node with value: " << this -> get_key() << " destroyed." << std::endl;
}



// deletes a heap-allocated tree
// and ALL of its children, children's children, etc
void TreeNode::destroy_tree() {
    if (this -> right_ != nullptr){
        right_ -> destroy_tree();
    }
    if(this -> left_ != nullptr){
        left_ -> destroy_tree();
    }
    
    delete this;   
}




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

std::string TreeNode::path_to_help(key_type k){
    // if the node would be to the left of current
    if (k < this -> key_){
        
        // if the next node exists
        if (this -> left_ != nullptr){
            return "L" + (this -> left_ -> path_to_help(k));
        }
        // otherwise, looking for next node where it doesn't exist:
        // node with that value is not in the tree
        else{
            return "X";
        }

    }
    // else if the node would be to the right of current
    else if (k > this -> key_){
        
        // if the next node exists
        if (this -> right_ != nullptr){
            return "R" + (this -> right_ -> path_to_help(k));
        }
        // otherwise, looking for next node where it doesn't exist:
        // node with that value is not in the tree
        else{
            return "X";
        }
    }
    // otherwise, we are on the node with value k
    else{
        return "";
    }
}

std::string TreeNode::path_to(key_type k){
    std::string str;
    // if the key is the root, return empty string
    if (this -> key_ == k){
        str = "";
    }
    
    // otherwise string = path_to_help()
    else{
        str = this -> path_to_help(k);
    }

    // loop through the string, if X is found that means the 
    // node isn't in the tree and the string becomes "-"
    for(int i = 0; i < str.length(); i++){
        if (str[i] == 'X'){
            str = "-";
        }
    }
    return str;
}

// 
//////////////////////////////////////////////////////////////////////////////
// node_at: Follow a path from the current TreeNode and return the node that is
// at the end of the path. For example, for the root of the tree above,
// root.node_at("LR") will return a pointer to the TreeNode whose key is 6.
// If the path leads to an invalid or empty child, or contains any character
// other than 'L' or 'R', return nullptr (don't crash)

// hint: recall that in C++, 'this' gives a pointer to the current object
TreeNode* TreeNode::node_at(std::string path){
    TreeNode* current = this;
    for (int i = 0; i < path.length(); i++){
        if (path[i] == 'L'){
            current = current -> left_;
        }
        else if (path[i] == 'R'){
            current = current -> right_;
        }
    }
    return current;
}



// IMPORTANT NOTE ABOUT THE REMAINING PROBLEMS:
// This material is important to learn, but it is not worth an excessive amount of suffering.
// I am making these problems worth only 10% of the grade of this homework in total,
// with the intention that if you are not a CS major and you are short for time, you will consider skipping them.
// Even if you ARE a CS major, if this part of the assignment is feeling overwhelming,
// PLEASE skip it and simply promise me that you'll learn it over the summer.


// hint:
// if you get an error like this while implementing BSTNode:
// error: no member named 'insert_help' in 'TreeNode'
//        return left_->insert_help();
// 
// Try casting the TreeNode pointer to a BSTNode pointer
// e.g. we would replace the above line with:
// return ((BSTNode*) left_)->insert_help();


// Constructor for BSTNodes
// This will NOT preserve the ordering of the BST
// Only use as a helper function for insert().
// Modify the variables of the new node inside insert()
// in order to attach it to the tree and preserve the ordering.
// LEAVE THIS CODE AS IS
BSTNode::BSTNode(key_type k, value_type v) : TreeNode(k,v) {
    parent_ = nullptr;
}

// static method for creating the root of a BST
// use it for your tests
// LEAVE THIS CODE AS IS
BSTNode* BSTNode::create_root(key_type k, value_type v) {
    BSTNode* root = new BSTNode(k, v);
    return root;
}

// optional: consider using a recursive helper function with this type signature
// to help implement insert().
void BSTNode::insert_help(key_type k, value_type v, BSTNode* new_node) {
    
    // if the new node is less than the current node (will end up to the left
    // of it)
    if (new_node -> key_ < this -> key_){
        cout << "left IH\n";
        
        // if there's a node there, recurse on it
        if (left_ != nullptr){
            cout << "recursed left IH\n";
            ((BSTNode*) left_) -> insert_help(k,v,new_node);
        }
        
        // once the bottom has been reached, add a new node and assign its parent
        // to the current node
        else{
            cout << "node added left IH\n";
            left_ = new_node;
            ((BSTNode*) new_node) -> parent_ = this;
        }
        
    }
    
    // otherwise, the new node is greater than the current node (ends up to the right
    // of it)
    else{
        
        // if there's a node there, recurse on it
        if (right_ != nullptr){
            cout << "recursed right IH\n";
            ((BSTNode*) right_) -> insert_help(k,v,new_node);
        }
        
        // once the bottom has been reached, add a new node and assign its parent to 
        // the current node
        else{
            cout << "node added right IH\n";
            right_ = new_node;
            ((BSTNode*) new_node) -> parent_ = this;
        }
    }
}


void BSTNode::insert(key_type k, value_type v) {
    BSTNode* new_node = new BSTNode(k,v);
    
    // if newnode < current (to the left)
    if (new_node -> key_ < this -> key_){
        cout << "left I\n";
        // if there is a node there, insert_help() on it
        if (left_ != nullptr){
            cout << "recursed left I\n";
            ((BSTNode*)left_) -> insert_help(k,v,new_node);
        }
        
        // otherwise it becomes the new node, with current node as its parent
        else{
            cout << "added node left I\n";
            left_ = new_node;
            ((BSTNode*) new_node) -> parent_ = this;
        }
        
    }
    
    // otherwise, the node >= the current node (to the right)
    // there will never be a node added with the same value as 
    // another node but this accounts for that just in case
    else{
        cout << "right I\n";
        // if there is a node to the right, insert_help() on it
        
        
        if (right_ != nullptr){
            cout << "recurse right I \n";
            ((BSTNode*)right_) -> insert_help(k,v,new_node);
        }
        
        // otherwise it becomes the new node, with current node as its parent
        else{
            cout << "added node right I\n";
            right_ = new_node;
            ((BSTNode*) new_node) -> parent_ = this;
        }
    }
}


    /*
            20
           /   \
          /     \
         8       22
        / \     
      4    12   
          /  \
         10   14
    */

// return a pointer to the node with the minimum key
// in this node's subtree
// for example, if the root node in the above BST is r,
// r->minimum() should return 4
// if the node containing 12 is a,
// then a->minimum() should return 10.


BSTNode* BSTNode::minimum() {
    // set current node to make things easier
    BSTNode* current = this;
    
    // go all the way down the leftmost branch 
    while (current -> left_ != nullptr){
        current = ((BSTNode*)current -> left_);
    }
    return current;
}

// OPTIONAL (but encouraged, especially for CS majors):
// return a pointer to the node with the smallest key that's greater
// than the current node. If no node in the BST is greater than the current node,
// return nullptr.
// Going back to the BST drawn above, the successor of the node
// containing 4 is (the node containing) 8. The successor of
// 8 is 10, and the successor of 14 is 20. 

BSTNode* BSTNode::successor() {
    // set current node to make things easier
    BSTNode* current = this;
    
    // if the node has a subtree greater than it
    if (current -> right_ != nullptr){
        // go to its first right value
        current = ((BSTNode*)current -> right_);

        // then all the way down the consequent left values 
        while (current -> left_ != nullptr){
            current = ((BSTNode*)current -> left_);
        }
        
        // the bottom node will be the lowest value in the subtree of
        // nodes larger than the original, also known as its successor
        return current;
    }
    // otherwise, the node is the last in a line
    else{
        // move up the tree as long as the node is larger than its parent;
        // while loop is set up this way to avoid seg faults
        while (current -> parent_ != nullptr){
            
            // jump up if node is the right child of its parent
            if (current == ((BSTNode*) current -> parent_ -> right_)){
                current = ((BSTNode*) current -> parent_);
            }
            // otherwise, get out of the loop
            else{
                break;
            }
        }
        
        // if the node doesn't have a parent, the original node is the farthest right in the 
        // tree, meaning it has the highest value. returns nullptr
        if (((BSTNode*)current -> parent_) == nullptr){
            return nullptr;
        }
        
        // otherwise, the current node's parent is the first node with higher value than the 
        // original, also known as its successor
        else{
            current = ((BSTNode*) current -> parent_);
            return current;
        }
    }
}