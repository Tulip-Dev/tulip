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
///@cond DOXYGEN_HIDDEN


#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef Tulip_PlanarConMap_H
#define Tulip_PlanarConMap_H
#include <vector>
#include <tulip/tuliphash.h>
#include <tulip/Face.h>
#include <tulip/GraphDecorator.h>

namespace tlp {

struct Face;
class IdManager;

/**
 * \brief interface for a topological graph
 */
/**
 * The class PlanarConMap is an interface for map in the Tulip Library. This only
 * considers connected planar map, moreover the graph must be simple.
 * After, its initialization, if modifications, such as additions or deletions of
 * edges or/and nodes, are made on the supergraph, the map will not be
 * valid any more. In this case, one can calls update() function to update the map
 * but it completely compute the map.
 */


class TLP_SCOPE PlanarConMap : public GraphDecorator {

  /* for test classes */
  friend class FaceIteratorTest;
  friend class PlanarConMapTest;


  friend class FaceIterator;
  friend class FaceAdjIterator;
  friend class NodeFaceIterator;
  friend class EdgeFaceIterator;

  friend TLP_SCOPE PlanarConMap* computePlanarConMap(Graph* graph);

protected:
  /** Constructor
   * Warning, the graph must be planar, connected and simple.
   */
  PlanarConMap(Graph* s);


public:

  //Destructor.
  virtual ~PlanarConMap();

  /**
   *  Remove all nodes, edges, faces and subgraphs of the map
   */
  void clear();

  /** Update the map : this recompute completely the map.
   * To do when an operation on one of the super-graphs of the map has been done.
   */
  void update();

  //==============================================================================
  // Modification of the graph structure
  //==============================================================================
  /**
   * Add and return an edge between two node u and v in the map. This edge is added in
   * face f, and e1 and e2 will be predecessor of respectively v and w in the
   * cycles around v and w. The new face is put into new_face.
   * This edge is also added in all the super-graph of the map to maintain
   * the sub-graph relation between graphs.
   * Warning, the edge must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   * Warning : One can't add an existing edge to the root graph
   */
  edge addEdgeMap(const node v, const node w, Face f, const edge e1, const edge e2, Face new_face= Face());

  /** Split the face by adding an edge between the two nodes and return the
   * new face. Possibility to specify which will be the new face, by giving a
   * node that will be contained into the new face.
   * Warning, the edge must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   */
  Face splitFace(Face, const node, const node, node = node());

  /** Split the face by adding the edge and return the new face.
   * Warning, the edge must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   */
  Face splitFace(Face, const edge);

  /** Merge two faces into one and put the new computed face into f.
   * Warning,  the edge must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   */
  void mergeFaces(Face f, Face g);


  //================================================================================
  //Iterators on the graph structure.
  //================================================================================

  ///Return an iterator on the faces.
  Iterator<Face>* getFaces();
  ///Return an iterator on the adjacent faces of a node.
  Iterator<Face>* getFacesAdj(const node);
  ///Return an iterator on the nodes of a face.
  Iterator<node>* getFaceNodes(const Face);
  ///Return an iterator on the edges of a face.
  Iterator<edge>* getFaceEdges(const Face);

  //================================================================================
  // Graph, nodes and edges information about the graph stucture
  //================================================================================
  ///Return the edge which is the succcessor of an edge in the cycle of a node.
  edge succCycleEdge(const edge, const node) const;
  ///Return the edge which is the predecessor of an edge in the cycle of a node.
  edge predCycleEdge(const edge, const node) const;
  ///Return the node which is the succcessor of a node in the cycle of a node.
  node succCycleNode(const node, const node) const;
  ///Return the node which is the predecessor of a node in the cycle of a node.
  node predCycleNode(const node, const node) const;

  ///Return the number of faces.
  unsigned int nbFaces();
  ///Return the number of nodes contained into a face.
  unsigned int nbFacesNodes(const Face);
  ///Return the number of edges contained into a face.
  unsigned int nbFacesEdges(const Face);

  ///Return true if the face contains the node.
  bool containNode(const Face, const node) ;
  ///Return true if the face contains the edge.
  bool containEdge(const Face, const edge);
  /** Returns the face containing the two nodes in this order
   * and the edge between this two nodes.
   * Warning, the edge must exists in the map.
   */
  Face getFaceContaining(const node, const node);
  /// Return a face containing the two nodes if it exists and Face() otherwise
  Face sameFace(const node, const node);


private:

  /** Compute faces and initialize all variables.
   */
  void computeFaces();
  /**
   * Delete the edge in the map. The new face can be put into a specified face,
   * otherwise, one of the two adjacent faces will be updated.
   * Warning, the edge must not be an isthm of the map, otherwize the map will be deconnected
   * and so won't be valid any more.
   */
  void delEdgeMap(edge, Face = Face());

  typedef TLP_HASH_MAP<Face , std::vector<edge> > faceMap;
  typedef faceMap::value_type faceMapEntry;
  typedef TLP_HASH_MAP<edge, std::vector<Face> > edgeMap;
  typedef edgeMap::value_type edgeMapEntry;
  typedef TLP_HASH_MAP<node, std::vector<Face> > nodeMap;
  typedef nodeMap::value_type nodeMapEntry;

  /** storage of faces */
  faceMap facesEdges;
  edgeMap edgesFaces;
  nodeMap nodesFaces;
  mutable std::vector<Face > faces;

  IdManager *faceId;

};

// Compute a PlanarConMap from a graph.
// return a NULL value if the graph is not connected
TLP_SCOPE PlanarConMap* computePlanarConMap(Graph* graph);

}

///Print the map (only faces, nodes and edges) in ostream, in the tulip format
TLP_SCOPE std::ostream& operator<< (std::ostream &, tlp::PlanarConMap *);

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
