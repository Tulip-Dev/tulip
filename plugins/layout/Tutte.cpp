#include <assert.h>
#include <set>
#include <tulip/TriconnectedTest.h>
#include "Tutte.h"


LAYOUTPLUGIN(Tutte,"3-Connected (Tutte)","David Auber","06/11/2002","Beta","0","2");

using namespace std;
//====================================================
Tutte::Tutte(const PropertyContext &context):LayoutAlgorithm(context) 
{}
//====================================================
Tutte::~Tutte() {}
//====================================================
list<node> findCycle(SuperGraph *sg) {
  stdext::hash_map<node,node> father;
  stdext::hash_map<node,bool> visited;
  std::list<node> bfs;
  Iterator<node> *it=sg->getNodes();
  node itn=it->next();
  node startNode=itn;
  int maxDeg=sg->deg(itn);
  while (it->hasNext()) {
    itn=it->next();
    if (sg->deg(itn)>maxDeg) {
      startNode=itn;
    }
  }delete it;
  
  node n1,n2;
  father[startNode]=startNode;
  bfs.push_front(startNode);
  while(!bfs.empty()){
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
    }delete itN;
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
  layoutObj->setAllEdgeValue(vector<Coord>(0));
  std::list<node> tmp;
  tmp=findCycle(superGraph);
  std::list<node>::iterator itL;
  //We place the nodes on the outer face
  Coord tmpCoord,tmpCoord2,baseCoord;
  float gamma;
  int i=0;
  int rayon=100;
  gamma=2*M_PI/tmp.size();
  for (itL=tmp.begin();itL!=tmp.end();++itL) {
    layoutObj->setNodeValue(*itL,Coord(rayon*cos(gamma*i)+rayon*2,rayon*sin(gamma*i)+rayon*2,0));
    i++;
  }
  std::list<node> toMove;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    toMove.push_front(itN->next());
  } delete itN;
  for (itL=tmp.begin();itL!=tmp.end();++itL) {
    toMove.remove(*itL);
  }
  std::list<node>::iterator itn;
  bool ok=true;
  while (ok) {
    ok=false;
    for (itn=toMove.begin();itn!=toMove.end();++itn) {
      tmpCoord.set(0,0,0);
      baseCoord=layoutObj->getNodeValue(*itn);
      int i=0;
      itN=superGraph->getInOutNodes(*itn);
      while (itN->hasNext()) {
	node itAdj=itN->next();
	tmpCoord2=layoutObj->getNodeValue(itAdj);
	tmpCoord.set(tmpCoord.getX()+tmpCoord2.getX(),tmpCoord.getY()+tmpCoord2.getY(),0);
	++i;
      } delete itN;
      layoutObj->setNodeValue(*itn,Coord(tmpCoord.getX()/i,tmpCoord.getY()/i,0));
      if (fabs(baseCoord.getX()-tmpCoord.getX()/i)>0.02) ok=true;
      if (fabs(baseCoord.getY()-tmpCoord.getY()/i)>0.02) ok=true;
    }
  }
  return true;
}
//====================================================
bool Tutte::check(string &erreurMsg) {
  bool result=true;
  if (!TriconnectedTest::isTriconnected(superGraph))
    result=false;
  else {
    Iterator<node> *it=superGraph->getNodes();
    while (it->hasNext()) {
      if (superGraph->deg(it->next())<3) {
	result=false;
	break;
      }
    } delete it;
  }
  if (!result)
    erreurMsg="Graph must be Triconnected";
  else
    erreurMsg="";
  return result;
}
//====================================================
void Tutte::reset() {
}
