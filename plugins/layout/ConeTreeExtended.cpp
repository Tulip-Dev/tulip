#include <map>
#include <cmath>
#include <climits>
#include <tulip/Circle.h>
#include <tulip/TreeTest.h>
#include <tulip/ForEach.h>
#include <tulip/Node.h>

#include "ConeTreeExtended.h"

LAYOUTPLUGINOFGROUP(ConeTreeExtended,"Cone Tree","David Auber","01/04/2001","Stable","1.0","Tree");

using namespace std;
using namespace tlp;
using namespace stdext;

//===============================================================
float sqr(float x) {
  return x*x;
}
//===============================================================
float minRadius(float radius1,float alpha1,float radius2,float alpha2) {
  return sqrt(sqr(radius1+radius2)/(sqr(cos(alpha1)-cos(alpha2)) + sqr(sin(alpha1)-sin(alpha2)))); 
}
//===============================================================
void ConeTreeExtended::computeLayerSize(node n, unsigned int level) {
  if (levelSize.size() < level+1 )
    levelSize.push_back(0);
  levelSize[level] = std::max(levelSize[level], nodeSize->getNodeValue(n)[1]); 
  node i;
  forEach(i, graph->getOutNodes(n)) {
    computeLayerSize(i, level + 1);
  }
}
//===============================================================
void ConeTreeExtended::computeYCoodinates(node root) {
  levelSize.clear();
  yCoordinates.clear();
  computeLayerSize(root, 0);
  yCoordinates.resize(levelSize.size());
  yCoordinates[0] = 0;
  for (unsigned int i = 1; i < levelSize.size(); ++i) {
    yCoordinates[i] = yCoordinates[i-1] + levelSize[i] / 2.0 + levelSize[i-1] / 2.0;
  }
}
//===============================================================
double ConeTreeExtended::treePlace3D(node n, 
				     hash_map<node,double> *posRelX, 
				     hash_map<node,double> *posRelY) {
  (*posRelX)[n]=0;
  (*posRelY)[n]=0;
  if (graph->outdeg(n)==0) {
    Coord tmp = nodeSize->getNodeValue(n);
    return sqrt(tmp[0]*tmp[0] + tmp[2]*tmp[2])/2.0;
  }
  
  if (graph->outdeg(n)==1) {
    Iterator<node> *itN=graph->getOutNodes(n);
    node itn=itN->next(); 
    delete itN;
    return treePlace3D(itn,posRelX,posRelY);
  }
  
  double sumRadius=0;
  double maxRadius=0;
  float newRadius;

  vector<double> subCircleRadius(graph->outdeg(n));
  Iterator<node> *itN=graph->getOutNodes(n);
  for (int i=0; itN->hasNext(); ++i)  {
    node itn = itN->next();
    subCircleRadius[i] = treePlace3D(itn,posRelX,posRelY);
    sumRadius += 2*subCircleRadius[i];
    maxRadius = std::max(maxRadius , subCircleRadius[i]);
  }delete itN;
  
  double radius=sumRadius/(2*M_PI);

  //treat angles
  vector<double> vangles(subCircleRadius.size());
  double angle=0;
  vangles[0]=0;
  for (unsigned int i=1;i<subCircleRadius.size();++i) {
    angle+=(subCircleRadius[i-1]+subCircleRadius[i])/radius;
    vangles[i]=angle;
  }
  
  // compute new radius
  newRadius=0;
  for (unsigned int i=0;i<subCircleRadius.size()-1;++i) {
    for (unsigned int j=i+1;j<subCircleRadius.size();++j) {
      newRadius = std::max(newRadius , minRadius(subCircleRadius[i],vangles[i],subCircleRadius[j],vangles[j])); 
    }
  }
  if (newRadius==0) newRadius=radius;

  //compute Circle Hull
  vector<tlp::Circle<float> > circles(subCircleRadius.size());
  for (unsigned int i=0;i<subCircleRadius.size();++i) {
    circles[i][0]=newRadius*cos(vangles[i]);
    circles[i][1]=newRadius*sin(vangles[i]);
    circles[i].radius=subCircleRadius[i];
  }
  tlp::Circle<float> circleH=tlp::enclosingCircle(circles);

  //Place relative position
  itN = graph->getOutNodes(n);
  for (unsigned int i=0; i<subCircleRadius.size(); ++i) {
    node itn = itN->next();
    (*posRelX)[itn]=newRadius*cos(vangles[i])-circleH[0];
    (*posRelY)[itn]=newRadius*sin(vangles[i])-circleH[1];
  } delete itN;
  return circleH.radius;
}
//===============================================================
void ConeTreeExtended::calcLayout(node n, hash_map<node,double> *px, hash_map<node,double> *py,
			double x, double y, int level) {
  layoutResult->setNodeValue(n,Coord(x+(*px)[n], - yCoordinates[level],y+(*py)[n]));
  node itn;
  forEach(itn, graph->getOutNodes(n)) {
    calcLayout(itn, px, py, x+(*px)[n], y+(*py)[n], level + 1);
  }
}
//===============================================================
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
    //Orientation
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "horizontal" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the orientation of the drawing"	\
    HTML_HELP_CLOSE()
  };
}
#define ORIENTATION "vertical;horizontal;"
//===============================================================
ConeTreeExtended::ConeTreeExtended(const PropertyContext &context):LayoutAlgorithm(context) {
  addParameter<SizeProperty>("nodeSize",paramHelp[0],"viewSize");
  addParameter<StringCollection> ("orientation", paramHelp[1], ORIENTATION );
}
//===============================================================
ConeTreeExtended::~ConeTreeExtended() {}
//===============================================================
bool ConeTreeExtended::run() {
  nodeSize = graph->getProperty<SizeProperty>("viewSize");
  string orientation = "vertical";
  if (dataSet!=0) {
    dataSet->get("nodeSize", nodeSize);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }
  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      Size tmp = nodeSize->getNodeValue(n);
      nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }
  //===========================================================
  layoutResult->setAllEdgeValue(vector<Coord>(0));
  hash_map<node,double> posX;
  hash_map<node,double> posY;
  node root;
  tlp::getSource(graph, root);
  treePlace3D(root,&posX,&posY);
  computeYCoodinates(root);
  calcLayout(root,&posX,&posY,0,0,0);
  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      Size  tmp = nodeSize->getNodeValue(n);
      nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      Coord tmpC = layoutResult->getNodeValue(n);
      layoutResult->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
  }
  return true;
}
//===============================================================
bool ConeTreeExtended::check(string &erreurMsg) {
  if (TreeTest::isTree(graph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}
//===============================================================
void ConeTreeExtended::reset() {
}
//===============================================================
