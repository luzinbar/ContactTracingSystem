//
// Created by spl211 on 03/11/2020.
//


//#include "include/json.hpp"
#include "Session.h"
#include "Agent.h"
#include "Graph.h"
#include "Tree.h"
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;


// ******************** Constructor ********************

Session::Session(const std::string &Path) : g(vector<vector<int>>()), treeType(),
                                            agents(vector<Agent *>()), cycleNumber(0) {

    ifstream ifs(Path);
    json j;
    ifs >> j;

    Graph matrix(j["graph"]);
    setGraph(matrix);

    string type = j["tree"];
    if (type == "M")
        treeType = MaxRank;
    else if (type == "R")
        treeType = Root;
    else
        treeType = Cycle;

    for (unsigned int i = 0; i < j["agents"].size(); i++) {
        if (j["agents"][i][0] == "V") {
            Virus vs(j["agents"][i][1]);
            addAgent(vs);
            g.setNodeAsSick(j["agents"][i][1]);
        } else
            addAgent(ContactTracer());
    }
}


// ******************** Rule of 5 ********************

//destructor
Session::~Session() {
    Session::clear();
}

//copy constructor
Session::Session(const Session &other) : g(other.g), treeType(other.treeType), agents(vector<Agent *>()),
                                         cycleNumber(other.cycleNumber) {
    for (unsigned int i = 0; i < other.agents.size(); i++) {
        Agent *otherAgent = other.agents[i];
        agents[i] = otherAgent->clone();
    }
}
// copy assignment
Session &Session::operator=(const Session &other) {
    if (this != &other) {
        this->clear();
        for (unsigned int i = 0; i < other.agents.size(); i++) {
            Agent *otherAgent = other.agents[i];
            agents[i] = otherAgent->clone();
        }
        g = other.g;
        treeType = other.treeType;
        cycleNumber = other.cycleNumber;
    }
    return *this;
}

//move constructor
Session::Session(Session &&other) : g(move(other.g)), treeType(other.treeType), agents(move(other.agents)), cycleNumber(other.cycleNumber) {
    for (auto &agent : other.agents)
        agent = nullptr;
}

// move assignment
Session &Session::operator=(Session &&other) {
    if (this != &other) {
        this->clear();
        g = other.g;
        agents = other.agents;
        cycleNumber = other.cycleNumber;
        treeType = other.treeType;

        for (auto &agent : other.agents)
            agent = nullptr;
    }
    return *this;
}

void Session::clear() {
    for (auto &agent : agents) {
        if (agent) {
            delete agent;
            agent = nullptr;
        }
    }
}

// ******************** Operate on members ********************

// ********** Getters **********

Graph& Session::getGraph()  {
    return g;
}

TreeType Session::getTreeType() const {
    return treeType;
}

int Session::getCycleNumber() const {
    return cycleNumber;
}


// ********** Setters **********

void Session::setGraph(const Graph &graph) {
    g = graph;
}


// ********** Add agent **********

void Session::addAgent(const Agent &agent) {
    Agent *toAdd = agent.clone();
    agents.push_back(toAdd);
}


// ********** Queue **********

int Session::dequeueInfected() {
    return g.removeFromInfectedQ();
}

void Session::enqueueInfected(int infectedNode) {
    (g.getInfectedQueue()).push_back(infectedNode);
}


// ******************** BFS ********************

Tree* Session::BFS() {
    int numOfVertices = g.getNumOfVertices();
    int rootLabel = dequeueInfected();

    // the queue is empty - return
    if (rootLabel == -1)
        return nullptr;

    // initialize an empty vector of size n to store the trees pointers
    vector<Tree *> treeByIndex(numOfVertices, nullptr);

    vector<bool> visited(numOfVertices, false);

    // create the root - to be returned later
    Tree *root = Tree::createTree(*this, rootLabel);
    treeByIndex[rootLabel] = root;

    // create an empty queue
    vector<int> q;

    q.push_back(rootLabel);
    visited[rootLabel] = true;
    int currNode;

    while (!q.empty()) {
        currNode = q[0];
        Tree *currTree = treeByIndex[currNode];
        q.erase(q.begin());

        // For every adjacent node of the curr node
        for (int i = 0; i < numOfVertices; i++) {
            if (g.isThereAnEdge(currNode,i) && (!visited[i])) {
                visited[i] = true;
                q.push_back(i);

                // create the children and set them as currTree children
                Tree *currChild = Tree::createTree(*this, i);
                currTree->getChildren().push_back(currChild);

                //insert the children to the treeByIndex vector
                treeByIndex[i] = currChild;
            }
        }
    }
    return root;
}


// ******************** Simulate - the real deal ********************

void Session::simulate() {

    // separating first cycle from others because in the first cycle Carriers list is Empty

    // only first cycle
    for (unsigned int i =0 ; i < agents.size(); i++) {
        agents[i]->act(*this);
    }
    cycleNumber++;

    // Termination condition is met if and only if there are no new carriers
    while (!g.isCarriersEmpty()) {

        // emptying the carriers queue and adding them as new agents
        while (!g.isCarriersEmpty()) {
            int ind = g.removeFirstCarrier();
            Agent *virus = new Virus (ind);
            agents.push_back(virus);
        }

        for (Agent *agent: agents) {
            agent->act(*this);
        }
        cycleNumber++;
    }


    // ----- output -----

    // creating json file output
    json j;

    int n =g.getNumOfVertices();
    vector<vector<int>> matrix (n, vector<int>(n,0));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (g.isThereAnEdge(i,j)) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
        }
    }

    j["graph"] = matrix;

    for (unsigned int i = 0; i < agents.size(); i++) {
        if (agents[i]->isFullyInfected()) {
            j["infected"].push_back(agents[i]->getIndex());
        }
    }

    ofstream output("output.json");
    output << j;
}

