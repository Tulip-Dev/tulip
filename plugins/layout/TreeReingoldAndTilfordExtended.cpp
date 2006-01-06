#include <tulip/LayoutProxy.h>
#include <tulip/MethodFactory.h>
#include <tulip/TreeTest.h>
#include "TreeReingoldAndTilfordExtended.h"

#define minDecal 1.0

LAYOUTPLUGIN(TreeReingoldAndTilfordExtended,"Hierarchical Tree (R-T Extended)","David Auber","06/11/2002","Beta","0","2");
using namespace std;

//=============================================================================
TreeReingoldAndTilfordExtended::TreeReingoldAndTilfordExtended(const PropertyContext &context):
  Layout(context),
  lengthMetric(0) {
}
//=============================================================================
TreeReingoldAndTilfordExtended::~TreeReingoldAndTilfordExtended() {
}
//=============================================================================
double TreeReingoldAndTilfordExtended::calcDecal(const list<LR> &arbreG,const list<LR> &arbreD) {
  list<LR>::const_iterator itG,itD;
  double decal=0;
  int iG=0,iD=0;
  itG=arbreG.begin();
  itD=arbreD.begin();
  decal = ((*itG).R-(*itD).L + 1);
  iG += std::min((*itG).size, (*itD).size);
  iD += std::min((*itG).size, (*itD).size);
  if (iG==(*itG).size) {
    ++itG;iG=0;
  }
  if (iD==(*itD).size) {
    ++itD;iD=0;
  }
  while ((itG!=arbreG.end()) && (itD!=arbreD.end())) {
    decal = std::max(decal, ((*itG).R-((*itD).L)+1));
    int min = std::min(((*itG).size-iG), ((*itD).size-iD));
    iG+=min;
    iD+=min;
    if (iG==(*itG).size) {
      ++itG;iG=0;
    }
    if (iD==(*itD).size) {
      ++itD;iD=0;
    }
  }
  return decal;
}
//=============================================================================
list<LR> * TreeReingoldAndTilfordExtended::mergeLRList(list<LR>*L,list<LR>*R,double decal) {
  assert (L!=NULL);assert (R!=NULL);
  list<LR>::iterator itL,itR;
  int iL=0,iR=0;
  itL=L->begin();itR=R->begin();
  LR tmp;
  while((itL!=L->end()) && (itR!=R->end())) {
    tmp.L = (*itL).L;
    tmp.R = (*itR).R + decal;
    int min = std::min(((*itL).size-iL), ((*itR).size-iR));
    tmp.size=min;
    
    if ((*itL).size==1) { //start
      (*itL)=tmp;
    } 
    else {
      if (iL==0) {
	if (iL+min>=(*itL).size) //block
	  (*itL)=tmp;
	else {
	  L->insert(itL,tmp);
	  (*itL).size-=min;
	  iL=-min;
	}
      }
      else {
	if (iL+min>=(*itL).size) { //end
	  (*itL).size-=min;
	  ++itL;
	  L->insert(itL,tmp);
	  iL=-min;
	} 
	else { //middle
	  LR tmp2=*itL;
	  (*itL).size=iL;
	  ++itL;
	  L->insert(itL,tmp);
	  tmp2.size-=iL+min;
	  L->insert(itL,tmp2);
	  --itL;
	  iL=-min;
	}
      }
    }
    
    iL += min;
    iR += min;
    
    if (iL>=(*itL).size) {
      ++itL;iL=0;
    }
    if (iR>=(*itR).size) {
      ++itR;iR=0;
    }
  }
  if (itL!=L->end()) {
    if (iL!=0) {
      tmp.L=(*itL).L;
      tmp.R=(*itL).R;
      tmp.size=(*itL).size-iL;
      ++itL;
    }
  }
  if (itR!=R->end()) {
    if (iR!=0) {
      tmp.L=(*itR).L+decal;
      tmp.R=(*itR).R+decal;
      tmp.size=(*itR).size-iR;
      L->push_back(tmp);
      ++itR;
    }
    for (;itR!=R->end();++itR) {
      tmp.L=(*itR).L+decal;
      tmp.R=(*itR).R+decal;
      tmp.size=(*itR).size;
      L->push_back(tmp);
    }
  }
  return L;
}
//=============================================================================
list<LR> * TreeReingoldAndTilfordExtended::TreePlace(node n, stdext::hash_map<node,double> *p) {
  //cerr << "TreeReingoldAndTilfordExtended::TreePlace n id:" << n.id() << endl;
  if (superGraph->outdeg(n)==0){
    list<LR> *result=new list<LR>();
    LR tmpLR;
    tmpLR.L = -sizesProxy->getNodeValue(n).getW()/2;
    tmpLR.R = +sizesProxy->getNodeValue(n).getW()/2;
    tmpLR.size = 1;
    (*p)[n] = 0;
    result->push_front(tmpLR);
    return (result);
  }
  else {
    Iterator<edge> *it;
    it=superGraph->getOutEdges(n);
      
    edge ite = it->next();
    node itn = superGraph->target(ite);
      
    list<LR> *leftTree,*rightTree;
    list<double> childPos;
    leftTree=TreePlace(itn,p);
    childPos.push_back( ( (*(leftTree->begin())).L + (*(leftTree->begin())).R )/2);
    if (superGraph->existProperty("treeEdgeLength")) {
      int tmpLength;
      IntProxy *lengthMetric=superGraph->getProperty<IntProxy>("treeEdgeLength");
      if ((tmpLength=lengthMetric->getEdgeValue(ite))>1) {
	LR tmpLR;
	tmpLR.L=leftTree->front().L;
	tmpLR.R=leftTree->front().R;
	tmpLR.size=tmpLength-1;
	leftTree->push_front(tmpLR);
      }
    }

    while (it->hasNext()) {
      ite=it->next();
      itn=superGraph->target(ite);
      rightTree=TreePlace(itn,p);
      if (superGraph->existProperty("treeEdgeLength")) {
	int tmpLength;
	IntProxy *lengthMetric=superGraph->getProperty<IntProxy>("treeEdgeLength");
	if ((tmpLength=lengthMetric->getEdgeValue(ite))>1) {
	  LR tmpLR;
	  tmpLR.L=rightTree->front().L;
	  tmpLR.R=rightTree->front().R;
	  tmpLR.size=tmpLength-1;
	  rightTree->push_front(tmpLR);
	}
      }
      double decal=calcDecal(*leftTree,*rightTree);
      double tmpL=( (*(rightTree->begin())).L + (*(rightTree->begin())).R )/2;
      if (mergeLRList(leftTree,rightTree,decal)==leftTree) {
	childPos.push_back(tmpL+decal);
	delete rightTree;
      }
      else {
	list<double>::iterator itI=childPos.begin();
	for(;itI!=childPos.end();++itI) (*itI)-=decal;
	childPos.push_back(tmpL);
	delete leftTree;
	leftTree=rightTree;
      }
    }delete it;
    double posFather=((((*(leftTree->begin())).L+(*(leftTree->begin())).R)/2));
    LR tmpLR;
    tmpLR.L=posFather-sizesProxy->getNodeValue(n).getW()/2;
    tmpLR.R=posFather+sizesProxy->getNodeValue(n).getW()/2;
    tmpLR.size=1;
    leftTree->push_front(tmpLR);

    list<double>::const_iterator itI=childPos.begin();
    it = superGraph->getOutEdges(n);
      
    while (it->hasNext()) {
      ite=it->next();
      itn=superGraph->target(ite);
      (*p)[itn]=*itI-posFather;
      ++itI;
    }delete it;
    childPos.clear();
    (*p)[n]=0;
    return(leftTree);
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::TreeLevelSizing(node n,map<int,double> &maxSize,int level) {
  if (maxSize.find(level)!=maxSize.end()) {
    if (maxSize[level]<sizesProxy->getNodeValue(n).getH()) {
      maxSize[level]=sizesProxy->getNodeValue(n).getH();
    }
  }
  else
    maxSize[level]=sizesProxy->getNodeValue(n).getH();

  if (superGraph->existProperty("treeEdgeLength")) {
    IntProxy *lengthMetric=superGraph->getProperty<IntProxy>("treeEdgeLength");
    Iterator<edge> *it=superGraph->getOutEdges(n);
    while (it->hasNext()) {
      edge ite=it->next();
      node itn=superGraph->target(ite);
      TreeLevelSizing(itn,maxSize,level+(lengthMetric->getEdgeValue(ite)));
    }delete it;
  }
  else {
    Iterator<node> *itN=superGraph->getOutNodes(n);
    for (;itN->hasNext();) {
      node itn=itN->next();
      TreeLevelSizing(itn,maxSize,level+1);
    }delete itN;
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::calcLayout(node n, stdext::hash_map<node,double> *p,
						double x, double y, int level,
						map<int,double> &maxLevelSize) {
  //cerr << "TreeReingoldAndTilfordExtended::calcLayout" << endl;
  Coord tmpCoord;
  tmpCoord.set(x+(*p)[n],y+maxLevelSize[level]/2,0);
  layoutProxy->setNodeValue(n,tmpCoord);
  if (superGraph->existProperty("treeEdgeLength")) {
    IntProxy *lengthMetric=superGraph->getProperty<IntProxy>("treeEdgeLength");
    Iterator<edge> *it=superGraph->getOutEdges(n);
    for (;it->hasNext();){
      edge ite = it->next();
      node itn = superGraph->target(ite);
      double decalY = y;
      int decalLevel = level;
      int tmp = lengthMetric->getEdgeValue(ite);
      while(tmp>0) {
	decalY += maxLevelSize[decalLevel]+minDecal;
	decalLevel++;
	tmp--;
      }
      calcLayout(itn,p,x+(*p)[n], decalY , decalLevel, maxLevelSize);
    } delete it;
  }
  else {
    Iterator<node> *it=superGraph->getOutNodes(n);
    for (;it->hasNext();){
      node itn=it->next();
      calcLayout(itn,p, x+(*p)[n], y+maxLevelSize[level]+minDecal, 
		 level+1, maxLevelSize);
    } delete it;
  }
  //cerr << "TreeReingoldAndTilfordExtended::EndCalcLayout" << endl;
}
//=============================================================================
bool TreeReingoldAndTilfordExtended::run() {
  stdext::hash_map<node,double> posRelative;
  node startNode;
  layoutProxy->setAllEdgeValue(vector<Coord>(0));
  sizesProxy=superGraph->getProperty<SizesProxy>("viewSize");
  Iterator<node> *it=superGraph->getNodes();
  node itn;
  startNode=it->next();
  while (it->hasNext()) {      
    itn=it->next();
    if (superGraph->indeg(itn)==0) {
      startNode=itn;
      break;
    }
  } delete it;
  map<int,double> maxSizeLevel;
  TreeLevelSizing(startNode,maxSizeLevel,0);
  list<LR> *tmpList = TreePlace(startNode,&posRelative);
  delete tmpList;
  calcLayout(startNode,&posRelative, 0, 0, 0, maxSizeLevel);
  return true;
}
//=============================================================================
bool TreeReingoldAndTilfordExtended::check(string &erreurMsg) {
  if (TreeTest::isTree(superGraph))
    {erreurMsg="";return true;}
  else
    {erreurMsg="The Graph must be a Tree";return false;}
}
//=============================================================================
void TreeReingoldAndTilfordExtended::reset() {
}
//=============================================================================
