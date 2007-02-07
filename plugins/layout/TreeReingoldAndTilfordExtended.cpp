#include <tulip/LayoutProperty.h>
#include <tulip/MethodFactory.h>
#include <tulip/TreeTest.h>
#include <tulip/ForEach.h>
#include "TreeReingoldAndTilfordExtended.h"

LAYOUTPLUGINOFGROUP(TreeReingoldAndTilfordExtended,"Hierarchical Tree (R-T Extended)","David Auber","06/11/2002","Beta","0","2","Tree");
using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // nodeSize
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Size" ) \
    HTML_HELP_DEF( "values", "An existing size property" ) \
    HTML_HELP_DEF( "default", "viewSize" ) \
    HTML_HELP_BODY() \
    "This parameter defines the property used for node's sizes." \
    HTML_HELP_CLOSE(),
    //edge length
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Int" ) \
    HTML_HELP_DEF( "values", "An existing int property" ) \
    HTML_HELP_DEF( "default", "None" ) \
    HTML_HELP_BODY() \
    "This parameter indicates the property used to compute the length of edges." \
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
    HTML_HELP_DEF( "default", "true" )					\
    HTML_HELP_BODY()							\
    "This parameter enables to choose if the tree is drawn orthogonally or not" \
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
    HTML_HELP_CLOSE(),
    //bounding circles
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "false" )	 \
    HTML_HELP_BODY() \
    "Indicates if the node bounding objects are boxes or bounding circles." \
    HTML_HELP_CLOSE()
  };
}
//=============================================================================
#define ORIENTATION "vertical;horizontal;"
//=============================================================================
TreeReingoldAndTilfordExtended::TreeReingoldAndTilfordExtended(const PropertyContext &context):
  LayoutAlgorithm(context),
  lengthMetric(0) {
  addParameter<SizeProperty>("nodeSize",paramHelp[0],"viewSize");
  addParameter<IntegerProperty>("edgeLength", paramHelp[1], 0, false);
  addParameter<StringCollection>("orientation", paramHelp[2], ORIENTATION );
  addParameter<bool>("orthogonal", paramHelp[3], "true" );
  addParameter<float>("layer spacing", paramHelp[4], "64." );
  addParameter<float>("node spacing", paramHelp[5], "18." );
  addParameter<bool>("bounding circles", paramHelp[6], "false");
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
  if (graph->outdeg(n)==0){
    list<LR> *result = new list<LR>();
    LR tmpLR;
    tmpLR.L = -sizes->getNodeValue(n).getW()/2.;
    tmpLR.R = +sizes->getNodeValue(n).getW()/2.;
    tmpLR.size = 1;
    (*p)[n] = 0;
    result->push_front(tmpLR);
    return (result);
  }
  else {
    Iterator<edge> *it;
    it=graph->getOutEdges(n);
      
    edge ite = it->next();
    node itn = graph->target(ite);
      
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
      itn=graph->target(ite);
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
    tmpLR.L = posFather - sizes->getNodeValue(n).getW()/2.;
    tmpLR.R = posFather + sizes->getNodeValue(n).getW()/2.;
    tmpLR.size = 1;
    leftTree->push_front(tmpLR);

    list<double>::const_iterator itI = childPos.begin();
    forEach(ite, graph->getOutEdges(n)) {
      itn = graph->target(ite);
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
    if (maxSize[level] < sizes->getNodeValue(n).getH()) {
      maxSize[level]=sizes->getNodeValue(n).getH();
    }
  }
  else
    maxSize[level]=sizes->getNodeValue(n).getH();
  
  if (useLength) {
    edge ite;
    forEach(ite, graph->getOutEdges(n)) {
      node itn=graph->target(ite);
      TreeLevelSizing(itn,maxSize,level+(lengthMetric->getEdgeValue(ite)), levels);
    }
  }
  else {
    node itn;
    forEach(itn, graph->getOutNodes(n)) {
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
  tmpCoord.set(x+(*p)[n], - (y+maxLevelSize[level]/2.), 0);
  layoutResult->setNodeValue(n,tmpCoord);
  if (useLength) {
    edge ite;
    forEach(ite, graph->getOutEdges(n)) {
      node itn = graph->target(ite);
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
    forEach(itn, graph->getOutNodes(n)) {
      calcLayout(itn,p, x+(*p)[n], y+maxLevelSize[level]+spacing, 
		 level+1, maxLevelSize);
    }
  }
  //cerr << "TreeReingoldAndTilfordExtended::EndCalcLayout" << endl;
}
//===============================================================
bool TreeReingoldAndTilfordExtended::run() {
  stdext::hash_map<node,double> posRelative;

  layoutResult->setAllEdgeValue(vector<Coord>(0));
  sizes = graph->getProperty<SizeProperty>("viewSize");
  orientation = "horizontal";
  lengthMetric = 0;
  spacing = 64.0;
  nodeSpacing = 18.0;
  ortho = true;
  useLength = false;
  bool boundingCircles = false;
  if (dataSet!=0) {
    useLength = dataSet->get("edgeLength", lengthMetric);
    dataSet->get("nodeSize", sizes);
    dataSet->get("orthogonal", ortho);
    dataSet->get("layer spacing", spacing);
    dataSet->get("node spacing", nodeSpacing);
    dataSet->get("bounding circles", boundingCircles);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }

  //use bounding circles if specified
  if (boundingCircles) {
    node n;
    SizeProperty *circleSizes = 
      graph->getLocalProperty<SizeProperty> ("bounding circle sizes");
    forEach(n, graph->getNodes()) {
      Size boundCircle = sizes->getNodeValue (n);
      double diam = 2*sqrt (boundCircle.getW()*boundCircle.getW()/4.0 +
			    boundCircle.getH()*boundCircle.getH()/4.0);
      circleSizes->setNodeValue (n, Size (diam, diam, 1.0));
    }//end forEach
    sizes = circleSizes;
  }//end if

  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      Size tmp = sizes->getNodeValue(n);
      sizes->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }
  //===========================================================

  node startNode;
  tlp::getSource(graph, startNode);

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
    forEach(e, graph->getEdges()) {
      LineType::RealType tmp;
      node src = graph->source(e);
      node tgt = graph->target(e);
      Coord srcPos = layoutResult->getNodeValue(src);
      Coord tgtPos = layoutResult->getNodeValue(tgt);
      double y = levelCoord[levels[tgt]-1];
      tmp.push_back(Coord(srcPos[0], -y, 0));
      tmp.push_back(Coord(tgtPos[0], -y, 0));
      layoutResult->setEdgeValue(e, tmp);
    }
    
    if (orientation == "horizontal") {
      forEach(e, graph->getEdges()) {
	LineType::RealType tmp = layoutResult->getEdgeValue(e);
	LineType::RealType tmp2;
	tmp2.push_back(Coord(-tmp[0][1], tmp[0][0], tmp[0][2]));
	tmp2.push_back(Coord(-tmp[1][1], tmp[1][0], tmp[1][2]));
	layoutResult->setEdgeValue(e, tmp2);
      }
    }
  }

  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      Size  tmp = sizes->getNodeValue(n);
      sizes->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      Coord tmpC = layoutResult->getNodeValue(n);
      layoutResult->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
  }

  if (boundingCircles)
    graph->delLocalProperty ("bounding circle sizes");

  return true;
}
//=============================================================================
bool TreeReingoldAndTilfordExtended::check(string &erreurMsg) {
  if (TreeTest::isTree(graph)) {
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
