#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include "Tree.h"
#include <json.hpp>

using namespace std;

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);

    //destructor
    virtual ~Session();
    //copy constructor
    Session (const Session &other);
    //move constructor
    Session (Session &&other);
    // copy assignment
    Session &operator=(const Session &other);
    // move assignment
    Session &operator=(Session &&other);

    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    // added by us:
    Graph &getGraph();
    int getCycleNumber() const;
    Tree* BFS ();

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;

    // added by us:
    int cycleNumber;
    void clear();

};


#endif
