
	void reverse();
%Docstring
tlp.BooleanProperty.reverse()

Reverses all values associated to graph elements,
i.e :const:`True` => :const:`False`, :const:`False` => :const:`True` .
%End

//===========================================================================================

	void reverseEdgeDirection();
%Docstring
tlp.BooleanProperty.reverseEdgeDirection()

Reverses all the direction of edges in the attached graph
which are set to :const:`True` in this boolean property.
%End
	
//===========================================================================================
	
	tlp::Iterator<tlp::node> *getNodesEqualTo(const bool val, tlp::Graph *g = 0);
%Docstring	
tlp.BooleanProperty.getNodesEqualTo(val[, graph = None])

Returns an iterator through all nodes belonging to the attached graph
whose associated value is equal to the one given in parameter.

:param val: a value
:type val: boolean
:param graph: a sub-graph can be given in parameter, in that case returns an iterator on nodes only belonging to that sub-graph
:type graph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorNode` 
%End   
	
//===========================================================================================	
		
	tlp::Iterator<tlp::edge> *getEdgesEqualTo(const bool val, tlp::Graph *g = 0);
%Docstring	
tlp.BooleanProperty.getEdgesEqualTo(val[, graph = None])

Returns an iterator through all edges belonging to the attached graph
whose associated value is equal to the one given in parameter.

:param val: a value
:type val: boolean
:param graph: a sub-graph can be given in parameter, in that case returns an iterator on edges only belonging to that sub-graph
:type graph: :class:`tlp.Graph`
:rtype: :class:`tlp.IteratorEdge` 
%End 	
