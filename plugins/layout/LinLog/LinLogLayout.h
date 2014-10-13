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
#ifndef LinLogLayout_H
#define LinLogLayout_H

#include <tulip/TulipPluginHeaders.h>
#include "OctTree.h"

class LinLogLayout {

public:
  LinLogLayout (tlp::Graph* _graph, tlp::PluginProgress* pluginProgress);
  ~LinLogLayout ();
  bool initAlgo (tlp::LayoutProperty* _layoutResult,
                 tlp::NumericProperty* _weight,
                 double _attrExponent,
                 double _repuExponent,
                 double _gravFactor,
                 unsigned int _max_iter,
                 bool _is3D,
                 bool _useOctTree,
                 tlp::BooleanProperty* _skipNodes);

  void setAttrExp (double);
  void setGravFact (double);
  void setRepExp (double);
  bool startAlgo ();

private:

  tlp::NumericProperty* edgeWeight;
  tlp::LayoutProperty* layoutResult;
  tlp::DoubleProperty* linLogWeight;
  tlp::BooleanProperty* skipNodes;
  tlp::Graph* graph;

  tlp::PluginProgress* pluginProgress;

  unsigned int _dim; //2 or 3;
  unsigned int _nbNodes; //number of nodes in the graph
  unsigned int max_iter; // the max number of iterations
  bool useOctTree;

  /** Factor for repulsion energy. */
  double repuFactor;
  /** Exponent of the Euclidean distance in the repulsion energy. */
  double repuExponent;
  /** Exponent of the Euclidean distance in the attraction energy. */
  double attrExponent;
  /** Factor for the gravitation energy = attraction to the barycenter.
      Set to 0.0 for no gravitation. */
  double gravFactor;

  /** Position of the barycenter of the nodes. */
  Coord baryCenter;

  double getGravitationEnergy (node u) ;
  double getAttractionEnergy (node u);
  double getRepulsionEnergy (node u);
  double getEnergy (node u);
  double getDist (const Coord& pos1, const Coord& pos2) ;
  double getDistForComparison (const Coord& pos1, const Coord& pos2) ;


  double addRepulsionDir (node u, double* dir);
  double addAttractionDir (node u, double* dir);
  double addGravitationDir (node u, double* dir);
  void getDirection (node u, double* dir);

  bool minimizeEnergyNoTree (int nrIterations);

  void initEnergyFactors ();
  void computeBaryCenter ();

  void setComputationalWeigths ();

  void initWeights ();
  void initWeights2 (); //old initialization

  OctTree* buildOctTree ();
  bool minimizeEnergy (int nrIterations);
  double addRepulsionDir (node u, double* dir, OctTree* tree);
  double getRepulsionEnergy (node u, OctTree* tree);
  double getEnergy (node u, OctTree* tree);
  void getDirection(node u, double* dir, OctTree* tree);
};
#endif
