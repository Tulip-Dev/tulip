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

#include <tulip/Ordering.h>
#include <tulip/BiconnectedTest.h>
#include <tulip/PlanarConMap.h>

#include <cassert>
#include <cstdlib>

using namespace std;
using namespace tlp;


#ifndef DOXYGEN_NOTFOR_DEVEL
struct tlp::augmentableAndNodes_ {
  bool augmentable;
  vector<node> Nodes;
};
#endif // DOXYGEN_NOTFOR_DEVEL

//==========================================================
/**
 * A function that return the number of node of the infinite face
 */
int Ordering::infFaceSize() {
  Iterator<unsigned int> * itn = contour.findAll(true);
  int cpt = 0;

  while(itn->hasNext()) {
    itn->next();
    cpt++;
  }

  delete itn;
  return cpt;
}

//==========================================================
/**
 * return the number of ci in f such as ci+1 is in f
 */
int Ordering::seqp(Face f) {
  MutableContainer<bool> seq_p;
  seq_p.setAll(false);
  Iterator<node> *it = Gp->getFaceNodes(f);

  while(it->hasNext()) {
    node no = it->next();

    if(contour.get(no.id))
      seq_p.set(no.id,true);
  }

  delete it;
  int cpt = 0;
  node n = v1[v1.size()-1];
  node n2 = right.get(n.id);;

  while(n != v1[0]) {
    if(seq_p.get(n2.id) && seq_p.get(n.id))
      cpt++;

    n = n2;
    n2 = right.get(n.id);
  }

  return cpt;
}

//==========================================================
/**
 * update members oute, outv and visitedFaces for face f
 */
void Ordering::updateOutAndVisitedFaces(Face f) {
  int cpt = 0;
  int cpt2 = 0;
  node node_tmp, node_first;
  bool first_was_on_c = false;
  bool pred_was_on_c = false;
  bool visited_face = false;
  Iterator<node> *it_node = Gp->getFaceNodes(f);

  if(it_node->hasNext()) {
    node_first = it_node->next();

    if(contour.get(node_first.id)) {
      cpt++;
      first_was_on_c = true;
      pred_was_on_c = true;

      if(visitedNodes.get(node_first.id) && Gp->deg(node_first) == 2)
        visited_face = true;
    }
  }

  while(it_node->hasNext()) {
    node_tmp = it_node->next();

    if(contour.get(node_tmp.id)) {
      cpt++;

      if(pred_was_on_c)
        cpt2++;

      pred_was_on_c = true;

      if(visitedNodes.get(node_tmp.id) && Gp->deg(node_tmp) == 2)
        visited_face = true;
    }
    else
      pred_was_on_c = false;
  }

  delete it_node;

  if(first_was_on_c && pred_was_on_c)
    cpt2++;

  outv.set(f.id,cpt);
  oute.set(f.id,cpt2);
  visitedFaces.set(f.id,visited_face);
}

//==========================================================
/**
 * update members left and right
 */
void Ordering::updateContourLeftRight(node pred, node n, edge e, node last) {
  node no = pred;
  node no2 =  n;

  while(no2 != last) {
    contour.set(no2.id,true);
    right.set(no.id,no2);
    left.set(no2.id,no);
    e = Gp->predCycleEdge(e,no2);
    no = no2;
    no2 = Gp->opposite(e,no);
  }

  right.set(no.id,no2);
  left.set(no2.id,no);
}

//==========================================================
/**
 * update is_selectable_visited and is_selectable
 */
void Ordering::updateNewSelectableNodes(node node_f, node no_tmp2 ,edge, node node_last, vector<Face> v_faces, bool one_face,bool was_visited, bool selection_face) {
  MutableContainer<bool> tried;
  tried.setAll(false);
  node n = node_f;
  node tmp = no_tmp2;
  node tmp2 = node() ;
  unsigned int lim = v_faces.size();

  while(n != node_last) {
    tmp2 = n;

    if(Gp->deg(n) > 2 && isSelectable(n)) {
      if(visitedNodes.get(n.id))
        is_selectable_visited.set(n.id,true);
      else
        is_selectable.set(n.id,true);
    }
    else {
      is_selectable_visited.set(n.id,false);
      is_selectable.set(n.id,false);
    }

    tried.set(n.id,true);
    n = tmp;
    tmp = right.get(n.id);
  }

  if(Gp->deg(n) > 2 && isSelectable(n))
    is_selectable_visited.set(n.id,true);
  else {
    is_selectable_visited.set(n.id,false);
    is_selectable.set(n.id,false);
  }

  if(one_face) {
    if(tmp2 == node())
      tmp2 = node_f;

    Iterator<node> * it_no = Gp->getFaceNodes(Gp->getFaceContaining(tmp2,node_last));

    while(it_no->hasNext()) {
      node no_tmp = it_no->next();

      if(!tried.get(no_tmp.id)) {
        bool on_c = contour.get(no_tmp.id);

        if(on_c) {
          bool sel = isSelectable(no_tmp);

          if(sel)
            if(visitedNodes.get(no_tmp.id))
              is_selectable_visited.set(no_tmp.id,true);
            else
              is_selectable.set(no_tmp.id, true);
          else {
            is_selectable_visited.set(no_tmp.id,false);
            is_selectable_visited.set(no_tmp.id,false);
          }
        }

        tried.set(no_tmp.id,true);
      }
    }

    delete it_no;
    lim--;
  }

  if((selection_face && was_visited) || !selection_face)
    for(unsigned int i = 0; i < lim; ++i) {
      bool face_sel = false;
      Face  f_tmp = v_faces[i];

      if(is_selectable_face.get(f_tmp.id) || is_selectable_visited_face.get(f_tmp.id))
        face_sel = true;

      Iterator<node> * itn = Gp->getFaceNodes(f_tmp);

      if(face_sel)
        while(itn->hasNext()) {
          node no_tmp = itn->next();
          is_selectable.set(no_tmp.id,false);
          is_selectable_visited.set(no_tmp.id,false);
          tried.set(no_tmp.id,true);
        }
      else {
        while(itn->hasNext()) {
          node no_tmp = itn->next();

          if(!tried.get(no_tmp.id) && (is_selectable_visited.get(no_tmp.id) || is_selectable.get(no_tmp.id))) {
            if(!isSelectable(no_tmp)) {
              is_selectable_visited.set(no_tmp.id,false);
              is_selectable.set(no_tmp.id,false);
            }
          }

          tried.set(no_tmp.id,true);
        }
      }

      delete itn;
    }
}

