/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "Grip.h"
#include "MISFiltering.h"
#include "Distances.h"

#include <string>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/ForEach.h>
#include <tulip/TreeTest.h>
#include <tulip/Graph.h>

#include <math.h>


using namespace std;
using namespace tlp;


static const char * paramHelp[] = {
  // 3D
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_BODY() \
  "If true the layout is in 3D else it is computed in 2D" \
  HTML_HELP_CLOSE(),
};

//======================================================
Grip::Grip(const tlp::PluginContext *context) : LayoutAlgorithm(context),
  misf(NULL), edgeLength(0), level(0), currentGraph(NULL), _dim(0) {
  addInParameter<bool>("3D layout", paramHelp[0], "false");
  addDependency("Connected Component", "1.0");
  addDependency("Equal Value", "1.1");
  addDependency("Connected Component Packing", "1.0");
}
Grip::~Grip() {}

//======================================================
bool Grip::run() {
  bool is3D = false;

  if(dataSet != NULL) {
    if(!dataSet->get("3D layout", is3D))
      is3D = false;
  }

  if(is3D) _dim = 3;
  else _dim = 2;

  DoubleProperty connectedComponent(graph);
  string err;
  graph->applyPropertyAlgorithm("Connected Component", &connectedComponent, err);
  DataSet tmp;
  tmp.set("Property", &connectedComponent);
  graph->applyAlgorithm("Equal Value", err, &tmp);
  unsigned int nbComp = 0;

  stableForEach(currentGraph , graph->getSubGraphs()) {
    nbComp++;

    if(currentGraph->numberOfNodes() <= 3) {
      unsigned int nb_nodes = currentGraph->numberOfNodes();

      if(nb_nodes == 1) {
        node n = currentGraph->getOneNode();
        result->setNodeValue(n, Coord(0,0,0));
      }

      if(nb_nodes == 2) {
        Iterator<node>*itn = currentGraph->getNodes();
        node n1 = itn->next();
        node n2 = itn->next();
        delete itn;
        result->setNodeValue(n1, Coord(0,0,0));
        result->setNodeValue(n2, Coord(1,0,0));
      }
      else if(nb_nodes == 3) {
        if(currentGraph->numberOfEdges() == 3) {
          Iterator<node>*itn = currentGraph->getNodes();
          node n1 = itn->next();
          node n2 = itn->next();
          node n3 = itn->next();
          delete itn;
          result->setNodeValue(n1, Coord(0,0,0));
          result->setNodeValue(n2, Coord(1,0,0));
          result->setNodeValue(n3, Coord(0.5,sqrt(0.5),0));
        }
        else {
          Iterator<edge>*ite = currentGraph->getEdges();
          edge e1 = ite->next();
          edge e2 = ite->next();
          delete ite;

          const std::pair<node, node>& ends1 = currentGraph->ends(e1);
          const std::pair<node, node>& ends2 = currentGraph->ends(e2);
          node n1 = ends1.first;
          node n2 = ends1.second;
          node n3;

          if(ends2.second== n1) {
            n1 = ends2.first;
            n3 = n2;
            n2 = ends2.second;
          }
          else if(ends2.first==n1) {
            n1 = ends2.second;
            n3 = n2;
            n2 = ends2.first;
          }
          else
            n3 = (ends2.first==n2) ? ends2.second : ends2.first;

          result->setNodeValue(n1, Coord(0,0,0));
          result->setNodeValue(n2, Coord(1,0,0));
          result->setNodeValue(n3, Coord(2,0,0));
        }
      }
    }
    else {
      // initialize a random sequence according the given seed
      tlp::initRandomSequence();

      misf = new MISFiltering(currentGraph);
      computeOrdering();
      init();
      firstNodesPlacement();
      placement();
      delete misf;
    }

    graph->delAllSubGraphs(currentGraph);
  }

  if(nbComp >1) {
    err ="";
    LayoutProperty layout(graph);
    tmp.set("coordinates", result);
    graph->applyPropertyAlgorithm("Connected Component Packing",&layout,
                                  err,NULL,&tmp);
    Iterator<node> *itN = graph->getNodes();

    while(itN->hasNext()) {
      node n = itN->next();
      result->setNodeValue(n, layout.getNodeValue(n));
    }

    delete itN;
  }

  return true;
}
//======================================================
void Grip::computeOrdering() {
  misf->computeFiltering();
}

