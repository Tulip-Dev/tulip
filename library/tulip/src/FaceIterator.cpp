#include <tulip/FaceIterator.h>

using namespace std;

//============================================================
//Iterator for Face : FaceIterator
//============================================================
FaceIterator::FaceIterator(SuperGraphMap* m){
  assert(m);
  mgraph = m;   i = 0;
  m->update();
}

//============================================================
Face* FaceIterator::next(){
  Face* tmp = mgraph->faces[i++];
  return tmp;
}

//============================================================
bool FaceIterator::hasNext(){
  if(i == mgraph->faces.size())
    return false;
  else return true;
}


//============================================================
//Iterator for Face : FaceAdjIterator
//============================================================
FaceAdjIterator::FaceAdjIterator(SuperGraphMap* m, const node n){
  assert(m->isElement(n));
  m->update();

  lfaces.clear();
  Iterator<Face*>* ii = m->getFaces();
  
  while(ii->hasNext()){
    Face* f = ii->next();
    Iterator<node>* i = m->getFaceNodes(f);
    while(i->hasNext()){
      node v = i->next();
      if(n == v){
	lfaces.push_back(f);
	break;
      }
    }
    delete i;
  }
  delete ii;

  it = lfaces.begin();
}

//============================================================
Face* FaceAdjIterator::next(){
  Face* tmp = (*it); it++;
  return tmp;
}

//============================================================
bool FaceAdjIterator::hasNext(){
  if(it == lfaces.end())
    return false;
  else return true;
}


//============================================================
// NodeFaceIterator
//============================================================
NodeFaceIterator::NodeFaceIterator(const SuperGraphMap* m, const Face* face){
  assert(face && m);
  i=0;
  edge first = face->getFirstEdge(), last = face->getLastEdge();
  node n, nlast;

  edge tmp = m->succCycleEdge(last, m->source(last));
  if(tmp == first){
    n = m->source(last);
    nlast = m->target(last);
  }
  else{
    n = m->target(last);
    nlast = m->source(last);
  }
  nodes.push_back(n);

  node v = (n == m->source(first))? m->target(first): m->source(first);

  for(unsigned int i=2; i<face->nbEdges(); i++){
    node ntmp = m->succCycleNode(v, n);
    nodes.push_back(v);
    n = v;
    v = ntmp;
  }
  nodes.push_back(nlast);
}

//============================================================
node NodeFaceIterator::next(){
  node n = nodes[i];
  i++;
  return n;
}

//============================================================
bool NodeFaceIterator::hasNext(){
  return (i != nodes.size());
}


//============================================================
// EdgeFaceIterator
//============================================================
EdgeFaceIterator::EdgeFaceIterator(const Face* face){
  assert(face);
  i = 0;
  f = face;
}

//============================================================
edge EdgeFaceIterator::next(){
  edge tmp = (*f)[i];
  i++;
  return tmp;
}

//============================================================
bool EdgeFaceIterator::hasNext(){
  return (i != f->size());
}
