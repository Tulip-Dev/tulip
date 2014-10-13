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

#include <string>
#include <ctime>
#include <math.h>

#include "LinLogLayout.h"

LinLogLayout::LinLogLayout (tlp::Graph* _graph,
                            tlp::PluginProgress* _pluginProgress):
  edgeWeight(NULL), layoutResult(NULL), linLogWeight(NULL), skipNodes(NULL),
  graph(_graph), pluginProgress(_pluginProgress), _dim(2), _nbNodes(0) {

  if (_graph == NULL)
    std::cerr<<"graph is Null\n";

  useOctTree = true;

  /** Exponent of the Euclidean distance in the repulsion energy. */
  repuExponent = 0.0;
  /** Exponent of the Euclidean distance in the attraction energy. */
  attrExponent = 1.0;
  /** Factor for the gravitation energy = attraction to the barycenter.
      Set to 0.0 for no gravitation. */
  gravFactor = 0.9;
  /** Factor for repulsion energy. */
  repuFactor = 0.0;
  max_iter = 100;
}

LinLogLayout::~LinLogLayout () {
  if(layoutResult)
    delete layoutResult;

  if(linLogWeight)
    delete linLogWeight;
}

bool LinLogLayout::initAlgo (tlp::LayoutProperty* _layout,
                             tlp::NumericProperty* _weight,
                             double _attrExponent,
                             double _repuExponent,
                             double _gravFactor,
                             unsigned int _max_iter,
                             bool _is3D,
                             bool _useOctTree,
                             tlp::BooleanProperty* _skipNodes) {
  //initializes with the current layout,
  //we might want to initialize it with a random layout too, not in this class
  layoutResult = _layout;
  skipNodes = _skipNodes;

  //initializes the weight
  edgeWeight = _weight;
  initWeights();

  //initializes the algorithm properties
  attrExponent = _attrExponent;
  repuExponent = _repuExponent;
  gravFactor = _gravFactor;
  useOctTree = _useOctTree;



  /* Handle parameters */
  _dim = (_is3D) ? 3 : 2;

  _nbNodes = graph->numberOfNodes();

  if (layoutResult == NULL) {
    std::cerr<<"layout result is null\n";
    return false;
  }

  max_iter = _max_iter;

  if (max_iter == 0)
    max_iter = 100;

  return true;
}

bool LinLogLayout::startAlgo () {
  return useOctTree ? minimizeEnergy (max_iter) :
         minimizeEnergyNoTree(max_iter);
}

/**
 * Computes values for the factors of the repulsion and gravitation energy,
 * <code>repuFactor</code> and <code>gravFactor</code>.
 * Chooses <code>repuFactor</code> such that the maximum distances
 * in the resulting layout approximate (very) roughly the square root
 * of the sum of the repuWeights, which is appropriate when each graph node
 * is visualized as a geometric object whose area is the node's repuWeight.
 */
void LinLogLayout::initEnergyFactors () {
  node u;
  double repuSum = 0.0;
  double attrSum = 0.0;

  forEach(u, graph->getNodes()) {
    double u_weight = linLogWeight->getNodeValue(u);
    repuSum += u_weight;
    edge e;
    forEach(e,  graph->getInOutEdges(u)) {
      double edgeweight = linLogWeight->getEdgeValue(e);
      attrSum += edgeweight;
    }
  }

  if (repuSum > 0.0 && attrSum > 0.0) {
    double density = attrSum / repuSum / repuSum;
    repuFactor = density * pow(repuSum, 0.5*(attrExponent-repuExponent));
    gravFactor = density * repuSum * pow(gravFactor, attrExponent-repuExponent);
  }
  else
    repuFactor = 1.0;
}


/**
 * Returns the total energy of a node.
 * @param node  u
 * @return total energy of the specified node
 */
double LinLogLayout::getEnergy (node u) {
  return getRepulsionEnergy (u)
         + getAttractionEnergy (u)
         + getGravitationEnergy (u);
}


