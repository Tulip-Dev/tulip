#include <tulip/MutableContainer.h>
#include <tulip/ForEach.h>
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

Coord maxForce(Coord force, double _max) {
  double max = std::max(0.001, _max);
  Coord result(force);
  float norm = result.norm();
  if (norm > max) {
    result /= norm;
    result *= max;
  }
  if (norm < 0.001)
    result *=0.0;
  return result;
}

//================================================================================
Coord repulsiveForces(node u, LayoutProxy *layout, SuperGraph * graph,   
		      const MutableContainer<double> &sizeNorm, double temperature, double maxforce) {
  Coord uCoord = layout->getNodeValue(u);
  Coord result(0,0,0);
  node v;
  forEach(v, graph->getNodes()) {
    if (u == v) continue;
    Coord uv = layout->getNodeValue(v) - uCoord;
    double dist = uv.norm();
    if (dist > 1.0E-3) {
      uv /= dist;
      dist -= (1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id));
      if (dist < 1.E-3)
	uv *= -(1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id)) + dist;//-(1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id));
      else
	uv *=  - sizeNorm.get(u.id) /sqr(dist);
    } else {
      uv[rand()%2] += 0.01 - (0.02 * (double(rand()%2)));
    }
    result += maxForce(uv, temperature * maxforce);
  }
  return result;
}
//================================================================================
Coord attractiveForces(node n, LayoutProxy *layout, SuperGraph * graph,   
		       const MutableContainer<double> &sizeNorm,
		       double temperature, double maxforce) {
  node nu = n;
  Coord u = layout->getNodeValue(nu);
  Coord result(0,0,0);
  node nv;
  forEach(nv, graph->getInOutNodes(n)) {
    Coord v = layout->getNodeValue(nv);
    Coord uv = v-u;
    double length = 1. +  (sizeNorm.get(nu.id) + sizeNorm.get(nv.id));
    double dist = uv.norm();
    if (dist < length) {
      //uv *= dist - length;
    }
    else {
      if (dist > 1e-3) {
	uv /= dist;
	uv *= pow((dist - length) , 2.0); //Hooke's law
	//uv *= pow((dist - length) , 1.0); //Hooke's law
	//uv *= length/1. * log( dist / length); //Eades's law
      }
      else
	uv[rand()%2] += 0.01 - (0.02 * (double(rand()%2)));
      
      result += maxForce(uv, temperature * maxforce);
    }
  }
  return result;
}
//================================================================================
bool overlap(node u, Coord &move, LayoutProxy *layout, SuperGraph * graph,
	     const MutableContainer<double> &sizeNorm ,
	     int &before) {
  Coord prev = layout->getNodeValue(u);
  Coord pos = layout->getNodeValue(u) + move;
  bool  overlap = false;
  node v;
  int nboverlapbefore = 0;
  int nboverlapafter  = 0;
  forEach(v, graph->getNodes()) {
    if (v==u) continue;
    Coord pos2 = layout->getNodeValue(v);
    if ((pos - pos2).norm() < (sizeNorm.get(u.id) + sizeNorm.get(v.id)))
      ++nboverlapafter;
    if ((prev - pos2).norm() < (sizeNorm.get(u.id) + sizeNorm.get(v.id)))
      ++nboverlapbefore;
  }
  before = nboverlapbefore;
  return nboverlapafter > nboverlapbefore;
}

//================================================================================  
bool SpringElectrical::run() {
  SuperGraph *graph = superGraph;
  
  int iterations = graph->numberOfNodes()/2;
  prevMove.setAll(Coord(0,0,0));
  if (iterations < 500) iterations = 500;// iterations = iterations >? 500;

  inputSelection = graph->getProperty<SelectionProxy>("viewSelection");
  inputSize = graph->getProperty<SizesProxy>("viewSize");
  LayoutProxy *inputLayout = graph->getProperty<LayoutProxy>("viewLayout");
  
  node n;
  forEach(n, graph->getNodes()) {
    layoutProxy->setNodeValue(n, inputLayout->getNodeValue(n));
    sizeNorm.set(n.id, inputSize->getNodeValue(n).norm()/2.0);
  }
  double maxforce   = sqrt(graph->numberOfNodes());
  double temperature = 1.0;
  bool overlapTest=false;

  for (int count = 0; count < iterations; ++count) {
    if (pluginProgress->progress(count,iterations)!=TLP_CONTINUE) break;
    node itn;
    forEach(itn, graph->getNodes()) {
      if (rand()%graph->numberOfNodes() < 100 && !inputSelection->getNodeValue(itn)) {
	Coord att = maxForce( attractiveForces(itn, layoutProxy, graph, sizeNorm, temperature, maxforce),  maxforce * temperature);
	Coord rep = maxForce( repulsiveForces(itn, layoutProxy, graph, sizeNorm, temperature, maxforce) ,  maxforce * temperature);
	Coord newMove = maxForce(att + rep, maxforce * temperature); //prevent too big jumps
	
	//test overlapping	
	if (!overlapTest) {
	  Coord tmp  = layoutProxy->getNodeValue(itn);
	  tmp[2] = 0;
	  layoutProxy->setNodeValue(itn, tmp + newMove);      
	  if (count==iterations - 1) {
	    overlapTest = true;
	    temperature = 1.0;
	    count = 0;
	  }
	}
	else {
	  //test overlapping
	  int i = 0;
	  bool _overlap;
	  int nbBefore = 0;
	  while( (_overlap = overlap(itn, newMove, layoutProxy, graph, sizeNorm, nbBefore)) && i<5 ) {
	    newMove /= 2.0;
	    ++i;
	    //	  cerr << "O";
	  }
	  if (!_overlap) {
	    Coord tmp  = layoutProxy->getNodeValue(itn);
	    tmp[2] = 0;
	    layoutProxy->setNodeValue(itn,  tmp + newMove);      
	  } 
	  else {
	    if (nbBefore>0) {
	      temperature += 0.5;
	      //	      cerr << ".";
	    }
	  }
	  //	  layout->setNodeValue(itn,  Coord(0,0,0));      
	}
      }
    }
    if (!overlapTest) {
      if (temperature > 0.05)
	temperature -= 1./(double)graph->numberOfNodes();
    }
    else {
      if (temperature > 0.01)
	temperature /= 1.01;
    }
  }
  return pluginProgress->state()!=TLP_CANCEL;
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
