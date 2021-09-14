#ifndef TREE_H_
#define TREE_H_

#include "Graph.h"
#include <vector>

using namespace std;

class Session;

class Tree{
public:
    Tree(int rootLabel);
    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

    //destructor
    virtual ~Tree();
    //copy constructor
    Tree (const Tree &other);
    //move constructor
    Tree (Tree &&other);
    // copy assignment
    Tree &operator=(const Tree &other);
    // move assignment
    Tree &operator=(Tree &&other);

    // added by us:
    int getNodeInd ();
    vector<Tree*> &getChildren ();
    virtual Tree* clone() const =0;

private:
    int node;
    std::vector<Tree*> children;

    // added by us:
    void clear();

};


class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();

    // added by us:
    virtual CycleTree* clone() const;

private:
    // added by us:
    int currCycle;

};


class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();

    // added by us:
    virtual MaxRankTree* clone() const;

};


class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();

    // added by us:
    virtual RootTree* clone() const;

};

#endif