//==========================================================
/**
 * update is_selectable_visted_face and is_selectable_face
 */
void Ordering::updateSelectableFaces(vector<Face> v_faces) {
  Face derniere = Gp->getFaceContaining(v1[0],v1[1]);

  for(unsigned int m = 0 ; m < v_faces.size() ; ++m) {
    Face f_tmp = v_faces[m];

    if(f_tmp == derniere || isOuterFace.get(f_tmp.id))
      continue;

    if(outv.get(f_tmp.id) < 3)
      continue;

    if(visitedFaces.get(f_tmp.id))
      if(outv.get(f_tmp.id) == oute.get(f_tmp.id) + 1)
        is_selectable_visited_face.set(f_tmp.id, true);
      else {
        is_selectable_visited_face.set(f_tmp.id, false);
        is_selectable_face.set(f_tmp.id,false);
      }
    else if(outv.get(f_tmp.id) == oute.get(f_tmp.id) + 1)
      is_selectable_face.set(f_tmp.id, true);
    else
      is_selectable_face.set(f_tmp.id, false);
  }
}

//==========================================================
/**
 *return a structure augmentableAndNodes_ where augmentable
 * equal true if and only if we can make an augmentation of
 * f and Nodes contain the nodes where we have to begin the
 * augmentation process
 */
struct augmentableAndNodes_ Ordering::getAugAndNodes(Face f) {
  struct augmentableAndNodes_ res;
  Iterator<node> * it = Gp->getFaceNodes(f);
  int small = infFaceSize();
  int big = 0;
  node nod = v1[v1.size()-2];
  node nod2 = v1[v1.size()-1];
  int pos = 0;
  bool f_was_first = false;
  bool f_was_last = true;
  bool found_ff = false;
  bool found_minl = false;
  node n_f ;
  node n_l ;

  if(existMarkedF) {
    n_f = minMarkedFace.n_first;
    n_l = minMarkedFace.n_last;
  }
  else {
    n_f = nod2;
    n_l = v1[0];
  }

  node no, no2, no3, no4;

  if(nod2 == v1[1]) {
    if(nod2 == n_f)
      f_was_first = true;

    while(it->hasNext()) {
      if(it->next() == nod2) {
        found_ff = true;
        small = pos;
        no2 = nod2;
        no = nod;
        break;
      }
    }

    nod = nod2;
    nod2 = right.get(nod.id);
    pos++;
  }

  delete it;

  while(nod2 != v1[1]) {
    if(nod2 == n_f && !found_ff)
      f_was_first = true;

    it = Gp->getFaceNodes(f);

    while(it->hasNext()) {
      if(it->next() == nod2) {
        if(pos<small) {
          found_ff = true;
          small = pos;
          no2 = nod2;
          no = nod;
        }

        if(pos>big) {
          big = pos;
          no3 = nod2;
          no4 = nod;

          if(found_minl) {
            f_was_last = false;
            break;
          }
        }

        break;
      }
    }

    delete it;

    if(nod2 == n_l)
      found_minl = true;

    if(!f_was_last)
      break;

    nod = nod2;
    nod2 = right.get(nod.id);
    pos++;
  }

  res.augmentable = (f_was_first && f_was_last);
  res.Nodes.push_back(no);
  res.Nodes.push_back(no2);
  res.Nodes.push_back(no4);
  res.Nodes.push_back(no3);
  return res;
}

//==========================================================
/**
 * set minMarkedFace at f
 */
void Ordering::setMinMarkedFace(Face f) {
  existMarkedF = true;
  int small = infFaceSize()-v1.size();
  int big = 0;
  node n_f, n_l;
  node nod = v1[v1.size()-1];
  node nod2 = v1[v1.size()-2];
  int pos=0;
  bool done = false;
  bool first_pass = true;

  while(!done) {
    if(!first_pass && nod2 == v1[0]) {
      done = true;
      continue;
    }

    if(first_pass)
      first_pass = false;

    Iterator<node> * itn = Gp->getFaceNodes(f);

    while(itn->hasNext()) {
      node tmp = itn->next();

      if(tmp == nod) {
        if(pos < small) {
          n_f = tmp;
          small = pos;
        }

        if(pos > big) {
          big = pos;
          n_l = tmp;
        }
      }
    }

    delete itn;
    nod2 = nod;
    nod = right.get(nod2.id);
    pos++;
  }

  minMarkedFace.n_first = n_f;
  minMarkedFace.n_last = n_l;
  minMarkedFace.face = f;
}

//==========================================================
/**
 * compute the "minimal" marked face
 */
void Ordering::minMarkedf() {
  int contourSize = infFaceSize()-v1.size();
  Iterator<unsigned int> * itf = markedFaces.findAll(true);

  minMarkedFace.face = Face();
  int first = contourSize;
  int last = 0;
  minMarkedFace.n_first = v1[v1.size()-1];
  minMarkedFace.n_last = v1[0];

  if(!itf->hasNext()) {
    existMarkedF = false;
  }

  while(itf->hasNext()) {
    int i = 0;
    int small = contourSize;
    int big = 0;
    Face f = static_cast<Face> (itf->next());
    node n_f,n_l;
    node n = v1[v1.size()-2];
    node n2 = v1[v1.size()-1];

    while(n!=v1[0]) {
      Iterator<node> * itn = Gp->getFaceNodes(f);

      while(itn->hasNext()) {
        node tmp = itn->next();

        if(tmp==n2) {
          if(i<small) {
            n_f = tmp;
            small = i;
          }

          if(i>big) {
            n_l = tmp;
            big = i;
          }
        }
      }

      delete itn;
      n = n2;
      n2 = right.get(n.id);
      i++;
    }

    if(small > first && big < last) {
      minMarkedFace.n_first = n_f;
      minMarkedFace.n_last = n_l;
      minMarkedFace.face = f;
      first = small;
      last = big;
    }
  }

  delete itf;
}

