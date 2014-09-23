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
#ifndef Tulip_GEMLayout_H
#define Tulip_GEMLayout_H

#include <vector>

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup layout */

/// An implementation of a spring-embedder layout.
/** This plugin is an implementation of the GEM-2d layout
 *  algorithm first published as:
 *
 *  A. Frick, A. Ludwig, and H. Mehldau, \n
 *  "A fast, adaptive layout algorithm for undirected graphs", \n
 *  In R. Tamassia and I. Tollis (Eds), \n
 *  Graph Drawing'94, \n
 *  Volume 894 of Lecture Notes in Computer Science, Springer Verlag, 1995.
 *
 *  <b>HISTORY</b>
 *
 *  The implementation started life as the public-domain
 *  code produced by Arne Frick and released at
 *
 *  www.frick-consulting.de/publications.html
 *
 *  The core "embedder" part of the algorithm was used in the
 *  implementation of a Java plugin for the CWI "Royere" tool,
 *  and then this code was ported to C++ to make the
 *  implementation given here.
 *
 *
 *  \note The embedder algorithm described by Frick involves three
 *  phases: insertion, arrangement, and optimization.  Only
 *  the first two of these phases are included here.
 *  Experiments with the Java implementation showed that the
 *  optimization phase consumed significantly more time than
 *  the first two and produced apparently marginal improvements
 *  in the final layout.\n
 *  As GEM, like other spring-embedder algorithms, is
 *  computationally expensive, I have tried to optimize the
 *  loops somewhat by storing all necessary node information
 *  into two arrays: "GemProp" carries the (scalar) values
 *  associated with each node by the layout algorithm, while
 *  "Adjacent" is an array of vectors, one vector per node,
 *  giving the index (integer) of each node.
 *
 * \note The new version has been reimplemented to manage edge length
 * it merges the 3D stuff and removes the use of integers (new CPU do not
 * require it anymore).
 *
 *  \author David Duke, University of Bath, UK: Email: D.Duke@bath.ac.uk
 *  \author David Auber,University of Bordeaux, FR: Email: david.auber@labri.fr
 *  Version 0.1: 23 July 2001.
 *  Version 0.2: September 2006
 */

class GEMLayout : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("GEM (Frick)","Tulip Team","16/10/2008","Stable","1.2","Force Directed")
  GEMLayout(const tlp::PluginContext* context);
  ~GEMLayout();
  bool run();

private:
  tlp::Coord computeForces(unsigned int v,
                           float shake,
                           float gravity,
                           bool testPlaced);

  struct GEMparticule {
    tlp::node n;
    tlp::Coord pos;        // position
    int in;
    tlp::Coord imp;        // impulse
    float dir;        // direction
    float heat;        // heat
    float mass;        // weight = nr incident edges
    unsigned int id;
    GEMparticule(float m = 0) : in(0), id(UINT_MAX) {
      pos.fill(0);
      imp.fill(0);
      dir  = 0.0;
      heat = 0;
      mass = m;
    }
  };

  /*
   * Functions used to implement the GEM3D layout.
   */

  unsigned int select();
  void vertexdata_init(const float starttemp);
  void insert();
  void displace(unsigned int v, tlp::Coord imp);
  void a_round();
  void arrange();
  void updateLayout();

  std::vector<GEMparticule> _particules;
  std::vector<int> _map; //for random selection
  tlp::MutableContainer<GEMparticule *> _nodeToParticules;

  /*
   * GEM3D variables
   */

  unsigned long Iteration;
  float         _temperature;
  tlp::Coord    _center;
  float         _maxtemp;
  float         _oscillation, _rotation;


  /*
   * Following parameters can be initialised in the original GEM3D
   * from a configuration file.  Here they are hard-wired, but
   * this could be replaced by configuration from a file.
   */
  const float    i_maxtemp;
  const float    a_maxtemp;

  const float    i_starttemp;
  const float    a_starttemp;

  const float    i_finaltemp;
  const float    a_finaltemp;

  const int      i_maxiter;
  const int      a_maxiter;

  const float    i_gravity;
  const float    a_gravity;

  const float    i_oscillation;
  const float    a_oscillation;

  const float    i_rotation;
  const float    a_rotation;

  const float    i_shake;
  const float    a_shake;

  unsigned int _dim; //2 or 3;
  unsigned int _nbNodes; //number of nodes in the graph
  bool _useLength; //if we manage edge length
  tlp::NumericProperty* metric; //metric for edge length
  tlp::BooleanProperty* fixedNodes; //selection of not movable nodes
  unsigned int max_iter; // the max number of iterations

};

#endif