//======================================================
void Grip::firstNodesPlacement() {
  node n1 = misf->ordering[0];
  node n2 = misf->ordering[1];
  node n3 = misf->ordering[2];

  float d12 = (float) getDist(currentGraph,n1,n2);
  float d13 = (float) getDist(currentGraph,n1,n3);
  float d23 = (float) getDist(currentGraph,n2,n3);

  result->setNodeValue(n1,Coord(0,0,0));
  result->setNodeValue(n2,Coord(d12,0,0));

  float x3 = (d13*d13 - d23*d23 + d12*d12)/ (d12 * 2.);
  float y3 = sqrt(d13*d13 - x3*x3);
  result->setNodeValue(n3,Coord(x3,y3,0));

  if(_dim == 2) {
    oldDisp[n1] = Coord(1., 0, 0);
    oldDisp[n2] = Coord(d12+1., 0, 0);
    oldDisp[n3] = Coord(x3+1., y3, 0);
  }
  else {
    Graph *g = currentGraph->addSubGraph();
    g->addNode(n1);
    g->addNode(n2);
    g->addNode(n3);
    result->rotateX(3.14159/2. - (3.14159 * (double)(rand()%2)), g->getNodes(), g->getEdges());
    currentGraph->delSubGraph(g);

    Coord c1 = result->getNodeValue(n1);
    Coord c2 = result->getNodeValue(n2);
    Coord c3 = result->getNodeValue(n3);
    oldDisp[n1] = c1;
    oldDisp[n2] = c2;
    oldDisp[n3] = c3;
  }

  neighbors[n1].push_back(n2);
  neighbors[n1].push_back(n3);
  neighbors_dist[n1].push_back((unsigned int)d12);
  neighbors_dist[n1].push_back((unsigned int)d13);
  neighbors[n2].push_back(n1);
  neighbors[n2].push_back(n3);
  neighbors_dist[n2].push_back((unsigned int)d12);
  neighbors_dist[n2].push_back((unsigned int)d23);
  neighbors[n3].push_back(n1);
  neighbors[n3].push_back(n2);
  neighbors_dist[n3].push_back((unsigned int)d13);
  neighbors_dist[n3].push_back((unsigned int)d23);

}
//======================================================
void Grip::placement() {
  unsigned int misf_size = misf->index.size();

  if(misf_size == 1) {
    initialPlacement(misf->index[0], misf->ordering.size()-1);
    fr_reffinement(0, misf->ordering.size()-1);
    return;
  }

  for(unsigned int i = 0; i < misf_size - 1; ++i) {
    initialPlacement(misf->index[i], misf->index[i+1]-1);
    kk_reffinement(0, misf->index[i+1]-1);
    init_heat(misf->index[i+1]-1);
    ++level;
  }

  initialPlacement(misf->index[misf->index.size()-1], misf->ordering.size()-1);
  fr_reffinement(0, misf->ordering.size()-1);
}
//======================================================
void Grip::seeLayout(unsigned int end) {
  cerr << "profondeur " << level << endl;

  for(unsigned int i = 0; i <= end ; ++i) {
    node n = misf->ordering[i];

    for(unsigned int j = 0; j < neighbors[n].size(); ++j) {
      cerr << "distance euclidienne " << (result->getNodeValue(n)-result->getNodeValue(neighbors[n][j])).norm() / edgeLength
           << " et distance dans le graphe " << neighbors_dist[n][j] << endl;
    }
  }
}
//======================================================
void Grip::initialPlacement(unsigned int start, unsigned int end) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  for(unsigned int i = start; i <= end ; ++i) {
    node currNode = misf->ordering[i];
    misf->getNearest(currNode, neighbors[currNode],
                     neighbors_dist[currNode], level,
                     levelToNbNeighbors[level+1]);
  }

  for(unsigned int i = start; i <= end; ++i) {
    node currNode = misf->ordering[i];
    Coord c_tmp(0,0,0);
    float nbConsidered = 0.;

    for(unsigned int j = 0; j < neighbors[currNode].size(); ++j) {
      c_tmp += result->getNodeValue(neighbors[currNode][j]);
      oldDisp[currNode] += oldDisp[neighbors[currNode][j]];
      nbConsidered += 1.;
    }

    double alpha = edgeLength / 6.0 * (double) rand()/ (double) RAND_MAX;
    Coord  alea  = Coord(alpha - (2. * alpha* (double)(rand()%2)), alpha - (2. * alpha * double(rand()%2)), (alpha - (2. * alpha* (double)(rand()%2))));

    if(_dim == 2) alea[2] = 0.;

    c_tmp /= nbConsidered;
    oldDisp[currNode] /= nbConsidered;
    oldDisp[currNode] += alea;
    c_tmp += alea;
    result->setNodeValue(currNode, c_tmp);
    heat[currNode] = edgeLength/6.0;
    kk_local_reffinement(currNode);
  }
}

