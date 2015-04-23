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

#include <algorithm>

#include <tulip/ConnectedTest.h>
#include <tulip/PlanarityTest.h>
#include <tulip/SimpleTest.h>
#include <tulip/MutableContainer.h>
#include <tulip/MapIterator.h>
#include <tulip/FaceIterator.h>
#include <tulip/Face.h>
#include <tulip/PlanarConMap.h>
#include <tulip/ForEach.h>
#include <tulip/IdManager.h>
#include <tulip/TreeTest.h>

using namespace std;
using namespace tlp;

//============================================================
// PlanarConMap
//============================================================
PlanarConMap::PlanarConMap(Graph* s): GraphDecorator(s), facesEdges(), edgesFaces() {
  assert(SimpleTest::isSimple(s));
  assert(ConnectedTest::isConnected(s));
  assert(PlanarityTest::isPlanar(s) || s->numberOfNodes()==0);

  faceId = new IdManager();

  if (!TreeTest::isFreeTree(s)) {//all map of trees are valid (do not change the existing order)
    if (!PlanarityTest::isPlanarEmbedding(s)) {
      PlanarityTest::planarEmbedding(s);
    }
  }

  computeFaces();
}

//============================================================
PlanarConMap::~PlanarConMap() {
  delete faceId;
}

//============================================================
void PlanarConMap::clear() {
  graph_component->clear();
  facesEdges.clear();
  edgesFaces.clear();
  nodesFaces.clear();
  faces.clear();
}

