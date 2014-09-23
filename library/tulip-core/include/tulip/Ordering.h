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

#ifndef ORDERING_H
#define ORDERING_H
#include <tulip/Face.h>
//#include <tulip/PlanarConMap.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
//#include <vector>
//#include <map>

#include <tulip/MutableContainer.h>

#ifndef DOXYGEN_NOTFOR_USER

namespace tlp {
class PluginProgress;
class PlanarConMap;

class TLP_SCOPE Ordering {
public :

  typedef struct FaceAndPos_ {
    Face face;
    node n_first;
    node n_last;
  } FaceAndPos;

  std::vector<edge> getDummyEdges() {
    return dummy_edge;
  }

  Ordering(PlanarConMap * G, PluginProgress* pluginProgress = 0,
           int minProgress = 0, int deltaProgress = 0, int maxProgress = 0);
  ~Ordering();
//   inline void push_back(std::vector<node> nodeVector) {
  inline size_t size() {
    return _data.size();
  }
  inline std::vector<node> operator[](const unsigned int i) const {
    return _data[i];
  }
  inline std::vector<node>& operator[](const unsigned int i) {
    return _data[i];
  }

private :
  std::vector<std::vector<node> > _data;
  PlanarConMap * Gp;
  MutableContainer<int> oute;
  MutableContainer<int> outv;
  MutableContainer<bool> visitedNodes;
  MutableContainer<bool> visitedFaces;
  MutableContainer<bool> markedFaces;
  MutableContainer<int> seqP;
  MutableContainer<bool> isOuterFace;
  MutableContainer<bool> contour;
  MutableContainer<bool> is_selectable;
  MutableContainer<bool> is_selectable_visited;
  MutableContainer<bool> is_selectable_face;
  MutableContainer<bool> is_selectable_visited_face;
  MutableContainer<node> left;
  MutableContainer<node> right;
  bool existMarkedF;
  FaceAndPos minMarkedFace ;
  Face ext;
  std::vector<node> v1;
  std::vector<edge> dummy_edge;

  node getLastOfQ(Face f, node prec, node n, edge e);
  node getLastOfP(Face f, node prec, node n,edge e);
  std::vector<node> getPathFrom(std::vector<node> fn, int from);
  int infFaceSize();

  void updateOutAndVisitedFaces(Face f);
  void updateContourLeftRight(node prec, node n, edge e, node last);
  void updateNewSelectableNodes(node node_f, node no_tmp2,edge ed_tmp, node node_last,std::vector<Face> v_faces, bool one_face = false, bool was_visited = false, bool selection_face = false);
  void updateSelectableFaces(std::vector<Face> v_faces);

  int seqp(Face f);
  void minMarkedf();
  void setMinMarkedFace(Face f);

  struct augmentableAndNodes_ getAugAndNodes(Face f);
  void augment(Face f, node prec, node n, node prec_last, node last, int nbNewFace, bool pair);
  void selectAndUpdate(Face f);
  void selectAndUpdate(node n);
  bool isSelectable(node n);

  void init();
  void init_v1(std::vector<node> fn);
  void init_selectableNodes();
  void init_selectableFaces();
  void init_outv_oute();
  void init_seqP();
  void init_outerface();

};

}
#endif
#endif
///@endcond
