#include <tulip/Bfs.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

Bfs::Bfs(Graph * G, BooleanProperty * resultatAlgoSelection): graph(tlp::newCloneSubGraph(G)),selectedNodes(), selectedEdges() {
  selectedNodes.setAll(false);
  selectedEdges.setAll(false);
  nbNodes = 0;

  node root;;
  bool unselected = false;
  Iterator<node> * itn = resultatAlgoSelection->getNodesEqualTo(true);
  if(itn->hasNext()){
    root = itn->next();
    if(!G->isElement(root))
      unselected = true;
  }
  else
    unselected = true;
  delete itn;

  s_proxy = G->getProperty<BooleanProperty>("viewSelection");
  s_proxy->setAllNodeValue(false);
  s_proxy->setAllEdgeValue(false);

  //resultatAlgoSelection->setAllNodeValue(false);
  s_proxy->setNodeValue(root,true);
  
  resultatAlgoSelection->setNodeValue(root, true);
  selectedNodes.set(root.id, true);
  //  graph->addNode(root);
  ++nbNodes;
  computeBfs(G,resultatAlgoSelection,root);
}

void Bfs::computeBfs(Graph * G,BooleanProperty * resultatAlgoSelection, node root){
  unsigned int taille = G->numberOfNodes();
  unsigned int nb_n = 1;
  unsigned int i = 0;

  vector<node> next_roots;
  next_roots.push_back(root);
  while(taille != nbNodes) {
    node r = next_roots[i];
    nb_n++;
    if(!G->isElement(r))
      cerr << "ERROR NODE R NOT IN G"<< endl;
    Iterator<edge> * ite = G->getInOutEdges(r);
    while(ite->hasNext()){
      edge e = ite->next();
      if(!selectedEdges.get(e.id)) {
	node tmp = G->opposite(e,r);
	if(!selectedNodes.get(tmp.id)) {
	  selectedNodes.set(tmp.id,true);
	  selectedEdges.set(e.id, true);
	  next_roots.push_back(tmp);
	  //  graph->addNode(tmp);
	  //graph->addEdge(e);
	  nbNodes++;
	  s_proxy->setNodeValue(tmp,true);
	  s_proxy->setEdgeValue(e,true);
	  resultatAlgoSelection->setNodeValue(tmp, true);
	  resultatAlgoSelection->setEdgeValue(e, true);
	}
      }
    } delete ite;
    i++;
  }
}

Bfs::~Bfs(){}
