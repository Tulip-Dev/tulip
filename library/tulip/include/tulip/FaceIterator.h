//-*-c++-*-
/*
 Author: Romain BOURQUI
 Email : bourqui@labri.fr
 Last modification : 02/11/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef FACEITERATOR2_H
#define FACEITERATOR2_H

#include <list>
#include <tulip/PlanarConMap.h>


#ifndef DOXYGEN_NOTFOR_DEVEL
//============================================================
/// Face iterator for PlanarConMap
namespace tlp {

class FaceIterator : public Iterator<Face>{
public:
  FaceIterator(PlanarConMap* m);
  virtual ~FaceIterator(){}
  
  Face next();
  bool hasNext();

private:
  unsigned int i;
  PlanarConMap* mgraph;
}; 

//============================================================
/// Adjacente Face iterator for PlanarConMap
class FaceAdjIterator : public Iterator<Face>{
public:
  FaceAdjIterator(PlanarConMap* m, const node n);
  virtual ~FaceAdjIterator(){}
  
  Face next();
  bool hasNext();

private:
    std::vector<Face> facesAdj;
    unsigned int i;
}; 

//============================================================
/// Node of face iterator for PlanarConMap
class NodeFaceIterator : public Iterator<node>{
public :
  NodeFaceIterator(PlanarConMap *m, const Face );
  virtual ~NodeFaceIterator(){}
  
  node next();
  bool hasNext();

private :
  /** contains the set of computed nodes */
  std::vector<node> nodes;
  unsigned int i;
};


//============================================================
/// Edge of face iterator for PlanarConMap
class EdgeFaceIterator : public Iterator<edge>{
public :
  EdgeFaceIterator(PlanarConMap *m, const Face );
  virtual ~EdgeFaceIterator(){}
  
  edge next();
  bool hasNext();

private :
  /** reference on the current face */
  std::vector<edge> ve;
  unsigned int i;
};

}
#endif
#endif