double LinLogLayout::getEnergy (node u, OctTree* tree) {
  return getRepulsionEnergy (u, tree)
         + getAttractionEnergy (u)
         + getGravitationEnergy (u);
}

/**
 * Returns the repulsion energy of a node.
 * @param node  repulsing node
 * @return repulsion energy of the specified node
 */
double LinLogLayout::getRepulsionEnergy (node u) {
  double u_weight = linLogWeight->getNodeValue(u);

  if (u_weight == 0.0) return 0.0;

  const Coord& u_layout = layoutResult->getNodeValue(u);
  double energy = 0.0;
  node v;
  forEach(v, graph->getNodes()) {
    double v_weight = linLogWeight->getNodeValue(v);

    if (v == u || v_weight == 0.0)
      continue;

    double dist = getDist(u_layout, layoutResult->getNodeValue(v));

    if (repuExponent == 0.0) {
      energy -= repuFactor * u_weight * v_weight
                * log(dist);
    }
    else {
      energy -= repuFactor * u_weight * v_weight
                * pow(dist, repuExponent) / repuExponent;
    }
  }

  return energy;
}

double LinLogLayout::getRepulsionEnergy (node u, OctTree* tree) {
  if (tree == NULL)
    return 0.0;

  node treeNode = tree->getNode();

  if (treeNode == u)
    return 0.0;

  double u_weight = linLogWeight->getNodeValue(u);

  if (u_weight == 0.0) return 0.0;

  double dist = getDist(layoutResult->getNodeValue(u), tree->position);

  if (tree->childCount > 0 && dist < 2.0 * tree->width()) {
    double energy = 0.0;

    for (unsigned int i = 0; i < tree->childCount; ++i)
      energy += getRepulsionEnergy(u, (tree->_children)[i]);

    return energy;
  }

  if (dist == 0.0) return 0.0;

  if (repuExponent == 0.0) {
    return -repuFactor * u_weight * tree->weight
           * log(dist);
  }
  else {
    return -repuFactor * u_weight * tree->weight
           * pow(dist, repuExponent) / repuExponent;
  }
}


/**
 * Returns the attraction energy of a node.
 * @param node  attracting node
 * @return attraction energy of the specified node
 */
double LinLogLayout::getAttractionEnergy (node u) {
  double energy = 0.0;
  const Coord& u_layout = layoutResult->getNodeValue(u);
  edge e;
  forEach(e,  graph->getInOutEdges(u)) {
    node v = graph->opposite(e, u);
    double dist = getDist(u_layout,
                          layoutResult->getNodeValue(v));
    double edgeweight = linLogWeight->getEdgeValue(e);

    if (attrExponent == 0.0)
      energy += edgeweight * log(dist);
    else
      energy += edgeweight * pow(dist, attrExponent) / attrExponent;
  }

  return energy;
}


/**
 * Returns the gravitation energy of a node.
 * @param node  gravitating node
 * @return gravitation energy of the specified node
 */
double LinLogLayout::getGravitationEnergy (node u) {
  double u_weight = linLogWeight->getNodeValue(u);

  double dist = getDist(layoutResult->getNodeValue(u), baryCenter);

  if (attrExponent == 0.0)
    return gravFactor * u_weight * log(dist);
  else
    return gravFactor * u_weight * pow(dist, attrExponent) / attrExponent;
}

/**
 * Returns the Euclidean distance between the positions pos1 and pos2.
 * @return Euclidean distance between the positions pos1 and pos2
 */
double LinLogLayout::getDist (const Coord& pos1, const Coord& pos2) {
  double dist = 0.0;

  for (unsigned int d = 0; d < _dim; ++d) {
    double diff = pos1[d] - pos2[d];
    dist += diff * diff;
  }

  return sqrt(dist);
}