//==========================================================
/**
 * return the last node of q, where q is the longest path
 * that can be induce (in the graph) on face f starting
 * at point n and in counter-clockwise
 */
node Ordering::getLastOfQ(Face f, node pred, node n, edge e) {
  vector<node> q;
  edge e_tmp = e;
  node tmp = pred;
  node tmp2 = n;
  bool done = false;
  e_tmp = Gp->succCycleEdge(e_tmp,tmp);

  while(!Gp->containEdge(f,e_tmp))
    e_tmp = Gp->succCycleEdge(e_tmp,tmp);

  tmp2 = Gp->opposite(e_tmp, tmp);
  q.push_back(tmp);

  while(!done) {
    for(unsigned int l = 0 ; l < q.size()-1 ; ++l)
      if(Gp->hasEdge(q[l],tmp2, false)) {
        done = true;
        break;
      }

    if(done)
      break;

    q.push_back(tmp2);
    tmp = tmp2;
    e_tmp = Gp->predCycleEdge(e_tmp, tmp);
    tmp2 = Gp->opposite(e_tmp, tmp);
  }

  return q.back();
}

//==========================================================
/**
 * return the last node of p, where p is the longest path
 * that can be induce (in the graph) on face f starting
 * at point n and in clockwise
 */
node Ordering::getLastOfP(Face f,node pred, node n, edge e) {
  vector<node> p;
  edge e_tmp = e;
  node tmp = pred;
  node tmp2 = n;
  bool done = false;
  e_tmp = Gp->predCycleEdge(e_tmp, tmp);

  while(!Gp->containEdge(f,e_tmp))
    e_tmp = Gp->predCycleEdge(e_tmp, tmp);

  tmp2 = Gp->opposite(e_tmp, tmp);
  p.push_back(tmp);

  while(!done) {
    for(unsigned int l = 0 ; l < p.size()-1 ; ++l)
      if(Gp->hasEdge(p[l], tmp2, false)) {
        done = true;
        break;
      }

    if(done)
      break;

    p.push_back(tmp2);
    tmp = tmp2;
    e_tmp = Gp->succCycleEdge(e_tmp, tmp2);
    tmp2 =  Gp->opposite(e_tmp,tmp);
  }

  return p.back();
}

//==========================================================
/**
 * return the longest path of degree 2 on face fn from node fn[from]
 */
vector<node> Ordering::getPathFrom(vector<node> fn, int from) {
  vector<node> res;
  int i = from;
  int l = fn.size();
  res.push_back(fn[i]);
  i=(l+i-1)%l;

  while(Gp->deg(fn[i])==2) {
    res.push_back(fn[i]);
    i=(l+i-1)%l;
  }

  if (res.size()==1 || !Gp->hasEdge(res[0],fn[i], false))
    res.push_back(fn[i]);

  return res;
}

//==========================================================
/**
 * augment the face f and update members
 */
