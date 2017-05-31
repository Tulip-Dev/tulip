/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "CliqueEnumeration.h"

#include <tulip/ForEach.h>
#include <tulip/SimpleTest.h>

#include <sstream>

PLUGIN(CliqueEnumeration)


using namespace tlp;
using namespace std;

//================================================================================
static const char * paramHelp[] = {
    //Min Size
    "Minimum size for cliques"
};
//================================================================================
CliqueEnumeration::CliqueEnumeration(tlp::PluginContext *context):Algorithm(context) {
    addInParameter<unsigned int>("minimum size", paramHelp[0],"0");
}

//================================================================================
void CliqueEnumeration::addClique(vector<node> & clique){
    unsigned int num = graph->numberOfSubGraphs()+1;
    stringstream ss;
    ss << "clique_" << num;
    graph->inducedSubGraph(clique, NULL, ss.str());
}
//================================================================================
void CliqueEnumeration::getNeighborhood(node u, set<node> &neigh){
    neigh.clear();
    node v;
    forEach(v,graph->getInOutNodes(u))
            neigh.insert(v);
}
//================================================================================
tlp::node CliqueEnumeration::choosePivot(set<node> & C){
    node pivot=tlp::node();
    unsigned int maxinter=0;
    for(set<node>::const_iterator its=C.begin();its!=C.end();++its){
        unsigned int inter=0;
        node v;
        forEach(v,graph->getInOutNodes(*its)){
            if(C.find(v)!=C.end())
                inter++;
        }
        if(inter>=maxinter)
            pivot=*its;
    }
    return pivot;
}
//================================================================================
void CliqueEnumeration::intersectSet(set<node> &s1, set<node> &s2, set<node> &res){
    for(set<node>::iterator it=s1.begin();it!=s1.end();++it){
        if(s2.find(*it)!=s2.end())
            res.insert(*it);
    }
}
//================================================================================
void CliqueEnumeration::unionSet(set<node> &s1, set<node> &s2, set<node> &res){
    for(set<node>::iterator it=s1.begin();it!=s1.end();++it){
        res.insert(*it);
    }
    for(set<node>::iterator it=s2.begin();it!=s2.end();++it){
        res.insert(*it);
    }
}

//================================================================================
void CliqueEnumeration::maxCliquePivot(set<node>& P, vector<node>& R, set<node>& X){

    set<node> C;
    unionSet(P,X,C);

    if(C.empty()){
        if(R.size()>=minsize)
            addClique(R);
    }else{
        tlp::node p = choosePivot(C);
        set<node> neighp;
        getNeighborhood(p,neighp);
        set<node> tovisit;
        for(set<node>::iterator its=P.begin();its!=P.end();++its){
            if(neighp.find(*its)==neighp.end())
                tovisit.insert(*its);
        }

        for(set<node>::iterator its=tovisit.begin();its!=tovisit.end();++its){
            set<node> neighx;
            getNeighborhood(*its,neighx);
            set<node> newP;
            intersectSet(P,neighx,newP);
            vector<node> newR(R);
            newR.push_back(*its);
            set<node> newX;
            intersectSet(X,neighx,newX);
            maxCliquePivot(newP,newR,newX);
            P.erase(*its);
            X.insert(*its);
        }
    }
}
//================================================================================

//================================================================================
struct DegreeOrderingElem{
    DegreeOrderingElem() : n(tlp::node()),deg(0){}
    DegreeOrderingElem(tlp::node n,unsigned int deg) : n(n),deg(deg){}
    bool operator ==(const DegreeOrderingElem &b) const{
        return n==b.n;
    }
    bool operator!=(const DegreeOrderingElem &b) const {
        return n != b.n;
    }
    tlp::node n;
    unsigned int deg;
};

//================================================================================
struct LessDegreeOrdering{
    bool operator() (const DegreeOrderingElem* u,const DegreeOrderingElem*  v){
        if(u->deg==v->deg)
            return u->n.id<v->n.id;
        return u->deg<v->deg;
    }
};
//================================================================================
void CliqueEnumeration::getDegenerateOrdering(vector<node> &ordering){
    ordering.clear();
    tlp::Graph* sub = graph->addCloneSubGraph();
    std::map<tlp::node,DegreeOrderingElem *> degrees;
    set<DegreeOrderingElem *,LessDegreeOrdering> sortednodes;
    node n;
    forEach(n,sub->getNodes()){
        DegreeOrderingElem* elem = new DegreeOrderingElem(n,sub->deg(n));
        degrees.insert(make_pair(n,elem));
        sortednodes.insert(elem);
    }

    while(!sortednodes.empty()){
        set<DegreeOrderingElem *,LessDegreeOrdering>::iterator it = sortednodes.begin();
        node n = (*it)->n;
        ordering.push_back(n);
        delete (*it);
        sortednodes.erase(it);
        sub->delNode(n);
        node v;
        forEach(v,sub->getInOutNodes(n)){
            DegreeOrderingElem* elem = degrees.find(v)->second;
            sortednodes.erase(elem);
            elem->deg=elem->deg-1;
            sortednodes.insert(elem);
        }
    }
    graph->delSubGraph(sub);
}

//================================================================================
bool CliqueEnumeration ::run() {
    minsize=0;
    if(dataSet!=NULL)
        dataSet->get("minimum size",minsize);

    vector<node> ordering;
    getDegenerateOrdering(ordering);
    for(unsigned int i=0;i<ordering.size();++i){
        set<node> neighu;
        //        cout << ordering[i].id << endl;
        getNeighborhood(ordering[i],neighu);
        set<node> P,X;
    vector<node> R;
        R.push_back(ordering[i]);
        for(unsigned int j=0;j<=i;++j)
            if(neighu.find(ordering[j])!=neighu.end())
                X.insert(ordering[j]);
        for(unsigned int j=i+1;j<ordering.size();++j)
            if(neighu.find(ordering[j])!=neighu.end())
                P.insert(ordering[j]);
        maxCliquePivot(P,R,X);
    }

    return true;
}
//================================================================================
bool CliqueEnumeration::check(string &erreurMsg) {
    erreurMsg="";
    if(!tlp::SimpleTest::isSimple(graph)){
        erreurMsg="The graph should be simple.";
        return false;
    }
    return true;
}
//================================================================================
