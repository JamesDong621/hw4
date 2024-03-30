#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
<<<<<<< HEAD
#include <utility> 
#include <algorithm>
=======

>>>>>>> 63b6c558d86310b9152efd250b57bb68e055ebba
#endif

#include "equal-paths.h"
using namespace std;

<<<<<<< HEAD
// Helper function to check if all paths are of equal length
std::pair<bool, int> checkEqualPaths(Node *node) {
    if (!node) return {true, 0};

    auto leftResult = checkEqualPaths(node->left);
    auto rightResult = checkEqualPaths(node->right);

    // Change starts here
    bool isBalanced = true;
    int depth = 0;

    if (node->left && node->right) {
        isBalanced = leftResult.first && rightResult.first && (leftResult.second == rightResult.second);
        depth = max(leftResult.second, rightResult.second) + 1;
    } else if (node->left) {
        isBalanced = leftResult.first;
        depth = leftResult.second + 1;
    } else if (node->right) {
        isBalanced = rightResult.first;
        depth = rightResult.second + 1;
    }
    // Change ends here

    return {isBalanced, depth};
}
=======

// You may add any prototypes of helper functions here
>>>>>>> 63b6c558d86310b9152efd250b57bb68e055ebba


bool equalPaths(Node * root)
{
<<<<<<< HEAD
    if (!root) return true; // An empty tree has equal paths by definition
    auto result = checkEqualPaths(root);
    return result.first;
}
=======
    // Add your code below

}

>>>>>>> 63b6c558d86310b9152efd250b57bb68e055ebba
