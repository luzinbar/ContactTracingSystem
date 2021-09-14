//
// Created by spl211 on 03/11/2020.
//

#include "Graph.h"
#include <iostream>
#include <vector>


// ********** Constructors **********

Graph::Graph(std::vector<std::vector<int>> matrix) :
edges(), sickList(matrix.size(), false), carriers(), infectedQueue(){
    edges = matrix;
}


// ******************** Getters ********************

int Graph::getNumOfVertices() {
    return edges.size();
}

vector<int> Graph::getInfectedQueue() {
    return infectedQueue;
}


// ******************** Operations on function members ********************

void Graph::addCarrier(int nodeInd) {
    carriers.push_back(nodeInd);
}

bool Graph::isCarriersEmpty() {
    return (carriers.empty());
}

int Graph::removeFirstCarrier() {
    int output = carriers[0];
    carriers.erase(carriers.begin());
    return output;
}

bool Graph::isThereAnEdge(int i, int j) {
    return (edges[i][j] == 1);
}

int Graph::removeFromInfectedQ () {
    if (infectedQueue.empty())
        return -1;
    int output = infectedQueue[0];
    infectedQueue.erase(infectedQueue.begin());
    return output;
}


// ******************** Infect operations ********************

bool Graph:: isInfected(int nodeInd) {
    return sickList[nodeInd];
}

void Graph ::infectNode(int nodeInd) {
    infectedQueue.push_back(nodeInd);
}

void Graph::setNodeAsSick(int nodeInd){
    sickList[nodeInd] = true;
}


// ******************** disconnect node ********************

void Graph::disconnectNode(int nodeInd) {
    for (unsigned int i = 0; i < edges.size(); i++) {
        edges[nodeInd][i] = 0;
        edges[i][nodeInd] = 0;
    }
}