double LinLogLayout::getDistForComparison (const Coord& pos1,
    const Coord& pos2) {
  double dist = 0.0;

  for (unsigned int d = 0; d < _dim; ++d) {
    double diff = pos1[d] - pos2[d];
    dist += diff * diff;
  }

  return dist;
}


/**
 * Computes the direction of the repulsion force on a node.
 * @param  node  repulsing node
 * @param  dir   direction of the repulsion force acting on the node
 *         is added to this variable (output parameter)
 * @return approximate second derivation of the repulsion energy
 */
double LinLogLayout::addRepulsionDir (node u, double* dir) {
  double u_weight = linLogWeight->getNodeValue(u);

  if (u_weight == 0.0)
    return 0.0;

  const Coord& position= layoutResult->getNodeValue(u);

  double dir2 = 0.0;
  node v;
  forEach(v, graph->getNodes()) {
    double v_weight = linLogWeight->getNodeValue(v);

    if (v == u|| v_weight == 0.0)
      continue;

    const Coord& position2 = layoutResult->getNodeValue(v);

    double dist = getDist(position, position2);

    if (dist == 0.0) continue;

    double tmp = repuFactor * u_weight * v_weight
                 * pow(dist, repuExponent-2);

    dir2 += tmp * fabs(repuExponent-1);

    for (unsigned int d = 0; d < _dim; ++d)
      dir[d] -= (position2[d] - position[d]) * tmp;
  }
  return dir2;
}

double LinLogLayout::addRepulsionDir (node u, double* dir, OctTree* tree) {
  if(tree==NULL || u == (tree->node))
    return 0.0;

  double u_weight = linLogWeight->getNodeValue(u);

  if (u_weight == 0.0)
    return 0.0;

  const Coord& position = layoutResult->getNodeValue(u);

  double dist = getDist(position, tree->position);

  if (dist == 0.0) return 0.0;

  if (tree->childCount > 0 && dist < 2.0 * tree->width()) {
    double dir2 = 0.0;

    for (unsigned int i = 0; i < tree->childCount; ++i) {
      dir2 += addRepulsionDir(u, dir, (tree->_children)[i]);
    }

    return dir2;
  }

  double tmp = repuFactor * u_weight * tree->weight
               * pow(dist, repuExponent-2);

  for (unsigned int d = 0; d < _dim; ++d)
    dir[d] -= (tree->position[d] - position[d]) * tmp;

  return tmp * fabs(repuExponent-1);
}

/**
 * Computes the direction of the attraction force on the a node.
 * @param  node  attracting node
 * @param  dir   direction of the attraction force acting on the node
 *         is added to this variable (output parameter)
 * @return approximate second derivation of the attraction energy
 */
double LinLogLayout::addAttractionDir (node u, double* dir) {
  double dir2 = 0.0;

  const Coord& position = layoutResult->getNodeValue(u);

  edge e;
  forEach(e,  graph->getInOutEdges(u)) {
    node v = graph->opposite(e, u);
    const Coord& position2 = layoutResult->getNodeValue(v);
    double dist = getDist(position, position2);

    if (dist == 0.0)
      continue;

    double edgeweight = linLogWeight->getEdgeValue(e);
    double tmp = edgeweight * pow(dist, attrExponent-2);

    dir2 += tmp * fabs(attrExponent-1);

    for (unsigned int d = 0; d < _dim; ++d)
      dir[d] += (position2[d] - position[d]) * tmp;

  }

  return dir2;
}

/**
 * Computes the direction of the gravitation force on the a node.
 * @param  node  gravitating node
 * @param  dir   direction of the gravitation force acting on the node
 *         is added to this variable (output parameter)
 * @return approximate second derivation of the gravitation energy
 */
double LinLogLayout::addGravitationDir(node u, double* dir) {
  const Coord& position = layoutResult->getNodeValue(u);

  double dist = getDist(position, baryCenter);

  double u_weight = linLogWeight->getNodeValue(u);

  double tmp = gravFactor * repuFactor * u_weight * pow(dist, attrExponent-2);

  for (unsigned int d = 0; d < _dim; ++d) {
    dir[d] += (baryCenter[d] - position[d]) * tmp;
  }

  return tmp * fabs(attrExponent-1.0);
}


