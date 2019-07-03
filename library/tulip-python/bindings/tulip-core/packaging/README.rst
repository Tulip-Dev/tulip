Module description
==================

Graphs play an important role in many research areas, such as biology, microelectronics, social
sciences, data mining, and computer science. Tulip (http://tulip.labri.fr) [1]_ [2]_ is an
Information Visualization framework dedicated to the analysis and visualization of such relational
data. Written in C++ the framework enables the development of algorithms, visual encodings,
interaction techniques, data models, and domain-specific visualizations.

The Tulip core library is available to the Python community through the Tulip-Python
bindings [3]_. The bindings have been developed using the `SIP <https://pypi.python.org/pypi/SIP>`_
tool [4]_ from Riverbank Computed Limited, allowing to easily create quality Python bindings for any C/C++ library.
The main features provided by the bindings are the following ones:

  - **Creation and manipulation of graphs** :
    Tulip provides an efficient graph data structure for storing large and complex networks.
    It is also one of the few that offer the possibility to
    efficiently define and navigate graph hierarchies or cluster trees (nested subgraphs).

  - **Storage of data on graph elements** :
    Tulip allows to associate different kind of serializable data (boolean, integer, float, string, ...)
    and visual attributes (layout, color, size, ...) to graph elements. All these data can be easily accessed
    from the Tulip graph data structure facilitating the development of algorithms.

  - **Application of algorithms of different types on graph** :
    Tulip has been designed to be easily extensible and provides a variety of graph algorithms (layout, metric, clustering, ...)
    implemented as C++ plugins. All these algorithms can be called from Python.
    As Tulip is dedicated to graph visualization, it is provided with numerous state of the art
    graph layout algorithms but also a bridge to the Open Graph Drawing Framework (http://www.ogdf.net) [5]_


References
==========

.. [1] David Auber, Romain Bourqui, Maylis Delest, Antoine Lambert,
       Patrick Mary, Guy Mélançon, Bruno Pinaud, Benjamin Renoust and Jason Vallet.
       TULIP 4. Research report. LaBRI - Laboratoire Bordelais de Recherche en Informatique. 2016.
       https://hal.archives-ouvertes.fr/hal-01359308

.. [2] David Auber, Daniel Archambault, Romain Bourqui, Antoine Lambert, Morgan Mathiaut,
       Patrick Mary, Maylis Delest, Jonathan Dubois, and Guy Mélançon. The Tulip 3 Framework:
       A Scalable Software Library for Information Visualization Applications Based on Relational
       Data. Technical report RR-7860, INRIA, January 2012.
       https://hal.archives-ouvertes.fr/hal-00659880

.. [3] Antoine Lambert and David Auber. Graph analysis and visualization with Tulip-Python.
       EuroSciPy 2012 - 5th European meeting on Python in Science, Bruxelles.
       https://hal.archives-ouvertes.fr/hal-00744969

.. [4] Riverbank Computing Limited. SIP - a tool for automatically generating Python bindings for
       C and C++ libraries. http://www.riverbankcomputing.co.uk/software/sip

.. [5] M. Chimani, C. Gutwenger, M. Jünger, G. W. Klau, K. Klein, P. Mutzel.
       The Open Graph Drawing Framework (OGDF).
       Chapter 17 in: R. Tamassia (ed.), Handbook of Graph Drawing and Visualization, CRC Press, 2014.
       https://cs.brown.edu/~rt/gdhandbook/chapters/ogdf.pdf
