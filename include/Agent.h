#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "Session.h"

using namespace std;

class Agent{
public:
    Agent();
    virtual void act(Session &session)=0;

    // added by us:
    virtual ~Agent() ;
    virtual Agent *clone() const=0;
    virtual bool isFullyInfected() = 0;
    virtual int getIndex() =0;
};


class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session &session);

    // added by us:
    virtual ContactTracer *clone () const;
    virtual bool isFullyInfected ();
    int getIndex();
    virtual ~ContactTracer();

};


class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual void act(Session &session);

    // added by us:
    virtual Virus *clone () const;
    int getIndex();
    virtual bool isFullyInfected();
    virtual~ Virus();

private:
    const int nodeInd;
    bool fullyInfected ;

};

#endif