void Ordering::augment(Face f, node pred, node n, node pred_last, node last, int nbNewFace, bool pair) {
  int nbTours = ( pair ? nbNewFace : nbNewFace - 1);
  int right_ov = 1, left_ov = 1, right_oe = 0, left_oe = 0;
  bool visited = false;
  node n_pred, n_n, l_pred, n_last;
  edge e1, e2;
  vector<Face > newFaces;
  Face  newFace;
  node no_selectable;
  node tmp, tmp2, l_tmp, l_tmp2;
  edge e_tmp, e_ltmp;
  bool was_last_face = (f == Gp->getFaceContaining(v1[0],v1[1]));

  n_pred = pred;
  n_n = n;
  l_pred = pred_last;
  n_last = last;
  e1 = Gp->existEdge(n_pred, n_n);

  if (!e1.isValid())
    e1 = Gp->existEdge(n_n, n_pred);

  e2 = Gp->existEdge(l_pred, n_last);

  if (!e2.isValid())
    e2 = Gp->existEdge(n_last,l_pred);

  e1 = Gp->predCycleEdge(e1, n_n);
  n_pred = n_n;
  n_n = Gp->opposite(e1, n_pred);

  for(int i = 1 ; i <= nbTours  ; ++i) {

    while(Gp->containEdge(f,e1)) {
      left_oe++;
      left_ov++;

      if(Gp->deg(n_pred) == 2 && visitedNodes.get(n_pred.id))
        visited = true;

      n_pred = n_n ;
      e1 = Gp->predCycleEdge(e1, n_n);
      n_n = Gp->opposite(e1,n_pred);
    }

    no_selectable = n_n;
    e_tmp = e1;
    tmp = n_pred;
    node lastP = getLastOfP(f,tmp,tmp2,e_tmp);

    while(Gp->containEdge(f,e2)) {
      right_ov++;
      right_oe++;

      if(Gp->deg(n_last ) == 2 && visitedNodes.get(n_last.id))
        visited = true;

      n_last = l_pred ;
      e2 = Gp->succCycleEdge(e2, n_last);
      l_pred = Gp->opposite(e2,n_last);
    }

    e_tmp = e2;
    tmp = n_last;
    node lastQ = getLastOfQ(f, tmp, tmp2,e_tmp);
    newFace = Gp->splitFace(f,lastP,lastQ,n_pred);
    edge dummy= Gp->existEdge(lastP,lastQ);

    if (!dummy.isValid())
      dummy = Gp->existEdge(lastQ,lastP);

    dummy_edge.push_back(dummy);
    newFaces.push_back(f);

    if(visited)
      visitedFaces.set(f.id, true);

    oute.set(f.id, left_oe + right_oe );
    outv.set(f.id, left_ov + right_ov);
    f = newFace;

    n_pred = n_n ;
    e1 = Gp->predCycleEdge(e1, n_n);
    n_n = Gp->opposite(e1,n_pred);
    n_last = l_pred ;
    e2 = Gp->succCycleEdge(e2, n_last);
    l_pred = Gp->opposite(e2,n_last);
    left_ov = 1;
    right_ov = 1;
    left_oe = 0;
    right_oe = 0;
    visited = false;
  }

  if(!pair) {
    while(Gp->containEdge(f,e1)) {
      n_pred = n_n ;
      e1 = Gp->predCycleEdge(e1, n_n);
      n_n = Gp->opposite(e1,n_pred);
    }

    no_selectable = n_pred;
    e_tmp = e1;
    tmp = n_n;
    node lastP = getLastOfP(f,tmp,tmp2,e_tmp);

    while(Gp->containEdge(f,e2)) {
      n_last = l_pred ;
      e2 = Gp->succCycleEdge(e2, l_pred);
      l_pred = Gp->opposite(e2,n_last);
    }

    e_tmp = e2;
    tmp = n_last;
    node lastQ = getLastOfQ(f,tmp,tmp2,e_tmp);

    newFace = Gp->splitFace(f,lastP,lastQ,n_pred);
    edge dummy= Gp->existEdge(lastP,lastQ);

    if (!dummy.isValid())
      dummy = Gp->existEdge(lastQ,lastP);

    dummy_edge.push_back(dummy);
    newFaces.push_back(f);
    newFaces.push_back(newFace);

    updateOutAndVisitedFaces(f);
    updateOutAndVisitedFaces(newFace);
  }

  else {
    newFaces.push_back(f);

    while(n_pred != n_last) {
      left_ov++;
      left_oe++;

      if(Gp->deg(n_pred) == 2 && visitedNodes.get(n_n.id))
        visited = true;

      n_pred = n_n;
      n_n = right.get(n_pred.id);
    }

    visitedFaces.set(f.id,visited);
    oute.set(f.id, left_oe);
    outv.set(f.id, left_ov);
  }

  if(was_last_face) {
    outv.add(newFaces[0].id, v1.size() - 2);
    oute.add(newFaces[0].id, v1.size() - 1);
  }

  // update markedFaces
  unsigned int taille = newFaces.size();

  if(pair && ((left_ov == 2 && left_oe == 1) || (left_ov == 1 && left_oe == 0))) {
    for(unsigned int i = 0 ; i < taille-1 ; ++i)
      markedFaces.set((newFaces[i]).id,true);

    setMinMarkedFace(newFaces[newFaces.size()-2]);
  }
  else {
    for(unsigned int i = 0 ; i < taille ; ++i)
      if( i != taille - 2)
        markedFaces.set((newFaces[i]).id,true);

    setMinMarkedFace(newFaces[newFaces.size()-1]);
  }


  // update of is_selectable_visited_face, is_selectable_face, is_selectable and is_selectable
  if(pair && left_ov == 2 && left_oe == 1) {
    if(isSelectable(n_n)) {
      if(visitedNodes.get(n_n.id))
        is_selectable_visited.set(n_n.id,true);
      else
        is_selectable.set(n_n.id,true);
    }

    if(isSelectable(n_pred)) {
      if(visitedNodes.get(n_pred.id))
        is_selectable_visited.set(n_pred.id,true);
      else
        is_selectable.set(n_pred.id,true);
    }
  }

  else if(pair) {
    Face f_tmp = newFaces[newFaces.size()-1];

    if(outv.get(f_tmp.id) >= 2)
      if(visitedFaces.get(f_tmp.id))
        is_selectable_visited_face.set(f_tmp.id,true);
      else
        is_selectable_face.set(f_tmp.id, true);
    else {
      if(visitedNodes.get(no_selectable.id))
        is_selectable_visited.set(no_selectable.id,true);
      else
        is_selectable.set(no_selectable.id,true);
    }
  }
  else {
    if(visitedNodes.get(no_selectable.id))
      is_selectable_visited.set(no_selectable.id,true);
    else
      is_selectable.set(no_selectable.id,true);
  }
}

//==========================================================
/**
 * select node n and update members
 */
