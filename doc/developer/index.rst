.. TulipDeveloperHandbook documentation master file, created by
   sphinx-quickstart on Mon Jul  1 10:58:47 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

######################################################
Welcome to the Tulip Developer oriented documentation!
######################################################

.. _introduction:

Tulip is a software system for the area of the Information visualization. It is becoming more and more useful for the correct analysis of existing data sets. This need results from progress in data acquisition methods, and from the huge effort made to build computer access to the human knowledge. As an example, for the human genome database, the raw data acquisition phase seems to be completed; however, to reach the ultimate goal of providing new medical treatment, it is necessary to understand these data. In such an application, the information visualization views of the data in order to explore and extend knowledge.

Here we focus on data that can be represented by a graph. In most of cases a graph structure can be extracted from existing data sets. The most well-known is the World Wide Web where links between pages can be considered as edges and pages as nodes. Another one is the human metabolism data-set where chemical reactions can be embedded in a Petri net, literature co-citations are modeled as edges between nodes of this network, and metabolic pathway are considered as clusters of the resulting graph.

Systems to visualize graphs have come to the fore during the last ten years. To our knowledge, no one provides the following capabilities simultaneously :

* Visualization and navigation in 2 or 3 dimensions

* Support of huge graphs

* Support of graph modifications

* Management of clusters

* Management of unbounded number of shared properties between graphs

* A mechanism for evaluating internal properties

* Extension and reuse without recompilation of the software

* Free of use and open source

To experiment with tools to handle graphs of the size of those induced by the human genome data set, one needs a software solution with all these capabilities. That's why we decided to build our own graph visualization software that meets these requirements. Tulip has been developed in C++, and uses two well-known libraries, OpenGL and Qt. The final program enables visualization, clustering and automatic drawing of graphs with up to 1.000.000 elements on personal computer.

This manual is an help necessary for the developers of an application using **Tulip** libraries and for the developers of the **Tulip** Team.

It explains how to compile the libraries and the software, it does a presentation of the main methods available on the libraries. Chapters relate the programming and documentation guidelines.


.. _contents:

Contents:
=========

.. toctree::
   :maxdepth: 2

   installation_unix
   installation_win
   tulip_lib
   tulip_tutorial
   graph_format
   plugin_development
   doc_guides

.. tulip_libogl.

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`