//============================================================
edge PlanarConMap::addEdgeMap(const node v, const node w, Face f, const edge e1, const edge e2, Face new_face) {
  assert(e1 != e2);
  assert(isElement(v) && isElement(w));
  assert(isElement(e1) && isElement(e2));
  assert(containNode(f,v) && containNode(f,w));
  assert(containEdge(f,e1) && containEdge(f,e2));
  assert(source(e1)==v || target(e1)==v);
  assert(source(e2)==w || target(e2)==w);
  assert(containEdge(f,e1) && containEdge(f,e2));
  assert(containEdge(f,succCycleEdge(e1,v)) && containEdge(f,succCycleEdge(e2,w)));

  if(new_face == Face())
    new_face = Face(faceId->get());

  edge e_tmp;
  vector<edge> v_edges1, v_edges2;
  edge succ1, succ2;
  unsigned int nb_edges = facesEdges[f].size();
  unsigned int nb_added = 0;
  MutableContainer<bool> isInVe2;
  MutableContainer<bool> isInNewFace;
  MutableContainer<unsigned int> nbAdjFace;
  isInVe2.setAll(false);
  isInNewFace.setAll(false);
  nbAdjFace.setAll(0);
  succ1 = succCycleEdge(e1,v);
  succ2 = succCycleEdge(e2,w);


  Graph * supergraph = getSuperGraph();
  edge e = supergraph->existEdge(v,w, false);

  if(!e.isValid()) {
    e = supergraph->addEdge(v,w);
    graph_component->addEdge(e);
  }
  else
    graph_component->addEdge(e);


  // Compute the cycle around v
  vector<edge> v_order(deg(v));
  unsigned int cpt = 0;
  edge e_order;
  forEach(e_order,getInOutEdges(v)) {
    if(e_order == e)
      continue;

    if(e_order != succ1)
      v_order[cpt] = e_order;
    else {
      v_order[cpt] = e;
      v_order[++cpt] = succ1;
    }

    ++cpt;
  }
  setEdgeOrder(v,v_order);

  // Compute the cycle around w
  vector<edge> v_order2(deg(w));
  cpt = 0;
  forEach(e_order,getInOutEdges(w)) {
    if(e_order == e)
      continue;

    if(e_order != succ2)
      v_order2[cpt] = e_order;
    else {
      v_order2[cpt] = e;
      v_order2[++cpt] = succ2;
    }

    ++cpt;
  }
  setEdgeOrder(w,v_order2);


  // Search for adjacent nodes and edges to the two faces
  e_tmp = facesEdges[f][0];
  unsigned int i = 0;

  while(e_tmp != succ1) {
    i = (i+1)%nb_edges;
    e_tmp = facesEdges[f][i];
  }

  if(e1==succ1 && e_tmp==facesEdges[f][(i+1)%nb_edges])
    i = (i+1)%nb_edges;

  bool e2_found = false;

  while(nb_added != nb_edges && !(e_tmp==succ2 && e2_found)) {
    v_edges1.push_back(e_tmp);
    nbAdjFace.set(e_tmp.id, nbAdjFace.get(e_tmp.id)+1);
    pair<node, node> eEnds = ends(e_tmp);
    isInNewFace.set(eEnds.first.id, true);
    isInNewFace.set(eEnds.second.id, true);
    nb_added++;

    if(e_tmp == e2)
      e2_found = true;

    i = (i+1)%nb_edges;
    e_tmp = facesEdges[f][i];
  }

  if(e2==succ2 && e_tmp==facesEdges[f][(i+1)%nb_edges]) {
    v_edges1.push_back(e_tmp);
    nbAdjFace.set(e_tmp.id, nbAdjFace.get(e_tmp.id)+1);
    pair<node, node> eEnds = ends(e_tmp);
    isInNewFace.set(eEnds.first.id, true);
    isInNewFace.set(eEnds.second.id, true);
    nb_added++;
    i = (i+1)%nb_edges;
  }

  while(nb_added != nb_edges  && !(e_tmp==succ1 && nbAdjFace.get(e_tmp.id)==2)) {
    v_edges2.push_back(e_tmp);
    nbAdjFace.set(e_tmp.id, nbAdjFace.get(e_tmp.id)+1);
    isInVe2.set(e_tmp.id,true);
    nb_added++;
    i = (i+1)%nb_edges;
    e_tmp = facesEdges[f][i];
  }

  if(e1==succ1 && e_tmp==facesEdges[f][(i+1)%nb_edges]) {
    v_edges2.push_back(e_tmp);
    isInVe2.set(e_tmp.id,true);
    nb_added++;
  }

  v_edges1.push_back(e);
  v_edges2.push_back(e);
  isInVe2.set(e.id,true);

  // initialize and update the list of faces and the two new faces adajcent edges
  facesEdges.insert(faceMapEntry(new_face,v_edges1));
  facesEdges[f] = v_edges2;
  faces.push_back(new_face);
  vector<Face> v_faces;
  v_faces.push_back(new_face);
  v_faces.push_back(f);

  // initialize the list of faces adjacent to all edges of the new face
  edgesFaces.insert(edgeMapEntry(e,v_faces));

  for(i = 0; i < v_edges1.size() - 1; ++i) {
    e_tmp = v_edges1[i];

    if(isInVe2.get(e_tmp.id))
      edgesFaces[e_tmp][0] = new_face;
    else if(edgesFaces[e_tmp][0] == f)
      edgesFaces[e_tmp][0] = new_face;
    else
      edgesFaces[e_tmp][1] = new_face;
  }

  // initialize the list of faces adjacent to all nodes of the new face
  Iterator<unsigned int> * itn = isInNewFace.findAll(true);

  while(itn->hasNext()) {
    node n_tmp = node(itn->next());
    vector<Face> v_faces;
    Iterator<Face> * itf = getFacesAdj(n_tmp);

    while(itf->hasNext())
      v_faces.push_back(itf->next());

    delete itf;
    nodesFaces[n_tmp] = v_faces;
  }

  delete itn;

  return e;
}

