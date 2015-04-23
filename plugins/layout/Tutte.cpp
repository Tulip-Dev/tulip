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
#include <set>
#include <tulip/tuliphash.h>
#include <tulip/TriconnectedTest.h>
#include "Tutte.h"

PLUGIN(Tutte)

using namespace std;
using namespace tlp;
//====================================================
Tutte::Tutte(const tlp::PluginContext* context):LayoutAlgorithm(context) {
}
//====================================================
Tutte::~Tutte() {}
//====================================================
list<node> findCycle(Graph *sg) {
  TLP_HASH_MAP<node,node> father;
  TLP_HASH_MAP<node,bool> visited;
  std::list<node> bfs;
  Iterator<node> *it=sg->getNodes();
  node itn=it->next();
  node startNode=itn;
  unsigned int maxDeg=sg->deg(itn);

  while (it->hasNext()) {
    itn=it->next();

    if (sg->deg(itn)>maxDeg) {
      startNode=itn;
    }
  }

  delete it;

  node n1,n2;
  father[startNode]=startNode;
  bfs.push_front(startNode);

  while(!bfs.empty()) {
    node curNode=bfs.front();
    bfs.pop_front();
    Iterator<node> *itN=sg->getInOutNodes(curNode);

    while (itN->hasNext()) {
      node itn=itN->next();

      if (itn!=father[curNode]) {
        if (!visited[itn]) {
          visited[itn]=true;
          father[itn]=curNode;
          bfs.push_back(itn);
        }
        else {
          n1=curNode;
          n2=itn;
          bfs.clear();
          break;
        }
      }
    }

    delete itN;
  }

  std::list<node> result;
  result.push_back(n1);
  result.push_back(n2);

  while(n1!=n2) {
    if (father[n1]==father[n2]) {
      if ((father[n1]!=n1) && (father[n2]!=n2))
        result.push_back(father[n1]);

      return result;
    }
    else {
      if (n1==father[n2]) {
        return result;
      }
      else {
        if (n2==father[n1]) {
          return result;
        }
        else {
          result.push_front(father[n1]);
          result.push_back(father[n2]);
          n1=father[n1];
          n2=father[n2];
        }
      }
    }
  }

  result.push_back(n1);
  return result;
}
//====================================================
bool Tutte::run() {
  result->setAllEdgeValue(vector<Coord>(0));
  std::list<node> tmp;
  tmp=findCycle(graph);
  std::list<node>::iterator itL;
  //We place the nodes on the outer face
  Coord tmpCoord,tmpCoord2,baseCoord;
  float gamma;
  int i=0;
  int rayon=100;
  gamma=2*M_PI/tmp.size();

  for (itL=tmp.begin(); itL!=tmp.end(); ++itL) {
    result->setNodeValue(*itL,Coord(rayon*cos(gamma*i)+rayon*2,rayon*sin(gamma*i)+rayon*2,0));
    i++;
  }

  std::list<node> toMove;
  Iterator<node> *itN=graph->getNodes();

  while (itN->hasNext()) {
    toMove.push_front(itN->next());
  }

  delete itN;

  for (itL=tmp.begin(); itL!=tmp.end(); ++itL) {
    toMove.remove(*itL);
  }

  std::list<node>::iterator itn;
  bool ok=true;

  while (ok) {
    ok=false;

    for (itn=toMove.begin(); itn!=toMove.end(); ++itn) {
      tmpCoord.set(0,0,0);
      Coord baseCoord=result->getNodeValue(*itn);
      int i=0;
      itN=graph->getInOutNodes(*itn);

      while (itN->hasNext()) {
        node itAdj=itN->next();
        const Coord& tmpCoord2=result->getNodeValue(itAdj);
        tmpCoord.set(tmpCoord.getX()+tmpCoord2.getX(),tmpCoord.getY()+tmpCoord2.getY(),0);
        ++i;
      }

      delete itN;
      result->setNodeValue(*itn,Coord(tmpCoord.getX()/i,tmpCoord.getY()/i,0));

      if (fabs(baseCoord.getX()-tmpCoord.getX()/i)>0.02) ok=true;

      if (fabs(baseCoord.getY()-tmpCoord.getY()/i)>0.02) ok=true;
    }
  }

  return true;
}
//====================================================
bool Tutte::check(std::string &erreurMsg) {
  bool result=true;

  if (!TriconnectedTest::isTriconnected(graph))
    result=false;
  else {
    Iterator<node> *it=graph->getNodes();

    while (it->hasNext()) {
      if (graph->deg(it->next())<3) {
        result=false;
        break;
      }
    }

    delete it;
  }

  if (!result)
    erreurMsg="The graph must be triconnected.";
  else
    erreurMsg="";

  return result;
}
