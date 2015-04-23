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

        tlp::Coord getMax(tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.getMax([subgraph = None])

Returns the maximum coordinate in that layout property,
i.e. the top-right corner of the graph drawing bounding box.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum coordinate of the sub-graph layout.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Coord` 
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End	

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipRes = new tlp::Coord(sipCpp->getMax(a0));
    }
%End
	
//===========================================================================================	
	
        tlp::Coord getMin(tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.getMin([subgraph = None])

Returns the minimum coordinate in that layout property,
i.e. the bottom-left corner of the graph drawing bounding box.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum coordinate of the sub-graph layout.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Coord` 
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipRes = new tlp::Coord(sipCpp->getMin(a0));
    }
%End

//===========================================================================================

        void translate(const tlp::Vec3f &move, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.translate(move[, subgraph = None])

Translates the current layout according to a movement vector.

:param move: a movement vector
:type move: :class:`tlp.Vec3f`
:param subgraph: a sub-graph can be given in parameter, in that case translates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End 

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->translate(*a0, a1);
    }
%End
 
 //===========================================================================================
 
        void translate(const tlp::Vec3f &move, tlp::Iterator<tlp::node> *itNodes, tlp::Iterator<tlp::edge> *itEdges);
%Docstring
tlp.LayoutProperty.translate(move, itNodes, itEdges)

Translates the layout of a set of nodes and edges provided through iterators according to a movement vector.

:param move: a movement vector
:type move: :class:`tlp.Vec3f`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End


	
//===========================================================================================	
	
        void scale(const tlp::Vec3f &scaleFactors, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.scale(scaleFactors[, subgraph = None])

Scales the layout according to a vector of scale factors (sx, sy, sz).

:param move: a vector of scale factors
:type move: :class:`tlp.Vec3f`
:param subgraph: a sub-graph can be given in parameter, in that case scales the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End 

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->scale(*a0, a1);
    }
%End

	
//===========================================================================================	
	
        void scale(const tlp::Vec3f &scaleFactors, tlp::Iterator<tlp::node> *itNodes, tlp::Iterator<tlp::edge> *itEdges);
%Docstring
tlp.LayoutProperty.scale(scaleFactors, itNodes, itEdges)

Scales the layout of the nodes and edges provided through iterators according to a vector of scale factors (sx, sy, sz).

:param move: a vector of scale factors
:type move: :class:`tlp.Vec3f`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End

//===========================================================================================

        void rotateX(const double &alpha, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.rotateX(alpha[, subgraph = None])

.. versionadded:: 3.7

Rotates the layout around the X-axis according to an angle in degrees.

:param alpha: an angle in degrees
:type alpha: float
:param subgraph: a sub-graph can be given in parameter, in that case rotates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->rotateX(a0, a1);
    }
%End


//===========================================================================================

        void rotateY(const double &alpha, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.rotateY(alpha[, subgraph = None])

.. versionadded:: 3.7

Rotates the layout around the Y-axis according to an angle in degrees.

:param alpha: an angle in degrees
:type alpha: float
:param subgraph: a sub-graph can be given in parameter, in that case rotates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->rotateY(a0, a1);
    }
%End

//===========================================================================================
  
        void rotateZ(const double &alpha, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.rotateZ(alpha[, subgraph = None])

Rotates the layout around the Z-axis according to an angle in degrees.

:param alpha: an angle in degrees
:type alpha: float
:param subgraph: a sub-graph can be given in parameter, in that case rotates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End 

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->rotateZ(a0, a1);
    }
%End

//===========================================================================================

        void rotateX(const double &alpha, tlp::Iterator<tlp::node> *itNodes, tlp::Iterator<tlp::edge> *itEdges);
%Docstring
tlp.LayoutProperty.rotateX(alpha, itNodes, itEdges)

Rotates the layout around the X-axis of the nodes and edges provided through iterators according to an angle in degrees. 

