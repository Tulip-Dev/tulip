
	double  getNodeMin(tlp::Graph *sg=0);
%Docstring
tlp.DoubleProperty.getNodeMin([subgraph = None])

Returns the minimum value on the nodes for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End	
	
	
//===========================================================================================	
	
	double  getNodeMax(tlp::Graph *sg=0);
%Docstring
tlp.DoubleProperty.getNodeMax([subgraph = None])

Returns the maximum value on the nodes for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the nodes belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
		
//===========================================================================================	
	
	double  getEdgeMin(tlp::Graph *sg=0);
%Docstring
tlp.DoubleProperty.getEdgeMin([subgraph = None])

Returns the minimum value on the edges for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the minimum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	double  getEdgeMax(tlp::Graph *sg=0);
%Docstring
tlp.DoubleProperty.getEdgeMax([subgraph = None])

Returns the maximum value on the edges for that double property.

:param subgraph: a sub-graph can be given in parameter, in that case return the maximum value on the edges belonging to that sub-graph.
:type subgraph: :class:`tlp.Graph`
:rtype: float 
%End
	
//===========================================================================================	
	
	void uniformQuantification(unsigned int);
%Docstring
tlp.DoubleProperty.uniformQuantification(n)

Performs an uniform quantification on nodes and edges based on the values stored in that double property.
An uniform quantification tries to separate the nodes and edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes and edges will be erased and replaced by the id of the class they belong to.
%End	
	
//===========================================================================================	
	
	void nodesUniformQuantification(unsigned int);
%Docstring
tlp.DoubleProperty.nodesUniformQuantification(n)

Performs an uniform quantification on nodes based on the values stored in that double property.
An uniform quantification tries to separate the nodes in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on nodes will be erased and replaced by the id of the class they belong to.
%End
	
//===========================================================================================	
	
	void edgesUniformQuantification(unsigned int);
%Docstring
tlp.DoubleProperty.edgesUniformQuantification(n)

Performs an uniform quantification on edges based on the values stored in that double property.
An uniform quantification tries to separate the edges in n classes of equivalent size according to theirs values in increasing order.

:param n: the number of desired classes
:type n: integer

.. warning:: All previous values on edges will be erased and replaced by the id of the class they belong to.
%End

