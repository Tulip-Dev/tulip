//-*-c++-*-
/*
 Author: Sophie BARDET & Alexandre BURNETT
 Email : sophie.bardet@gmail.com, aburnett397@gmail.com
 Last modification : 19/06/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_FACE_H
#define Tulip_FACE_H

#include <vector>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <cassert>

class Face : protected std::vector<edge>{

  friend class NodeFaceIterator;
  friend class EdgeFaceIterator;

public :
 
  Face();
  Face(Face*);

 ~Face();

  /** add an edge to the face, possibility to specify the predecessor */
  void addEdge(const edge e, const edge prec=edge());
  /** delete an edge of the face */
  void delEdge(const edge e);  
  /** delete a continuation of edges of the face, 
   *  first and last included
   */
  void delEdges(const edge first, const edge last);
  /** test if the edge e is included in the face */
  bool containEdge(const edge e) const;

  /** return the last element in the storage of the face */ 
  edge getLastEdge() const;
  /** return the first element in the storage of the face */
  edge getFirstEdge() const;
  
  /** return the successor of e in the storage of the face */
  edge succEdge(const edge e) const;
  /** return the predecessor of e in the storage of the face */
  edge precEdge(const edge e) const;
  /** return the number of edges in the face */
  unsigned int nbEdges()const{
    return this->size();
  }
  
  /** display the list of edges for this face F */
  friend std::ostream & operator <<(std::ostream &os,const Face& F);
};

#endif
