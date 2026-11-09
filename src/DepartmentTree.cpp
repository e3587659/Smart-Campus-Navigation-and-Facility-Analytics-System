#include "DepartmentTree.h"
#include "FileHelp.h"
#include <fstream>
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

static std::string normalizeString(const std::string& s) {
    return toLowerStr(trim(s));
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

std::string DepartmentTree::addDepartment(const std::string& rawName, const std::string& rawParent) {
    std::string name = trim(rawName);
    std::string parentName = trim(rawParent);
    if (name.empty()) return "empty department name";

    if (search(name) != nullptr) return "duplicate department '" + name + "'";

    if (isEmpty()) {
        root = new DepartmentNode(name, nullptr);
        return "";
    }

    if (parentName.empty()) return "empty parent name";
    DepartmentNode* parentNode = search(parentName);
    if (!parentNode) return "unknown parent '" + parentName + "'";

    DepartmentNode* newNode = new DepartmentNode(name, parentNode);
    parentNode->children.push_back(newNode);
    return "";
}

void DepartmentTree::loadFromFile(const std::string& path, const std::string& fileLabel) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error: cannot open " << path << ". Treating it as empty.\n";
        std::cout << "Loaded " << fileLabel << ": 0 accepted, 0 rejected\n";
        return;
    }

    int accepted = 0, rejected = 0, lineNumber = 0;
    std::string line;
    while (std::getline(in, line)) {
        lineNumber++;
        std::string t = trim(line);
        if (isSkippableLine(t)) continue;

        std::vector<std::string> f = splitPipe(t);
        std::string reason;
        if (f.size() != 2) {
            reason = "expected 2 fields but found " + std::to_string(f.size());
        } else if (f[0].empty() || f[1].empty()) {
            reason = "empty required field";
        } else if (equalsIgnoreCase(f[1], "NONE")) {
            if (!isEmpty()) reason = "root already exists; '" + f[0] + "' needs a parent";
            else reason = addDepartment(f[0], "");
        } else if (isEmpty()) {
            reason = "unknown parent '" + f[1] + "'";
        } else {
            reason = addDepartment(f[0], f[1]);
        }

        if (reason.empty()) {
            accepted++;
        } else {
            printWarning(fileLabel, lineNumber, reason);
            rejected++;
        }
    }
    std::cout << "Loaded " << fileLabel << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

int DepartmentTree::getDepth(const DepartmentNode* node) const {
    int depth = 0;
    while (node != nullptr && node->parent != nullptr) {
        depth++;
        node = node->parent;
    }
    return depth;
}

void DepartmentTree::displaySearchResult(const std::string& name) const {
    DepartmentNode* node = search(name);
    if (!node) {
        std::cout << "Department not found: " << trim(name) << "\n";
        return;
    }
    std::cout << "Department: " << node->name << "\n";
    std::cout << "Parent: " << (node->parent ? node->parent->name : "None") << "\n";
    std::cout << "Direct children: " << node->children.size() << "\n";
    std::cout << "Depth: " << getDepth(node) << "\n";
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
        std::cout << seq[i] << "\n";
    }
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
        std::cout << seq[i] << "\n";
    }
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
        std::cout << seq[i] << "\n";
    }
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
