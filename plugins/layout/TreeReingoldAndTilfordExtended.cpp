#include <tulip/TlpTools.h>
#include <tulip/LayoutProxy.h>
#include <tulip/MethodFactory.h>
#include <tulip/TreeTest.h>
#include <tulip/ForEach.h>
#include "TreeReingoldAndTilfordExtended.h"

LAYOUTPLUGIN(TreeReingoldAndTilfordExtended,"Hierarchical Tree (R-T Extended)","David Auber","06/11/2002","Beta","0","2");
using namespace std;

namespace {
  const char * paramHelp[] = {
    // nodeSize
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "SizeProxy" ) \
    HTML_HELP_DEF( "values", "An existing size property" ) \
    HTML_HELP_DEF( "default", "viewSize" ) \
    HTML_HELP_BODY() \
    "This parameter defines the property used for node's sizes." \
    HTML_HELP_CLOSE(),
    //Orientation
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "horizontal" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the orientation of the drawing" \
    HTML_HELP_CLOSE(),
    //Orthogonal
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "true" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose if the tree tree is drawn orthogonally or not" \
    HTML_HELP_CLOSE(),
    //Orthogonal
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "true" )					\
    HTML_HELP_BODY()							\
    "This parameter enables to choose if the tree tree is drawn orthogonally or not" \
    HTML_HELP_CLOSE(),
    //Spacing
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "64." )	 \
    HTML_HELP_BODY() \
    "This parameter enables to set up the minimum space between two layers in the drawing" \
    HTML_HELP_CLOSE(),
    //Spacing
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "18." )	 \
    HTML_HELP_BODY() \
    "This parameter enables to set up the minimum space between two nodes in the same layer" \
    HTML_HELP_CLOSE()
  };
}
//=============================================================================
#define ORIENTATION "vertical;horizontal;"
//=============================================================================
TreeReingoldAndTilfordExtended::TreeReingoldAndTilfordExtended(const PropertyContext &context):
  Layout(context),
  lengthMetric(0) {
  addParameter<SizesProxy>("nodeSize",paramHelp[0],"viewSize");
  addParameter<IntProxy>("edgeLength",paramHelp[1]);
  addParameter<bool>("orthogonal", paramHelp[2], "true" );
  addParameter<bool>("use length", paramHelp[3], "false" );
  addParameter<StringCollection> ("orientation", paramHelp[4], ORIENTATION );
  addParameter<float> ("layer spacing", paramHelp[4], "64." );
  addParameter<float> ("node spacing", paramHelp[5], "18." );
}
//=============================================================================
TreeReingoldAndTilfordExtended::~TreeReingoldAndTilfordExtended() {
}
//=============================================================================
double TreeReingoldAndTilfordExtended::calcDecal(const list<LR> &arbreG, const list<LR> &arbreD) {
  list<LR>::const_iterator itG,itD;
  double decal=0;
  int iG=0,iD=0;
  itG=arbreG.begin();
  itD=arbreD.begin();
  decal = ((*itG).R-(*itD).L + nodeSpacing);
  iG += std::min((*itG).size, (*itD).size);
  iD += std::min((*itG).size, (*itD).size);
  if (iG==(*itG).size) {
    ++itG;iG=0;
  }
  if (iD==(*itD).size) {
    ++itD;iD=0;
  }
  while ((itG!=arbreG.end()) && (itD!=arbreD.end())) {
    decal = std::max(decal, ((*itG).R-((*itD).L)+nodeSpacing));
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
list<LR> * TreeReingoldAndTilfordExtended::mergeLRList(list<LR>*L, list<LR>*R, double decal) {
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
    list<LR> *result = new list<LR>();
    LR tmpLR;
    tmpLR.L = -sizesProxy->getNodeValue(n).getW()/2.;
    tmpLR.R = +sizesProxy->getNodeValue(n).getW()/2.;
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
    childPos.push_back( ( (*(leftTree->begin())).L + (*(leftTree->begin())).R )/2.);
    if (useLength) {
      int tmpLength;
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
      if (useLength) {
	int tmpLength;
	if ((tmpLength=lengthMetric->getEdgeValue(ite)) > 1) {
	  LR tmpLR;
	  tmpLR.L=rightTree->front().L;
	  tmpLR.R=rightTree->front().R;
	  tmpLR.size = tmpLength-1;
	  rightTree->push_front(tmpLR);
	}
      }
      double decal = calcDecal(*leftTree, *rightTree);
      double tmpL = ( (*(rightTree->begin())).L + (*(rightTree->begin())).R )/2.;
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
    } delete it;
    double posFather=((((*(leftTree->begin())).L + (*(leftTree->begin())).R)/2.));
    LR tmpLR;
    tmpLR.L = posFather - sizesProxy->getNodeValue(n).getW()/2.;
    tmpLR.R = posFather + sizesProxy->getNodeValue(n).getW()/2.;
    tmpLR.size = 1;
    leftTree->push_front(tmpLR);

    list<double>::const_iterator itI = childPos.begin();
    forEach(ite, superGraph->getOutEdges(n)) {
      itn = superGraph->target(ite);
      (*p)[itn] = *itI - posFather;
      ++itI;
    } 
    childPos.clear();
    (*p)[n] = 0;
    return(leftTree);
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::TreeLevelSizing(node n, map<int,double> &maxSize,int level, map<node,int> &levels) {
  levels[n] = level;
  if (maxSize.find(level)!=maxSize.end()) {
    if (maxSize[level] < sizesProxy->getNodeValue(n).getH()) {
      maxSize[level]=sizesProxy->getNodeValue(n).getH();
    }
  }
  else
    maxSize[level]=sizesProxy->getNodeValue(n).getH();
  
  if (useLength) {
    edge ite;
    forEach(ite, superGraph->getOutEdges(n)) {
      node itn=superGraph->target(ite);
      TreeLevelSizing(itn,maxSize,level+(lengthMetric->getEdgeValue(ite)), levels);
    }
  }
  else {
    node itn;
    forEach(itn, superGraph->getOutNodes(n)) {
      TreeLevelSizing(itn, maxSize, level+1, levels);
    }
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::calcLayout(node n, stdext::hash_map<node,double> *p,
						double x, double y, int level,
						map<int,double> &maxLevelSize) {
  //cerr << "TreeReingoldAndTilfordExtended::calcLayout" << endl;
  Coord tmpCoord;
  tmpCoord.set(x+(*p)[n], y+maxLevelSize[level]/2., 0);
  layoutProxy->setNodeValue(n,tmpCoord);
  if (useLength) {
    edge ite;
    forEach(ite, superGraph->getOutEdges(n)) {
      node itn = superGraph->target(ite);
      double decalY = y;
      int decalLevel = level;
      int tmp = lengthMetric->getEdgeValue(ite);
      while(tmp>0) {
	decalY += maxLevelSize[decalLevel]+spacing;
	decalLevel++;
	tmp--;
      }
      calcLayout(itn,p,x+(*p)[n], decalY , decalLevel, maxLevelSize);
    }
  }
  else {
    node itn;
    forEach(itn, superGraph->getOutNodes(n)) {
      calcLayout(itn,p, x+(*p)[n], y+maxLevelSize[level]+spacing, 
		 level+1, maxLevelSize);
    }
  }
  //cerr << "TreeReingoldAndTilfordExtended::EndCalcLayout" << endl;
}
//===============================================================
bool TreeReingoldAndTilfordExtended::run() {
  stdext::hash_map<node,double> posRelative;

  layoutProxy->setAllEdgeValue(vector<Coord>(0));
  sizesProxy = superGraph->getProperty<SizesProxy>("viewSize");
  orientation = "horizontal";
  lengthMetric = 0;
  spacing = 64.0;
  nodeSpacing = 18.0;
  ortho = true;
  useLength = false;
  if (dataSet!=0) {
    dataSet->get("edgeLength", lengthMetric);
    dataSet->get("use length", useLength);
    dataSet->get("nodeSize", sizesProxy);
    dataSet->get("orthogonal", ortho);
    dataSet->get("layer spacing", spacing);
    dataSet->get("node spacing", nodeSpacing);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }

  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, superGraph->getNodes()) {
      Size tmp = sizesProxy->getNodeValue(n);
      sizesProxy->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }
  //===========================================================
  node startNode;
  tlp::getSource(superGraph, startNode);

  map<int,double> maxSizeLevel;
  map<node, int> levels;
  TreeLevelSizing(startNode, maxSizeLevel,0, levels);
  
  list<LR> *tmpList = TreePlace(startNode,&posRelative);
  delete tmpList;

  calcLayout(startNode,&posRelative, 0, 0, 0, maxSizeLevel);

  if (ortho) {
    //Edge bends
    //compute layer value 
    map<int,double>::reverse_iterator itos = maxSizeLevel.rbegin();
    //==========================
    //compute the coordinate of each interval between layer.
    vector<double> levelCoord(itos->first+1);
    for (int i=0; i<itos->first; ++i) {
      levelCoord[i] = 0;
    }
    map<int,double>::iterator itas = maxSizeLevel.begin();
    for (;itas!=maxSizeLevel.end(); ++itas) {
      levelCoord[itas->first] = itas->second;
    }
    for (int i=1; i<itos->first; ++i) {
      levelCoord[i] += levelCoord[i-1] + spacing;
    }
    //============================
    edge e;
    forEach(e, superGraph->getEdges()) {
      LineType::RealType tmp;
      node src = superGraph->source(e);
      node tgt = superGraph->target(e);
      Coord srcPos = layoutProxy->getNodeValue(src);
      Coord tgtPos = layoutProxy->getNodeValue(tgt);
      double y = levelCoord[levels[tgt]-1];
      tmp.push_back(Coord(srcPos[0], y, 0));
      tmp.push_back(Coord(tgtPos[0], y, 0));
      layoutProxy->setEdgeValue(e, tmp);
    }
    
    if (orientation == "horizontal") {
      forEach(e, superGraph->getEdges()) {
	LineType::RealType tmp = layoutProxy->getEdgeValue(e);
	LineType::RealType tmp2;
	tmp2.push_back(Coord(-tmp[0][1], tmp[0][0], tmp[0][2]));
	tmp2.push_back(Coord(-tmp[1][1], tmp[1][0], tmp[1][2]));
	layoutProxy->setEdgeValue(e, tmp2);
      }
    }
  }

  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, superGraph->getNodes()) {
      Size  tmp = sizesProxy->getNodeValue(n);
      sizesProxy->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      Coord tmpC = layoutProxy->getNodeValue(n);
      layoutProxy->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
  }
  
  return true;
}
//=============================================================================
bool TreeReingoldAndTilfordExtended::check(string &erreurMsg) {
  if (TreeTest::isTree(superGraph)) {
    erreurMsg="";
    return true;
  } else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::reset() {
}
//=============================================================================
