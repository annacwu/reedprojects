#include "tree.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

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
    return this -> value_; 
} 

  // get_child(dir):
  // if dir='L', return left child
  // if dir='R', return right child
  // note that dir needs to be a CHAR and not a STRING
  // if the indicated child is nullptr, return nullptr
  // if dir= any other char, return nullptr
TreeNode* TreeNode::get_child(char dir){
    if (dir == 'L'){
        return this -> left_;
    } else if (dir == 'R') {
        return this -> right_;
    } else {
        return nullptr; 
    }
}

// Destructor for a TreeNode. 
// LEAVE AS IS
TreeNode::~TreeNode(){
    std::cout << "node with value: " << value_ << " destroyed." << std::endl;
}



// deletes a heap-allocated tree
// and ALL of its children, children's children, etc
void TreeNode::destroy_tree() {
    if (this -> left_ == nullptr && this -> right_ == nullptr){
        delete this;
    } else {
        if (this -> left_ != nullptr){
            this -> left_ -> destroy_tree();
        } 
        if (this -> right_ != nullptr){
            this -> right_ -> destroy_tree();
        }
    }
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
bool checkL(std::string str){
    for (int i = 0; i < str.length(); i++){
        if (str[i] == 'L' || str == ""){
            return true;
        }
    }
    return false;
}

bool checkString(std::vector<std::string> arr, std::string string){
    for (int i = 0; i < arr.size(); i++){
        if (arr[i] == string){
            return false;
        }
    }
    return true;
}

std::string TreeNode::path_to(key_type k){
    std::string referencestr = "";
    std::string newstr = "";
    TreeNode* curr = this;

    std::vector<std::string> arr;

    // go left all the way
    while (curr -> left_ != nullptr){
        if (curr -> key_ == k){
            return newstr;
        }
        curr = curr -> left_;
        newstr += 'L';
    } 
    if (curr -> key_ == k){
        return newstr;
    }

    arr.push_back(referencestr);
    referencestr = newstr;
    newstr = "";
    // while we haven't traversed the whole tree
    while (checkL(referencestr)){
        curr = this; 
        for (int i = 0; i < referencestr.length() - 1; i++){ // follow refernece string
            if(referencestr[i] == 'R'){ 
                curr = curr -> right_;
                newstr += 'R';
            } else {
            curr = curr -> left_;
            newstr += 'L';
            }
        }
        // if right exists and we haven't already done that
        if (curr -> right_ != nullptr && checkString(arr, referencestr)) { // go right isntead of left
            curr = curr -> right_;
            newstr += 'R';
            if (curr -> key_ == k){
                return newstr;
            }
            // go left again
            while (curr -> left_ != nullptr){
                if (curr -> key_ == k){
                    return newstr;
                }
                curr = curr -> left_;
                newstr += 'L';
            } 
            if (curr -> key_ == k){
                return newstr;
            }
        }
        arr.push_back(referencestr);
        referencestr = newstr;
        newstr = "";
    }
    return "-";
}

// WE COULD'VE DONE IT RECURSIVELY but we didn't know if we could add another parameter to the header since we weren't supposed to edit it
// std::string path_too(key_type k, std::string str, TreeNode* node){
    // std::string str = "";
    // str = path_too(k, str, this);
    // return str;
//     if (node -> key_ == k){
//         return str;
//     } else if (node -> left_ != nullptr){
//         *str += 'L';
//         node -> path_to(k);
//     } else if (node -> right_ != nullptr){
//         *str += 'R';
//         node -> path_to(k);
//     } else if (node -> right_ == nullptr && node -> left_ == nullptr){
//         *str = "";
//     }
//     return "-"; 
// }

//////////////////////////////////////////////////////////////////////////////
// node_at: Follow a path from the current TreeNode and return the node that is
// at the end of the path. For example, for the root of the tree above,
// root.node_at("LR") will return a pointer to the TreeNode whose key is 6.
// If the path leads to an invalid or empty child, or contains any character
// other than 'L' or 'R', return nullptr (don't crash)

// hint: recall that in C++, 'this' gives a pointer to the current object
TreeNode* TreeNode::node_at(std::string path){
    TreeNode* curr = this; 
    for (int i = 0; i < path.length(); i++){
        if (path[i] == 'R' && curr -> right_ != nullptr){
            curr = curr -> right_;
        } else if (path[i] == 'L' && curr -> left_ != nullptr){
            curr = curr -> left_;
        } else {
            return nullptr;
        }
    }
    return curr; 
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
    return;
}


void BSTNode::insert(key_type k, value_type v) {
    BSTNode* curr = this;

    // for first node
    if (curr -> left_ == nullptr && curr -> right_ == nullptr){
            if (k > curr -> get_key()){
                // make node to right
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> right_ = newNode;
                // traverse to that node to leave loop
                curr = (BSTNode*) curr -> right_;
            } else {
                // make node to left
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> left_ = newNode;
                // traverse to that node to leave loop
                curr = (BSTNode*) curr -> left_;
            }
        }


    while (curr -> left_ != nullptr || curr -> right_ != nullptr){
        while (curr -> left_ != nullptr && curr -> right_ != nullptr){
            if (k < curr -> left_ -> get_key()){
                curr = (BSTNode*) curr -> left_;
            } else {
                curr = (BSTNode*) curr -> right_;
            }
        }

        if (curr -> left_ != nullptr){
            if (k < curr -> left_ -> get_key()){
                curr = (BSTNode*) curr -> left_;
            } else {
                // make new node to right
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> right_ = newNode;
                // traverse to that node to leave loop
                // curr = (BSTNode*) curr -> right_;
                break;
            }
        } else if (curr -> right_ != nullptr){
            if (k > curr -> right_ -> get_key()){
                curr = (BSTNode*) curr -> right_;
            } else {
                // make new node to left?
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> left_ = newNode;
                // traverse to that node to leave loop
                // curr = (BSTNode*) curr -> left_;
                break;
            }
        } else if (curr -> left_ == nullptr && curr -> right_ == nullptr){
            if (k > curr -> get_key()){
                // make node to right
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> right_ = newNode;
                // traverse to that node to leave loop
                // curr = (BSTNode*) curr -> right_;
                break;
            } else {
                // make node to left
                BSTNode* newNode = new BSTNode(k, v);
                newNode -> parent_ = (BSTNode*) curr;
                curr -> left_ = newNode;
                // traverse to that node to leave loop
                // curr = (BSTNode*) curr -> left_;
                break;
            }
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
    BSTNode* curr = this;
    while (curr -> left_ != nullptr){
        curr = (BSTNode*) curr -> left_;
    }
    return curr; 
}

// OPTIONAL (but encouraged, especially for CS majors):
// return a pointer to the node with the smallest key that's greater
// than the current node. If no node in the BST is greater than the current node,
// return nullptr.
// Going back to the BST drawn above, the successor of the node
// containing 4 is (the node containing) 8. The successor of
// 8 is 10, and the successor of 14 is 20. 
bool greaterThan (BSTNode* current, BSTNode* great, int k){
    if (current -> get_key() < great -> get_key() && current -> get_key() > k){
        return true;
    }
    return false;
}

BSTNode* BSTNode::successor() {
    BSTNode* curr = this;
    int k = curr -> key_;
    // get to root
    while (curr -> parent_ != nullptr){
        curr = (BSTNode*) curr -> parent_;
    }
    // set greater to the root
    BSTNode* greater = (BSTNode*) curr;
    if (k == curr -> key_ && curr -> right_ != nullptr){
        curr = (BSTNode*) curr -> right_;
        greater = (BSTNode*) curr;
    } else if (k == curr -> key_) {
        return nullptr;
    }
    // edge case: given a root only
    if (curr -> left_ == nullptr && curr -> right_ == nullptr){
        return nullptr;
        }
    // traverse the tree towards where it would be inserted
    while (curr -> left_ != nullptr || curr -> right_ != nullptr){
        while (curr -> left_ != nullptr && curr -> right_ != nullptr){
            if (k < curr -> key_ ){
                if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
                }
                curr = (BSTNode*) curr -> left_;
            } else {
                if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
                }
                curr = (BSTNode*) curr -> right_;
            }
        }

        // traverse to direction of insert
        if (curr -> left_ != nullptr){
            if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
            }
            curr = (BSTNode*) curr -> left_;
            if (curr -> left_ == nullptr && curr -> right_ == nullptr){
                if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
            }
                return greater;
            }
        } else if (curr -> right_ != nullptr){
            if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
            }
            curr = (BSTNode*) curr -> right_;
            if (curr -> left_ == nullptr && curr -> right_ == nullptr){
                if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
            }
                return greater;
            }
        } else if (curr -> left_ == nullptr && curr -> right_ == nullptr){
            if (greaterThan(curr,greater,k)){
                    greater = (BSTNode*) curr;
            }
            return greater;
        } else {
            std::cout << "Error." << std::endl;
        }
    }
    return nullptr; 
}