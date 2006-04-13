#include <map>
#include <cmath>
#include <stack>
#include <vector>
#include <map>
#include <tulip/TulipPlugin.h>


struct angle {
  angle(double s,double e,double m,node no):start(s),end(e),midle(m),n(no){}
  angle(){}
  double start,end,midle;
  node n;
};

struct LessThanNodos {
  Metric *metric;
  bool operator() (node n1,node n2) {
    return (metric->getNodeValue(n1) > metric->getNodeValue(n2));
  } 
};

struct TreeRadialExtended:public LayoutAlgorithm {


  TreeRadialExtended(const PropertyContext &context):LayoutAlgorithm(context)  {}
  void dfsPlacement(node n, int depth,double alphaStart,double alphaEnd) {
    if (superGraph->outdeg(n)==0) return;


    //calcul le nombre de feuille des sous-arbres
    //On enlève les feuilles.
    stack<node> leaves;
    double nbLeafSubTrees=0;
    unsigned int nbChild=0;
    Iterator<node> *itN=superGraph->getOutNodes(n);
    for (;itN->hasNext();) {
      node itn=itN->next();
      if (superGraph->outdeg(itn)>0) {
	nbLeafSubTrees+=m->getNodeValue(itn);
	++nbChild;
      }
      else {
	leaves.push(itn);
      }
    }delete itN;

    double ta=alphaStart;
    vector<angle> angles(nbChild);    
    double newAlphaStart,newAlphaEnd;
    double counto=0;
    double deltaAlpha;
    list<node> tmpMap;
    if (nbChild>0) {
      itN=superGraph->getOutNodes(n);
      for (;itN->hasNext();) {
	node itn=itN->next();
	if (superGraph->outdeg(itn)>0) {
	  tmpMap.push_back(itn);
	}
      }delete itN;
      LessThanNodos lessos;
      lessos.metric=m;
      tmpMap.sort(lessos);
      list<node>::iterator itMap=tmpMap.begin();
      for (unsigned int i=0;itMap!=tmpMap.end();++itMap) {
	node itn=(*itMap);
	deltaAlpha=(alphaEnd-alphaStart)/(nbLeafSubTrees);
	newAlphaStart=alphaStart;
	newAlphaEnd=alphaStart+((double)(m->getNodeValue(itn)))*deltaAlpha;
	double arcCos=acos((double)depth+1/((double)(depth+2)));
	if ((newAlphaEnd-newAlphaStart)> 2*arcCos) {
	  newAlphaEnd=alphaStart+2*arcCos;
	}
	alphaStart=newAlphaEnd;
	nbLeafSubTrees-=m->getNodeValue(itn);
	angles[i]=angle(newAlphaStart,newAlphaEnd,(newAlphaEnd+newAlphaStart)/2,itn);
	i++;
      }
    }
    alphaStart=ta;
    deltaAlpha=(alphaEnd-alphaStart)/(superGraph->outdeg(n));
    double midle;
    unsigned int index=0;
    counto=0;
    for (int i=0;i<superGraph->outdeg(n);++i) {
      midle=alphaStart+((double)counto)*deltaAlpha;
      counto++;
      if (index<nbChild) {
	if (leaves.empty()) {
	  layoutObj->setNodeValue(angles[index].n,Coord(((double)depth+1)*cos(midle),((double)depth+1)*sin(midle),0));
	  dfsPlacement(angles[index].n,depth+2,angles[index].start,angles[index].end);
	  ++index;
	}
	else {
	  double midle2=alphaStart+((double)counto)*deltaAlpha;
	  if (fabs(midle-angles[index].midle)<fabs(midle2-angles[index].midle)){
	    layoutObj->setNodeValue(angles[index].n,Coord(((double)depth+1)*cos(midle),((double)depth+1)*sin(midle),0));
	    dfsPlacement(angles[index].n,depth+2,angles[index].start,angles[index].end);
	    ++index;
	  }
	  else {
	    node no=leaves.top();
	    leaves.pop();
	    layoutObj->setNodeValue(no,Coord(((double)depth+1)*cos(midle),((double)depth+1)*sin(midle),0));
	  }
	}
      }
      else {
	node no=leaves.top();
	leaves.pop();
	Coord tmp(((double)depth+1)*cos(midle),((double)depth+1)*sin(midle),0);
	layoutObj->setNodeValue(no,tmp);
	tmp=layoutObj->getNodeValue(no);
      }
    }
    

  }

  bool run() {
    bool cached,resultBool;
    string erreurMsg;
    m=getLocalProperty<Metric>(superGraph,"Leaf",cached,resultBool,erreurMsg);
    Iterator<node> *itN=superGraph->getNodes();
    node startNode;
    for (;itN->hasNext();) {
      startNode=itN->next();
      if (superGraph->indeg(startNode)==0) break;
    } delete itN;
    getLocalProperty<Sizes>(superGraph,"viewSize")->setAllNodeValue( Size(0.5,0.5,0.5));
    layoutObj->setAllNodeValue(Coord(0,0,0));
    dfsPlacement(startNode,0,0,6.283);
    superGraph->getPropertyManager()->delLocalProperty("Leaf");
    return true;
  }

  bool check(string &erreurMsg) {
    if (superGraph->isTree()) {
      erreurMsg="";
      return true;
    }
    else {
      erreurMsg="The Graph must be a Tree";
      return false;
    }
  }
  void reset() {}
private:
  Metric *m;
};

LAYOUTPLUGIN(TreeRadialExtended,"Tree Radial Extended","David Auber","03/03/2001","developpement","0","1")