//============================================================
void PlanarConMap::delEdgeMap(edge e, Face f) {
  assert((edgesFaces[e][0] != edgesFaces[e][1]) || deg(source(e))==1 || deg(target(e))==1);

  if(f==Face())
    f = edgesFaces[e][0];

  Face f1, f2;
  node n1, n2;
  vector<edge> v_edges;
  MutableContainer<bool> isInF2;
  isInF2.setAll(false);
  pair<node, node> eEnds = ends(e);
  n1 = eEnds.first;
  n2 = eEnds.second;
  f1 = f;
  f2 = edgesFaces[e][1]== f ? edgesFaces[e][0] : edgesFaces[e][1];

  // if there is only one face adjacent to e.
  if(f1 == f2) {
    // clear the map if it will contain any node.
    if(numberOfNodes()==2)
      clear();
    else {
      node n_tmp = (deg(n1) != 1 ? n1 : n2);
      unsigned int nb_edges = facesEdges[f1].size();
      bool found = false;

      for(unsigned int i =  0; v_edges.size() < nb_edges - 2 ; i = (i+1)%nb_edges) {
        edge e_tmp = facesEdges[f1][i];

        if(e_tmp == e)
          found = true;
        else if(found) {
          v_edges.push_back(e_tmp);
        }
      }

      facesEdges[f1] = v_edges;
      edgeMap::iterator it = edgesFaces.find(e);
      edgesFaces.erase(it,++it);
      vector<Face> v_faces;
      v_faces.push_back(f1);
      nodesFaces[n_tmp] = v_faces;

      if(n2 == n_tmp) {
        nodeMap::iterator it2 = nodesFaces.find(n1);
        nodesFaces.erase(it2,++it2);
        delNode(n1);
      }
      else {
        nodeMap::iterator it2 = nodesFaces.find(n2);
        nodesFaces.erase(it2,++it2);
        delNode(n2);
      }
    }
  }

  else {
    unsigned int nb_edges = facesEdges[f1].size();
    bool found = false;

    for(unsigned int i =  0; v_edges.size() < nb_edges - 1 ; i = (i+1)%nb_edges) {
      edge e_tmp = facesEdges[f1][i];

      if(e_tmp == e)
        found = true;
      else if(found) {
        if(edgesFaces[e_tmp][0] == f2)
          edgesFaces[e_tmp][0] = f1;

        if(edgesFaces[e_tmp][1] == f2)
          edgesFaces[e_tmp][1] = f1;

        v_edges.push_back(e_tmp);
      }
    }

    unsigned int nb_added = 0;
    nb_edges = facesEdges[f2].size();
    found = false;

    for(unsigned int i =  0; nb_added < nb_edges - 1 ; i = (i+1)%nb_edges) {
      edge e_tmp = facesEdges[f2][i];
      pair<node, node> eEnds = ends(e_tmp);
      isInF2.set(eEnds.first.id,true);
      isInF2.set(eEnds.second.id,true);

      if(e_tmp == e)
        found = true;
      else if(found) {
        if(edgesFaces[e_tmp][0] == f2)
          edgesFaces[e_tmp][0] = f1;

        if(edgesFaces[e_tmp][1] == f2)
          edgesFaces[e_tmp][1] = f1;

        v_edges.push_back(e_tmp);
        nb_added++;
      }
    }

    facesEdges[f1] = v_edges;
    edgeMap::iterator it = edgesFaces.find(e);
    edgesFaces.erase(it,++it);
    faceMap::iterator it2 = facesEdges.find(f2);
    facesEdges.erase(it2,++it2);

    Iterator<unsigned int> * itn = isInF2.findAll(true);

    while(itn->hasNext()) {
      node n_tmp = node(itn->next());
      vector<Face> v_faces;
      Iterator<Face> * itf = getFacesAdj(n_tmp);

      while(itf->hasNext())
        v_faces.push_back(itf->next());

      delete itf;
      nodesFaces[n_tmp] = v_faces;
    }

    delete itn;

    vector<Face>::iterator itf = faces.begin();

    for(; *itf != f2; ++itf) {}

    faces.erase(itf);
    delEdge(e);
  }
}

//============================================================
edge PlanarConMap::succCycleEdge(const edge e, const node n) const {
  assert(isElement(e) && isElement(n));
  assert(source(e)==n || target(e)==n);

  if(deg(n)==1)
    return e;

  int i = 0;
  edge e1;
  Iterator<edge>* it = getInOutEdges(n);

  while(it->hasNext()) {
    e1 = it->next();
    i++;

    if((e == e1) && (it->hasNext())) {
      e1 = it->next();
      delete it;
      return e1;
    }
    else if((e == e1) && (i==1)) { // cycle = an edge
      delete it;
      return e1;
    }
  }

  delete it;
  assert(e==e1);

  it = getInOutEdges(n); // the last edge is e, so first edge is returned
  assert(it->hasNext());
  e1 = it->next();
  delete it;
  return e1;
}