void Ordering::selectAndUpdate(node n) {

  //update visitedNodes, contour, is_selectable and is_selectable_visited
  visitedNodes.set(n.id,false);
  contour.set(n.id, false);
  is_selectable_visited.set(n.id,false);
  is_selectable.set(n.id,false);

  vector<node> res;
  vector<node> noeuds;
  vector<Face > faces = vector<Face>();
  vector<bool> splited;
  MutableContainer<bool> on_cont;
  node n1;

  res.push_back(n);
  _data.push_back(res);

  node v = left.get(n.id);
  n1 = v;
  on_cont.setAll(false);

  // search the neighborhood of v and update visitedNodes
  on_cont.set(n1.id,true);
  faces.push_back(Gp->getFaceContaining(n1,n));
  noeuds.push_back(n1);
  visitedNodes.set(n1.id,true);
  edge e_first = Gp->existEdge(n1,n);

  if (!e_first.isValid())
    e_first = Gp->existEdge(n,n1);

  edge e = Gp->succCycleEdge(e_first,n);
  n1=Gp->opposite(e,n);

  while(n1 != v) {
    visitedNodes.set(n1.id,true);
    noeuds.push_back(n1);
    on_cont.set(n1.id,true);
    e=Gp->succCycleEdge(e,n);

    if(Gp->opposite(e,n) != v)
      faces.push_back(Gp->getFaceContaining(n1,n));

    n1 = Gp->opposite(e,n);
  }

  node tmp;
  node tmp2;
  edge e_tmp;
  node node_f;
  node node_last;
  bool done = false;
  tmp = noeuds[noeuds.size()-1];
  tmp2 = right.get(tmp.id);

  while(!done && tmp2 != v1[1]) {
    done = true;

    for(unsigned int i = 0; i < faces.size() ; ++i)
      if(Gp->containNode(faces[i],tmp2)) {
        done = false;
        break;
      }

    if(done)
      continue;

    tmp = tmp2;
    tmp2 = right.get(tmp.id);
  }

  node_last = tmp;
  tmp = v;
  tmp2 = n;

  while(tmp != v1[v1.size()-2] && tmp != node_last) {
    done = false;
    tmp2 = tmp;
    tmp = left.get(tmp2.id);

    for(unsigned int i = 0; i < faces.size() ; ++i)
      if(Gp->containNode(faces[i],tmp)) {
        done = true;
        break;
      }

    if(!done)
      break;
  }

  node_f = tmp2;
  node no_tmp = tmp;

  // for the update of outv and oute
  node n2 = node_f;
  n2 = right.get(n2.id);

  while(n2 != node_last) {
    Iterator<Face> * ite_face = Gp->getFacesAdj(n2);

    while(ite_face->hasNext()) {
      Face f_tmp = static_cast<Face>(ite_face->next());
      outv.add(f_tmp.id, -1);
    }

    delete ite_face;
    oute.add(ext.id, -1);
    n2 = right.get(n2.id);
  }

  oute.add(ext.id, -1);

  // split and merge old faces
  e = Gp->predCycleEdge(e_first,n1);
  n2 = Gp->opposite(e,n1);
  bool spli = false;

  for(int i = 0; static_cast<unsigned int>(i) < noeuds.size()-1; i++) {
    bool entered=false;

    while(n2 != noeuds[i+1]) {
      entered = true;

      if(on_cont.get(n2.id) || contour.get(n2.id)) {
        spli=true;
        Face  f1 = Gp->splitFace(faces[i],n1,noeuds[i+1],n2);
        edge dummy= Gp->existEdge(n1,noeuds[i+1]);

        if (!dummy.isValid())
          dummy = Gp->existEdge(noeuds[i+1],n1);

        dummy_edge.push_back(dummy);
        Gp->mergeFaces(ext,f1);
        n2=noeuds[i+1];
        e = Gp->existEdge(n1,n2);

        if (!e.isValid())
          e = Gp->existEdge(n2,n1);
      }
      else {
        on_cont.set(n2.id,true);
        e= Gp->predCycleEdge(e,n2);
        n1=n2;
        n2=Gp->opposite(e,n1);
      }
    }

    if(!entered) {
      Gp->mergeFaces(ext,faces[i]);
    }

    if( entered && !spli) {
      Gp->mergeFaces(ext,faces[i]);
    }

    e = Gp->predCycleEdge(e,n2);
    n1 = n2;
    n2 = Gp->opposite(e,n1);

    if(n2 == n) {
      e = Gp->predCycleEdge(e,n1);
      n2 = Gp->opposite(e,n1);
    }

    splited.push_back(spli);
    spli=false;
  }

  // update visitedFaces
  for(int i = 0; static_cast<unsigned int>(i)<noeuds.size(); i++) {
    int deg = Gp->deg(noeuds[i]);
    Iterator<Face > * itf = Gp->getFacesAdj(noeuds[i]);

    while(itf->hasNext()) {
      Face f2 = itf->next();

      if(deg == 2 && !isOuterFace.get(f2.id))
        visitedFaces.set(f2.id,true);
    }

    delete itf;
  }

  Face  derniere = Gp->getFaceContaining(v1[0],v1[1]);
  //
  e_tmp = Gp->existEdge(node_f,no_tmp);

  if (!e_tmp.isValid())
    e_tmp = Gp->existEdge(no_tmp,node_f);

  e_tmp = Gp->predCycleEdge(e_tmp,node_f);
  no_tmp = Gp->opposite(e_tmp,node_f);
  edge ed_tmp = e_tmp;
  node no_tmp2 = no_tmp;

//update contour, left , right
  updateContourLeftRight(node_f,no_tmp,e_tmp,node_last);

  // update outv, oute, seqP
  bool one_face = true;
  int addNode = 0;
  MutableContainer<bool> update_seqp;
  vector<Face> v_face;
  update_seqp.setAll(false);

  for(int i=0; static_cast<unsigned int>(i)<faces.size(); i++) {
    if(splited[i]) {
      outv.set((faces[i]).id,0);
      oute.set((faces[i]).id,0);
      seqP.set((faces[i]).id,seqp(faces[i]));
      update_seqp.set((faces[i]).id,true);
      v_face.push_back(faces[i]);

      if(faces[i]==derniere) {
        outv.set((faces[i]).id,v1.size()-2);
        oute.set((faces[i]).id,v1.size()-1);
      }

      if(Gp->containNode(faces[i],node_f))
        outv.add((faces[i]).id, 1);

      if(Gp->containNode(faces[i],node_last))
        outv.add((faces[i]).id, 1);
    }
  }

  n1 = node_f;
  n2 = no_tmp;
  Face f1 = Gp->getFaceContaining(n1,n2);

  if(!update_seqp.get(f1.id)) {
    seqP.set(f1.id,seqp(f1));
    update_seqp.set(f1.id,true);
    v_face.push_back(f1);
  }

  while(n2 != node_last) {
    f1 = Gp->getFaceContaining(n1,n2);
    oute.add(f1.id, 1);
    addNode++;
    int cpt = 0;
    Iterator<Face > * itf = Gp->getFacesAdj(n2);

    while(itf->hasNext()) {
      Face f2 = itf->next();

      if(isOuterFace.get(f2.id))
        continue;

      cpt++;

      if(!update_seqp.get(f2.id)) {
        seqP.set(f2.id,seqp(f2));
        update_seqp.set(f2.id,true);
        v_face.push_back(f2);

      }

      outv.add(f2.id, 1);
    }

    delete itf;
    n1 = n2;
    n2 = right.get(n1.id);

    if(cpt != 1)
      one_face = false;
  }

  f1 = Gp->getFaceContaining(n1,n2);
  oute.add(f1.id, 1);

  if(!update_seqp.get(f1.id)) {
    seqP.set(f1.id,seqp(f1));
    v_face.push_back(f1);
    update_seqp.set(f1.id,true);
  }

  outv.add(ext.id, addNode);
  oute.add(ext.id, addNode+1);

  if(Gp->deg(node_f) != 2 && Gp->deg(node_last) != 2)
    one_face = false;

  // update is_selectable_visited_face and is_selectable_face
  updateSelectableFaces(v_face);

  // update of is_selectable and is_selectable_visited
  updateNewSelectableNodes(node_f,no_tmp2, ed_tmp, node_last,v_face,one_face);
}

