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

#include "GEMLayout.h"
// An implementation of the GEM3D layout algorithm, based on
// code by Arne Frick placed in the public domain.  See GEMLayout.h for further details.


using namespace std;
using namespace tlp;

static const char * paramHelp[] = {
  // 3D
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_BODY() \
  "If true, the layout is in 3D else it is computed in 2D." \
  HTML_HELP_CLOSE(),
  // edge length
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "NumericProperty" ) \
  HTML_HELP_BODY() \
  "This metric is used to compute the length of edges." \
  HTML_HELP_CLOSE(),
  // initial layout
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "LayoutProperty" ) \
  HTML_HELP_BODY() \
  "The layout property used to compute the initial position of the graph elements. If none is given the initial position will be computed by the algorithm." \
  HTML_HELP_CLOSE(),
  // selection of unmovable nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "BooleanProperty" ) \
  HTML_HELP_BODY() \
  "This property is used to indicate the unmovable nodes, the ones for which a new position will not be computed by the algorithm. This property is taken into account only if a layout property has been given to get the initial position of the unmovable nodes." \
  HTML_HELP_CLOSE(),
  // max iterations
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned integer" ) \
  HTML_HELP_DEF( "default", "0" ) \
  HTML_HELP_BODY() \
  "This parameter allows to choose the number of iterations. The default value of 0 corresponds to (3 * nb_nodes * nb_nodes) if the graph has more than 100 nodes."\
  " For smaller graph, the number of iterations is set to 30 000." \
  HTML_HELP_CLOSE(),
};

/*
 * GEM3D Constants
 */

static const float EDGELENGTH = 10;
static const float EDGELENGTHSQR = EDGELENGTH * EDGELENGTH;
static const float MAXATTRACT = 8192;

/*
 * GEM3D Default Parameter Values
 */
static const float IMAXTEMPDEF     = 1.0f;
static const float ISTARTTEMPDEF   = 0.3f;
static const float IFINALTEMPDEF   = 0.05f;
static const int   IMAXITERDEF     = 10;
static const float IGRAVITYDEF     = 0.05f;
static const float IOSCILLATIONDEF = 0.5f;
static const float IROTATIONDEF    = 0.5f;
static const float ISHAKEDEF       = 0.2f;

static const float AMAXTEMPDEF     = 1.5f;
static const float ASTARTTEMPDEF   = 1.0f;
static const float AFINALTEMPDEF   = 0.02f;
static const int   AMAXITERDEF     = 3;
static const unsigned int MIN_ITER = 30000;  //minimum number of iteration (equivalent to a graph with 100 nodes)
static const float AGRAVITYDEF     = 0.1f;
static const float AOSCILLATIONDEF = 1.f;
static const float AROTATIONDEF    = 1.f;
static const float ASHAKEDEF       = 0.3f;

PLUGIN(GEMLayout)

GEMLayout::GEMLayout(const tlp::PluginContext* context) : LayoutAlgorithm(context),
  Iteration(0), _temperature(0), _maxtemp(0), _oscillation(0), _rotation(0),
  i_maxtemp(IMAXTEMPDEF), a_maxtemp(AMAXTEMPDEF), i_starttemp(ISTARTTEMPDEF),
  a_starttemp(ASTARTTEMPDEF), i_finaltemp(IFINALTEMPDEF), a_finaltemp(AFINALTEMPDEF), i_maxiter(IMAXITERDEF), a_maxiter(AMAXITERDEF), i_gravity(IGRAVITYDEF),
  a_gravity(AGRAVITYDEF), i_oscillation(IOSCILLATIONDEF), a_oscillation(AOSCILLATIONDEF), i_rotation(IROTATIONDEF), a_rotation(AROTATIONDEF), i_shake(ISHAKEDEF),
  a_shake(ASHAKEDEF), _dim(2), _nbNodes(0), _useLength(false), metric(NULL), fixedNodes(NULL), max_iter(0) {
  addInParameter<bool>("3D layout", paramHelp[0], "false");
  addInParameter<NumericProperty*>("edge length", paramHelp[1], "", false);
  addInParameter<LayoutProperty>("initial layout", paramHelp[2], "", false);
  addInParameter<BooleanProperty>("unmovable nodes", paramHelp[3], "", false);
  addInParameter<unsigned int>("max iterations", paramHelp[4], "0");
  addDependency("Connected Component Packing", "1.0");
}
//=========================================================
GEMLayout::~GEMLayout() {
}
//=========================================================
unsigned int GEMLayout::select() {
  return rand() % graph->numberOfNodes();
}
//=========================================================
void GEMLayout::vertexdata_init(const float starttemp) {
  _temperature = 0;
  _center.fill(0);
  std::vector<GEMparticule>::iterator it;

  for (it = _particules.begin(); it!=_particules.end(); ++it) {
    it->heat = starttemp;
    _temperature += it->heat * it->heat;
    it->imp.fill(0);
    it->dir  = 0;
    it->mass = 1.f + it->mass / 3.f;
    _center += it->pos;
  }
}
//=========================================================
void GEMLayout::updateLayout() {
  for (unsigned int i = 0; i < graph->numberOfNodes(); ++i) {
    //    tlp::warning() << "pos up ==> :" << _particules[i].pos << endl;
    result->setNodeValue(_particules[i].n, _particules[i].pos);
  }
}
//=========================================================
/*
 * compute force exerced on node v
 * if testPlaced is equal to true, only already placed nodes
 * are considered
 */