//======================================================
void Grip::kk_local_reffinement(node currNode) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  unsigned int cpt = 6;

  while(cpt > 1) {
    disp[currNode] = Coord(0,0,0);
    Coord c = result->getNodeValue(currNode);

    for(unsigned int j = 0; j < neighbors[currNode].size() /*&& j < 3*/ ; ++j) {
      node n = neighbors[currNode][j];
      Coord c_n = result->getNodeValue(n);
      Coord c_tmp = c_n - c;
      float euclidian_dist_sqr = c_tmp[0] * c_tmp[0] + c_tmp[1] * c_tmp[1];

      if(_dim == 3) euclidian_dist_sqr += c_tmp[2] * c_tmp[2];

      float th_dist = (float)neighbors_dist[currNode][j];
      c_tmp *=  (euclidian_dist_sqr / (th_dist * th_dist * edgeLength * edgeLength))-1.;
      disp[currNode] += c_tmp;
    }

    displace(currNode);
    --cpt;
  }
}
//======================================================
void Grip::displace(node n) {
  updateLocalTemp(n);
  float disp_norm = disp[n].norm();

  if(disp_norm > 1E-4) {
    disp[n]    /= disp_norm;
    oldDisp[n] = disp[n];
    disp[n]    *= (float) heat[n];
    result->setNodeValue(n, result->getNodeValue(n) + disp[n]);
  }
}
//======================================================
void Grip::kk_reffinement(unsigned int start, unsigned int end) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  unsigned int cpt = rounds(end, 0, 20, currentGraph->numberOfNodes(), 30)+2;

  while(cpt >= 1) {
    for(unsigned int i = start; i <= end; ++i) {
      node currNode = misf->ordering[i];
      disp[currNode] = Coord(0,0,0);
      Coord c = result->getNodeValue(currNode);

      for(unsigned int j = 0; j < neighbors[currNode].size(); ++j) {
        node  n = neighbors[currNode][j];
        Coord c_n = result->getNodeValue(n);
        Coord c_tmp = c_n - c;
        float euclidian_dist_sqr = c_tmp[0] * c_tmp[0] + c_tmp[1] * c_tmp[1];

        if(_dim == 3) euclidian_dist_sqr += c_tmp[2] * c_tmp[2];

        float th_dist = (float)neighbors_dist[currNode][j];
        c_tmp *= (euclidian_dist_sqr / (th_dist * th_dist * edgeLength * edgeLength))-1.;
        disp[currNode] +=  c_tmp;
      }
    }

    //update node position
    for(unsigned int i = 0; i <= end ; ++i) {
      displace(misf->ordering[i]);
    }

    --cpt;
  }
}
//======================================================
void Grip::fr_reffinement(unsigned int start, unsigned int end) {
  //cerr << __PRETTY_FUNCTION__ << endl;

  unsigned int cpt = rounds(end, 0, 20, currentGraph->numberOfNodes(), 30)+2;

  while(cpt >= 1) {
    for(unsigned int i = start; i <= end; ++i) {
      node  currNode = misf->ordering[i];
      Coord curCoord = result->getNodeValue(currNode);
      disp[currNode] = Coord(0,0,0);

      // attractive force calculation
      node n;
      forEach(n,currentGraph->getInOutNodes(currNode)) {
        Coord c_n = result->getNodeValue(n);
        Coord c_tmp = c_n - curCoord;
        float euclidian_dist_sqr = c_tmp[0] * c_tmp[0] + c_tmp[1] * c_tmp[1];

        if(_dim == 3) euclidian_dist_sqr += c_tmp[2] * c_tmp[2];

        c_tmp *= euclidian_dist_sqr/((float) edgeLength * (float) edgeLength);
        disp[currNode] += c_tmp;
      }

      // repulsive force calculation
      for(unsigned int j = 0; j < neighbors[currNode].size(); ++j) {
        node  n     = neighbors[currNode][j];
        Coord c_n   = result->getNodeValue(n);
        Coord c_tmp = curCoord - c_n;
        double euclidian_dist_sqr = (double)c_tmp[0] * (double)c_tmp[0] + (double)c_tmp[1] * (double)c_tmp[1];

        if(_dim == 3) euclidian_dist_sqr += c_tmp[2] * c_tmp[2];

        if(!(euclidian_dist_sqr > 1E-4)) {
          double alpha = 2. * (double) rand()/ (double) RAND_MAX;
          c_tmp = Coord (alpha - (2. * alpha* (double)(rand()%2)),alpha -(2. * alpha * double(rand()%2)),alpha - (2. * alpha* (double)(rand()%2)));

          if(_dim == 2) c_tmp[2] = 0.;

          euclidian_dist_sqr = 0.01;
        }

        c_tmp *= (0.05 * edgeLength * edgeLength) /(float)euclidian_dist_sqr;
        disp[currNode] +=  c_tmp;
      }
    }

    //update node position
    for(unsigned int i = 0; i <= end ; ++i) {
      displace(misf->ordering[i]);
    }

    --cpt;
  }
}

