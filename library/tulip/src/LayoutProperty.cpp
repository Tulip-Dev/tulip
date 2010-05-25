//-*-c++-*-
#include <cmath>
#include <float.h>
#include <fstream>
#include "tulip/LayoutProperty.h"
#include "tulip/Coord.h"
#include "tulip/PluginContext.h"

using namespace std;
using namespace tlp;

inline double sqr(double x){return (x*x);}

// define a specific MetaValueCalculator
class LayoutMetaValueCalculator
  :public AbstractLayoutProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractLayoutProperty* layout,
			node mN, Graph* sg, Graph*) {
    switch(sg->numberOfNodes()) {
    case 0:
      layout->setNodeValue(mN, Coord(0, 0, 0));
      return;
    case 1:
      layout->setNodeValue(mN,((LayoutProperty *)layout)->getMax(sg));
      return;
    default:
    // between the min and max computed values
    layout->setNodeValue(mN,
			 (((LayoutProperty *)layout)->getMax(sg) +
			  ((LayoutProperty *)layout)->getMin(sg)) / 2.0);
    }
  }
};

static LayoutMetaValueCalculator mvLayoutCalculator;

//======================================================
LayoutProperty::LayoutProperty(Graph *sg, std::string n, bool updateOnEdgeReversal):AbstractLayoutProperty(sg, n) {
  minMaxOk[(unsigned long)graph]=false;
  // the property observes itself; see beforeSet... methods
  addPropertyObserver(this);
  // but do not need to be in observables
  PropertyObserver::removeObservable(this);
  // if needed the property observes the graph (see reverseEdge)
  if (updateOnEdgeReversal)
    graph->addGraphObserver(this);
  // set default MetaValueCalculator
  setMetaValueCalculator(&mvLayoutCalculator);
}
//======================================================
LayoutProperty::~LayoutProperty() {
  if (graph)
    graph->removeGraphObserver(this);
}
//======================================================
Coord LayoutProperty::getMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOk.find(sgi)==minMaxOk.end()) minMaxOk[sgi]=false;
  if (!minMaxOk[sgi]) computeMinMax(sg);
  return max[sgi];
}
//======================================================
Coord  LayoutProperty::getMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOk.find(sgi)==minMaxOk.end()) minMaxOk[sgi]=false;
  if (!minMaxOk[sgi]) computeMinMax(sg);
  return min[sgi];
}
//=================================================================================
#define X_ROT 0
#define Y_ROT 1
#define Z_ROT 2
static void rotateVector(Coord &vec, double alpha, int rot) {
  Coord backupVec(vec);
  double aRot =  2.0*M_PI * alpha / 360.0;
  float cosA = cos(aRot);
  float sinA = sin(aRot);
  switch(rot) {
  case Z_ROT:
    vec[0] = backupVec[0]*cosA - backupVec[1]*sinA;
    vec[1] = backupVec[0]*sinA + backupVec[1]*cosA;
    break;
  case Y_ROT:
    vec[0] = backupVec[0]*cosA + backupVec[2]*sinA;
    vec[2] = backupVec[2]*cosA - backupVec[0]*sinA;
    break;
  case X_ROT:
    vec[1] = backupVec[1]*cosA - backupVec[2]*sinA;
    vec[2] = backupVec[1]*sinA + backupVec[2]*cosA;
    break;
  }
}
//=================================================================================
void LayoutProperty::rotate(const double& alpha, int rot, Iterator<node> *itN, Iterator<edge> *itE) {
  Observable::holdObservers();
  while (itN->hasNext()) {
    node itn = itN->next();
    Coord tmpCoord(getNodeValue(itn));
    rotateVector(tmpCoord, alpha, rot);
    setNodeValue(itn,tmpCoord);
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (!getEdgeValue(ite).empty()) {
      LineType::RealType tmp = getEdgeValue(ite);
      LineType::RealType::iterator itCoord;
      itCoord=tmp.begin();
      while(itCoord!=tmp.end()) {
	rotateVector(*itCoord, alpha, rot);
	++itCoord;
      }
      setEdgeValue(ite,tmp);
    }
  }
  Observable::unholdObservers();
}
//=================================================================================
void LayoutProperty::rotateX(const double& alpha,  Iterator<node> *itN, Iterator<edge> *itE) {
  rotate(alpha, X_ROT, itN, itE);
}
//=================================================================================
void LayoutProperty::rotateY(const double& alpha,  Iterator<node> *itN, Iterator<edge> *itE) {
  rotate(alpha, Y_ROT, itN, itE);
}
//=================================================================================
void LayoutProperty::rotateZ(const double& alpha,  Iterator<node> *itN, Iterator<edge> *itE) {
  rotate(alpha, Z_ROT, itN, itE);
}
//=================================================================================
void LayoutProperty::rotateZ(const double& alpha, Graph *sg) {
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Iterator<node> *itN = sg->getNodes();
  Iterator<edge> *itE = sg->getEdges();
  rotateZ(alpha, itN, itE);
  delete itN;
  delete itE;
}
//=================================================================================
void LayoutProperty::scale(const tlp::Vector<float,3>& v, Iterator<node> *itN, Iterator<edge> *itE) {
  Observable::holdObservers();
  while (itN->hasNext()) {
    node itn = itN->next();
    Coord tmpCoord(getNodeValue(itn));
    tmpCoord *= *(Coord*)&v;
    setNodeValue(itn,tmpCoord);
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (!getEdgeValue(ite).empty()) {
      LineType::RealType tmp = getEdgeValue(ite);
      LineType::RealType::iterator itCoord;
      itCoord=tmp.begin();
      while(itCoord!=tmp.end()) {
	*itCoord *= *(Coord*)&v;
	++itCoord;
      }
      setEdgeValue(ite,tmp);
    }
  }
  Observable::unholdObservers();
}
//=================================================================================
void LayoutProperty::scale(const tlp::Vector<float,3>& v, Graph *sg) {
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Iterator<node> *itN = sg->getNodes();
  Iterator<edge> *itE = sg->getEdges();
  scale(v, itN, itE);
  delete itN;
  delete itE;
}
//=================================================================================
void LayoutProperty::translate(const tlp::Vector<float,3>& v, Iterator<node> *itN, Iterator<edge> *itE) {
  Observable::holdObservers();
  while (itN->hasNext()) {
    node itn = itN->next();
    Coord tmpCoord(getNodeValue(itn));
    tmpCoord += *(Coord*)&v;
    setNodeValue(itn,tmpCoord);
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (!getEdgeValue(ite).empty()) {
      LineType::RealType tmp = getEdgeValue(ite);
      LineType::RealType::iterator itCoord;
      itCoord=tmp.begin();
      while(itCoord!=tmp.end()) {
	*itCoord += *(Coord*)&v;
	++itCoord;
      }
      setEdgeValue(ite,tmp);
    }
  }
  resetBoundingBox();
  notifyObservers();
  Observable::unholdObservers();
}
//=================================================================================
void LayoutProperty::translate(const tlp::Vector<float,3>& v, Graph *sg) {
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Iterator<node> *itN = sg->getNodes();
  Iterator<edge> *itE = sg->getEdges();
  translate(v, itN, itE);
  delete itN;
  delete itE;
}
//=================================================================================
void LayoutProperty::center(Graph *sg) {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__  << endl;
#endif
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Observable::holdObservers();
  Coord tr=getMax(sg)+getMin(sg);
  tr /= -2.0;
  translate(tr, sg);
  resetBoundingBox();
  notifyObservers();
  Observable::unholdObservers();
}
//=================================================================================
void LayoutProperty::normalize(Graph *sg) {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << endl;
#endif
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Observable::holdObservers();
  center();
  double dtmpMax = 1.0;
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext())  {
    node itn=itN->next();
    const Coord& tmpCoord = getNodeValue(itn);
    dtmpMax  = std::max(dtmpMax, sqr(tmpCoord[0])+sqr(tmpCoord[1])+sqr(tmpCoord[2]));
  } delete itN;
  dtmpMax = 1.0 / sqrt(dtmpMax);
  scale(Coord(dtmpMax,dtmpMax,dtmpMax), sg);  
  resetBoundingBox();
  notifyObservers();
  Observable::unholdObservers();
}
//=================================================================================
void LayoutProperty::perfectAspectRatio() {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << endl;
#endif
  if (graph->numberOfNodes()==0) return;
  Observable::holdObservers();
  center();
  double scaleX,scaleY,scaleZ;
  double deltaX,deltaY,deltaZ;
  deltaX = (double )getMax()[0]-(double )getMin()[0];
  deltaY = (double )getMax()[1]-(double )getMin()[1];
  deltaZ = (double )getMax()[2]-(double )getMin()[2];
  double delta = std::max(deltaX , deltaY);
  delta = std::max(delta, deltaZ);
  if (delta<0.001) return;
  if (deltaX<0.001) deltaX=delta;
  if (deltaY<0.001) deltaY=delta;
  if (deltaZ<0.001) deltaZ=delta;
  scaleX = delta / deltaX;
  scaleY = delta / deltaY;
  scaleZ = delta / deltaZ;
  scale(Coord(scaleX,scaleY,scaleZ));
  notifyObservers();
  Observable::unholdObservers();
}