//============================================================
edge PlanarConMap::predCycleEdge(const edge e, const node n) const {
  assert(isElement(e) && isElement(n));
  assert(source(e)==n || target(e)==n);

  if(deg(n)==1)
    return e;

  Iterator<edge>* it = getInOutEdges(n);
  edge prec, e1;
  int i=0;
  bool stop=false;

  while(it->hasNext() && (!stop)) {
    e1 = it->next();
    i++;

    if(e == e1)
      stop = true;
    else  prec = e1;
  }

  assert(e==e1);

  if(i!=1) { // return the precedent edge
    delete it;
    return prec;
  }
  else {
    if(!it->hasNext()) { // cycle = an edge
      delete it;
      return e1;
    }
    else {
      while(it->hasNext()) // Return the last element
        e1 = it->next();

      delete it;
      return e1;
    }
  }
}

//============================================================
node PlanarConMap::succCycleNode(const node v , const node w) const {
  assert(isElement(v) && isElement(w));

  int i = 0;
  node n;
  Iterator<node>* it = getInOutNodes(v);

  while(it->hasNext()) {
    n = it->next();
    i++;

    if((n == w) && (it->hasNext())) {
      n = it->next();
      delete it;
      return n;
    }
    else if((n == w)&&(i==1)) {
      delete it;
      return n;
    }
  }

  delete it;
  assert(w==n);

  it = getInOutNodes(v);
  assert(it->hasNext());
  n = it->next();
  delete it;
  return n;
}

//============================================================
node PlanarConMap::predCycleNode(const node v, const node w) const {
  assert(isElement(v) && isElement(w));

  node prec, n1;
  int i=0;
  bool stop=false;

  Iterator<node>* it = getInOutNodes(v);

  while(it->hasNext() && (!stop)) {
    n1 = it->next();
    i++;

    if(w == n1)
      stop = true;
    else  prec = n1;
  }

  assert(w==n1);

  if(i!=1) {
    delete it;
    return prec;
  }
  else {
    if(!(it->hasNext())) {
      delete it;
      return n1;
    }

    while(it->hasNext())
      n1 = it->next();

    delete it;
    return n1;
  }
}

//============================================================
void PlanarConMap::update() {
  assert(SimpleTest::isSimple(this));
  //assert(ConnectedTest::isConnected(this));
  assert(PlanarityTest::isPlanar(this));

  PlanarityTest::planarEmbedding(this);
  computeFaces();
}

//============================================================
void PlanarConMap::computeFaces() {
  facesEdges.clear();
  edgesFaces.clear();
  faces.clear();
  nodesFaces.clear();

  unsigned int nbNodes = numberOfNodes();

  if(nbNodes < 3) {
    Face f(faceId->get());
    faces.push_back(f);
    vector<Face> v_faces;
    v_faces.push_back(f);

    // Compute the list of adjacent faces of each edge
    edge e;
    forEach(e,getEdges()) {
      edgesFaces.insert(edgeMapEntry(e,v_faces));
      faceMap::iterator itf = facesEdges.find(f);

      if( itf == facesEdges.end()) {
        vector<edge> v_tmp;
        v_tmp.push_back(e);
        facesEdges.insert(faceMapEntry(f,v_tmp));
      }
      else
        facesEdges[f].push_back(e);
    }

    // Compute the list of adjacent faces of each node
    node n;
    forEach(n,getNodes())
    nodesFaces.insert(nodeMapEntry(n,v_faces));

    if(facesEdges.size() == 0) {
      vector<edge> v;
      facesEdges.insert(faceMapEntry(f,v));
    }
  }

  else {
    MutableContainer<int> considered;
    MutableContainer<bool> sens;
    Face tmp;

    considered.setAll(0);
    sens.setAll(false);

    // Each edge must be considered two times
    for(int k=0; k<2; k++) {
      vector<edge> edges;
      edge e;
      forEach(e,getEdges()) {
        edges.clear();

        if (considered.get(e.id)<2) {
          Face lf(faceId->get());
          faces.push_back(lf);
          edge e1 = e;
          node n_tmp, n;
          int i = 0;

          if (sens.get(e1.id))
            n = target(e1);
          else
            n = source(e1);

          n_tmp = n;

          // Search for the other edges and nodes of the face lf and compute/update the list of adjacents faces of those edges/nodes
          do {
            considered.set(e1.id, considered.get(e1.id) + 1);
            EdgeMapIterator it_e(graph_component, e1, n);
            e1 = it_e.next();
            n = opposite(e1,n);
            edges.push_back(e1);
            nodeMap::iterator it_n = nodesFaces.find(n);

            if( it_n == nodesFaces.end()) {
              vector<Face> v_tmp;
              v_tmp.push_back(lf);
              nodesFaces.insert(nodeMapEntry(n,v_tmp));
            }
            else
              nodesFaces[n].push_back(lf);

            edgeMap::iterator ite = edgesFaces.find(e1);

            if( ite == edgesFaces.end()) {
              vector<Face> v_tmp;
              v_tmp.push_back(lf);
              edgesFaces.insert(edgeMapEntry(e1,v_tmp));
            }
            else
              edgesFaces[e1].push_back(lf);

            if(source(e1) == n)
              sens.set(e1.id,true);

            ++i;
          }
          while((e1 != e) || (n_tmp != n));

          facesEdges.insert(faceMapEntry(lf, edges));
        }
      }
    }
  }
}

