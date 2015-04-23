/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

namespace tlp {

/** \brief Class use to represent a graph
 *
 * GlComposite use to represent a graph with nodes, metanodes and edges
 */
class GlGraphComposite : tlp::GlComposite /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/GlGraphComposite.h>
%End

public:

  /**
   * Build a GlGraphComposite with the graph data
   */
  GlGraphComposite(tlp::Graph* graph);

  ~GlGraphComposite();

  /**
   * Set the rendering parameters use for rendering
   */
  void setRenderingParameters(const tlp::GlGraphRenderingParameters &parameter);

  /**
   * Return a pointer on rendering parameters used for rendering
   */
  tlp::GlGraphRenderingParameters* getRenderingParameters();
%MethodCode
    sipRes = new tlp::GlGraphRenderingParameters(sipCpp->getRenderingParameters());
%End

  /**
   * Return the inputData use by the composite
   */
  tlp::GlGraphInputData* getInputData();

  /**
   * Function used to visit composite's children
   */
  virtual void acceptVisitor(tlp::GlSceneVisitor *visitor);

};
};

