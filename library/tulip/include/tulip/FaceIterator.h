//-*-c++-*-
/*
 Author: Sophie BARDET  Alexandre BURNETT
 Email : sophie.bardet@gmail.com, aburnett397@gmail.com
 Last modification : 19/06/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef FACEITERATOR_TLP
#define FACEITERATOR_TLP

#include <list>
#include <tulip/SuperGraphMap.h>

#ifndef DOXYGEN_NOTFOR_DEVEL
//============================================================
/// Face iterator for SuperGraphMap
class FaceIterator : public Iterator<Face>{
public:
  FaceIterator(SuperGraphMap* m);
  virtual ~FaceIterator(){}
  
  /** return the next element */
  Face next();
  /** test if there's a next element */
  bool hasNext();

private:
  unsigned int i;
  SuperGraphMap* mgraph;
}; 

//============================================================
/// Adjacente Face iterator for SuperGraphMap
class FaceAdjIterator : public Iterator<Face>{
public:
  /** compute the adjacent faces of the node n */
  FaceAdjIterator(SuperGraphMap* m, const node n);
  virtual ~FaceAdjIterator(){}
  
  /** return the next element */
  Face next();
  /** test if there's a next element */
  bool hasNext();

private:
    std::vector<Face> facesAdj;
    unsigned int i;
}; 

//============================================================
/// Node of face iterator for SuperGraphMap
class NodeFaceIterator : public Iterator<node>{
public :
  /** compute the node of the face */
  NodeFaceIterator(SuperGraphMap *m, const Face );
  virtual ~NodeFaceIterator(){}
  
  /** return the next element */
  node next();
  /**  test if there's a next element */
  bool hasNext();

private :
  /** contains the set of computed nodes */
  std::vector<node> nodes;
  unsigned int i;
};


//============================================================
/// Edge of face iterator for SuperGraphMap
class EdgeFaceIterator : public Iterator<edge>{
public :
  EdgeFaceIterator(SuperGraphMap *m, const Face );
  virtual ~EdgeFaceIterator(){}
  
  /** return the next element */
  edge next();
  /**  test if there's a next element */
  bool hasNext();

private :
  /** reference on the current face */
  std::vector<edge> ve;
  unsigned int i;
};
#endif
#endif