/**
 * Computes the direction of the total force acting on a node.
 * @param  node  node
 * @param  dir   direction of the total force acting on the node
 *               (output parameter)
 */
void LinLogLayout::getDirection (node u, double* dir) {

  for (unsigned int d=0; d<_dim; ++d)
    dir[d] = 0.0;

  double dir2 = addRepulsionDir(u, dir);
  dir2 += addAttractionDir(u, dir);
  dir2 += addGravitationDir(u, dir);

  const Coord& position = layoutResult->getNodeValue(u);

  // compute average Euclidean distance to other nodes
  double avgDist = 0.0;
  node v;
  forEach(v, graph->getNodes()) {
    const Coord& position2 = layoutResult->getNodeValue(v);

    avgDist += getDist(position, position2);
  }
  avgDist /= graph->numberOfNodes()-1;

  if (dir2 != 0.0) {
    // normalize force vector with second derivation of energy
    for (unsigned int d=0; d<_dim; ++d)
      dir[d] /= dir2;

    // ensure that the length of dir is not greater
    // than average Euclidean distance to other nodes

    double length = 0;//calculate length of Dir //getDist(dir, new double[_dim]);

    if (avgDist > 0.0 && length > avgDist) {
      length /= avgDist;

      for (unsigned int d=0; d<_dim; ++d)
        dir[d] /= length;
    }
  }
  else {
    for (unsigned int d=0; d<_dim; ++d)
      dir[d] = 0.0;
  }
}

void LinLogLayout::getDirection (node u, double* dir, OctTree* tree) {
  for (unsigned int d=0; d<_dim; ++d)
    dir[d] = 0.0;

  double dir2 = addRepulsionDir(u, dir, tree);
  dir2 += addAttractionDir(u, dir);
  dir2 += addGravitationDir(u, dir);

  if (dir2 != 0.0) {
    // normalize force vector with second derivation of energy
    for (unsigned int d=0; d<_dim; ++d)
      dir[d] /= dir2;

    // ensure that the length of dir is not greater
    // than 1/16 of the octtree width,
    // to prevent the node from leaving the octtree region
    double scale = 1.0;

    for (unsigned int d = 0; d < _dim; ++d) {
      double width = tree->maxPos[d] - tree->minPos[d];

      if (width > 0.0)
        scale = std::min(scale, fabs(width/16/dir[d]));
    }

    for (unsigned int d=0; d<_dim; ++d)
      dir[d] *= scale;
  }
  else {
    for (unsigned int d=0; d<_dim; ++d)
      dir[d] = 0.0;
  }
}

