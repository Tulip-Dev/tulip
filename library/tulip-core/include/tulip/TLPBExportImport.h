/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#ifndef TLPBIMPORTEXPORT_H
#define TLPBIMPORTEXPORT_H

#include <iostream>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/ExportModule.h>
#include <tulip/ImportModule.h>

// some compilation environments define major and minor as preprocessor macros
// undefine them in that case
#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

/**
 * The plugins below export/import a Tulip graph
 * using the following binary format:
 * format header = <magic_number, major, minor> (uint16 + uint8 +uint8)
 * nb_nodes = uint32
 * nb_edges = uint32
 * edges = nb_edges * <source, target> (uint32+uint32)
 * nb_subgraphs = uint32
 * subgraphs = nb_subgraphs * <subgraph_id, parent_graph_id, nodes_desc, edges_desc>
 * nodes_desc = nb_nodes_intervals * <first_node, last_node>
 * edges_desc = nb_edges_intervals *<first_edge, last_edge>
 * nb_properties = uint32
 * properties = <prop_name, graph_id, type, default_node_val, default_edge_val, nodes_val, edges_val>
 * prop_name = length + utf8 text
 * graph_id = uint32
 * type = length + utf8 text
 * default_node_val = type dependant (method readb)
 * default_edge_val = type dependant (method readb)
 * nb_nodes_val = uint32
 * nodes_val = nb_nodes_val * <node, node_val> (uint32 + type dependant)
 * nb_edges_val = uint32
 * edges_val = nb_edges_val * <edge, edge_val> (uint32 + type dependant)
 * graph_attributes = (nb_subgraphs + 1) * <graph_id, graph_attributes_list>*
 */

/*@{*/
/// Export plugin for TLPB format
/**
 *
 * \brief This plugin saves a Tulip graph using a binary format
 *
 */
class TLPBExport: public tlp::ExportModule {
public:

  PLUGININFORMATION("TLPB Export", "David Auber, Patrick Mary","13/07/2012","Saves a graph in Tulip binary format","1.0","File")

  std::string fileExtension() const {
    return "tlpb";
  }

  TLPBExport(const tlp::PluginContext *context) :ExportModule(context) {}
  ~TLPBExport() {}

  bool exportGraph(std::ostream &);

  std::string icon() const {
    return ":/tulip/gui/icons/tlpb32x32.png";
  }

  tlp::MutableContainer<tlp::node> nodeIndex;
  tlp::MutableContainer<tlp::edge> edgeIndex;

  tlp::node getNode(tlp::node n) {
    return nodeIndex.get(n.id);
  }

  tlp::edge getEdge(tlp::edge e) {
    return edgeIndex.get(e.id);
  }

  void getSubGraphs(tlp::Graph*, std::vector<tlp::Graph*>&);

  void writeAttributes(std::ostream&, tlp::Graph*);
};

/// Import plugin for TLPB format
/**
 *
 * \brief This plugin reads a Tulip graph using a binary format
 *
 */
class TLPBImport:public tlp::ImportModule {
public:
  PLUGININFORMATION("TLPB Import", "David Auber, Patrick Mary", "13/07/2012",
                    "Reads a graph in Tulip binary format", "1.0", "File")

  TLPBImport(tlp::PluginContext* context);
  ~TLPBImport() {}

  std::string icon() const {
    return ":/tulip/gui/icons/tlpb32x32.png";
  }

  std::list<std::string> fileExtensions() const {
    return std::list<std::string>(1, "tlpb");
  }

  bool importGraph();
};

/*@}*/

//Don't ask why it is David favorite 9 digit number.
#define TLPB_MAGIC_NUMBER 578374683
#define TLPB_MAJOR 1
#define TLPB_MINOR 0

// structures used in both tlpb import/export plugins
struct TLPBHeader {
  unsigned int magicNumber;
  unsigned char major;
  unsigned char minor;
  unsigned int numNodes;
  unsigned int numEdges;

  TLPBHeader(unsigned int nbN = 0, unsigned int nbE = 0)
    : magicNumber(TLPB_MAGIC_NUMBER), major(TLPB_MAJOR), minor(TLPB_MINOR), numNodes(nbN), numEdges(nbE) {}

  bool checkCompatibility() {
    return ((magicNumber == TLPB_MAGIC_NUMBER) &&
            (major == TLPB_MAJOR) &&
            (minor <= TLPB_MINOR));
  }
};

#define MAX_EDGES_TO_WRITE 64000
#define MAX_EDGES_TO_READ MAX_EDGES_TO_WRITE
#define MAX_RANGES_TO_WRITE MAX_EDGES_TO_WRITE
#define MAX_RANGES_TO_READ MAX_RANGES_TO_WRITE
#define MAX_VALUES_TO_WRITE MAX_EDGES_TO_WRITE
#define MAX_VALUES_TO_READ MAX_VALUES_TO_WRITE

#endif // TLPBIMPORTEXPORT_H