:param alpha: an angle in degrees
:type alpha: float
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End
	
//===========================================================================================	
	
        void rotateY(const double &alpha, tlp::Iterator<tlp::node> *itNodes, tlp::Iterator<tlp::edge> *itEdges);
%Docstring
tlp.LayoutProperty.rotateY(alpha, itNodes, itEdges)

Rotates the layout around the Y-axis of the nodes and edges provided through iterators according to an angle in degrees. 

:param alpha: an angle in degrees
:type alpha: float
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End
	
//===========================================================================================	
	
        void rotateZ(const double &alpha, tlp::Iterator<tlp::node> *itNodes, tlp::Iterator<tlp::edge> *itEdges);
%Docstring
tlp.LayoutProperty.rotateZ(alpha, itNodes, itEdges)

Rotates the layout around the Z-axis of the nodes and edges provided through iterators according to an angle in degrees. 

:param alpha: an angle in degrees
:type alpha: float
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End	
	
//===========================================================================================	
	
     void center(tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.center([subgraph = None])

Centers the layout, meaning translating it in order that the center of its bounding box is (0,0,0)

:param subgraph: a sub-graph can be given in parameter, in that case only centers the layout of that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->center(a0);
    }
%End

//===========================================================================================

     void center(const tlp::Vec3f &newCenter, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.center(newCenter[, subgraph = None])

.. versionadded:: 3.7

Centers the layout to newCenter, meaning translating it in order that
the center of its bounding box is equal to newCenter.

:param newCenter: the new center of the layout
:type newCenter: :class:`tlp.Vec3f`
:param subgraph: a sub-graph can be given in parameter, in that case only centers the layout of that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->center(*a0, a1);
    }
%End

//===========================================================================================	
	
        void normalize(tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.normalize([subgraph = None])

Normalizes the layout, meaning dividing each nodes and edges coordinate by the maximum magnitude of the whole coordinates set

:param subgraph: a sub-graph can be given in parameter, in that case only normalizes the layout of that sub-graph.
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
        sipCpp->normalize(a0);
    }
%End
	
//===========================================================================================	
	
	void perfectAspectRatio();
%Docstring
tlp.LayoutProperty.perfectAspectRatio()

Scales the layout in order to approach an aspect ratio (width / height) of 1.0 . 
%End  
  
//=========================================================================================== 
  
        double edgeLength(const tlp::edge e);
%Docstring
tlp.LayoutProperty.edgeLength(edge)

Returns the length of an edge, the bends are taken into account.
Thus, it measures the length of a polyline.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: float
:throws: an exception if the edge does not belong to the graph attached to the property

.. warning:: This function only measures the length of the polyline between bends, when using curves like Bezier etc... the result will not be the length of the curve.
%End

