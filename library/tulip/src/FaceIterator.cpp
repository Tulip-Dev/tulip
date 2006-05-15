#include <tulip/FaceIterator.h>

#ifdef __GNUC__
#if __GNUC__ < 3
#include <hash_map.h>
namespace Sgi { using ::hash_map; }; // inherit globals
#else
#include <ext/hash_map>
namespace Sgi = ::__gnu_cxx;
#endif
#else // ...  there are other compilers, right?
namespace Sgi = std;
using namespace tlp;
#endif

using namespace Sgi;
using namespace std;
using namespace tlp;

//============================================================
//Iterator for Face : FaceIterator
//============================================================
FaceIterator::FaceIterator(SuperGraphMap* m){
  assert(m);
  msg = m;   i = 0;
  m->update();
}

//============================================================
Face FaceIterator::next(){
  Face tmp = msg->faces[i++];
  return tmp;
}

//============================================================
bool FaceIterator::hasNext(){
  if(i == msg->faces.size())
    return false;
  else return true;
}


//============================================================
//Iterator for Face : FaceAdjIterator
//============================================================
FaceAdjIterator::FaceAdjIterator(SuperGraphMap* m, const node n){
  assert(m->isElement(n));
  m->update();
  i = 0;
  facesAdj.erase(facesAdj.begin(),facesAdj.end());
  edge e;
  Face f_tmp, f_tmp2;
  Iterator<edge> * ite = m->getInOutEdges(n);
  if(ite->hasNext()){
    e = ite->next();
    f_tmp = (m->edgesFaces[e])[0];
    f_tmp2 = (m->edgesFaces[e])[1];
  }
  if(ite->hasNext()){
    e = ite->next();
    if( f_tmp == (m->edgesFaces[e])[0]){
      facesAdj.push_back(f_tmp);
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back(f_tmp);
    }
    else if(f_tmp == (m->edgesFaces[e])[1]){
      facesAdj.push_back(f_tmp);
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back(f_tmp);
    }
    else if(f_tmp2 == (m->edgesFaces[e])[0]){
      facesAdj.push_back(f_tmp2);
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back(f_tmp);
    }
    else {
      facesAdj.push_back(f_tmp2);
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back(f_tmp);
    }
    
  }
  while(ite->hasNext()){
    e = ite->next();
    if(f_tmp == (m->edgesFaces[e])[0]){
      f_tmp = (m->edgesFaces[e])[1];
      facesAdj.push_back((m->edgesFaces[e])[1]);
    }
    else {
      f_tmp = (m->edgesFaces[e])[0];
      facesAdj.push_back((m->edgesFaces[e])[0]);
    }
  } delete ite;
}

//============================================================
Face FaceAdjIterator::next(){
  Face tmp = facesAdj[i]; ++i;
  return tmp;
}

//============================================================
bool FaceAdjIterator::hasNext(){
  return (i != facesAdj.size());
}


//============================================================
// NodeFaceIterator
//============================================================
NodeFaceIterator::NodeFaceIterator(SuperGraphMap* m, const Face face){
  //assert(face && m);
  i=0;

  vector<edge> e = m->facesEdges[face];
  edge e1 = e[0];
  edge e2 = e[1];
  node prev;
  if( m->source(e1) == m->source(e2) || m->source(e1) == m->target(e2))
    prev = m->source(e1);
  else 
    prev = m->target(e1);
  nodes.push_back(prev);
  for(unsigned int j = 1; j < m->facesEdges[face].size(); ++j){
    e1 = m->facesEdges[face][j];
    if( m->source(e1) == prev ){
      prev = m->target(e1);
      nodes.push_back(prev);
    }
    else {
      prev = m->source(e1);
      nodes.push_back(prev);
    }
  }

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
EdgeFaceIterator::EdgeFaceIterator(SuperGraphMap * m,const Face f){
  //assert(f);
  i = 0;
  ve = m->facesEdges[f];
}

//============================================================
edge EdgeFaceIterator::next(){
  edge tmp = ve[i];
  i++;
  return tmp;
}

//============================================================
bool EdgeFaceIterator::hasNext(){
  return (i != ve.size());
}