//============================================================
unsigned int PlanarConMap::nbFaces() {
  return faces.size();
}

//============================================================
unsigned int PlanarConMap::nbFacesNodes(const Face f) {
  return facesEdges[f].size();
}

//============================================================
unsigned int PlanarConMap::nbFacesEdges(const Face f) {
  return facesEdges[f].size();
}

//============================================================
Iterator<Face>* PlanarConMap::getFaces() {
  return new FaceIterator(this);
}

//============================================================
Iterator<Face>* PlanarConMap::getFacesAdj(const node n) {
  return new FaceAdjIterator(this, n);
}

//============================================================
Iterator<node>* PlanarConMap::getFaceNodes(const Face f) {
  return new NodeFaceIterator(this, f);
}

//============================================================
Iterator<edge>* PlanarConMap::getFaceEdges(const Face f) {
  return new EdgeFaceIterator(this,f);
}

//============================================================
Face PlanarConMap::splitFace(Face f, const edge e) {
  pair<node, node> eEnds = ends(e);
  return splitFace(f, eEnds.first, eEnds.second);
}

//============================================================
Face PlanarConMap::splitFace(Face f, const node v, const node w, node n) {
  assert(std::find(faces.begin(), faces.end(),f) != faces.end());
  assert(containNode(f,v) && containNode(f,w));

  edge e1, e2, e_tmp, e_tmp2;
  bool pred_was_v = false;
  bool pred_was_w = false;
  bool first_was_v = false;
  bool first_was_w = false;
  bool v_found = false;
  bool w_found = false;

  if(!n.isValid()) {
    Iterator<edge>* it = getInOutEdges(v);

    while(it->hasNext()) {
      edge e = it->next();

      if(edgesFaces[e][0] == f || edgesFaces[e][1] == f) {
        n = opposite(e, v);
        break;
      }
    }

    delete it;
  }

  // Search for the predecessors of the futur edge (v,w) around v and w
  if(deg(v) == 1) {
    Iterator<edge>* ite = getInOutEdges(v);
    e1 = ite->next();
    v_found = true;
    delete ite;
  }

  if(deg(w) == 1) {
    Iterator<edge>* ite = getInOutEdges(w);
    e2 = ite->next();
    w_found = true;
    delete ite;
  }

  Iterator<edge> * ite = getFaceEdges(f);
  e_tmp = ite->next();
  pair<node, node> eEnds = ends(e_tmp);

  if(eEnds.first == v || eEnds.second == v) {
    first_was_v = true;
    pred_was_v =  true;
    e_tmp2 = e_tmp;
  }

  if(eEnds.first == w || eEnds.second == w) {
    first_was_w = true;
    pred_was_w = true;
    e_tmp2 = e_tmp;
  }

  while(ite->hasNext() && !(v_found && w_found)) {
    e_tmp = ite->next();
    eEnds = ends(e_tmp);

    if(!v_found && pred_was_v && (eEnds.first == v || eEnds.second == v)) {
      e1 = e_tmp2;
      v_found = true;
    }
    else if(!v_found && (eEnds.first == v || eEnds.second == v)) {
      pred_was_v = true;
      e_tmp2 = e_tmp;
    }
    else if(!v_found)
      pred_was_v = false;

    if(!w_found && pred_was_w && (eEnds.first == w || eEnds.second == w)) {
      e2 = e_tmp2;
      w_found = true;
    }
    else if(!w_found && (eEnds.first == w || eEnds.second == w)) {
      pred_was_w = true;
      e_tmp2 = e_tmp;
    }
    else if(!w_found)
      pred_was_w = false;

  }

  delete ite;

  if(!v_found && first_was_v)
    e1 = e_tmp;

  if(!w_found && first_was_w)
    e2 = e_tmp;

  Face new_face = Face(faceId->get());

  // Add the edge and update the map
  addEdgeMap(v,w,f,e1,e2,new_face);


  // As n must be contained into f, reverse f and new_face so that n is in f
  if(!containNode(f,n)) {
    vector<edge> ve_tmp = facesEdges[f];
    facesEdges[f] = facesEdges[new_face];
    facesEdges[new_face] = ve_tmp;
    MutableContainer<bool> isShared;
    MutableContainer<bool> nodeToUpdate;
    isShared.setAll(false);
    nodeToUpdate.setAll(false);

    for(unsigned int i = 0 ; i < facesEdges[f].size(); ++i) {
      edge e = facesEdges[f][i];
      eEnds = ends(e);
      nodeToUpdate.set(eEnds.first.id,true);
      nodeToUpdate.set(eEnds.second.id,true);

      if((edgesFaces[e][0] == f && edgesFaces[e][1] == new_face) || (edgesFaces[e][0] == new_face && edgesFaces[e][1] == f))
        isShared.set(e.id,true);
      else {
        if(edgesFaces[e][0] == new_face)
          edgesFaces[e][0] = f;

        if(edgesFaces[e][1] == new_face)
          edgesFaces[e][1] = f;
      }
    }

    for(unsigned int i = 0 ; i < facesEdges[new_face].size(); ++i) {
      edge e = facesEdges[new_face][i];

      if(!isShared.get(e.id)) {
        if(edgesFaces[e][0] == f)
          edgesFaces[e][0] = new_face;

        if(edgesFaces[e][1] == f)
          edgesFaces[e][1] = new_face;
      }
    }

    Iterator<unsigned int> * itn = nodeToUpdate.findAll(true);

    while(itn->hasNext()) {
      node n_tmp = node(itn->next());
      vector<Face> v_faces;
      Iterator<Face> * itf = getFacesAdj(n_tmp);

      while(itf->hasNext())
        v_faces.push_back(itf->next());

      delete itf;
      nodesFaces[n_tmp] = v_faces;
    }

    delete itn;

  }

  return new_face;
}