void maxV(Coord &res, const Coord &cmp){
  for (unsigned int i = 0; i<3; ++i) {
    res[i] = std::max(res[i], cmp[i]);
  }
}
void minV(Coord &res, const Coord &cmp){
  for (unsigned int i = 0; i<3; ++i) {
    res[i] = std::min(res[i], cmp[i]);
  }
}
//=================================================================================
void LayoutProperty::computeMinMax(Graph *sg) {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << endl;
#endif
  Coord maxT(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  Coord minT(FLT_MAX, FLT_MAX, FLT_MAX);

  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  if  (itN->hasNext()) {
    node itn=itN->next();
    const Coord& tmpCoord=getNodeValue(itn);
    maxV(maxT, tmpCoord);
    minV(minT, tmpCoord);
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    const Coord& tmpCoord=getNodeValue(itn);
    maxV(maxT, tmpCoord);
    minV(minT, tmpCoord);
  } delete itN;
  Iterator<edge> *itE=sg->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    LineType::RealType::const_iterator itCoord;
    for (itCoord=getEdgeValue(ite).begin();itCoord!=getEdgeValue(ite).end();++itCoord) {
      const Coord& tmpCoord = *itCoord;
      maxV(maxT, tmpCoord);
      minV(minT, tmpCoord);
    }
  } delete itE;
  unsigned int sgi = sg->getId();
  minMaxOk[sgi]=true;  
  min[sgi] = minT;
  max[sgi] = maxT;
  //  cerr << "LayoutProperty::computeMinMax end" << endl;
}
//=================================================================================
void LayoutProperty::clone_handler(AbstractProperty<PointType,LineType> &proxyC) {
  if (typeid(this)==typeid(&proxyC)) {
    LayoutProperty *proxy=(LayoutProperty *)&proxyC;
    minMaxOk = proxy->minMaxOk;
    min = proxy->min;
    max = proxy->max;
  }
}
//=================================================================================
void LayoutProperty::resetBoundingBox() {
  minMaxOk.clear();
  min.clear();
  max.clear();
}
//================================================================================
void LayoutProperty::beforeSetNodeValue(PropertyInterface*, const node) {
  resetBoundingBox();
}
void LayoutProperty::beforeSetEdgeValue(PropertyInterface*, const edge) {
  resetBoundingBox();
}
void LayoutProperty::beforeSetAllNodeValue(PropertyInterface*) {
  resetBoundingBox();
}
void LayoutProperty::beforeSetAllEdgeValue(PropertyInterface*) {
  resetBoundingBox();
}
//================================================================================
void LayoutProperty::reverseEdge(Graph *sg, const edge e) {
  assert(sg == graph);
  std::vector<Coord> bends = getEdgeValue(e);
  // reverse bends if needed
  if (bends.size() > 1) {
    unsigned int halfSize = bends.size()/2;
    for (unsigned int i = 0, j = bends.size() - 1; i < halfSize; ++i, j--) {
      Coord tmp = bends[i];
      bends[i] = bends[j];
      bends[j] = tmp;
    }
    setEdgeValue(e, bends);
  }
}
//=================================================================================
double LayoutProperty::averageAngularResolution(const Graph *sg) const {
  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  double result=0;
  while (itN->hasNext()) {
    result+=averageAngularResolution(itN->next(),sg);
  } delete itN;
  return result/(double)sg->numberOfNodes();
}
//=================================================================================
#ifndef DOXYGEN_NOTFOR_DEVEL
struct AngularOrder {
  bool operator() (const Coord &c1, const Coord &c2) {
    //if the vectors have not the same direction on y-coordiantes
    //the result is direct.
    if (c1[1]>=0 && c2[1]<0) return false;  
    if (c2[1]>=0 && c1[1]<0) return true; 
    //If the vectors have the same size on the y-coordinates, we compare
    //their x-coordinates
    if (c2[1]>=0 && c1[1]>=0) 
      return c1[0]>c2[0];
    else
      return c1[0]<c2[0];
  }
};
#endif // DOXYGEN_NOTFOR_DEVEL
//=================================================================================
vector<double> LayoutProperty::angularResolutions(const node n, const Graph *sg) const {
  vector<double> result;
  if (sg == 0) sg = graph;
  double degree = sg->deg(n);
  if (sg->deg(n)==0) return result;
  if (sg->deg(n)==1) {
    result.push_back(0.0);
    return result;
  }
  //===========
  list<Coord> adjCoord;
  //Extract all adjacent edges, the bends are taken
  //into account.
  Iterator<edge> *itE=sg->getInOutEdges(n);
  for (unsigned int i=0;itE->hasNext();++i) {
    edge ite=itE->next();
    if (getEdgeValue(ite).size()>0) {
      if (sg->source(ite)==n)
	adjCoord.push_back(getEdgeValue(ite).front());
      else
	adjCoord.push_back(getEdgeValue(ite).back());
    }
    else {
      adjCoord.push_back(getNodeValue(sg->opposite(ite,n)));
    }
  } delete itE;
  
  //Compute normalized vectors associated to incident edges.
  const Coord& center=getNodeValue(n);
  list<Coord>::iterator it;
  for (it=adjCoord.begin();it!=adjCoord.end();++it) {
    (*it)-=center;
    (*it)/=(*it).norm();
  }
  //Sort the vector to compute angles between two edges
  //Correctly.
  adjCoord.sort(AngularOrder());
  //Compute the angles
  it=adjCoord.begin();
  Coord first=(*it);
  Coord current=first;
  ++it;

  int stop=2;
  for (;stop>0;) {
    Coord next=*it;
    double cosTheta = current.dotProduct(next);//current * next;
    double sinTheta = (current^next)[2];
    if (cosTheta+0.0001>1) cosTheta-=0.0001;
    if (cosTheta-0.0001<-1) cosTheta+=0.0001;
    if (sinTheta+0.0001>1) sinTheta-=0.0001;
    if (sinTheta-0.0001<-1) sinTheta+=0.0001;
    if (sinTheta>=0) {
      result.push_back(2.0*M_PI/degree-acos(cosTheta));
    }
    else {
      result.push_back(2.0*M_PI/degree-(2.0*M_PI-acos(cosTheta)));
    }
    current=next;
    ++it;
    if (stop<2) stop=0;
    if (it==adjCoord.end()){
      it=adjCoord.begin();
      stop--;
    }
  }
  return result;
}
//=================================================================================
double LayoutProperty::averageAngularResolution(const node n, const Graph *sg) const {
    vector<double> tmp(angularResolutions(n, sg));
    if (tmp.empty()) return 0.;
    double sum = 0.;
    vector<double>::const_iterator it = tmp.begin();
    for (;it != tmp.end(); ++it)
        sum += *it;
    return sum / double(tmp.size());
}
//=================================================================================
double LayoutProperty::edgeLength(const edge e) const {
  pair<node, node> eEnds = graph->ends(e);
  Coord start=getNodeValue(eEnds.first);
  const Coord& end=getNodeValue(eEnds.second);
  double result=0;
  const vector<Coord> & tmp=getEdgeValue(e);
  for (unsigned int i=0;i<tmp.size();++i) {
    result+=(tmp[i]-start).norm();
    start=tmp[i];
  }
  result+=(end-start).norm();
  return result;
}
//=================================================================================
unsigned int LayoutProperty::crossingNumber() const {
  cerr << "!!! Warning: Not Implemented function :" << endl;
  cerr << __PRETTY_FUNCTION__ << endl;
  return 0;
}
//=================================================================================
PropertyInterface* LayoutProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  LayoutProperty * p = g->getLocalProperty<LayoutProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* CoordVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  CoordVectorProperty * p = g->getLocalProperty<CoordVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