//======================================================
void Grip::updateLocalTemp(node v) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  float oldDisp_norm = oldDisp[v].norm();
  float curDisp_norm = disp[v].norm();

  if(curDisp_norm * oldDisp_norm > 1E-4) {

    double scalar = (double) disp[v].dotProduct(oldDisp[v]);
    double cos = (double) scalar /(curDisp_norm * oldDisp_norm);

    Coord tmp1 = oldDisp[v] / oldDisp_norm;
    Coord tmp2 = disp[v] / curDisp_norm;

    double sin = (tmp2 ^ tmp1).norm();

    double r = 6.;
    double o = 6.0;

    heat[v] +=  cos * r * heat[v];
    /*
    heat[v] = std::max(heat[v], 0.001);
    heat[v] = std::min(heat[v], 2. * edgeLength/6.0);
    */
    heat[v] +=  sin * o * heat[v];

    heat[v] = std::max(heat[v], edgeLength/300.);
    heat[v] = std::min(heat[v],  edgeLength/4.0);

  }
}
//======================================================
unsigned int Grip::rounds(unsigned int x, unsigned int max, unsigned int maxVal, unsigned int min, unsigned int minVal) {
  if( x <= max )
    return maxVal;
  else if( max <= x && x <= min ) {
    double k = -log((double)minVal/maxVal)/min;
    return (unsigned int )(ceil(maxVal * exp( -k * x )));
  }
  else
    return minVal;
}
//======================================================
void Grip::init() {
  //cerr << __PRETTY_FUNCTION__ << endl;
  set_nbr_size();
  edgeLength = 32.;
  level = 0;

  double diam = sqrt(currentGraph->numberOfNodes());
  node n;
  forEach(n,currentGraph->getNodes()) {
    Coord  alea  = Coord(diam - (2. * diam * (double)(rand()%2)), diam - (2. * diam * double(rand()%2)), diam - (2. * diam * double(rand()%2)));

    if(_dim == 2) alea[2] = 0.;

    result->setNodeValue(n, alea);
    disp[n] = Coord(0,0,0);
    oldDisp[n] = Coord(0,0,0);
    heat[n] = edgeLength/6.;
  }
}
//======================================================
void Grip::init_heat(unsigned int end) {

  for(unsigned int i = 0; i <= end ; ++i)
    heat[misf->ordering[i]] = edgeLength/6.;

}
//======================================================
void Grip::set_nbr_size() {
  unsigned int maxCxty = 0;
  int initCxty = 10000;
  unsigned int maxLevel = 0;

  node n;
  forEach(n,currentGraph->getNodes())
  maxCxty += currentGraph->deg(n);

  if(maxCxty < (unsigned int)initCxty)
    maxCxty = initCxty;

  for(unsigned int i = 1; i < misf->index.size() ; ++i) {
    if((int)(misf->index[i] * misf->index[i]) - initCxty >= 0) {
      maxLevel = i;
      break;
    }
  }

  if(maxLevel == 0 &&  (int)(currentGraph->numberOfNodes() * currentGraph->numberOfNodes()) - initCxty >= 0)
    maxLevel = misf->index.size();


  for(unsigned int i = 1; i < misf->index.size() ; ++i) {
    if( i >= maxLevel)
      levelToNbNeighbors[i] =  min((unsigned int)(sched(misf->index.size()-i,0,2,10000,1) * maxCxty/(misf->index[i])),  (unsigned int)(misf->index[i]-1));
    else
      levelToNbNeighbors[i] = max(misf->index[i]-1, (unsigned int) 3);
  }


  if( misf->index.size() >= maxLevel)
    levelToNbNeighbors[misf->index.size()] =  min((unsigned int)(sched(currentGraph->numberOfNodes(),0,2,10000,1) * (float)maxCxty/((float)currentGraph->numberOfNodes())), (unsigned int)(currentGraph->numberOfNodes()-1));
  else
    levelToNbNeighbors[misf->index.size()] = max(currentGraph->numberOfNodes()-1, (unsigned int) 3);

  levelToNbNeighbors[misf->index.size()] = min(2*levelToNbNeighbors[misf->index.size()], currentGraph->numberOfNodes()-1);
}
//======================================================
float Grip::sched(int x, int max, int maxVal, int min, int minVal) {
  if( x <= max )
    return (float)maxVal;
  else if( max <= x && x <= min ) {
    return ((minVal - maxVal)/(float)(min-max))*(x - max) + maxVal;
  }
  else
    return (float)minVal;
}
//======================================================
PLUGIN(Grip)
