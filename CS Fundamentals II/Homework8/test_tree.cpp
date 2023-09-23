#include <iostream>
#include <memory>
#include "tree.hpp"

using namespace std;

int main() {

  TreeNode* tree = new TreeNode(2,2);
  cout << "Key: " << tree -> get_key() << endl;
  cout << "Value: " << tree -> get_value() << endl;

  tree -> destroy_tree();

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

    // TreeNode* z = new TreeNode(11,0);
    // TreeNode* y = new TreeNode(4, 0);
    // TreeNode* x = new TreeNode(14, 0);
    // TreeNode* w = new TreeNode(10, 0, z);
    // TreeNode* v = new TreeNode(12, 0, w, x);
    // TreeNode* u = new TreeNode(22, 0);
    // TreeNode* t = new TreeNode(8, 0, y, v);
    // TreeNode* bstr = new TreeNode(20, 0, t, u);


    
    cout << "The path to 126: " << root -> path_to(126) << "\t expected: ''\n";
    cout << "The path to 6: " << root -> path_to(6) << "\t expected: LR\n";
    cout << "The path to -5: " << root -> path_to(-5) << "\t expected: L\n";
    cout << "The path to 12: " << root -> path_to(12) << "\t expected: LL\n";
    cout << "The path to 9: " << root -> path_to(9) << "\t expected: LRL\n";
    cout << "The path to 3: " << root -> path_to(3) << "\t expected: RL\n";
    cout << "The path to 20: " << root -> path_to(20) << "\t expected: -\n";
    cout << "\n";
    cout << "Node at path LRL: " << root -> node_at("LRL") -> get_key() << "\texpected: 9"<< endl;
    cout << "Node at path LR: " << root -> node_at("LR") -> get_key() << "\texpected: 6"<< endl;
    cout << "Node at path L: " << root -> node_at("L") -> get_key() << "\texpected: -5"<< endl;
    // cout << "Node at path RR: " << root -> node_at("RR") -> get_key() << "\texpected: error"<< endl;
    
    root -> destroy_tree();
    // bstr -> destroy_tree();


    /*
            20
           /   \
          /     \
         8       22
        / \     
      4    12   
          /  \
         10   14
          \
            11
           
    */

    // Uncomment this block once you have implemented the BSTNode insert() method
    // it should generate the tree depicted above.
    BSTNode* bstr = BSTNode::create_root(20, 0); // binary search tree root
    bstr->insert(8, 0);
    bstr->insert(22, 0);
    //bstr->insert(21, 0); only one that makes it work
    bstr->insert(12, 0);
    //bstr->insert(21, 0);
    bstr->insert(10, 0);
    //bstr->insert(21, 0);
    bstr->insert(14, 0);
    bstr->insert(4, 0);
    bstr->insert(11,0);
    //bstr->insert(21, 0);
    
    BSTNode* bstrmin = bstr -> minimum();
    BSTNode* bsttest = (BSTNode*) bstr -> node_at(bstr -> path_to(10));
    cout << "path to 11:" << bstr -> path_to(11) << endl;
    cout << "min: " << bstr -> minimum() -> get_key() << endl;
    cout << "successor to 4: " << bstrmin -> successor() -> get_key() << endl;
    cout << "successor to 20: " << bstr -> successor() -> get_key() << endl;\
    if (bsttest == nullptr){
      cout << "successor is nullptr" << endl;
    } else {
      cout << "successor to 10: " << bsttest -> successor() -> get_key() << endl;
    }

    bstr->destroy_tree();

    return 0;
}