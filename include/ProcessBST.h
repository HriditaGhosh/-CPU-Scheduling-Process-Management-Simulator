#ifndef PROCESS_BST_H
#define PROCESS_BST_H

#include <vector>

// ProcessBST.h  (Lab 9 : Binary Search Tree)
// -------------------------------------------------
// Manual BST keyed on Process ID. Stores, for each pid, the index of that
// process inside the master vector (kept in sync on Add/Delete), giving
// O(log n) average search by PID plus sorted traversal.

struct BSTNode {
    int pid = 0;
    int vectorIndex = -1;
    BSTNode *left = nullptr;
    BSTNode *right = nullptr;
};

struct ProcessBST {
    BSTNode *root = nullptr;
};

void insertBST(ProcessBST &t, int pid, int idx);
bool removeBST(ProcessBST &t, int pid);
int searchBST(const ProcessBST &t, int pid); // returns vector index, or -1
std::vector<int> inorderBST(const ProcessBST &t);
void clearBST(ProcessBST &t);

#endif
