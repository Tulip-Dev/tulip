//-*-c++-*-
/** 
 * Author: Romain BOURQUI
 * Email : bourqui@labri.fr
 * Last Version : 10/01/2006
 * implementation of {Gutwenger and Mutzel} 's
 * Canonical Ordering for biconnected planar graphs.
 * ref: Extended abstract, Max-Planck-Institut für Informatik,
 * Saarbrücken, Germany, 1997 :
 * Grid Embedding for Biconnected Planar Graphs
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#ifndef Tulip_CanonicalOrdering_H
#define Tulip_CanonicalOrdering_H
#include <tulip/Face.h>
#include <tulip/SuperGraphMap.h>
#include <tulip/Node.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <tulip/MutableContainer.h>


/*<<<<<<< CanonicalOrdering.h
#ifndef DOXYGEN_NOTFOR_USER
class Ordering : public std::vector<std::vector<node> >
=======*/
class TLP_SCOPE Ordering : public std::vector<std::vector<node> >
			   /*>>>>>>> 1.2*/
{
  
 public :

  typedef struct FaceAndPos_ {
    Face face;
    node n_first;
    node n_last;
  } FaceAndPos;

  std::vector<edge> getDummyEdges(){ return dummy_edge; }
  
  Ordering(SuperGraphMap * G);
  ~Ordering();
  std::vector<std::vector<node> > computeCanonicalOrdering();

 private :
 
  SuperGraphMap::SuperGraphMap * Gp;
  MutableContainer<int>::MutableContainer oute;
  MutableContainer<int>::MutableContainer outv;
  MutableContainer<bool>::MutableContainer visitedNodes;
  MutableContainer<bool>::MutableContainer visitedFaces;
  MutableContainer<bool>::MutableContainer markedFaces;
  MutableContainer<int>::MutableContainer seqP;
  MutableContainer<bool>::MutableContainer isOuterFace;
  MutableContainer<bool>::MutableContainer contour;
  MutableContainer<bool>::MutableContainer is_selectable;
  MutableContainer<bool>::MutableContainer is_selectable_visited;
  MutableContainer<bool>::MutableContainer is_selectable_face;
  MutableContainer<bool>::MutableContainer is_selectable_visited_face;
  MutableContainer<node>::MutableContainer left;
  MutableContainer<node>::MutableContainer right;
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

#endif
#endif
