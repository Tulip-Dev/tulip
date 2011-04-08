	
	int  getNodeMin(tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getNodeMin([subgraph = None])

Returns the minimum value on the nodes for that integer property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

	int  getNodeMax(tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getNodeMax([subgraph = None])

Returns the maximum value on the nodes for that integer property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

	int  getEdgeMin(tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getEdgeMin([subgraph = None])

Returns the minimum value on the edges for that integer property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End

//===========================================================================================

	int  getEdgeMax(tlp::Graph *sg=0);
%Docstring
tlp.IntegerProperty.getEdgeMax([subgraph = None])

Returns the maximum value on the edges for that integer property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: integer 
%End
