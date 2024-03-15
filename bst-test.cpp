#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    bt.insert(std::make_pair('c',3));
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }

    std::cout << "Before any removals, tree is: " << std::endl;
	bt.print();

    bt.remove('a');
    std::cout << "After removing a, tree is: " << std::endl;
	bt.print();
    // AVL Tree Tests
    AVLTree<int,float> at;
    at.insert(std::make_pair(5, 1.0));
	at.insert(std::make_pair(6, 1.0));
	at.insert(std::make_pair(3, 1.0));
	at.insert(std::make_pair(4, 1.0));
    at.print();
    cout << "Remove 6" << endl;
	at.remove(6);
    at.print();
    return 0;
}
