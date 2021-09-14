#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
//#include "include/Tree.h"
//#include "include/Agent.h"

using namespace std;

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    // added by us:
    int getNumOfVertices();
    vector <int> getInfectedQueue();
    void setNodeAsSick(int);
    void addCarrier(int);
    int removeFirstCarrier();
    bool isCarriersEmpty();
    bool isThereAnEdge (int, int);
    int removeFromInfectedQ ();
    void disconnectNode(int);

private:
    vector<vector<int>> edges;

    // added by us:
    vector<bool> sickList; // all infected nodes - yellow or red
    vector<int> carriers; // only yellow nodes
    vector <int> infectedQueue;
};

#endif