//==========================================================
/**
 * select face f and update members
 */
void Ordering::selectAndUpdate(Face f) {
  int compteur = 0;
  node n;
  node pred;
  Iterator<node> * it_n = Gp->getFaceNodes(f);

  while(it_n->hasNext()) {
    pred = n;
    n = it_n->next();

    if(Gp->deg(n) == 2 && contour.get(n.id))
      break;

    compteur++;
  }

  if(compteur == 0) {
    pred = left.get(n.id);

    while(Gp->deg(pred) == 2) {
      n = pred;
      pred = left.get(n.id);
    }
  }

  delete it_n;

  vector<node> noeuds;
  node n1 = pred;
  node n2 = n;
  edge edge_pred = Gp->existEdge(n1,n2);

  if (!edge_pred.isValid())
    edge_pred = Gp->existEdge(n2,n1);

  edge_pred = Gp->succCycleEdge(edge_pred,pred);
  bool min_markded_face = false;
  int add_node = 0;
  bool was_visited = visitedFaces.get(f.id);
  int suppNodes = 1;

// update is_selectable_visited_face, is_selectable_face, visitedNodes, contour, outv, oute, visitedFaces and seqP
  is_selectable_visited_face.set(f.id,false);
  is_selectable_face.set(f.id,false);
  visitedNodes.set(n2.id,false);
  contour.set(n2.id,false);
  outv.set(f.id,0);
  oute.set(f.id,0);
  visitedFaces.set(f.id,false);
  seqP.set(f.id,0);

  noeuds.push_back(n2);
  n1 = n2;
  n2 = right.get(n1.id);

  while(Gp->deg(n2) == 2 && n2 != v1[0]) {
    suppNodes++;
    noeuds.push_back(n2);
    visitedNodes.set(n2.id,false);
    contour.set(n2.id,false);
    n1 = n2;
    n2 = right.get(n1.id);
  }

  _data.push_back(noeuds);


  // test if face f is marked and if it is the "minimal" one
  if(markedFaces.get(f.id)) {
    markedFaces.set(f.id,false);

    if(f == minMarkedFace.face)
      min_markded_face=true;
  }

  Gp->mergeFaces(ext,f);

  // update visitedNodes and visitedFaces  for extremities of the path
  visitedNodes.set(pred.id,true);

  if(Gp->deg(pred) == 2) {
    Iterator<Face > * itf = Gp->getFacesAdj(pred);

    while(itf->hasNext()) {
      Face fa = itf->next();
      visitedFaces.set(fa.id,true);
    }

    delete itf;
  }

  visitedNodes.set(n2.id,true);

  if(Gp->deg(n2) == 2) {
    Iterator<Face > * itf = Gp->getFacesAdj(n2);

    while(itf->hasNext()) {
      Face  fa = itf->next();
      visitedFaces.set(fa.id,true);
    }

    delete itf;
  }

  //update contour, left and right
  node n4 = pred;
  edge_pred = Gp->predCycleEdge(edge_pred,pred);
  node n3 = Gp->opposite(edge_pred,n4);
  updateContourLeftRight(n4,n3,edge_pred,n2);

  // update outv, oute, seqP
  bool one_face = true;
  MutableContainer<bool> update_seqp;
  vector<Face> v_face;
  node no_tmp2 = n3;
  edge ed_tmp = edge_pred;
  update_seqp.setAll(false);
  outv.add(ext.id, -suppNodes);
  oute.add(ext.id, - suppNodes - 1);

  while(n3!=n2) {
    Iterator<Face > * itf = Gp->getFacesAdj(n3);
    int cpt = 0;

    while(itf->hasNext()) {
      Face  fa = itf->next();

      if(isOuterFace.get(fa.id))
        continue;

      cpt++;
      outv.add(fa.id, 1);

      if(!update_seqp.get(fa.id)) {
        seqP.set(fa.id,seqp(fa));
        v_face.push_back(fa);
        update_seqp.set(fa.id,true);
      }
    }

    delete itf;
    Face fa = Gp->getFaceContaining(n4,n3);

    if(!update_seqp.get(fa.id)) {
      seqP.set(fa.id,seqp(fa));
      v_face.push_back(fa);
      update_seqp.set(fa.id,true);
    }

    add_node++;
    oute.add(fa.id, 1);
    n4 = n3;
    n3 = right.get(n4.id);

    if(cpt != 1)
      one_face = false;
  }

  Face  fa = Gp->getFaceContaining(n4,n3);
  oute.add(fa.id, 1);

  if(!update_seqp.get(fa.id)) {
    seqP.set(fa.id,seqp(fa));
    v_face.push_back(fa);
    update_seqp.set(fa.id,true);
  }

  if(Gp->deg(n3) != 2 && Gp->deg(pred) != 2)
    one_face = false;

  if(add_node == 0) {
    Iterator<Face> * itf = Gp->getFacesAdj(n2);

    while(itf->hasNext()) {
      Face tmp_f = itf->next();

      if(isOuterFace.get(tmp_f.id))
        continue;

      if(!update_seqp.get(tmp_f.id)) {
        v_face.push_back(tmp_f);
        seqP.set(tmp_f.id,seqp(tmp_f));
        update_seqp.set(tmp_f.id,true);
      }
    }

    delete itf;
  }

  outv.add(ext.id, add_node);
  oute.add(ext.id, add_node+1);

  // Update of is_selectable_visited_face and is_selectable_face
  updateSelectableFaces(v_face);

  // update of is_selectable and is_selectable_visited
  updateNewSelectableNodes(pred,no_tmp2,ed_tmp,n2,v_face,one_face,was_visited,true);

  // update minMarkedFace
  if(min_markded_face)
    minMarkedf();
}

//==========================================================
/**
 * return true if node n can be selected
 */