/**
 * Iteratively minimizes energy using the Barnes-Hut algorithm.
 * Starts from the positions in the parameter <code>positions</code>,
 * and stores the computed positions in <code>positions</code>.
* @param positions  position in <code>nrDims</code>-dimensional space for each node.
*   Is not copied and serves as input and output parameter.
*   Each position must be a <code>double[nrDims]</code>.
*   If the input is two-dimensional (i.e. the third array element
*   is 0.0 for all nodes), the output is also two-dimensional.
*   Different nodes with nonzero weights must have different positions.
*   Random initial positions are appropriate.
* @param nrIterations  number of iterations. Choose appropriate values
*   by observing the convergence of energy.  A typical value is 100.
*/
bool LinLogLayout::minimizeEnergyNoTree (int nrIterations) {
  if (graph->numberOfNodes() <= 1)
    return true;

  initEnergyFactors();

  double finalAttrExponent = attrExponent;
  double finalRepuExponent = repuExponent;

  // compute initial energy
  computeBaryCenter();

  double energySum = 0.0;

  node u;
  forEach(u, graph->getNodes())
  energySum += getEnergy(u);

  // minimize energy
  double oldPos[3] = {0,0,0};
  double bestDir[3] = {0,0,0};

  for (int step = 1; step <= nrIterations; ++step) {
    computeBaryCenter();

    if (nrIterations >= 50 && finalRepuExponent < 1.0) {
      attrExponent = finalAttrExponent;
      repuExponent = finalRepuExponent;

      if (step <= 0.6*nrIterations) {
        // use energy model with few local minima
        attrExponent += 1.1 * (1.0 - finalRepuExponent);
        repuExponent += 0.9 * (1.0 - finalRepuExponent);
      }
      else if (step <= 0.9*nrIterations) {
        // gradually move to final energy model
        attrExponent += 1.1 * (1.0 - finalRepuExponent)
                        * (0.9 - ((double)step)/nrIterations) / 0.3;
        repuExponent += 0.9 * (1.0 - finalRepuExponent)
                        * (0.9 - ((double)step)/nrIterations) / 0.3;
      }
    }

    // move each node
    int count = 0;
    energySum = 0.0;
    forEach(u, graph->getNodes()) {

      count++;

      double oldEnergy = getEnergy(u);
      // compute direction of the move of the node
      getDirection(u, bestDir);

      // line search: compute length of the move
      Coord pos = layoutResult->getNodeValue(u);

      for (unsigned int d=0; d<_dim; ++d)
        oldPos[d] = pos[d];

      double bestEnergy = oldEnergy;
      int bestMultiple = 0;

      for (unsigned int d=0; d<_dim; ++d)
        bestDir[d] /= 32;

      for (int multiple = 32;
           multiple >= 1 && (bestMultiple==0 || bestMultiple/2==multiple);
           multiple /= 2) {
        for (unsigned int d=0; d<_dim; ++d)
          pos[d] = oldPos[d] + bestDir[d] * multiple;

        if(!skipNodes || !skipNodes->getNodeValue(u))
          //if(!noackPillar->getNodeValue(u))
          layoutResult->setNodeValue(u, pos);

        double curEnergy = getEnergy(u);

        if (curEnergy < bestEnergy) {
          bestEnergy = curEnergy;
          bestMultiple = multiple;
        }
      }

      for (int multiple = 64;
           multiple <= 128 && bestMultiple == multiple/2;
           multiple *= 2) {
        for (unsigned int d=0; d<_dim; ++d)
          pos[d] = oldPos[d] + bestDir[d] * multiple;

        if(!skipNodes || !skipNodes->getNodeValue(u))
          //if(!noackPillar->getNodeValue(u))
          layoutResult->setNodeValue(u, pos);

        double curEnergy = getEnergy(u);

        if (curEnergy < bestEnergy) {
          bestEnergy = curEnergy;
          bestMultiple = multiple;
        }
      }

      for (unsigned int d=0; d<_dim; ++d)
        pos[d] = oldPos[d] + bestDir[d] * bestMultiple;

      if(!skipNodes || !skipNodes->getNodeValue(u))
        //if(!noackPillar->getNodeValue(u))
        layoutResult->setNodeValue(u, pos);

      energySum += bestEnergy;

    }

    if ((step *100 / nrIterations) % 10 == 0 && pluginProgress->progress(step, nrIterations)!=TLP_CONTINUE)
      return pluginProgress->state()!=TLP_CANCEL;
  }

  return true;
}

