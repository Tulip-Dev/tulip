
	tlp::Size getMax(tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.getMax([subgraph = None])

Returns the maximum value on the nodes for that size property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Size`
%End

//===========================================================================================
	
	tlp::Size getMin(tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.getMin([subgraph = None])

Returns the minimum value on the nodes for that size property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Size`
%End
	
//===========================================================================================	
	
	void scale(const tlp::Size&, tlp::Graph *sg=0);
%Docstring
tlp.SizeProperty.scale(scaleFactors, [subgraph = None])

Scales the sizes of the nodes and edges according to a scale factors vector (sx, sy, sz).

:param scaleFactors: a vector of scales factor
:type scaleFactors: :class:`tlp.Size`
:param subgraph: a sub-graph can be given in parameter, in that case scales the size of the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
%End	
	
	
//===========================================================================================	
	
	void scale(const tlp::Size&, tlp::Iterator<tlp::node> *, tlp::Iterator<tlp::edge> *);
%Docstring
tlp.SizeProperty.scale(scaleFactors, itNodes, itEdges)

Scales the sizes of the nodes and edges provided through iterators according to a scale factors vector (sx, sy, sz).

:param scaleFactors: a vector of scale factors
:type scaleFactors: :class:`tlp.Size`
:param itNodes: an iterator on graph nodes
:type itNodes: :class:`tlp.IteratorNode`
:param itEdges: an iterator on graph edges
:type itEdges: :class:`tlp.IteratorEdge`
%End