bool Ordering::isSelectable(node n) {
  if(n == v1[0] || n == v1[v1.size()-1])
    return false;

  if(Gp->deg(n)>=3) {
    int sepf = 0, ispf = 0;
    Iterator<Face > * itf = Gp->getFacesAdj(n);
    node n_predCont, n_succCont;
    Face derniere = Gp->getFaceContaining(v1[0],v1[1]);

    n_predCont = left.get(n.id);
    n_succCont = right.get(n.id);

    while(itf->hasNext()) {
      Face  faces = itf->next();

      if(isOuterFace.get(faces.id))
        continue;

      if(markedFaces.get(faces.id)) {
        return false;
      }

      int out_vertex = outv.get(faces.id);
      int out_edge = oute.get(faces.id);

      if( out_vertex>=3 || (out_vertex==2 && out_edge==0)) {
        sepf++;
        edge e1= Gp->existEdge(n_predCont,n);

        if (!e1.isValid())
          e1 = Gp->existEdge(n,n_predCont);

        edge e2 = Gp->existEdge(n_succCont,n);

        if (!e2.isValid())
          e2 = Gp->existEdge(n,n_succCont);

        if((Gp->containNode(faces,n_succCont) && !(Gp->containEdge(faces,e2))) || (Gp->containNode(faces,n_predCont) && !(Gp->containEdge(faces,e1)))) {
          int seq = seqP.get((faces).id);

          if(faces == derniere) {
            out_vertex -= v1.size()-2;
            out_edge -= v1.size()-1;
          }

          if(out_vertex == seq+1 && seq-out_edge == 1)
            ispf++;
        }
      }
    }

    delete itf;
    return (ispf==sepf);
  }

  return false;
}

//==========================================================
Ordering::Ordering(PlanarConMap * G, PluginProgress* pluginProgress,
                   int minProgress, int deltaProgress, int maxProgress)
  :  Gp(computePlanarConMap(G->addCloneSubGraph())), oute(),
     outv(),visitedNodes(), visitedFaces(), seqP(), isOuterFace(),
     contour(), is_selectable(), is_selectable_visited(),
     is_selectable_face(), is_selectable_visited_face(),
     left(), right() {
  assert(BiconnectedTest::isBiconnected(G));
  assert(BiconnectedTest::isBiconnected(Gp));
  init();
  assert(v1.size()>1);

  unsigned int nbTours = Gp->numberOfNodes();
  Face  derniere = Gp->getFaceContaining(v1[0],v1[1]);
  node n;

  int nbMax = nbTours;

  while((Gp->nbFaces() > 2) && (nbTours > 1)) {
    nbTours--;

    // give some feedback
    if (pluginProgress &&
        (pluginProgress->progress(minProgress +
                                  (deltaProgress * (nbMax - nbTours))/ nbMax,
                                  maxProgress) != TLP_CONTINUE))
      return;

    // search for a visited node that can be selected
    Iterator<unsigned int> * itn = is_selectable_visited.findAll(true);

    while(itn->hasNext()) {
      n = static_cast<node>(itn->next());
      selectAndUpdate(n);

      if(Gp->nbFaces()<=2)
        break;

      delete itn;
      itn = is_selectable_visited.findAll(true);
    }

    delete itn;

    if(Gp->nbFaces()<=2)
      break;

    // search for a visited face that can be selected
    bool selVisitedFace = false;
    Iterator<unsigned int> * itf = is_selectable_visited_face.findAll(true);

    while(itf->hasNext()) {
      Face f = static_cast<Face>(itf->next());
      selectAndUpdate(f);
      selVisitedFace=true;
      break;
    }

    delete itf;

    // search for a non-visited and selectable node, occur when no visited node or face has beeen selected
    bool selNode = false;

    if(!selVisitedFace) {
      Iterator<unsigned int> * itn2 = is_selectable.findAll(true);

      while(itn2->hasNext()) {
        n = static_cast<node>(itn2->next());
        selNode=true;
        selectAndUpdate(n);
        break;
      }

      delete itn2;
    }

    // search for a non-visited selectable face, occur when no node or visited face has been selected
    bool selFace = false;

    if(!selNode && !selVisitedFace) {
      Iterator<unsigned int> * itf = is_selectable_face.findAll(true);

      while(itf->hasNext()) {
        Face  f = static_cast<Face>(itf->next());
        selectAndUpdate(f);
        selFace=true;
        break;
      }

      delete itf;
    }

    // search for a face to augment, occur when no node or face has been selected
    if(!selNode && !selVisitedFace && !selFace) {
      Iterator<Face > * itf = Gp->getFaces();

      while(itf->hasNext()) {
        Face fac = itf->next();

        if(isOuterFace.get((fac).id))
          continue;

        int s = seqP.get((fac).id);
        int ov = outv.get(((fac).id));
        int oe = oute.get(((fac).id));
        int nbNewFace;
        bool pair;

        if(fac == derniere) {
          ov -= v1.size()-2;
          oe -= v1.size()-1;
        }

        if((s-oe)%2) {
          pair = false;
          nbNewFace = (s-oe)/2 + 1;
        }
        else {
          pair = true;
          nbNewFace = (s-oe)/2;
        }

        if( ov == s+1 && s-oe >= 2 ) {
          struct augmentableAndNodes_ res = getAugAndNodes(fac) ;

          if( !existMarkedF || res.augmentable) {
            augment(fac,res.Nodes[0],res.Nodes[1],res.Nodes[2],res.Nodes[3], nbNewFace,pair);
            break;
          }
        }
      }

      delete itf;
    }
  }

  if(nbTours == 1) {
    tlp::error() << "Error : non initialized partition" << std::endl;
    exit(-1);
  }

  vector<node> nodes;
  n = v1[v1.size()-1];
  node n2;
  n2 = right.get(n.id);

  while(n2 != v1[0]) {
    nodes.push_back(n2);
    n = n2;
    n2 = right.get(n.id);
  }

  _data.push_back(nodes);
  vector<node> res;

  for(unsigned int k = v1.size() ; k>=1 ; --k)
    res.push_back(v1[k-1]);

  _data.push_back(res);

}


//==========================================================
/**
 * initialize v1
 */