bool LinLogLayout::minimizeEnergy (int nrIterations) {
  if (graph->numberOfNodes() <= 1)
    return true;

  initEnergyFactors();

  double finalAttrExponent = attrExponent;
  double finalRepuExponent = repuExponent;

  // compute initial energy
  computeBaryCenter();
  OctTree* octTree = buildOctTree();

  double energySum = 0.0;

  node u;
  forEach(u, graph->getNodes())
  energySum += getEnergy(u, octTree);

  // minimize energy
  double oldPos[3] = {0,0,0};
  double bestDir[3] = {0,0,0};

  for (int step = 1; step <= nrIterations; ++step) {
    computeBaryCenter();
    octTree = buildOctTree();

    if (nrIterations >= 50 && finalRepuExponent < 1.0) {
      attrExponent = finalAttrExponent;
      repuExponent = finalRepuExponent;

      if (step <= 0.6*nrIterations) {
        // use energy model with few local minima
        attrExponent += 1.1 * (1.0 - finalRepuExponent);
        repuExponent += 0.9 * (1.0 - finalRepuExponent);
      }
      else if (step <= 0.9*nrIterations) {
        // gradually move to final energy model
        attrExponent += 1.1 * (1.0 - finalRepuExponent)
                        * (0.9 - ((double)step)/nrIterations) / 0.3;
        repuExponent += 0.9 * (1.0 - finalRepuExponent)
                        * (0.9 - ((double)step)/nrIterations) / 0.3;
      }
    }

    // move each node
    int count = 0;
    energySum = 0.0;
    forEach(u, graph->getNodes()) {
      ++count;
      double oldEnergy = getEnergy(u, octTree);

      // compute direction of the move of the node
      getDirection(u, bestDir, octTree);

      // line search: compute length of the move
      Coord pos = layoutResult->getNodeValue(u);

      for (unsigned int d=0; d<_dim; ++d)
        oldPos[d] = pos[d];

      double bestEnergy = oldEnergy;
      int bestMultiple = 0;

      for (unsigned int d=0; d<_dim; ++d)
        bestDir[d] /= 32;

      for (int multiple = 32;
           multiple >= 1 && (bestMultiple==0 || bestMultiple/2==multiple);
           multiple /= 2) {
        octTree->removeNode(u, pos, 0);

        for (unsigned int d=0; d<_dim; ++d)
          pos[d] = oldPos[d] + bestDir[d] * multiple;

        octTree->addNode(u, pos, 0);


        if(!skipNodes || !skipNodes->getNodeValue(u))
          layoutResult->setNodeValue(u, pos);

        double curEnergy = getEnergy(u);

        if (curEnergy < bestEnergy) {
          bestEnergy = curEnergy;
          bestMultiple = multiple;
        }
      }

      for (int multiple = 64;
           multiple <= 128 && bestMultiple == multiple/2;
           multiple *= 2) {
        octTree->removeNode(u, pos, 0);

        for (unsigned int d=0; d<_dim; ++d)
          pos[d] = oldPos[d] + bestDir[d] * multiple;

        octTree->addNode(u, pos, 0);

        if(!skipNodes || !skipNodes->getNodeValue(u))
          layoutResult->setNodeValue(u, pos);

        double curEnergy = getEnergy(u);

        if (curEnergy < bestEnergy) {
          bestEnergy = curEnergy;
          bestMultiple = multiple;
        }
      }

      for (unsigned int d=0; d<_dim; ++d)
        pos[d] = oldPos[d] + bestDir[d] * bestMultiple;

      if(!skipNodes || !skipNodes->getNodeValue(u))
        layoutResult->setNodeValue(u, pos);

      energySum += bestEnergy;
    }

    if ((step *100 / nrIterations) % 10 == 0 &&
        pluginProgress->progress(step, nrIterations)!=TLP_CONTINUE)
      return pluginProgress->state()!=TLP_CANCEL;
  }

  return true;
}



/**
* Computes the position of the barycenter of all nodes
* and stores it in the attribute <code>baryCenter</code>.
*/
void LinLogLayout::computeBaryCenter () {
  for (unsigned int d=0; d<_dim; ++d)
    baryCenter[d] = 0.0;

  double weightSum = 0.0;
  node u;
  forEach(u, graph->getNodes()) {
    double u_weight = linLogWeight->getNodeValue(u);
    weightSum += u_weight;
    const Coord& position = layoutResult->getNodeValue(u);

    for (unsigned int d=0; d<_dim; ++d)
      baryCenter[d] += u_weight * position[d];
  }

  if (weightSum > 0.0) {
    for (unsigned int d=0; d<_dim; ++d)
      baryCenter[d] /= weightSum;
  }
}


