//
// Created by spl211 on 03/11/2020.
//

#include "Session.h"
#include "Tree.h"
#include <vector>
#include <iostream>


// ******************** Constructor ********************

Tree:: Tree(int rootLabel) : node(rootLabel), children(vector<Tree*>()){}


// ******************** rule of 5 ********************

//destructor
Tree::~Tree() {
    Tree::clear();
}

//copy constructor
Tree::Tree(const Tree &other) : node(other.node), children(vector<Tree*>(other.children.size())){
    for (unsigned int i = 0;!(other.children.empty()) && i < other.children.size(); i++) {
        Tree *otherChild = other.children[i];
        children.push_back(otherChild -> clone());
    }
}

// copy assignment
Tree &Tree::operator=(const Tree &other) {
    if (this != &other) {
        this->clear();
        for (unsigned int i = 0; i < other.children.size(); i++) {
            Tree *otherChild = other.children[i];
            children.push_back(otherChild ->clone());
        }
        node = other.node;
    }
    return *this;
}

//move constructor
Tree::Tree(Tree &&other) : node(move(other.node)), children(move(vector<Tree*>())) {
    for (auto & i : other.children)
        i = nullptr;
}

// move assignment
Tree &Tree::operator=(Tree &&other) {
    if (this != &other) {
        this->clear();
        node = other.node;

        for (auto & i : other.children) {
            children.push_back(i);
            i = nullptr;
        }
        other.children.clear();
    }
    return *this;
}

void Tree::clear() {
    for (auto &child : children) {
        if (child) {
            delete child;
            child = nullptr;
        }
    }
}


// ******************** Clones & creation ********************

Tree* Tree::createTree(const Session &session, int rootLabel) {

    TreeType treeType = session.getTreeType();
    Tree *tree;

    if (treeType == MaxRank) {
        tree = new MaxRankTree(rootLabel);
    } else if (treeType == Cycle) {
        tree = new CycleTree(rootLabel,session.getCycleNumber());
    } else {
        tree = new RootTree(rootLabel);
    }

    return tree;
}

RootTree* RootTree:: clone () const  {
    return new RootTree(*this);
}

CycleTree* CycleTree:: clone () const {
    return new CycleTree(*this);
}

MaxRankTree* MaxRankTree:: clone () const {
    return new MaxRankTree(*this);
}


// ******************** Operate on members ********************

int Tree::getNodeInd() {
    return node;
}

vector <Tree *>  &Tree::getChildren() {
    return children;
}


void Tree::addChild(const Tree &child) {
    children.push_back(child.clone());
}


// ******************** Root tree ********************

RootTree::RootTree(int rootLabel) : Tree(rootLabel) {}

int RootTree::traceTree() {
    return getNodeInd();
}


// ******************** Cycle tree ********************

CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel) , currCycle(currCycle){}

int CycleTree::traceTree() {
    vector<Tree *> myChildren = this->getChildren();
    Tree *tree = this;

    // go "currCycle" number of times left, or as many as possible
    // if the root does not have any children, return root
    for (int i = 0; !(tree->getChildren().empty()) && i < currCycle; i++) {
        tree = myChildren[0];
        myChildren = tree->getChildren();
    }
    return tree->getNodeInd();
}


// ******************** MaxRank tree ********************

MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {}

int MaxRankTree::traceTree() {

    int maxNumOfChildren = -1;
    int toReturn = -1;

    vector<Tree*> q;
    q.push_back(this);

    // traversing the tree by level. the first node with the HIGHEST number of children will have also the least depth and is leftMost
    while (!q.empty()) {
        Tree* currNode = q[0];
        q.erase(q.begin());
        vector<Tree*> currNodeChildren = currNode->getChildren();
        int currNumOfChildren = (currNode->getChildren()).size();

        if (maxNumOfChildren < currNumOfChildren) {
            maxNumOfChildren = currNumOfChildren;
            toReturn = currNode->getNodeInd();
        }
        // add curr nodes children to queue
        for (int i = 0; i < currNumOfChildren; i++)
            q.push_back(currNodeChildren[i]);
    }
    return toReturn;
}
