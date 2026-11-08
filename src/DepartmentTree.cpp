#include "DepartmentTree.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <cctype>

DepartmentTree::DepartmentTree() : root(nullptr) {}

DepartmentTree::~DepartmentTree() {
    clearTree(root);
}

void DepartmentTree::clearTree(DepartmentNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        clearTree(child);
    }
    delete node;
}

bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

std::string normalizeString(const std::string& s) {
    std::string res = "";
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            res += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return res;
}

DepartmentNode* DepartmentTree::searchHelper(DepartmentNode* node, const std::string& name) const {
    if (!node) return nullptr;
    if (normalizeString(node->name) == normalizeString(name)) {
        return node;
    }
    for (auto child : node->children) {
        DepartmentNode* found = searchHelper(child, name);
        if (found) return found;
    }
    return nullptr;
}

DepartmentNode* DepartmentTree::search(const std::string& name) const {
    return searchHelper(root, name);
}

bool DepartmentTree::addDepartment(const std::string& name, const std::string& parentName) {
    if (name.empty()) return false;
    
    // Check if name already exists
    if (search(name) != nullptr) return false;

    if (isEmpty()) {
        root = new DepartmentNode(name, nullptr);
        return true;
    }

    DepartmentNode* parentNode = search(parentName);
    if (!parentNode) return false;

    DepartmentNode* newNode = new DepartmentNode(name, parentNode);
    parentNode->children.push_back(newNode);
    return true;
}

void DepartmentTree::displayHierarchyHelper(DepartmentNode* node, int depth) const {
    if (!node) return;
    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << node->name << "\n";
    for (auto child : node->children) {
        displayHierarchyHelper(child, depth + 1);
    }
}

void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) {
        std::cout << "The department tree is empty.\n";
        return;
    }
    displayHierarchyHelper(root, 0);
}

void DepartmentTree::preorderHelper(DepartmentNode* node, std::vector<std::string>& seq) const {
    if (!node) return;
    seq.push_back(node->name);
    for (auto child : node->children) {
        preorderHelper(child, seq);
    }
}

void DepartmentTree::preorderTraversal() const {
    if (isEmpty()) {
        std::cout << "The department tree is empty.\n";
        return;
    }
    std::vector<std::string> seq;
    preorderHelper(root, seq);
    for (size_t i = 0; i < seq.size(); ++i) {
        std::cout << seq[i] << (i + 1 < seq.size() ? " " : "");
    }
    std::cout << "\n";
}

void DepartmentTree::postorderHelper(DepartmentNode* node, std::vector<std::string>& seq) const {
    if (!node) return;
    for (auto child : node->children) {
        postorderHelper(child, seq);
    }
    seq.push_back(node->name);
}

void DepartmentTree::postorderTraversal() const {
    if (isEmpty()) {
        std::cout << "The department tree is empty.\n";
        return;
    }
    std::vector<std::string> seq;
    postorderHelper(root, seq);
    for (size_t i = 0; i < seq.size(); ++i) {
        std::cout << seq[i] << (i + 1 < seq.size() ? " " : "");
    }
    std::cout << "\n";
}

void DepartmentTree::levelOrderTraversal() const {
    if (isEmpty()) {
        std::cout << "The department tree is empty.\n";
        return;
    }
    std::vector<std::string> seq;
    std::queue<DepartmentNode*> q;
    q.push(root);

    while (!q.empty()) {
        DepartmentNode* curr = q.front();
        q.pop();
        seq.push_back(curr->name);
        for (auto child : curr->children) {
            q.push(child);
        }
    }

    for (size_t i = 0; i < seq.size(); ++i) {
        std::cout << seq[i] << (i + 1 < seq.size() ? " " : "");
    }
    std::cout << "\n";
}

int DepartmentTree::calculateHeight(DepartmentNode* node) const {
    if (!node || node->children.empty()) return 0;
    int maxHeight = 0;
    for (auto child : node->children) {
        int h = calculateHeight(child);
        if (h > maxHeight) maxHeight = h;
    }
    return 1 + maxHeight;
}

void DepartmentTree::countLeaves(DepartmentNode* node, int& leafCount) const {
    if (!node) return;
    if (node->children.empty()) {
        leafCount++;
        return;
    }
    for (auto child : node->children) {
        countLeaves(child, leafCount);
    }
}

int DepartmentTree::getTotalDepartments() const {
    if (isEmpty()) return 0;
    std::vector<std::string> seq;
    preorderHelper(root, seq);
    return static_cast<int>(seq.size());
}

int DepartmentTree::getLeafCount() const {
    if (isEmpty()) return 0;
    int count = 0;
    countLeaves(root, count);
    return count;
}

int DepartmentTree::getHeight() const {
    if (isEmpty()) return -1;
    return calculateHeight(root);
}

void DepartmentTree::displayStatistics() const {
    if (isEmpty()) {
        std::cout << "No departments are loaded.\n";
        std::cout << "Total departments: 0\nLeaf departments: 0\nTree height: N/A\n";
        return;
    }
    std::cout << "Total departments: " << getTotalDepartments() << "\n";
    std::cout << "Leaf departments: " << getLeafCount() << "\n";
    std::cout << "Tree height: " << getHeight() << "\n";
}