Coord GEMLayout::computeForces(unsigned int v,
                               float shake,
                               float gravity,
                               bool testPlaced) {
  Coord force;
  Coord vPos  = _particules[v].pos;
  float vMass = _particules[v].mass;
  node  vNode = _particules[v].n;

  //Init force in a random position
  for (unsigned int cnt = 0; cnt<_dim; ++cnt) {
    force[cnt] =  shake  - static_cast<float>(((double(rand()) * (2. * shake)))/double(RAND_MAX));
  }

  //Add central force
  force += (_center / float(_nbNodes) - vPos) * vMass * gravity;
  //
  double  maxEdgeLength;

  if (_useLength)
    maxEdgeLength = std::max(2.0f, (float)metric->getEdgeDoubleMin());
  else
    maxEdgeLength = EDGELENGTH;

  maxEdgeLength *= maxEdgeLength;

  //repulsive forces (magnetic)
  for (unsigned int u = 0; u < _nbNodes; ++u) {
    if (!testPlaced || _particules[u].in > 0) { //test whether the node is already placed
      Coord d(vPos - _particules[u].pos);
      float n = d[0]*d[0] + d[1]*d[1] + d[2]*d[2]; //d.norm() * d.norm();

      if (n > 0.)
        force += d * (float)maxEdgeLength / n;
    }
  }

  //attractive forces
  edge e;
  forEach(e,  graph->getInOutEdges(vNode)) {
    node uNode = graph->opposite(e, vNode);

    if (uNode == vNode)
      // nothing to do if it is a self loop
      continue;

    GEMparticule *gemQ = _nodeToParticules.get(uNode.id);

    if (!testPlaced || gemQ->in > 0) { //test whether the node is already placed
      float edgeLength;

      if (_useLength)
        edgeLength = static_cast<float>(metric->getEdgeDoubleValue(e));
      else
        edgeLength = EDGELENGTH;

      Coord d(vPos - gemQ->pos);
      float n = d.norm() / vMass;
      n = std::min(n, MAXATTRACT);  //   1048576L
      force -= (d * n) / (float)(edgeLength * edgeLength + 1.);
    }
  }
  return force;
}
//==========================================================================
void GEMLayout::insert() {
  GEMparticule *gemP, *gemQ;
  int startNode;

  this->vertexdata_init(i_starttemp);

  _oscillation = i_oscillation;
  _rotation    = i_rotation;
  _maxtemp     = i_maxtemp;

  node nCenter = graphCenterHeuristic(graph);
  unsigned int v = _nodeToParticules.get(nCenter.id)->id;

  for (unsigned int i = 0; i < _nbNodes; ++i)
    _particules[i].in = 0;

  _particules[v].in = -1;

  startNode = -1;

  for (unsigned int i = 0; i < _nbNodes; ++i) {
    if (pluginProgress->isPreviewMode()) updateLayout();

    if (pluginProgress->progress(i, _nbNodes)!=TLP_CONTINUE) return;

    //choose particule with the minimum value
    int d = 0;

    for (unsigned int j = 0; j < _nbNodes; ++j)
      if (_particules[j].in < d) {
        d = _particules[j].in;
        v = j;
      }

    //
    _particules[v].in = 1;
    node vNode = _particules[v].n;

    // nothing to do if vNode is a fixed node
    if (fixedNodes && fixedNodes->getNodeValue(vNode))
      continue;

    node uNode;
    //remove one to non-visited nodes
    forEach(uNode, graph->getInOutNodes(vNode)) {
      if (uNode == vNode)
        // nothing to do if it is a self loop
        continue;

      if (_nodeToParticules.get(uNode.id)->in <= 0)
        --(_nodeToParticules.get(uNode.id)->in);
    }

    gemP = &_particules[v];
    gemP->pos.fill(0);

    if (startNode >= 0) {
      int d = 0;
      node uNode;
      forEach(uNode, graph->getInOutNodes(vNode)) {
        if (uNode == vNode)
          // nothing to do if it a self loop
          continue;

        gemQ = _nodeToParticules.get(uNode.id);

        if (gemQ->in > 0) {
          gemP->pos += gemQ->pos;
          ++d;
        }
      }

      if (d > 1) {
        gemP->pos /= static_cast<float>(d);
      }

      d = 0;

      while ((d++ < i_maxiter) && (gemP->heat > i_finaltemp))
        this->displace(v, computeForces(v, i_shake, i_gravity, true));
    }
    else
      startNode = i;

  }
}
//==========================================================================
void GEMLayout::displace(unsigned int v, Coord imp) {

  float nV = imp.norm();

  if (nV > 0) {
    float t  = _particules[v].heat;
    imp /= nV; //normalize imp

    _temperature -= t * t;

    //oscillation
    float cosA = imp.dotProduct(_particules[v].imp);
    t += _oscillation *  cosA * t;
    t = std::min(t, _maxtemp);
    //rotation
    float sinA  = (imp ^ _particules[v].imp).norm();
    t -= _rotation * sinA * t;
    t = std::max(t, 0.01f);

    _temperature += t * t;

    _particules[v].heat = t;
    _particules[v].pos += imp * t;
    _center += imp * t;
    _particules[v].imp = imp;
  }
}
//==========================================================================
void GEMLayout::a_round() {
  for (unsigned int i = 0; i < _nbNodes; ++i) {
    unsigned int v = this->select();
    node vNode = _particules[v].n;

    // nothing to do if vNode is a fixed node
    if (fixedNodes && fixedNodes->getNodeValue(vNode))
      continue;

    Coord force = computeForces(v, a_shake, a_gravity, false);
    this->displace(v, force );
    Iteration++;
  }
}
//============================================================================
void GEMLayout::arrange() {
  float stop_temperature;

  double  maxEdgeLength;

  if (_useLength)
    maxEdgeLength = std::max(2.0f, (float)metric->getEdgeDoubleMin());
  else
    maxEdgeLength = EDGELENGTH;

  maxEdgeLength *= maxEdgeLength;

  this->vertexdata_init(a_starttemp);

  _oscillation      = a_oscillation;
  _rotation         = a_rotation;
  _maxtemp          = a_maxtemp;
  stop_temperature  = static_cast<float>(a_finaltemp * a_finaltemp * maxEdgeLength * _nbNodes);
  Iteration         = 0;

  while (_temperature > stop_temperature && Iteration < max_iter) {
    //    tlp::warning() << "t°:"<< _temperature << "/" << stop_temperature << " it:" << Iteration << endl;
    if (pluginProgress->progress(Iteration, max_iter/2)!=TLP_CONTINUE) return;

    if (pluginProgress->isPreviewMode()) updateLayout();

    this->a_round();
  }
}
//============================================================================
bool GEMLayout::run() {
  if (!ConnectedTest::isConnected(graph)) {
    // for each component draw
    std::vector<std::set<node> > components;
    string err;
    // push a temporary graph state (not redoable)
    graph->push(false);
    ConnectedTest::computeConnectedComponents(graph, components);

    for (size_t i = 0; i < components.size(); ++i) {
      Graph * tmp = graph->inducedSubGraph(components[i]);
      tmp->applyPropertyAlgorithm("GEM (Frick)", result, err, pluginProgress, dataSet);
    }

    // call connected component packing
    LayoutProperty tmpLayout(graph);
    DataSet ds;
    ds.set("coordinates", result);
    graph->applyPropertyAlgorithm("Connected Component Packing", &tmpLayout, err, pluginProgress, &ds);
    // forget last temporary graph state
    graph->pop();
    *result = tmpLayout;
    return true;
  }


  /* Handle parameters */
  metric = NULL;
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");

  bool is3D = false;
  bool initLayout = false;
  _useLength = false;
  max_iter = 0;

  if ( dataSet!=NULL ) {
    dataSet->get("3D layout", is3D);
    _useLength = dataSet->get("edge length", metric) && metric!=NULL;
    dataSet->get("max iterations", max_iter);
    initLayout = !dataSet->get("initial layout", layout);

    if (initLayout)
      dataSet->get("unmovable nodes", fixedNodes);
  }

  if (is3D) _dim = 3;
  else _dim = 2;

  _nbNodes = graph->numberOfNodes();

  // no bends
  result->setAllEdgeValue(vector<Coord>(0));

  // initialize a random sequence according the given seed
  tlp::initRandomSequence();

  if (max_iter == 0)
    max_iter = std::max(a_maxiter * _nbNodes * _nbNodes, MIN_ITER);

  _particules.resize(_nbNodes);
  /* Max Edge to scale actual edges lentgh to preferres lentgh */
  node n;
  unsigned int i = 0;
  forEach(n, graph->getNodes()) {
    _particules[i] = GEMparticule(static_cast<float>(graph->deg(n)));
    _particules[i].n = n;
    _particules[i].id = i;

    if (!initLayout && layout != NULL)
      _particules[i].pos = layout->getNodeValue(n);
    else
      _particules[i].pos.fill(0);

    _nodeToParticules.set(n.id, &_particules[i]);
    ++i;
  }

  if (initLayout && layout != NULL) {
    if (i_finaltemp < i_starttemp) this->insert();
  }

  if (pluginProgress->progress(100,100)==TLP_CONTINUE)
    if (a_finaltemp < a_starttemp) this->arrange();

  if (pluginProgress->progress(100,100)!=TLP_CANCEL)
    updateLayout();

  return pluginProgress->state()!=TLP_CANCEL;
}
//=========================================================
