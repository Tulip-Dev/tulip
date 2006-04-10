#include <tulip/MutableContainer.h>
#include <tulip/Rectangle.h>
#include "SpringElectrical.h"

LAYOUTPLUGIN(SpringElectrical,"Spring_Electrical","David Auber","14/01/2002","Alpha","0","1");

using namespace std;

inline double sqr(double x) {
  return (x*x);
}
//================================================================================
SpringElectrical::SpringElectrical(const PropertyContext &context):Layout(context){
}
//================================================================================
SpringElectrical::~SpringElectrical() {
}
//================================================================================
Coord SpringElectrical::repulsiveForces(node n) {
  node u = n;
  Coord uCoord = layoutProxy->getNodeValue(u);
  Coord result(0,0,0);
  Iterator<node> *itN = superGraph->getNodes();
  while(itN->hasNext()) {
    node v = itN->next();
    if (u == v) continue;
    Coord uv = layoutProxy->getNodeValue(v)-uCoord;
    double dist = uv.norm();
    if (dist > 1.0E-3) {
      uv /= dist;
      dist -= (sizeNorm.get(u.id) + sizeNorm.get(v.id));
      if (dist > 5.0) continue;
      if (dist < 1.E-3)
	uv *= -(1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id));
      else
	uv *= - k2 /sqr(dist);
    } else {
      uv[rand()%2] += 0.01 - (0.02 * (double(rand()%2)));
    }
    result += uv;
  } delete itN;
  return result;
}
//================================================================================
Coord SpringElectrical::attractiveForces(node n) {
  node nu = n;
  Coord u = layoutProxy->getNodeValue(nu);
  Coord result(0,0,0);
  Iterator<node> *itN=superGraph->getInOutNodes(n);
  while(itN->hasNext()) {
    node nv = itN->next();
    Coord v = layoutProxy->getNodeValue(nv);
    Coord uv = v-u;
    double length = 1.0 + (sizeNorm.get(nu.id) + sizeNorm.get(nv.id));
    double dist = uv.norm();
    if (dist < length) continue; //force only attractive force
    if (dist > 1e-3) {
      uv /= dist;
      uv *= 1.0/k * (dist - length); //Hooke's law
      //uv *= 1.0/k * log( dist / length); //Eades's law
    }
    else
      uv[rand()%2] += 0.01 - (0.02 * (double(rand()%2)));
    result += uv;
  } delete itN;
  return result;
}
//================================================================================
static const double MAXFORCE = .0;
Coord maxForce(Coord force, double max) {
  Coord result(force);
  float norm = result.norm();
  if (norm > max) {
    result /= norm;
    result *= max;
  }
  return result;
}
//================================================================================  
bool SpringElectrical::run() {
  int iterations = superGraph->numberOfNodes();
  if (iterations < 500) iterations = 500;// iterations = iterations >? 500;
  inputSelection = superGraph->getProperty<SelectionProxy>("viewSelection");
  inputSize = superGraph->getProperty<SizesProxy>("viewSize");
  LayoutProxy *inputLayout = superGraph->getProperty<LayoutProxy>("viewLayout");
  
  Iterator<node> *itN = superGraph->getNodes();
  while(itN->hasNext()){
    node itn = itN->next();
    layoutProxy->setNodeValue(itn,inputLayout->getNodeValue(itn));
    sizeNorm.set(itn.id,inputSize->getNodeValue(itn).norm()/2.0);
  } delete itN;

  k = 5.0;
  k2 = 2.0*k;
  double maxforce = log(double(superGraph->numberOfNodes()));
  double deltaForce = maxforce / iterations;

  for (int count = 0; count < iterations; ++count) {
    if (pluginProgress->progress(count,iterations)!=TLP_CONTINUE) break;
    itN = superGraph->getNodes();
    maxforce -= deltaForce;
    if (maxforce < 0.2) maxforce = 0.2;
    while(itN->hasNext()) {
      node itn = itN->next();
      if (inputSelection->getNodeValue(itn)) continue;
      Coord move = maxForce(attractiveForces(itn) + repulsiveForces(itn), maxforce); //prevent too big jumps
      Coord tmp  = layoutProxy->getNodeValue(itn);
      layoutProxy->setNodeValue(itn,tmp + move);
      /* New test to preserve embedding, need more work
	for (unsigned int i=0; i<10; ++i) {
	if (!checkEdgeIntersection(itn, move + move * ((1.0+sizeNorm.get(itn.id)) / move.norm()))) {
	layoutProxy->setNodeValue(itn,tmp + move);
	break;
	}
	move /= 3.0;
	}
      */
      
    } delete itN;
  }
  return pluginProgress->state()!=TLP_CANCEL;
}
//================================================================================
bool SpringElectrical::check(string &erreurMsg){
  erreurMsg="";
  return true;
}
//================================================================================
void SpringElectrical::reset() {
}
//================================================================================
bool intersect2D(const Coord &a, const Coord &b, const Coord &c, const Coord &d) {
  Coord AB = b - a;
  Coord AC = c - a;
  Coord AD = d - a;
  float ABAC = (AB^AC)[2];
  ABAC /= fabs(ABAC);
  float ABAD = (AB^AD)[2];
  ABAD /= fabs(ABAD);
  if (fabs(ABAC -  ABAD) < 1E-6 ) return false;
  Coord CD = d - c;
  Coord CA = a - c;
  Coord CB = b - c;  
  float CDCA = (CD^CA)[2];
  CDCA /= fabs(CDCA);
  float CDCB = (CD^CB)[2];
  CDCB /= fabs(CDCB);
  if (fabs(CDCA - CDCB) < 1E-6 ) return false;
  return true;
}
//================================================================================
bool SpringElectrical::checkEdgeIntersection(const node n, const Coord & move) {
  Coord a = layoutProxy->getNodeValue(n);
  Coord b = a + move;
  Iterator<edge> *itE = superGraph->getEdges();
  while(itE->hasNext()) {
    edge e = itE->next();
    node src = superGraph->source(e);
    node tgt = superGraph->target(e);
    if (src == n || tgt == n) continue;
    Coord c = layoutProxy->getNodeValue(src);
    Coord d = layoutProxy->getNodeValue(tgt);
    if (intersect2D(a, b, c, d)) {
      delete itE;
      return true;
    }
  } delete itE;
  itE = superGraph->getInOutEdges(n);
  while(itE->hasNext()) {
    edge e = itE->next();
    node src = superGraph->source(e);
    node tgt = superGraph->target(e);
    Coord a = layoutProxy->getNodeValue(src);
    Coord b = layoutProxy->getNodeValue(tgt);
    Iterator<node> *itN = superGraph->getNodes();
    while(itN->hasNext()) {
      node currentNode = itN->next();
      if((currentNode == src) || (currentNode == tgt))
	continue;
      Coord c = layoutProxy->getNodeValue(currentNode);
      Coord d = c - move;
      if(intersect2D(a, b, c, d)) {
	delete itE;
	delete itN;
	return true;
      }
    } delete itN;
    
  } delete itE;
  return false;
}