//============================================================
void PlanarConMap::mergeFaces(Face f, Face g) {
  assert(f != g);

  edge last;
  bool last_found = false;
  bool first_found = false;
  vector<edge> toDel;
  MutableContainer<bool> isInF;
  isInF.setAll(false);

  // Search for edges to delete
  vector<edge> ve = facesEdges[f];

  for(unsigned int i = 0 ; i < ve.size(); ++i) {
    isInF.set(ve[i].id,true);

    if(!last_found && containEdge(g,ve[i])) {
      first_found = true;
      last = ve[i];
    }
    else if(first_found)
      last_found = true;
  }

  ve = facesEdges[g];

  for(unsigned int i = 0 ; i < ve.size(); ++i)
    if(isInF.get(ve[i].id))
      toDel.push_back(ve[i]);

  assert(!toDel.empty());
  assert(toDel.size()!=facesEdges[g].size() && toDel.size()!=facesEdges[f].size());

  // Search for the first edge to delete on vector toDel
  unsigned int cpt = 0;

  for(; cpt < toDel.size() && toDel[cpt]!=last; ++cpt) {}

  assert(cpt < toDel.size());
  cpt = (cpt+1)%toDel.size();

  // Deletion of the edges and update of the map
  delEdgeMap(toDel[cpt],f);
  cpt = (cpt+1)%toDel.size();

  for(unsigned int i = 1; i < toDel.size(); ++i, cpt = (cpt+1)%toDel.size()) {
    edge e = toDel[cpt];
    pair<node, node> eEnds = ends(e);

    if(deg(eEnds.first)==1 || deg(eEnds.second)==1)
      delEdgeMap(e,f);
    else
      break;
  }
}