void LinLogLayout::initWeights2 () {
  linLogWeight = new tlp::DoubleProperty(graph);
  linLogWeight->setAllNodeValue(0.0);

  double weight = 0.0;
  node u;

  if (edgeWeight == NULL)
    forEach(u, graph->getNodes()) {
    weight = 0.0;
    edge e;
    forEach(e,  graph->getInOutEdges(u)) {
      ++weight;
      linLogWeight->setEdgeValue(e,1.0);
    }

    linLogWeight->setNodeValue (u, weight);
  }
  else
    forEach(u, graph->getNodes()) {
    weight = 0.0;
    edge e;
    forEach(e,  graph->getInOutEdges(u)) {
      double tmpweight = edgeWeight->getEdgeDoubleValue(e)*100.0+1.0;
      weight+= tmpweight ;
      linLogWeight->setEdgeValue(e,tmpweight);
    }

    linLogWeight->setNodeValue (u, weight);
  }
}

void LinLogLayout::initWeights () {
  linLogWeight = new tlp::DoubleProperty(graph);
  linLogWeight->setAllNodeValue(0.0);
  linLogWeight->setAllEdgeValue(0.0);

  node u;

  if (edgeWeight == NULL) {
    edge e;
    forEach(e,  graph->getEdges()) {
      const std::pair<node, node>& eEnds = graph->ends(e);
      node u = eEnds.first;
      node v = eEnds.second;
      double wu = linLogWeight->getNodeValue(u);
      double wv = linLogWeight->getNodeValue(v);

      linLogWeight->setEdgeValue(e,1.0);
      linLogWeight->setNodeValue(u,wu+1.0);
      linLogWeight->setNodeValue(v,wv+1.0);
    }
  }
  else
    forEach(u, graph->getNodes()) {
    double weight = 0.0;
    edge e;
    forEach(e,  graph->getInOutEdges(u)) {
      double tmpweight = edgeWeight->getEdgeDoubleValue(e)*100.0+1.0;
      weight+= tmpweight ;
      linLogWeight->setEdgeValue(e,tmpweight);
    }

    linLogWeight->setNodeValue (u, weight);
  }
}


OctTree* LinLogLayout::buildOctTree() {
  // compute mimima and maxima of positions in each dimension
  Coord minPos (100000, 100000, 100000);
  Coord maxPos (-100000,-100000,-100000);
  Coord zero (0,0,0);

  node u;
  forEach(u, linLogWeight->getNonDefaultValuatedNodes()) {
    const Coord& position = layoutResult->getNodeValue(u);

    for (unsigned int d = 0; d < _dim; ++d) {
      minPos[d] = std::min (position[d], minPos[d]);
      maxPos[d] = std::max (position[d], maxPos[d]);
    }
  }

  // provide additional space for moving nodes
  for (unsigned int d = 0; d < _dim; ++d) {
    double posDiff = maxPos[d] - minPos[d];
    maxPos[d] += posDiff / 2;
    minPos[d] -= posDiff / 2;
  }

  node n;
  // add nodes with non-zero weight to the octtree
  OctTree* result = new OctTree(u, zero, minPos, maxPos, linLogWeight, true);

  forEach (u, linLogWeight->getNonDefaultValuatedNodes()) {
    result->addNode (u, layoutResult->getNodeValue(u), 0);
  }
  return result;
}

void LinLogLayout::setAttrExp (double d) {
  attrExponent = d;
}

void LinLogLayout::setRepExp (double d) {
  repuExponent = d;
}

void LinLogLayout::setGravFact (double d) {
  gravFactor = d;
}
