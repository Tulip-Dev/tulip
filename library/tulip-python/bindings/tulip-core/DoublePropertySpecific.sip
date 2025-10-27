/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+


//===========================================================================================
enum StandardMetaValueCalculator /BaseType=UIntEnum/ {
  NO_CALC = StandardMetaValueCalculator::NO_CALC,
  AVG_CALC = StandardMetaValueCalculator::AVG_CALC,
  SUM_CALC = StandardMetaValueCalculator::SUM_CALC,
  MAX_CALC = StandardMetaValueCalculator::MAX_CALC,
  MIN_CALC = StandardMetaValueCalculator::MIN_CALC
};

void setMetaValueCalculator(tlp::DoubleProperty::StandardMetaValueCalculator nodeCalc = tlp::DoubleProperty::StandardMetaValueCalculator::AVG_CALC,
                            tlp::DoubleProperty::StandardMetaValueCalculator edgeCalc = tlp::DoubleProperty::StandardMetaValueCalculator::AVG_CALC);

%Docstring
tlp.DoubleProperty.setMetaValueCalculator(nodeCalc = tlp.DoubleProperty.StandardMetaValueCalculator.AVG_CALC, edgeCalc = tlp.DoubleProperty.StandardMetaValueCalculator.AVG_CALC)

Sets the predefined calculator for meta-nodes and meta-edges. It is used to compute the value of a double property for each meta-node
and meta-edge based on the aggregated nodes and edges. The available calculators are:

  * tlp.DoubleProperty.StandardMetaValueCalculator.NO_CALC: do nothing
  * tlp.DoubleProperty.StandardMetaValueCalculator.AVG_CALC: computes the average value of the grouped elements
  * tlp.DoubleProperty.StandardMetaValueCalculator.SUM_CALC: computes the sum of the grouped elements
  * tlp.DoubleProperty.StandardMetaValueCalculator.MAX_CALC: computes the maximum value of the grouped elements
  * tlp.DoubleProperty.StandardMetaValueCalculator.MIN_CALC: computes the minimum value of the grouped elements

:param nodeCalc: a predefined calculator value used for nodes
:type nodeCalc: tlp.DoubleProperty.StandardMetaValueCalculator
:param edgeCalc: a predefined calculator value used for edges
:type edgeCalc: tlp.DoubleProperty.StandardMetaValueCalculator
%End

//===========================================================================================
  double getNodeMin(const tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getNodeMin(subgraph=None)

Returns the minimum value on the nodes for that double property.

:param subgraph: a subgraph can be given in parameter, in that case return the minimum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End

//===========================================================================================

  double getNodeMax(const tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getNodeMax(subgraph=None)

Returns the maximum value on the nodes for that double property.

:param subgraph: a subgraph can be given in parameter, in that case return the maximum value on the nodes belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End

//===========================================================================================

  double getEdgeMin(const tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getEdgeMin(subgraph=None)

Returns the minimum value on the edges for that double property.

:param subgraph: a subgraph can be given in parameter, in that case return the minimum value on the edges belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End

//===========================================================================================

  double getEdgeMax(const tlp::Graph *subgraph=0);
%Docstring
tlp.DoubleProperty.getEdgeMax(subgraph = None)

Returns the maximum value on the edges for that double property.

:param subgraph: a subgraph can be given in parameter, in that case return the maximum value on the edges belonging to that subgraph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End

//===========================================================================================

  void uniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.uniformQuantification(n)

Performs an uniform quantification on nodes and edges based on the values stored in that double property.
An uniform quantification tries to separate the nodes and edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes and edges will be erased and replaced by the id of the class they belong to.
%End

//===========================================================================================

  void nodesUniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.nodesUniformQuantification(n)

Performs an uniform quantification on nodes based on the values stored in that double property.
An uniform quantification tries to separate the nodes in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes will be erased and replaced by the id of the class they belong to.
%End

//===========================================================================================

  void edgesUniformQuantification(unsigned int n);
%Docstring
tlp.DoubleProperty.edgesUniformQuantification(n)

Performs an uniform quantification on edges based on the values stored in that double property.
An uniform quantification tries to separate the edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on edges will be erased and replaced by the id of the class they belong to.
%End

private:

  void treatEvent(const tlp::Event&);
