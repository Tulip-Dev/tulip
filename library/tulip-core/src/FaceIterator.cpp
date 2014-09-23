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
#include <tulip/FaceIterator.h>
#include <tulip/PlanarConMap.h>

#include <cassert>

using namespace std;
using namespace tlp;


//============================================================
//Iterator for Face : FaceIterator
//============================================================
FaceIterator::FaceIterator(PlanarConMap* m) {
  assert(m);
  mgraph = m;
  i = 0;
}

//============================================================
/**
 * return the next element
 */
Face FaceIterator::next() {
  Face tmp = mgraph->faces[i++];
  return tmp;
}

//============================================================
/**
 * test if there's a next element
 */
bool FaceIterator::hasNext() {
  if(i == mgraph->faces.size())
    return false;
  else return true;
}


//============================================================
//Iterator for Face : FaceAdjIterator
//============================================================
FaceAdjIterator::FaceAdjIterator(PlanarConMap* m, const node n) {
  assert(m->isElement(n));
  i = 0;
  facesAdj.erase(facesAdj.begin(),facesAdj.end());
  edge e;
  Face f_tmp, f_tmp2;
  Iterator<edge> * ite = m->getInOutEdges(n);

  if(ite->hasNext()) {
    e = ite->next();
    f_tmp = (m->edgesFaces[e])[0];
    f_tmp2 = (m->edgesFaces[e])[1];
  }

  if(ite->hasNext()) {
    e = ite->next();

    if( f_tmp == (m->edgesFaces[e])[0]) {
      facesAdj.push_back(f_tmp);
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back(f_tmp);
    }
    else if(f_tmp == (m->edgesFaces[e])[1]) {
      facesAdj.push_back(f_tmp);
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back(f_tmp);
    }
    else if(f_tmp2 == (m->edgesFaces[e])[0]) {
      facesAdj.push_back(f_tmp2);
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back(f_tmp);
    }
    else  if(f_tmp2 == (m->edgesFaces[e])[1]) {
      facesAdj.push_back(f_tmp2);
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back(f_tmp);
    }

  }
  else {
    facesAdj.push_back(f_tmp);
    //  facesAdj.push_back(f_tmp2);
  }

  while(ite->hasNext()) {
    e = ite->next();

    if(f_tmp == (m->edgesFaces[e])[0]) {
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back((m->edgesFaces[e])[1]);
    }
    else {
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back((m->edgesFaces[e])[0]);
    }
  }

  delete ite;
}

//============================================================
/**
 * return the next element
 */
Face FaceAdjIterator::next() {
  Face tmp = facesAdj[i];
  ++i;
  return tmp;
}

//============================================================
/**
 * test if there's a next element
 */
bool FaceAdjIterator::hasNext() {
  return (i != facesAdj.size());
}


//============================================================
// NodeFaceIterator
//============================================================
NodeFaceIterator::NodeFaceIterator(PlanarConMap* m, const Face face) {
  i=0;
  vector<edge> e = m->facesEdges[face];
  edge e1 = e[0];
  edge e2 = e[1];
  node prev;
  pair<node, node> e1Ends = m->ends(e1);
  pair<node, node> e2Ends = m->ends(e2);

  if( e1Ends.first == e2Ends.first || e1Ends.first == e2Ends.second)
    prev = e1Ends.first;
  else
    prev = e1Ends.second;

  nodes.push_back(prev);

  for(unsigned int j = 1; j < m->facesEdges[face].size(); ++j) {
    e1 = m->facesEdges[face][j];
    e1Ends = m->ends(e1);

    if (e1Ends.first == prev ) {
      prev = e1Ends.second;
      nodes.push_back(prev);
    }
    else {
      prev = e1Ends.first;
      nodes.push_back(prev);
    }
  }

}

//============================================================
/**
 * return the next element
 */
node NodeFaceIterator::next() {
  node n = nodes[i];
  i++;
  return n;
}

//============================================================
/**
 * test if there's a next element
 */
bool NodeFaceIterator::hasNext() {
  return (i != nodes.size());
}


//============================================================
// EdgeFaceIterator
//============================================================
EdgeFaceIterator::EdgeFaceIterator(PlanarConMap * m,const Face f) {
  i = 0;
  ve = m->facesEdges[f];
}

//============================================================
/**
 * return the next element
 */
edge EdgeFaceIterator::next() {
  edge tmp = ve[i];
  i++;
  return tmp;
}

//============================================================
/**
 * test if there's a next element
 */
bool EdgeFaceIterator::hasNext() {
  return (i != ve.size());
}
