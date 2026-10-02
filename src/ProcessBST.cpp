#include "ProcessBST.h"
using namespace std;

// ---- internal recursive helpers (plain functions, not class members) ----
static BSTNode *insertHelper(BSTNode *node, int pid, int idx) {
    if (node == nullptr) {
        BSTNode *n = new BSTNode();
        n->pid = pid;
        n->vectorIndex = idx;
        return n;
    }
    if (pid < node->pid) node->left = insertHelper(node->left, pid, idx);
    else if (pid > node->pid) node->right = insertHelper(node->right, pid, idx);
    else node->vectorIndex = idx; // duplicate pid: refresh index
    return node;
}

static BSTNode *findMinNode(BSTNode *node) {
    while (node != nullptr && node->left != nullptr) node = node->left;
    return node;
}

static BSTNode *removeHelper(BSTNode *node, int pid, bool &removed) {
    if (node == nullptr) return nullptr;
    if (pid < node->pid) {
        node->left = removeHelper(node->left, pid, removed);
    } else if (pid > node->pid) {
        node->right = removeHelper(node->right, pid, removed);
    } else {
        removed = true;
        if (node->left == nullptr) {
            BSTNode *right = node->right;
            delete node;
            return right;
        } else if (node->right == nullptr) {
            BSTNode *left = node->left;
            delete node;
            return left;
        } else {
            BSTNode *successor = findMinNode(node->right);
            node->pid = successor->pid;
            node->vectorIndex = successor->vectorIndex;
            bool dummy = false;
            node->right = removeHelper(node->right, successor->pid, dummy);
        }
    }
    return node;
}

static void inorderHelper(BSTNode *node, vector<int> &out) {
    if (node == nullptr) return;
    inorderHelper(node->left, out);
    out.push_back(node->pid);
    inorderHelper(node->right, out);
}

static void destroyBST(BSTNode *node) {
    if (node == nullptr) return;
    destroyBST(node->left);
    destroyBST(node->right);
    delete node;
}

// ------------------------------------------------------------ public API --
void insertBST(ProcessBST &t, int pid, int idx) {
    t.root = insertHelper(t.root, pid, idx);
}

bool removeBST(ProcessBST &t, int pid) {
    bool removed = false;
    t.root = removeHelper(t.root, pid, removed);
    return removed;
}

int searchBST(const ProcessBST &t, int pid) {
    BSTNode *cur = t.root;
    while (cur != nullptr) {
        if (pid == cur->pid) return cur->vectorIndex;
        cur = (pid < cur->pid) ? cur->left : cur->right;
    }
    return -1;
}

vector<int> inorderBST(const ProcessBST &t) {
    vector<int> out;
    inorderHelper(t.root, out);
    return out;
}

void clearBST(ProcessBST &t) {
    destroyBST(t.root);
    t.root = nullptr;
}
