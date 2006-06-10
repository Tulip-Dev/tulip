#include <tulip/MutableContainer.h>
#include <tulip/ForEach.h>
#include <tulip/Rectangle.h>
#include "SpringElectrical.h"

LAYOUTPLUGINOFGROUP(SpringElectrical,"Spring_Electrical","David Auber","14/01/2002","Alpha","0","1","Force Directed");

using namespace std;
//================================================================================
inline double sqr(double x) {
  return (x*x);
}

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN()			   \
    HTML_HELP_DEF( "type", "LayoutProxy" ) \
    HTML_HELP_BODY()							\
    "This layoutProxy is the one used for the initial positions of nodes" \
    HTML_HELP_CLOSE(),
    // property
    HTML_HELP_OPEN()			  \
    HTML_HELP_DEF( "type", "SizesProxy" ) \
    HTML_HELP_BODY()			     			\
    "This SizesProxy is the one used for the size of each node" \
    HTML_HELP_CLOSE(),
    // property
    HTML_HELP_OPEN()			      \
    HTML_HELP_DEF( "type", "SelectionProxy" ) \
    HTML_HELP_BODY()							\
    "This SelectionProxy enables to forbid the move of selected nodes" \
    HTML_HELP_CLOSE(),
    // Edge length
    HTML_HELP_OPEN()			   \
    HTML_HELP_DEF( "type", "MetricProxy" ) \
    HTML_HELP_BODY()							\
    "This metricProxy is the one used if need to have edge of a specific length " \
    "By default, the lenght of each edge is equal to 1."		\
    HTML_HELP_CLOSE(),
    // Use edge length
    HTML_HELP_OPEN()			      \
    HTML_HELP_DEF( "type", "Boolean" )	      \
    HTML_HELP_DEF( "values", "true / false" ) \
    HTML_HELP_DEF( "default", "false" )					\
    HTML_HELP_BODY()							\
    "If enable the values given in the edge_length property will be used "\
    "for the length  of each edge in the graph"				\
    HTML_HELP_CLOSE(),
    // remove overlaps
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Boolean" )	      \
    HTML_HELP_DEF( "values", "true / false" ) \
    HTML_HELP_DEF( "default", "false" )					\
    HTML_HELP_BODY()							\
    "If enable the algorithm will try to remove overlapping in the "	\
    "final drawing"\
    HTML_HELP_CLOSE(),
    // Forbid movements
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Boolean" )	      \
    HTML_HELP_DEF( "values", "true / false" ) \
    HTML_HELP_DEF( "default", "false" )					\
    HTML_HELP_BODY()							\
    "If enable the algorithm will forbid move of selected nodes"	\
    HTML_HELP_CLOSE()
  };
}
//================================================================================
SpringElectrical::SpringElectrical(const PropertyContext &context):Layout(context){
  addParameter<LayoutProxy>    ("initial_layout", paramHelp[0], "viewLayout");
  addParameter<SizesProxy>     ("node_size",      paramHelp[1], "viewSizes");
  addParameter<SelectionProxy> ("node_move",      paramHelp[2], "viewSelection");
  addParameter<MetricProxy>    ("edge_length",    paramHelp[3], "viewMetric");
  addParameter<bool>           ("use_edge_length",paramHelp[4], "false");
  addParameter<bool>           ("prevent_ovelap",  paramHelp[5], "true");
  addParameter<bool>           ("forbid_move",    paramHelp[6], "true");
  //addParameter<int>          ("edge_bends",     paramHelp[7], "0");
}
//================================================================================
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
    if (dist > 1.1*(1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id))) continue;
    if (dist > 1.0E-3) {
      uv /= dist;
      dist -= (1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id));
      if (dist < 1.E-3)
	uv *= -(1.0 + sizeNorm.get(u.id) + sizeNorm.get(v.id)) + dist;
      else
	uv *=  - sizeNorm.get(u.id) /sqr(dist);
    } else {
      uv[rand()%2] += 0.1 - (0.2 * (double(rand()%2)));
    }
    result += maxForce(uv, temperature * maxforce);
  }
  return result;
}
//================================================================================
Coord attractiveForces(node nu, LayoutProxy *layout, SuperGraph * graph,   
		       const MutableContainer<double> &sizeNorm,
		       double temperature, double maxforce, MetricProxy* edgeLength) {
  Coord u = layout->getNodeValue(nu);
  Coord result(0,0,0);
  node nv;
  edge ne;
  forEach(ne, graph->getInOutEdges(nu)) {
    nv = graph->opposite(ne, nu);
    Coord v = layout->getNodeValue(nv);
    Coord uv = v-u;
    double length;
    if (edgeLength != 0) 
      length = edgeLength->getEdgeValue(ne) +  (sizeNorm.get(nu.id) + sizeNorm.get(nv.id));
    else
      length = 1. +  (sizeNorm.get(nu.id) + sizeNorm.get(nv.id));
    double dist = uv.norm();
    if (dist > length) {
      uv /= dist;
      //uv *= pow((dist - length) , 2.0); //Hooke's law
      //uv *= pow((dist - length) , 1.0); //Hooke's law
      uv *= length/1. * log( dist / length); //Eades's law
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
  if (iterations < 500) iterations = 500;// iterations = iterations >? 500;

  originalLayout = 0;
  edgeLength     = 0;
  selectedNodes  = 0;
  sizeOfNodes    = 0;
  removeOverlaps = true;
  forbidMoveOfSelectedNodes = false;
  useEdgeLength = false;

  if (dataSet != 0) {
    dataSet->get("node_size", sizeOfNodes);
    dataSet->get("initial_layout", originalLayout);
    dataSet->get("use_edge_length", useEdgeLength);
    dataSet->get("edge_length", edgeLength);
    dataSet->get("forbid_move", forbidMoveOfSelectedNodes);
    dataSet->get("node_nodes", selectedNodes);
    dataSet->get("prevent_overlaps", removeOverlaps);
    //dataSet->get("edge_bends", nbBends);
  }
  
  if (originalLayout == 0) 
    originalLayout = graph->getProperty<LayoutProxy>("viewLayout");
  if (edgeLength == 0 && useEdgeLength)
    edgeLength = graph->getProperty<MetricProxy>("viewMetric");
  if (forbidMoveOfSelectedNodes && selectedNodes == 0)
    selectedNodes = graph->getProperty<SelectionProxy>("viewSelection");
  if (sizeOfNodes == 0)
    sizeOfNodes = graph->getProperty<SizesProxy>("viewSize");

  node n;
  forEach(n, graph->getNodes()) {
    layoutProxy->setNodeValue(n, originalLayout->getNodeValue(n));
    sizeNorm.set(n.id, sizeOfNodes->getNodeValue(n).norm()/2.0);
  }
  double maxforce   = sqrt((float) graph->numberOfNodes());
  double temperature = 1.0;
  bool overlapTest = false;

  for (int count = 0; count < iterations; ++count) {
    if (pluginProgress->progress(count,iterations)!=TLP_CONTINUE) break;
    node itn;
    forEach(itn, graph->getNodes()) {
      if (forbidMoveOfSelectedNodes && selectedNodes->getNodeValue(itn)) continue;
      if (rand()%graph->numberOfNodes() < 100) {
	Coord att = maxForce( attractiveForces(itn, layoutProxy, graph, sizeNorm, temperature, maxforce, edgeLength),  maxforce * temperature);
	Coord rep = maxForce( repulsiveForces(itn, layoutProxy, graph, sizeNorm, temperature, maxforce) ,  maxforce * temperature);
	Coord newMove = maxForce(att + rep, maxforce * temperature); //prevent too big jumps
	//test overlapping	
	if (!overlapTest ) {
	  Coord tmp  = layoutProxy->getNodeValue(itn);
	  tmp[2] = 0;
	  layoutProxy->setNodeValue(itn, tmp + newMove);      
	  if (count==iterations - 1) {
	    overlapTest = true;
	    temperature = 1.0;
	    if (removeOverlaps)
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
	  }
	  if (!_overlap) {
	    Coord tmp  = layoutProxy->getNodeValue(itn);
	    tmp[2] = 0;
	    layoutProxy->setNodeValue(itn,  tmp + newMove);      
	  } 
	  else {
	    if (nbBefore>0) {
	      temperature += 0.5;
	    }
	  }
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
//================================================================================
