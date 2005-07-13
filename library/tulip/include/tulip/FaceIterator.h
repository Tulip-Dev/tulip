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

#include <tulip/SuperGraphMap.h>

//============================================================
class FaceIterator : public Iterator<Face*>{
public:
  FaceIterator(SuperGraphMap*);
  virtual ~FaceIterator(){}
  
  /** return the next element */
  Face* next();
  /** test if there's a next element */
  bool hasNext();

private:
  unsigned int i;
  SuperGraphMap* mgraph;
}; 


//============================================================
class FaceAdjIterator : public Iterator<Face*>{
public:
  /** compute the adjacent faces of the node n */
  FaceAdjIterator(SuperGraphMap* m, const node n);
  virtual ~FaceAdjIterator(){}
  
  /** return the next element */
  Face* next();
  /** test if there's a next element */
  bool hasNext();

private:
  std::list<Face*>::iterator it;
  /** contain a reference on the adjacent faces */
  std::list<Face*> lfaces;
}; 

//============================================================
class NodeFaceIterator : public Iterator<node>{
public :
  /** compute the node of the face */
  NodeFaceIterator(const SuperGraphMap*, const Face*);
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
class EdgeFaceIterator : public Iterator<edge>{
public :
  EdgeFaceIterator(const Face*);
  virtual ~EdgeFaceIterator(){}
  
  /** return the next element */
  edge next();
  /**  test if there's a next element */
  bool hasNext();

private :
  /** reference on the current face */
  const Face* f;
  unsigned int i;
};