//============================================================
bool PlanarConMap::containNode(Face f, node v) {
  Iterator<Face> * it_f = getFacesAdj(v);

  while(it_f->hasNext())
    if((it_f->next()) == f) {
      delete it_f;
      return true;
    }

  delete it_f;
  return false;
}

//============================================================
bool PlanarConMap::containEdge(Face f, edge e) {
  for(unsigned int i = 0; i < 2; ++i)
    if(edgesFaces[e][i] == f)
      return true;

  return false;
}

//============================================================
Face PlanarConMap::getFaceContaining(node v, node w) {
  edge e = existEdge(v, w, false);
  assert(e.isValid());
  Face f1 = edgesFaces[e][0];
  Face f2 = edgesFaces[e][1];

  if( f1 == f2)
    return f1;

  Face f_tmp, f_tmp2;
  int taille1 = facesEdges[f1].size();
  int taille2 = facesEdges[f2].size();

  if(taille1 < taille2) {
    f_tmp = f1;
    f_tmp2 = f2;
  }
  else {
    taille1 = taille2;
    f_tmp = f2;
    f_tmp2 = f1;
  }

  int cpt = 0;

  while(cpt < taille1 && facesEdges[f_tmp][cpt] != e )
    cpt++;

  if(cpt != 0) {
    pair<node, node> eEnds = ends(facesEdges[f_tmp][cpt-1]);
    return ((eEnds.first == v) || (eEnds.second == v)) ? f_tmp : f_tmp2;
  }

  pair<node, node> eEnds = ends(facesEdges[f_tmp][taille1-1]);
  return ((eEnds.first == v) || (eEnds.second == v)) ? f_tmp : f_tmp2;
}


//=================================================================
Face PlanarConMap::sameFace(node v, node n) {
  Face f;
  forEach(f, getFacesAdj(v)) {
    if(containNode(f,n))
      return f;
  }
  return Face();
}

//============================================================
ostream & operator << (ostream &os, PlanarConMap *sp) {
  os<< "Faces : " << endl << endl;
  Iterator<Face>* it = sp->getFaces();

  while(it->hasNext()) {
    Face tmp = it->next();
    os << "Face " << tmp.id << " : ";
    os << "(edges : ";
    Iterator<edge>* ite = sp->getFaceEdges(tmp);

    while(ite->hasNext()) {
      edge e = ite->next();
      os << e.id << " ";
    }

    delete ite;
    os<<") and ";

    os<<"(nodes : ";
    Iterator<node>* itn = sp->getFaceNodes(tmp);

    while(itn->hasNext()) {
      node n = itn->next();
      os << n.id << " ";
    }

    delete itn;
    os << ")" << endl;
  }

  delete it;

  Iterator<node>* it_n = sp->getNodes();

  while(it_n->hasNext()) {
    node n = it_n->next();
    os << "node " << n.id << " : ";
    os << "(edge : ";
    Iterator<edge> * it_e = sp->getInOutEdges(n);

    while(it_e->hasNext()) {
      edge e = it_e->next();
      os << e.id << " ";
    }

    delete it_e;
    os << ") and ";
    os << "(Faces : ";
    Iterator<Face>* it_f = sp->getFacesAdj(n);

    while(it_f->hasNext()) {
      Face f = it_f->next();
      os << f.id << " ";
    }

    delete it_f;
    os << ")" << endl;
  }

  delete it_n;

  os << endl;
  return os;
}

tlp::PlanarConMap* tlp::computePlanarConMap(tlp::Graph* graph) {
  return (graph && ConnectedTest::isConnected(graph))
         ? new PlanarConMap(graph) : NULL;
}
