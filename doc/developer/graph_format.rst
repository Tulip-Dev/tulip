.. _graph-format:

******************
Tulip graph format
******************


.. _nodes-def:

Nodes
=====

The nodes are stored with a list of indices. The indices must be non negative integers.
Syntax::

  (nodes id_node1 id_node2 ...)

Sample::

  (nodes 0 1 2 3 4 5 )
        
    
.. _edges-def:

Edges
=====

An edge is defined by providing three non negative integers. The first is the id of the edge, the second is the id of source node of the edge and the third is the id of target node of the edge.

Syntax::

  (edge id id_source id_target)

Sample::

  (edge 2 2 1)

It defines one edge with the node that has the id 2 as source and the node that has the id 1 as target.
        
    
.. _clusters-def:

Clusters
========

A cluster is defined by an integer which represent the cluster id, one string which is the name of the cluster(Two clusters can have the same name). Then it is define with a list of nodes and a list of edges. To define a subcluster we use the same method. One important point is that the id zero is reserved for the root graph (thus it cannot be used).
        
Syntax::

  (cluster id name
    (nodes id_node1 id_node2 ...)
    (edges id_edge1 id_edge2 ...)
    (cluster id name
      (nodes id_node1 id_node2 ...)
      (edges id_edge1 id_edge2 ...)
    )
  )

Sample::

  (cluster 3 "cluster"
    (nodes 1 2 3 )
    (edges 2 8 )
    (cluster 4 "Sub Cluster"
      (nodes 1 2 )
      (edges 2 )
    )
  )
        
    
.. _pro-def:

Definitions of properties
=========================

The definition of properties is the following:

Syntax::

  (property cluster_id property_type "property_name"
    (default "default_node_value" "default_edge_value" )
    (node id value)
    ...
    (edge id value)
    ...
  )        

Sample::

  (property  0 bool "viewSelection"
    (default "false" "false" )
    (node 1 "true")
    (node 2 "true")
    (node 3 "true")
    (edge 2 "true")
    (edge 8 "true")
  )

        
.. _type-pro:

Property Type
-------------
          
* layout : This type enables to store nodes position in 3D. The position of nodes is defined by a set of 3 doubles *(x_coord,y_coord,z_coord)*. The position of edges is a list of 3D points. These points are the bends of edges. *((x_coord1,y_coord1,z_coord1)(x_coord2,y_coord2,z_coord2))*
* size : This type enables to store the size of elements. The size is defined with a sequence of three double. *(width,heigth,depth)*
* color : This type enables to store the color of elements. The color is defined with a sequence of four integer from 0 to 255. *(red,green,blue,alpha)*
* string : This enables to store text on elements.
* metric : This enables to store real on elements.
* bool : This type enables to store boolean on elements.
* int : This type enables to store integers on elements.  

        
        
    
.. _pro-tlp:

Properties of Tulip
===================

*viewSelection*

  type : bool, this property is the one used for selected elements in Tulip. ::

    (property  0 bool "viewSelection"
      (default "false" "false")
      (node 1 "true")
      (node 2 "true")
      (node 3 "true")
      (edge 2 "true")
      (edge 8 "true")
    )

*viewLayout*

  type : layout, this property is the one used for displaying graph in Tulip. ::

    (property  0 layout "viewLayout"
      (default "(0,0,0)" "()" )
      (node 1 "(10,10,10)")
      (node 2 "(20,20,20)")
      (edge 1 "(15,15,15)(25,25,25)")
    )

*viewColor*

  type : color, this property is the one used for coloring graphs in Tulip. ::

    (property  0 color "viewColor"
      (default "(235,0,23,255)" "(0,0,0,0)" )
      (node 1 "(200,0,200,255)")
      (node 2 "(100,100,0,255)")
      (node 3 "(100,100,0,255)")
      (edge 2 "(200,100,100)")
    )

*viewLabel*

  type : string, this property is the one used for labeling the graphs in Tulip(in label mode). ::

    (property  0 string "viewLabel"
      (default "" "" )
      (node 1 "Hello")
      (node 2 "Bonjour")
      (node 3 "Bye")
      (edge 2 "Aurevoir")
    )

*viewSize*

  type : size, this property is the one used for the size of elements displayed. ::

    (property  0 size "viewSize"
      (default "(0,0,0)" "(1,1,1)" )
      (node 1 "(10,10,10)")
      (node 2 "(20,20,20)")
    )

*viewShape*

  type : int, this property is used for defining the shape of elements. ::

    (property  0 int "viewShape"
      (default "0" "0" )
      (node 1 "1")
      (node 2 "2")
    )

*viewTexture*

*viewRotation*
        
    
