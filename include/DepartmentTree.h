#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    DepartmentNode(std::string name, DepartmentNode* parent = nullptr)
        : name(name), parent(parent) {}
};

class DepartmentTree {
private:
    DepartmentNode* root;

    void clearTree(DepartmentNode* node);
    void preorderHelper(DepartmentNode* node, std::vector<std::string>& seq) const;
    void postorderHelper(DepartmentNode* node, std::vector<std::string>& seq) const;
    DepartmentNode* searchHelper(DepartmentNode* node, const std::string& name) const;
    int calculateHeight(DepartmentNode* node) const;
    void countLeaves(DepartmentNode* node, int& leafCount) const;

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const;
    bool addDepartment(const std::string& name, const std::string& parentName);
    DepartmentNode* search(const std::string& name) const;
    
    void displayHierarchy() const;
    void displayHierarchyHelper(DepartmentNode* node, int depth) const;

    void preorderTraversal() const;
    void postorderTraversal() const;
    void levelOrderTraversal() const;

    void displayStatistics() const;
    int getTotalDepartments() const;
    int getLeafCount() const;
    int getHeight() const;
};

#endif
