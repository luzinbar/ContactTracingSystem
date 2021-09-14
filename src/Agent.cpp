//
// Created by spl211 on 03/11/2020.
//

#include <iostream>
#include "Agent.h"
#include "Graph.h"
#include "Tree.h"
#include "Session.h"


// ******************** Constructors & Destructors ********************

Agent::Agent() {}

ContactTracer::ContactTracer() {}

Virus::Virus(int nodeInd) : nodeInd(nodeInd), fullyInfected(false) {}

Agent:: ~Agent(){}

Virus:: ~Virus() {}

ContactTracer:: ~ContactTracer() {}


// ******************** Virus ********************

Virus *Virus::clone() const {
    return new Virus(*this);
}

void Virus::act(Session &session) {

    Virus *currVirus = this;
    int virusInd = currVirus->getIndex();
    Graph &g = session.getGraph();

    // virus infects itself first
    // if the virus has never been added to the infected queue (then it is still a carrier) - add it
    if (!currVirus->fullyInfected) {
        currVirus->fullyInfected = true;
        g.infectNode(virusInd);
    }

    // virus spreads itself to a virus free neighbor
    for (int neighborInd = 0; neighborInd < g.getNumOfVertices(); neighborInd++) {
        if (g.isThereAnEdge(virusInd, neighborInd) && !(g.isInfected(neighborInd))) {
            // adding neighbor as carrier AND setting it as a sick node (so it won't be infected again)
            g.addCarrier(neighborInd);
            g.setNodeAsSick(neighborInd);
            break;
        }
    }
}

int Virus::getIndex() {
    return nodeInd;
}

bool Virus::isFullyInfected() {
    return fullyInfected;
}


// ******************** Contact tracer ********************

ContactTracer *ContactTracer::clone() const {
    return new ContactTracer(*this);
}

void ContactTracer::act(Session &session) {
    Tree *root = session.BFS();

    // the queue was empty - BFS returned null
    if (root == nullptr)
        return;

    int nodeToDisconnect = root->traceTree();
    Graph &g = session.getGraph();
    g.disconnectNode(nodeToDisconnect);

    // no further need in the Tree
    delete root;
}

bool ContactTracer::isFullyInfected() {
    return false;
}

int ContactTracer::getIndex() {
    return -1;
}