%MethodCode
        if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->edgeLength(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================

  double averageEdgeLength(const tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.averageEdgeLength([subgraph=None])

.. versionadded:: 3.7

Returns the average edge length of the layout, the bends are taken into account.

:param subgraph: a sub-graph can be given in parameter, in that case only computes the average edge length for that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }

    if (sipIsErr == 0) {
        sipRes = sipCpp->averageEdgeLength(a0);
    }
%End

//===========================================================================================
  
        double averageAngularResolution(const tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.averageAngularResolution([subgraph = None])

Returns the average angular resolution of the layout.
It is only defined for 2D drawing, meaning the third coordinate is omitted

:param subgraph: a sub-graph can be given in parameter, in that case computes the average angular resolution for the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End
  
%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }

    if (sipIsErr == 0) {
        sipRes = sipCpp->averageAngularResolution(a0);
    }
%End

//===========================================================================================  
  
        double averageAngularResolution(const tlp::node n, const tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.averageAngularResolution(node[, subgraph = None])

Returns the average angular resolution of a node.
It is only defined for 2D drawing, meaning the third coordinate is omitted

:param node: an existing node
:type node: :class:`tlp.node`
:param subgraph: a sub-graph can be given in parameter, in that case computes the average angular resolution of a node in that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float
:throws: an exception if the node does not belong to the graph attached to the property or if the provided sub-graph is not a descendant of the graph attached to the property
%End 


%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
	if (sipCpp->getGraph()->isElement(*a0)) {
		if (a1 && a1->isElement(*a0)) {
			sipRes = sipCpp->averageAngularResolution(*a0, a1);
		} else {
			if (!a1) {
				sipRes = sipCpp->averageAngularResolution(*a0);
			} else {
				sipIsErr = throwInvalidNodeException(const_cast<tlp::Graph*>(a1), *a0);
			}
		}
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
        }
    }
%End
  
//===========================================================================================  
  
        std::vector<double> angularResolutions(const tlp::node n, const tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.angularResolutions(node[, subgraph=None])

Returns a list of all angular resolution of a node.
It is only defined for 2D drawing, meaning the third coordinates is omitted

:param node: an existing node
:type node: :class:`tlp.node`
:param subgraph: a sub-graph can be given in parameter, in that case returns the list of angular resolution for the node in that sub-graph
:type subgraph: :class:`tlp.Graph`
:rtype: list of float
:throws: an exception if the node does not belong to the graph attached to the property or if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode
    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
	if (sipCpp->getGraph()->isElement(*a0)) {
		if (a1 && a1->isElement(*a0)) {
			sipRes = new std::vector<double>(sipCpp->angularResolutions(*a0, a1));
		} else {
			if (!a1) {
				sipRes = new std::vector<double>(sipCpp->angularResolutions(*a0));
			} else {
				sipIsErr = throwInvalidNodeException(const_cast<tlp::Graph*>(a1), *a0);
			}
		}
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
        }
    }
%End

//===========================================================================================

        void computeEmbedding(tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.computeEmbedding([subgraph=None])

Fixes embedding of the graph according to the layout,
ie. sort edges around nodes according to their neighbors/bends position in the layout.
Only works in 2D, the third coordinate is not taken into account.

:param subgraph: a sub-graph can be given in parameter, in that case fixes embedding of that sub-graph
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the provided sub-graph is not a descendant of the graph attached to the property
%End  

%MethodCode
    if (a0) {
        if (!sipCpp->getGraph()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }

    if (sipIsErr == 0) {
        sipCpp->computeEmbedding(a0);
    }
%End
  
 //===========================================================================================
  
        void computeEmbedding(const tlp::node n, tlp::Graph *subgraph=NULL);
%Docstring
tlp.LayoutProperty.computeEmbedding(node[, subgraph=None])

Fixes embedding of the node according to the layout,
ie. sort edges around the node according to their neighbors/bends position in the layout.
Only works in 2D, the third coordinate is not taken into account.

:param subgraph: a sub-graph can be given in parameter, in that case fixes embedding of the node in that sub-graph
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the node does not belong to the graph attached to the property or if the provided sub-graph is not a descendant of the graph attached to the property
%End

%MethodCode

    if (a1) {
        if (!sipCpp->getGraph()->isDescendantGraph(a1) && a1 != sipCpp->getGraph()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : <graph " << a1->getName() << " (id " << a1->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
    if (sipIsErr == 0) {
	if (sipCpp->getGraph()->isElement(*a0)) {
		if (a1 && a1->isElement(*a0)) {
			sipCpp->computeEmbedding(*a0, a1);
		} else {
			if (!a1) {
				sipCpp->computeEmbedding(*a0);
			} else {
				sipIsErr = throwInvalidNodeException(const_cast<tlp::Graph*>(a1), *a0);
			}
		}
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
	} 
    }
%End

 //===========================================================================================
  
//  	unsigned int crossingNumber() const;
//%Docstring
//tlp.LayoutProperty.crossingNumber()

//Returns the number of edges crossings in the layout.
//%End
  	

private:

  void treatEvent(const tlp::Event&);
