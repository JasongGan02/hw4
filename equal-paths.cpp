#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalPathsHelper(Node* root, int& depth, int currentDepth) {
    if (root == nullptr) {
        return true;
    }
    
    // If it's a leaf node
    if (root->left == nullptr && root->right == nullptr) {
        if (depth == -1) {
            depth = currentDepth;
            return true;
        }
        
        return depth == currentDepth;
    }
    
    
    return equalPathsHelper(root->left, depth, currentDepth + 1) && equalPathsHelper(root->right, depth, currentDepth + 1);
}

bool equalPaths(Node* root) {
    int depth = -1; // To store the depth of the first leaf node encountered
    return equalPathsHelper(root, depth, 0);
}


