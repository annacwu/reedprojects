#include <iostream>
#include <memory>
#include "tree.hpp"

using namespace std;

int main() {
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
    
    // Here is some sample code to get you started with your tests.
    // Uncomment this block once you have implemented the TreeNode constructor
    // It should generate the tree depicted above.
    TreeNode* a = new TreeNode(9, 0);
    TreeNode* b = new TreeNode(6, 0, a, nullptr);
    TreeNode* c = new TreeNode(12, 0);
    TreeNode* d = new TreeNode(-5, 0, c, b);
    TreeNode* e = new TreeNode(3, 0);
    TreeNode* f = new TreeNode(12, 0, e);
    TreeNode* root = new TreeNode(126, 0, d, f);

    std::string yes = root -> path_to(9);
    cout << yes << endl;
    TreeNode* g = root -> node_at("LRL");

    cout << "LRL node has key " << g -> get_key() << endl;
    root -> destroy_tree();
    

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

    
    // Uncomment this block once you have implemented the BSTNode insert() method
    // it should generate the tree depicted above.
    BSTNode* bstr = BSTNode::create_root(20, 0); // binary search tree root
    bstr->insert(8, 0);
    bstr->insert(22, 0);
    bstr->insert(12, 0);
    bstr->insert(4, 0);
    bstr->insert(10, 0);
    bstr->insert(14, 0);
    
    cout << "\n\n\n\n\n\n\n\n\n\n";
    bstr->insert(11, 0);

    BSTNode* min = bstr -> minimum();
    cout << "minimum value is " << min -> get_key() << endl;
    

    std::string str2 = bstr -> path_to(11);
    cout << str2 << endl;
    BSTNode* node1 = ((BSTNode*) bstr -> node_at(str2));
    cout << "grabbed node with value " << node1 -> get_key() << endl;
    BSTNode* node2 = ((BSTNode*)node1 -> successor());
    if (node2 != nullptr){
        cout << "node (11)'s successor has value " << node2 -> get_key() << endl;
    }
    else{
        cout << "node (11) has no successors\n";
    }
    




    bstr->destroy_tree();
    return 0;
}