void Ordering::init_v1( vector<node> fn) {
  int l = fn.size();
  int i = 0;
  int cpt2 = 0;

  while(Gp->deg(fn[i]) == 2 && cpt2 < l) {
    i = (l+i+1)%l;
    cpt2++;
  }

  if(cpt2 == l)
    for(int i = l/2 ; i > 0 ; --i)
      v1.push_back(fn[i]);
  else {
    cpt2 = 1;
    v1.push_back(fn[i]);
    i = (l+i-1)%l;

    while(Gp->deg(fn[i]) == 2) {
      v1.push_back(fn[i]);
      cpt2++;
      i = (l+i-1)%l;
    }

    if (v1.size() == 1 || !Gp->hasEdge(v1[0],fn[i], false)) {
      v1.push_back(fn[i]);
      cpt2++;
    }

    vector<node> tmp;

    while(cpt2 < l) {
      tmp = getPathFrom(fn,i);

      if( tmp.size() > v1.size())
        v1 = tmp;

      cpt2 += tmp.size()-1;
      i = (l+i-tmp.size()+1)%l;
      tmp.clear();
    }
  }
}

//==========================================================
/**
 * initialize ext, isOuterFace
 */
void Ordering::init_outerface() {
  Face max;
  unsigned int max_size = 0;
  Iterator<Face>* itf = Gp->getFaces();

  while(itf->hasNext()) {
    Face tmp = itf->next();

    if(Gp->nbFacesNodes(tmp)>max_size) {
      max_size = Gp->nbFacesNodes(tmp);
      ext = tmp;
    }
  }

  delete itf;
  isOuterFace.setAll(false);
  isOuterFace.set(ext.id, true);
}

//==========================================================
/**
 * initialize seqP
 */
void Ordering::init_seqP() {
  seqP.setAll(0);
  Iterator<Face > * itf = Gp->getFaces();

  while(itf->hasNext()) {
    Face fa = itf->next();

    if(isOuterFace.get(fa.id))
      continue;

    seqP.set(fa.id,seqp(fa));
  }

  delete itf;
}

//==========================================================
/**
 * initialize outv and oute
 */
void Ordering::init_outv_oute() {
  oute.setAll(0);
  outv.setAll(0);
  int cpt = 0;
  node no_first, no_pred, n;
  Iterator<node> * it_node2 = Gp->getFaceNodes(ext);

  if(it_node2->hasNext()) {
    no_first = it_node2->next();
    Iterator<Face > * it_face = Gp->getFacesAdj(no_first);

    while(it_face->hasNext()) {
      Face f = it_face->next();
      outv.add(f.id, 1);
    }

    delete it_face;
  }

  no_pred = no_first;

  while(it_node2->hasNext()) {
    if(cpt != 0)
      no_pred = n;

    cpt++;
    n = it_node2->next();
    Iterator<Face > * it_face2 = Gp->getFacesAdj(n);

    while(it_face2->hasNext()) {
      Face f = it_face2->next();
      outv.add(f.id, 1);
    }

    delete it_face2;
    Face face = Gp->getFaceContaining(n,no_pred);
    oute.add(face.id, 1);
  }

  delete it_node2;
  Face face = Gp->getFaceContaining(no_first,n);
  oute.add(face.id, 1);
  outv.set(ext.id,cpt+1);
  oute.set(ext.id,cpt+1);
}

//==========================================================
/**
 * initialize is_selectable and is_selectable_visited
 */
void Ordering::init_selectableNodes() {
  is_selectable_visited.setAll(false);
  is_selectable.setAll(false);

  Iterator<node> * it_nodes = Gp->getFaceNodes(ext);

  while(it_nodes->hasNext()) {
    node no_tmp = it_nodes->next();

    if(Gp->deg(no_tmp) < 3 || no_tmp == v1[0] || no_tmp == v1[v1.size()-1])
      continue;

    if(isSelectable(no_tmp))
      is_selectable.set(no_tmp.id,true);
  }

  delete it_nodes;
}

//==========================================================
/**
 * initialize is_selectable_face and is_selectable_visited_face
 */
void Ordering::init_selectableFaces() {
  is_selectable_visited_face.setAll(false);
  is_selectable_face.setAll(false);

  Iterator<Face> *it_f = Gp->getFaces();
  Face derniere = Gp->getFaceContaining(v1[0],v1[1]);

  while(it_f->hasNext()) {
    Face f = it_f->next();

    if(f == derniere || isOuterFace.get(f.id))
      continue;

    if(outv.get(f.id) < 3)
      continue;

    if(outv.get(f.id) == oute.get(f.id) + 1)
      is_selectable_face.set(f.id, true);
  }

  delete it_f;
}

//==========================================================
/**
 *initialize all attributes
 */
void Ordering::init() {

  init_outerface();

  // initialize contour, left, right
  contour.setAll(false);
  Iterator<node> * itn = Gp->getFaceNodes(ext);
  vector<node> fn;
  node no, pred, first;
  int cpt = 0;

  if(itn->hasNext()) {
    first = itn->next();
    contour.set(first.id,true);
    fn.push_back(first);
    pred = first;
  }

  while(itn->hasNext()) {
    if(cpt != 0)
      pred = no;
    else
      cpt = 1;

    no = itn->next();
    contour.set(no.id,true);
    fn.push_back(no);
    left.set(pred.id,no);
    right.set(no.id,pred);
  }

  delete itn;
  right.set(first.id,no);
  left.set(no.id,first);

  //  Initialize markedFaces and visitedFaces and visitedNodes
  markedFaces.setAll(false);
  visitedFaces.setAll(false);
  visitedNodes.setAll(false);

  init_v1(fn);
  init_seqP();
  init_outv_oute();
  init_selectableNodes();
  init_selectableFaces();

  //Initialize minMarkedFace
  existMarkedF = false;
  minMarkedFace.face = Face();
  minMarkedFace.n_first = v1[v1.size()-1];
  minMarkedFace.n_last = v1[0];
}

//==========================================================
Ordering::~Ordering() {
  delete Gp;
}
