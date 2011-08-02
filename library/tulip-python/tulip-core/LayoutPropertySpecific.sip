
	tlp::Coord getMax(tlp::Graph *graph=0);
%Docstring
tlp.LayoutProperty.getMax([subgraph = None])

Returns the maximum coordinate on the nodes in that layout property, 
i.e. the top-right corner of the graph drawing bounding box.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum coordinate on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Coord` 
%End	
	
	
//===========================================================================================	
	
	tlp::Coord getMin(tlp::Graph *graph=0);
%Docstring
tlp.LayoutProperty.getMin([subgraph = None])

Returns the minimum coordinate on the nodes in that layout property, 
i.e. the bottom-left corner of the graph drawing bounding box.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum coordinate on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Coord` 
%End

//===========================================================================================

	void translate(const tlp::Coord& v, tlp::Graph *graph=0);
%Docstring
tlp.LayoutProperty.translate(move[, subgraph = None])

Translates the current layout according to a movement vector.

:param move: a movement vector
:type move: :class:`tlp.Coord`
:param subgraph: a sub-graph can be given in parameter, in that case translates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End 
 
 //===========================================================================================
 
	void translate(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
%Docstring
tlp.LayoutProperty.translate(move, itNodes, itEdges)

Translates the layout of the nodes and edges provided through iterators according to a movement vector.

:param move: a movement vector
:type move: :class:`tlp.Coord`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End
	
//===========================================================================================	
	
	void scale(const tlp::Coord& v, tlp::Graph *graph=0);
%Docstring
tlp.LayoutProperty.scale(scaleFactors[, subgraph = None])

Scales the current layout according to a vector of scale factors (sx, sy, sz).

:param move: a vector of scale factors
:type move: :class:`tlp.Coord`
:param subgraph: a sub-graph can be given in parameter, in that case scales the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End 
	
//===========================================================================================	
	
	void scale(const tlp::Coord& v, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
%Docstring
tlp.LayoutProperty.scale(scaleFactors, itNodes, itEdges)

Scales the layout of the nodes and edges provided through iterators according to a vector of scale factors (sx, sy, sz).

:param move: a vector of scale factors
:type move: :class:`tlp.Coord`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End
  
 //=========================================================================================== 
  
	void rotateZ(const double& alpha, tlp::Graph *graph=0);
%Docstring
tlp.LayoutProperty.rotateZ(alpha[, subgraph = None])

Rotates the current layout around the Z-axis according to an angle in degrees.

:param alpha: an angle in degrees
:type alpha: flot
:param subgraph: a sub-graph can be given in parameter, in that case rotates the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End 

//===========================================================================================

	void rotateX(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
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
	
	void rotateY(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
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
	
	void rotateZ(const double& alpha, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
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
	
	void center(tlp::Graph *graph = 0);
%Docstring
tlp.LayoutProperty.center([subgraph = None])

Centers the layout of the attached graph, meaning translating it in order that the center of its bounding box is (0,0,0)

:param subgraph: a sub-graph can be given in parameter, in that case centers the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End
	
//===========================================================================================	
	
	void normalize(tlp::Graph *graph = 0);
%Docstring
tlp.LayoutProperty.normalize([subgraph = None])

Normalized the layout of the attached graph, meaning dividing each nodes and edges coordinate by the maximum magnitude of the whole coordinates set 

:param subgraph: a sub-graph can be given in parameter, in that case normalizes the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End
	
//===========================================================================================	
	
	void perfectAspectRatio();
%Docstring
tlp.LayoutProperty.perfectAspectRatio()

Scales the layout in order to approach an aspect ratio (width / height) of 1.0 . 
%End  
  
//=========================================================================================== 
  
	double edgeLength(const tlp::edge e) const;
%Docstring
tlp.LayoutProperty.edgeLength(edge)
	
Returns the length of an edge, the bends are taken into account.
Thus, it measures the length of a polyline. Warning this function 
only measures the length of the polyline between bends, when using
curves like bezier etc... the result will not be the length of the curve.

:param edge: an existing edge
:type edge: :class:`tlp.edge`
:rtype: float
:throws: an exception if the edge does not belong to the graph attached to the property
%End  

%MethodCode
	if (sipCpp->getGraph()->isElement(*a0)) {
		sipRes = sipCpp->edgeLength(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
	}
%End

//===========================================================================================
  
	double averageAngularResolution(const tlp::Graph *graph = 0) const;
%Docstring
tlp.LayoutProperty.averageAngularResolution([subgraph = None])

Returns the average angular resolution of the layout.
It is only defined for 2D drawing, the third coordinate is omitted

:param subgraph: a sub-graph can be given in parameter, in that case computes the average angular resolution for the layout of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float
%End
  
//===========================================================================================  
  
  	double averageAngularResolution(const tlp::node n, const tlp::Graph *graph = 0) const;
%Docstring
tlp.LayoutProperty.averageAngularResolution(node[, subgraph = None])

Returns the average angular resolution of a node.
It is only defined for 2D drawing, the third coordinate is omitted

:param node: an existing node
:type node: :class:`tlp.node`
:param subgraph: a sub-graph can be given in parameter, in that case computes the average angular resolution of a node in that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float
:throws: an exception if the node does not belong to the graph attached to the property
%End 

%MethodCode
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
%End
  
//===========================================================================================  
  
  	std::vector<double> angularResolutions(const tlp::node n, const tlp::Graph *graph = 0) const;
%Docstring
tlp.LayoutProperty.angularResolutions(node[, subgraph=None])

Returns a list of all angular resolution of a node.
It is defined for 2D drawing, the third coordinates is omitted

:param node: an existing node
:type node: :class:`tlp.node`
:param subgraph: a sub-graph can be given in parameter, in that case returns the list of angular resolution for the node in that sub-graph
:type subgraph: :class:`tlp.Graph`
:rtype: list of float
:throws: an exception if the node does not belong to the graph attached to the property
%End

%MethodCode
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
%End

//===========================================================================================

	void computeEmbedding(tlp::Graph *sg = 0);
%Docstring
tlp.LayoutProperty.computeEmbedding([subgraph=None])

Fixes embedding of the graph according to the layout,
ie. sort edges around nodes according to their neighbors/bends position in the layout.
Only works in 2D, the third coordinate is not taken into account.

:param subgraph: a sub-graph can be given in parameter, in that case fixes embedding of that sub-graph
:type subgraph: :class:`tlp.Graph`
%End  
  
 //===========================================================================================
  
  	void computeEmbedding(const tlp::node n, tlp::Graph *sg = 0);
%Docstring
tlp.LayoutProperty.computeEmbedding(node[, subgraph=None])

Fixes embedding of the node according to the layout,
ie. sort edges around the node according to their neighbors/bends position in the layout.
Only works in 2D, the third coordinate is not taken into account.

:param subgraph: a sub-graph can be given in parameter, in that case fixes embedding of the node in that sub-graph
:type subgraph: :class:`tlp.Graph`
:throws: an exception if the node does not belong to the graph attached to the property
%End

%MethodCode
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
%End

 //===========================================================================================
  
  	unsigned int crossingNumber() const;
%Docstring
tlp.LayoutProperty.crossingNumber()

Returns the number of edges crossings in the layout.
%End 	
  	