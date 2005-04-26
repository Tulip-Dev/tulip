#include <map>
#include <cmath>
#include <climits>
#include <tulip/Circle.h>
#include <tulip/TreeTest.h>
#include "ConeTreeExtended.h"

LAYOUTPLUGIN(ConeTreeExtended,"Cone Tree Extended","David Auber","01/04/2001","Stable","1","0");

using namespace std;
using namespace stdext;

float sqr(float x) { return x*x;}

float minRadius(float radius1,float alpha1,float radius2,float alpha2) {
  return sqrt(sqr(radius1+radius2)/(sqr(cos(alpha1)-cos(alpha2))+sqr(sin(alpha1)-sin(alpha2)))); 
}

double ConeTreeExtended::treePlace3D(node n, 
				     hash_map<node,double> *posRelX, 
				     hash_map<node,double> *posRelY) {
  (*posRelX)[n]=0;
  (*posRelY)[n]=0;
  if (superGraph->outdeg(n)==0) {return 1.0;}

  if (superGraph->outdeg(n)==1) {
    Iterator<node> *itN=superGraph->getOutNodes(n);
    node itn=itN->next(); 
    delete itN;
    return treePlace3D(itn,posRelX,posRelY);
  }

  double sumRadius=0;
  double maxRadius=0;
  double newRadius;

  vector<double> subCircleRadius(superGraph->outdeg(n));
  Iterator<node> *itN=superGraph->getOutNodes(n);
  for (int i=0;itN->hasNext();++i)  {
    node itn = itN->next();
    subCircleRadius[i] = treePlace3D(itn,posRelX,posRelY);
    sumRadius += 2*subCircleRadius[i];
    maxRadius = maxRadius >? subCircleRadius[i];
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
      newRadius = newRadius >? minRadius(subCircleRadius[i],vangles[i],subCircleRadius[j],vangles[j]); 
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
  itN=superGraph->getOutNodes(n);
  for (unsigned int i=0;i<subCircleRadius.size();++i) {
    node itn = itN->next();
    (*posRelX)[itn]=newRadius*cos(vangles[i])-circleH[0];
    (*posRelY)[itn]=newRadius*sin(vangles[i])-circleH[1];
  }delete itN;
  return circleH.radius;
}

void ConeTreeExtended::calcLayout(node n, hash_map<node,double> *px, hash_map<node,double> *py,
			double x, double y, int level) {
  layoutProxy->setNodeValue(n,Coord(x+(*px)[n],level,y+(*py)[n]));
  Iterator<node> *it=superGraph->getOutNodes(n);
  for (;it->hasNext();) {
    node itn;
    itn=it->next();
    calcLayout(itn,px,py,x+(*px)[n],y+(*py)[n] , level+2);
  } delete it;
}

ConeTreeExtended::ConeTreeExtended(const PropertyContext &context):Layout(context) {}

ConeTreeExtended::~ConeTreeExtended() {}

bool ConeTreeExtended::run() {
  layoutProxy->setAllEdgeValue(vector<Coord>(0));
  hash_map<node,double> posX;
  hash_map<node,double> posY;
  superGraph->getLocalProperty<SizesProxy>("viewSize")->setAllNodeValue(Size(1,1,1));
  superGraph->getLocalProperty<SizesProxy>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  node startNode;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (superGraph->indeg(itn)==0) {
      startNode=itn;
      break;
    }
  }delete itN;  
  treePlace3D(startNode,&posX,&posY);
  calcLayout(startNode,&posX,&posY,0,0,0);
  return true;
}

bool ConeTreeExtended::check(string &erreurMsg) {
  if (TreeTest::isTree(superGraph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}

void ConeTreeExtended::reset() {}
