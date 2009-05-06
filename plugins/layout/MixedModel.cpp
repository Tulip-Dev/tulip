#include <tulip/TulipPlugin.h>

#include <tulip/MapIterator.h>
#include <tulip/PlanarConMap.h>
#include <tulip/Ordering.h>
#include <tulip/MutableContainer.h>
#include <tulip/Bfs.h>
#include <tulip/GraphTools.h>

#include "MixedModel.h"
#include "DatasetTools.h"

LAYOUTPLUGINOFGROUP(MixedModel,"Mixed Model","Romain BOURQUI ","09/11/2005","Ok","1.0","Planar");

using namespace std;
using namespace tlp;

float spacing = 2;
float edgeNodeSpacing = 2;

//===============================================================
namespace {
  const char * paramHelp[] = {
    //Orientation
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "vertical" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the orientation of the drawing"	\
    HTML_HELP_CLOSE(),
    // y node-node spacing
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "2" ) \
    HTML_HELP_BODY() \
    "This parameter defines the minimum y-spacing between any two nodes." \
    HTML_HELP_CLOSE(),
    // x node-node and edge-node spacing
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "2" ) \
    HTML_HELP_BODY() \
    "This parameter defines the minimum x-spacing between any two nodes or between a node and an edge." \
    HTML_HELP_CLOSE()
  };
}
#define ORIENTATION "vertical;horizontal;"
//====================================================
MixedModel::MixedModel(const PropertyContext &context):LayoutAlgorithm(context)  {
  addNodeSizePropertyParameter(this);
  addParameter<StringCollection> ("orientation", paramHelp[0], ORIENTATION );
  addParameter<float> ("y node-node spacing",paramHelp[1],"2");
  addParameter<float> ("x node-node and edge-node spacing",paramHelp[2],"2");
  addDependency<LayoutAlgorithm>("Connected Component Packing", "1.0");
}
//====================================================
MixedModel::~MixedModel(){
}
//====================================================
bool MixedModel::run() {
  size = graph->getProperty<SizeProperty>("viewSize");
  string orientation = "vertical";
  if (dataSet!=0) {
    getNodeSizePropertyParameter(dataSet, size);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
    dataSet->get("y node-node spacing",spacing);
    dataSet->get("x node-node and edge-node spacing",edgeNodeSpacing);
  }
  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      const Size& tmp = size->getNodeValue(n);
      size->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }
  //===========================================================
  IntegerProperty * intProxy = graph->getProperty<IntegerProperty>("viewShape");
  intProxy->setAllEdgeValue(0);
  
  // give some empirical feedback of what we are doing 1 %
  pluginProgress->progress(1, 1000);
  
  Pere = tlp::newCloneSubGraph(graph, "Father");
  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(Pere, components);
  for (unsigned int i = 0; i < components.size(); ++i) {
    tlp::inducedSubGraph(Pere, components[i]);
  }

  vector<edge> edge_planar;

  
  int nbConnectedComponent = 0;
  Iterator<Graph *> *it = Pere->getSubGraphs();
  while(it->hasNext()) {
    nbConnectedComponent++;
    currentGraph = it->next();
    //====================================================
    //don't compute the canonical ordering if the nmber of nodes is less than 3
    /* cout << currentGraph->numberOfNodes() << " nodes and " << currentGraph->numberOfEdges() << " edges " << endl;
    cout << "Create map" << endl; */
    if(currentGraph->numberOfNodes() == 1){
      node n = currentGraph->getOneNode();
      layoutResult->setNodeValue(n, Coord(0,0,0));
      continue;
    }
    else if(currentGraph->numberOfNodes() == 2 || currentGraph->numberOfNodes() == 3){
      Iterator<node> * itn = currentGraph->getNodes();
      node n = itn->next();
      const Coord& c = currentGraph->getProperty<SizeProperty>("viewSize")->getNodeValue(n);
      layoutResult->setNodeValue(n, Coord(0,0,0));
      node n2 = itn->next();
      Coord c2 = currentGraph->getProperty<SizeProperty>("viewSize")->getNodeValue(n2);
      layoutResult->setNodeValue(n2, Coord(spacing + c.getX()/2+c2.getX()/2,0,0));
      if(currentGraph->numberOfNodes() == 3){
	node n3 = itn->next();
	const Coord& c3 = currentGraph->getProperty<SizeProperty>("viewSize")->getNodeValue(n2);
	layoutResult->setNodeValue(n3, Coord(2. * spacing + c.getX()/2 + c2.getX()+c3.getX()/2,0,0));
	edge e = currentGraph->existEdge(n,n3).isValid() ? currentGraph->existEdge(n,n3) :currentGraph->existEdge(n3,n);
	if(e.isValid()){
	  vector<Coord> bends(2);
	  bends.clear();
	  unsigned int max = (unsigned int) (c[1]>c2[1]? c[1]/2 : c2[1]/2);
	  max += (unsigned int) edgeNodeSpacing;
	  if(currentGraph->source(e) == n){
	    bends.push_back(Coord(0,max,0));
	    bends.push_back(Coord(2. * spacing + c.getX()/2 + c2.getX()+c3.getX()/2,max,0));
	  }
	  else {
	    bends.push_back(Coord(2. * spacing + c.getX()/2 + c2.getX()+c3.getX()/2,max,0));
	    bends.push_back(Coord(0,max,0));
	  }
	  layoutResult->setEdgeValue(e, bends);
	}
      
      }
      delete itn;      
      edge e;
      forEach(e,currentGraph->getEdges())
	edge_planar.push_back(e);
      continue;
    }
    
    //====================================================
    planar = PlanarityTest::isPlanar(currentGraph);      
    Graph * G;
    if(!planar){
      // cout << "Graph is not planar ...";
      BooleanProperty * resultatAlgoSelection = currentGraph->getProperty<BooleanProperty>("viewSelection");
      Bfs::Bfs sp(currentGraph,resultatAlgoSelection);
      currentGraph->delSubGraph(sp.graph);
      G = tlp::newSubGraph(currentGraph);
      Iterator<edge> * ite = currentGraph->getEdges();
      while(ite->hasNext()){
	edge e_tmp = ite->next();
	if(resultatAlgoSelection->getEdgeValue(e_tmp)){
	  G->addNode(currentGraph->source(e_tmp));
	  G->addNode(currentGraph->target(e_tmp));
	  G->addEdge(e_tmp);
	  edge_planar.push_back(e_tmp);
	}
	else
	  unplanar_edges.push_back(e_tmp);
      } delete ite;
      //===================================================
      
      graphMap = new PlanarConMap(G);
      vector<edge> re_added = getPlanarSubGraph(graphMap,unplanar_edges);
      
      for (unsigned int ui = 0; ui < re_added.size() ; ++ui){
	edge e = re_added[ui];
	G->addEdge(e);
	resultatAlgoSelection->setEdgeValue(e,true);
	edge_planar.push_back(e);
	vector<edge>::iterator ite = find(unplanar_edges.begin(),unplanar_edges.end(),e);
	unplanar_edges.erase(ite);
      }
      delete graphMap;
      resultatAlgoSelection->setAllEdgeValue(false);
      resultatAlgoSelection->setAllNodeValue(false);
      // cout << "... Planar subGraph computed" << endl;
    }
    else {
      G = tlp::newCloneSubGraph(currentGraph);
      edge e;
      forEach(e,currentGraph->getEdges())
	edge_planar.push_back(e);
    }
    //===============================================
    
    // give some empirical feedback of what we are doing 2%
    pluginProgress->progress(5, 1000);
    vector<edge> added_edges;
    if(!BiconnectedTest::isBiconnected(G))    
      BiconnectedTest::makeBiconnected(G,added_edges);
    assert(BiconnectedTest::isBiconnected(G));
    carte = new PlanarConMap(G);
    assert(BiconnectedTest::isBiconnected(G));
    assert(BiconnectedTest::isBiconnected(carte));

    // give some empirical feedback of what we are doing 2%
    pluginProgress->progress(1, 100);

    // give some empirical feedback (5%)
    pluginProgress->progress(2, 100); 
    // cout << "Make the map planar ...";
    // cout << "... end" << endl;
    // give some empirical feedback of what we are doing (10%)
    if (pluginProgress->progress(5, 100) !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;
    
    InPoints.clear(); OutPoints.clear();
    EdgesIN.clear();  EdgesOUT.clear();
    outl.clear();     outr.clear();
    inl.clear();      inr.clear();
    
    rank.clear();

    V.clear();
    
    NodeCoords.clear();
    // Cout << "Partition initialization ...";
    initPartition() ;
    // cout<<"... Partition initialized"<<endl;
    if (pluginProgress->state() == TLP_CANCEL)
      return false;
    // cout << "InOutPoint computation ..."  ;
    assignInOutPoints();
    // cout<<"... InOutPoints computed"<<endl;

    // cout << "Coordinate computation ..."  ;
    computeCoords();
    // cout<<"... Coordinate computed"<<endl;
    // cout << "Drawing edges and nodes ..." ;
    placeNodesEdges();
    // cout << "... Edges and Nodes drawn" <<endl;
    
    vector<edge>::const_iterator ite = dummy.begin();
    for (; ite!=dummy.end(); ++ite)
      currentGraph->delAllEdge(*ite);
    
    ite = added_edges.begin();
    for (; ite!=added_edges.end(); ++ite)
      currentGraph->delAllEdge(*ite);
    
      delete carte;
      currentGraph->delAllSubGraphs(G);
      
  } delete it;
  if(nbConnectedComponent != 1){
    string err ="";
    LayoutProperty layout(graph);
    DataSet tmp;
    tmp.set("coordinates", layoutResult);
    graph->computeProperty<LayoutProperty *>(string("Connected Component Packing"),&layout,err,NULL,&tmp);
    Iterator<node> *itN = graph->getNodes();
    while(itN->hasNext()){
      node n = itN->next();
      layoutResult->setNodeValue(n, layout.getNodeValue(n));
    } delete itN;
    Iterator<edge> *itE = graph->getEdges();
    while(itE->hasNext()){
      edge e = itE->next();
      layoutResult->setEdgeValue(e, layout.getEdgeValue(e));
    } delete itE;
  }
  graph->delAllSubGraphs(Pere);


  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      const Size& tmp = size->getNodeValue(n);
      size->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      const Coord& tmpC = layoutResult->getNodeValue(n);
      layoutResult->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
    edge e;
    forEach(e, graph->getEdges()) {
      LineType::RealType tmp = layoutResult->getEdgeValue(e);
      LineType::RealType tmp2;
      LineType::RealType::iterator it;
      for (it = tmp.begin(); it!= tmp.end(); ++it) {
	tmp2.push_back(Coord(-(*it)[1], (*it)[0], (*it)[2]));
      }
      layoutResult->setEdgeValue(e, tmp2);
    }
  }

  dataSet->set("planar_edges",edge_planar);

  // if in tulip gui, keep node size updates
  // the test below indicates if we are invoked from the tulip gui
  // cf MainController.cpp & GlGraphInputData.cpp
  LayoutProperty* elementLayout;
  if (graph->getAttribute("viewLayout", elementLayout)) {
    graph->removeAttribute("viewLayout");
    graph->push();
  }

  return true;
}


//====================================================
bool MixedModel::check(string &err) {  
  bool result = true ;
  err = "The graph must be ";
  if(!SimpleTest::isSimple(graph)){
    err += "simple and without self-loop ";
    result = false;
  }
  return result;
}

      
//====================================================
vector<edge> MixedModel::getPlanarSubGraph(PlanarConMap *sg, vector<edge> unplanar_edges){
  vector<edge> res;
  for(unsigned int ui = 0; ui < unplanar_edges.size() ; ++ui){
    edge e = unplanar_edges[ui];
    node n = sg->source(e); 
    node n2 =  sg->target(e);
    Face f = sg->sameFace(n, n2);
    if( f != Face()){
      sg->splitFace(f,e);
      res.push_back(e);
    }
  } 
  return res;
}


//====================================================
void MixedModel::placeNodesEdges(){
  float maxX = 0, maxY = 0;

  Iterator<node> *it = carte->getNodes();
  while(it->hasNext()){
    node n = it->next();
    Coord c = nodeSize.get(n.id);
    c[0] -= edgeNodeSpacing;
    graph->getProperty<SizeProperty>("viewSize")->setNodeValue(n,Size(c[0],c[1],0.3));
    layoutResult->setNodeValue(n, NodeCoords[n]);
  }
  delete it;
  
  Iterator<edge> *ite = carte->getEdges();
  
  while(ite->hasNext()){
    edge e = ite->next();
    Coord cs, ct, c;
    
    unsigned int rs = rank[carte->source(e)], rt = rank[carte->target(e)];
    if(rs != rt){
      vector<Coord> bends;
      
      if(rs>rt){
	cs = InPoints[e][0] + NodeCoords[carte->source(e)]; 
	ct = OutPoints[e] + NodeCoords[carte->target(e)];
	c = Coord(ct.getX(),cs.getY(), 0);
      }
      else{
	ct = InPoints[e][0] + NodeCoords[carte->target(e)]; 
	cs = OutPoints[e] + NodeCoords[carte->source(e)];
	c = Coord(cs.getX(),ct.getY(), 0);
      }
      if(ct.getX() >= maxX)
	maxX = ct.getX();
      if(cs.getX() >= maxX)
	maxX = cs.getX();
      if(ct.getY() >= maxY)
	maxY = ct.getY();
      if(cs.getY() >= maxY)
	maxY = cs.getY();
      
      if((cs != NodeCoords[carte->source(e)]) && (cs != ct))
	bends.push_back(cs);
      if((c != cs) && (c != ct)) bends.push_back(c);
      if((ct != NodeCoords[carte->target(e)]) && (ct != cs)) 
	bends.push_back(ct);
       
		    

      if(bends.size() != 0)
	layoutResult->setEdgeValue(e, bends);
    }
    // rs == rt, même partition donc pas de points intermédiaire à calculer
    // en cas de post-processing, il faudra pe y changer
  } delete ite;

  if(!planar){
    double z_size = (maxX+maxY)/3;//sqrt(maxX + maxY);
    maxX /= 8;
    maxY /= 8;
    for(unsigned int ui = 0; ui < unplanar_edges.size(); ++ui){
      edge e = unplanar_edges[ui];
      node n = currentGraph->source(e);
      node v = currentGraph->target(e);
      Coord c_n(-maxX+(NodeCoords[n].getX()+NodeCoords[v].getX())/2, -maxY+(NodeCoords[n].getY()+NodeCoords[v].getY())/2, -z_size);
      vector<Coord> bends;
      bends.push_back(c_n);
      layoutResult->setEdgeValue(e, bends);
      graph->getProperty<IntegerProperty>("viewShape")->setEdgeValue(e,4);  
      graph->getProperty<ColorProperty>("viewColor")->setEdgeValue(e,Color(218,218,218));    
    }
  }  
}

//====================================================
void MixedModel::initPartition(){
  V = tlp::computeCanonicalOrdering(carte, &dummy, pluginProgress);
  
  if (pluginProgress->state() == TLP_CANCEL)
    return;

  for(unsigned int i = 0; i < V.size();++i)
    for(unsigned int j = 0; j < V[i].size();++j)
      rank[V[i][j]] = i;
}

//====================================================
void afficheCycle(PlanarConMap* m){
  assert(m);
  cout<<"Cycles :"<<endl;
  Iterator<node>* itn = m->getNodes();
  
  node n;
  
  while(itn->hasNext()){
    n = itn->next();
    cout<<n.id<<" ( ";
    
    edge e; Iterator<edge>* ite = m->getInOutEdges(n);
    while(ite->hasNext()){
      e = ite->next();
      cout<<e.id<<" ";
    }  
    cout<<" )"<<endl;
    delete ite;
  }
  delete itn;
}

//====================================================
void MixedModel::assignInOutPoints(){  // on considère qu'il n'y a pas d'arc double
  assert(carte);
  vector<node> C;  // chemin courant

  // empirical feedback (95% -> 99%)
  int minProgress = 950, deltaProgress = 40;

  for(unsigned int k = 0; k < V.size(); k++){
    // give pluginProgress feedback
    if (pluginProgress->progress(minProgress +
				 (deltaProgress * k)/ V.size(),
				 1000) != TLP_CONTINUE)
      return;

    unsigned int p = V[k].size();
    
    vector<node>::iterator il, ir;
    node nl = node(), nr = node();
    if(k!=0){
      // ordonné les arcs "in"
      unsigned int i;
      for(i = 0; !nl.isValid() && (i<C.size()-1); i++){
	if(existEdge(C[i], V[k][0]).isValid()){ 
	  nl = C[i];
	}
      }
      
      assert(nl.isValid());

      for(i = C.size()-1; !nr.isValid() && (i>0); i--){
	if(existEdge(C[i], V[k][p-1]).isValid()){ 
	  nr = C[i];
	}
      }
      assert(nr.isValid());

      il = find(C.begin(), C.end(), nl);    assert(il != C.end());
      ir = find(il, C.end(), nr);           assert(ir != C.end());
    }
    
    for(unsigned int i = 0; i < p; i++){
      node v = V[k][i];
      unsigned nbIn = 0, nbOut = 0;
      vector<edge> listOfEdgesIN;
      vector<edge> listOfEdgesOUT;
      
      listOfEdgesIN.clear();      listOfEdgesOUT.clear();
      
      
      // build in-edge vector and out-edge vector in the good order
      Iterator<edge> *it = carte->getInOutEdges(v);
      vector<edge> tmp;
      char pred = 'p';
      
      tmp.clear();
      while(it->hasNext()){
	edge e = it->next();
	node n = (carte->source(e) == v)? carte->target(e):carte->source(e);
	
       	bool trouve = false;
	unsigned int r = rank[n];


	if(r < k) {
	  nbIn++; trouve = true;
	  if(pred == 'o'){
	    pred = 'i';
	    listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
	    tmp.clear();
	  }
	  else if( pred == 'p') pred = 'i';
	  tmp.push_back(e);
	}
	
	if(r > k && !trouve) {
	  nbOut++; trouve = true;
	  if(pred == 'i'){
	    pred = 'o';
	    listOfEdgesIN.insert(listOfEdgesIN.begin(), tmp.begin(), tmp.end());
	    tmp.clear();
	  }
	  else if( pred == 'p') pred = 'o';
	  tmp.push_back(e);
	}
	
	if((i != 0)&&(!trouve)){ // test le voisin de gauche
	  if(V[k][i-1] == n){
	    nbIn++; trouve = true;
	    if(pred == 'o'){
	      pred = 'i';
	      listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
	      tmp.clear();
	    }
	    else if( pred == 'p') pred = 'i';
	    tmp.push_back(e);
	  }
	}
	if( (i != V[k].size()-1) && !trouve){ // test le voisin de droite
	  if(V[k][i+1] == n){
	    nbIn++;
	    if(pred == 'o'){
	      pred = 'i';
	      listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
	      tmp.clear();
	    }
	    else if( pred == 'p') pred = 'i';
	    tmp.push_back(e);
	  }
	}
      }
      delete it;
      
      if(pred == 'o') listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
      else if( pred == 'i') listOfEdgesIN.insert(listOfEdgesIN.begin(), tmp.begin(), tmp.end());
      
      if(k!=0){
	if(i==0){
	  edge e = existEdge(nl, v);
	  vector<edge>::iterator eil = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
	  assert(eil != listOfEdgesIN.end());
	  
	  if(!((*listOfEdgesIN.begin()) == e)){
	    vector<edge> tmp;
	    tmp.insert(tmp.begin(), eil, listOfEdgesIN.end());
	    tmp.insert(tmp.end(), listOfEdgesIN.begin(), eil);
	    listOfEdgesIN = tmp;
	  }
	}
	else if(i == p-1){
	  edge e = existEdge(nr, v);
	  vector<edge>::iterator eir = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
	  assert(eir != listOfEdgesIN.end());
	  
	  if(!((*listOfEdgesIN.rbegin()) == e)){
	    vector<edge> tmp;
	    tmp.insert(tmp.begin(), eir+1, listOfEdgesIN.end());
	    tmp.insert(tmp.end(), listOfEdgesIN.begin(), eir+1);
	    listOfEdgesIN = tmp;
	  }
	}
	else{
	  edge e = existEdge(V[k][i-1], v);
	  if(e.isValid()){
	    if(!((*listOfEdgesIN.begin()) == e)){
	      vector<edge> tmp;
	      vector<edge>::iterator ei = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
	      tmp.insert(tmp.begin(), ei, listOfEdgesIN.end());
	      tmp.insert(tmp.end(), listOfEdgesIN.begin(), ei);
	      listOfEdgesIN = tmp;
	    }
	  }
	  else{
	    e = existEdge(V[k][i+1], v);
	    assert(e.isValid());
	    if(!((*listOfEdgesIN.rbegin()) == e)){
	      vector<edge> tmp;
	      vector<edge>::iterator ei = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
	      tmp.insert(tmp.begin(), ei+1, listOfEdgesIN.end());
	      tmp.insert(tmp.end(), listOfEdgesIN.begin(), ei+1);
	      listOfEdgesIN = tmp;
	    }
	  }
	}
      }
      
      int out_l = 0, out_r = 0, dl, dr;
      // determinate the coords out-in points
      
      float dtmp = (nbOut-1.)/2.;
      int outPlus = int(dtmp);
      
      int outMoins = (0 >= outPlus ? 0 : outPlus);
      if(dtmp != float(outPlus))
	outPlus++;  // valeur entière supérieure
      
      if( nbIn >= 2){
	dl = 1; dr = 1;
	out_l = outMoins;
	out_r = outPlus;
      }
      else if(nbIn == 0){
	dl = dr = 0;
	out_l = outMoins;
	out_r = outPlus;
      }
      else{// cas nbIn == 1
	if( k == 0 ){  
	  if( i == 0 ){ // (Z0,Z1), existe forcément par définition 
	    dl = 0; dr = 1;
	    out_l = outPlus; out_r = outMoins;
	  }
	  else if( i == V[k].size()-1){
	    dl = 1; dr = 0;
	    out_l = outMoins; out_r = outPlus;
	  }
	  else{
	    bool trouve = false;
	    edge e = existEdge(v, V[k][i-1]); // (Zi, Zi-1)
	    if(!e.isValid()){
	      trouve = true;
	      dl = 0; dr = 1;
	      out_l = outPlus; out_r = outMoins;
	    }
	    
	    if(!trouve){
	      edge e = existEdge(v, V[k][i+1]); // (Zi, Zi+1)
	      if(!e.isValid()){
		dl = 1; dr = 0;
		out_l = outMoins; out_r = outPlus;
	      }
	    }
	  }
	}
	else{
	  if(i == 0){  // existe un arc entre left(V[k]),Z0
	    dl = 1; dr = 0;
	    out_l = outMoins; out_r = outPlus;
	  }
	  else if(i == V[k].size()-1){
	    dl = 0; dr = 1;
	    out_l = outPlus; out_r = outMoins;
	  }
	  else{
	    bool trouve = false;
	    edge e = existEdge(v, V[k][i-1]); // (Zi, Zi-1)
	    if(!e.isValid()){
	      trouve = true;
	      dl = 0; dr = 1;
	      out_l = outPlus; out_r = outMoins;
	    }
	    
	    if(!trouve){
	      edge e = existEdge(v, V[k][i+1]); // (Zi, Zi+1)
	      if(!e.isValid()){
		dl = 1; dr = 0;
		out_l = outMoins; out_r = outPlus;
	      }
	    }
	  }
	}
      }
      
      outl[v] = out_l;
      outr[v] = out_r;

      //  
      if(nbOut>=1){
	Coord c;
	for(int x = -out_l, y = dl; x<=-1 ; x++,y++){
	  c.set(x, y); out_points[v].push_back(c);
	}
	c.set(0,(out_l+dl-1 > out_r+dr-1) ? out_l+dl-1 : out_r+dr-1);
	out_points[v].push_back(c); 
	for(int x = 1, y = out_r+dr-1; y>=dr ; x++,y--){
	  c.set(x,y); out_points[v].push_back(c);
	}
      }

      if(k !=0){
	for(unsigned int ui = 0; ui < listOfEdgesIN.size();++ui){
	  edge e_tmp = listOfEdgesIN[ui];
	  node n_tmp = (carte->source(e_tmp) == v ? carte->target(e_tmp) : carte->source(e_tmp));
	  if(rank[n_tmp] < k){
	    if( i == 0){
	      unsigned int t = out_points[n_tmp].size();
	      if(n_tmp == nl){
		OutPoints[e_tmp] = out_points[n_tmp][t - 1];
		out_points[n_tmp].erase(out_points[n_tmp].end());
	      }
	      
	      else {                
		OutPoints[e_tmp] = out_points[n_tmp][0];
		out_points[n_tmp].erase(out_points[n_tmp].begin());
	      }
	    }
	    else if(i == p-1){
	      // n_tmp is equal to nr
	      OutPoints[e_tmp] = out_points[n_tmp][0];
	      out_points[n_tmp].erase(out_points[n_tmp].begin());
	    }
	  }
	}  
      }

      int in_l = (nbIn - 3)/2;
      float ftmp = (nbIn - 3.)/2.;
      int in_r = in_l;
      
      if(0>in_l) in_l = 0;
      if(ftmp != float(in_r)) in_r++;
      if(0>in_r) in_r = 0;
      
      inr[v] = in_r;
      inl[v] = in_l;
      
      if(nbIn > 3){
	unsigned int j = 0;
	Coord c(-in_l,0);
	InPoints[listOfEdgesIN[j]].push_back(c); j++;
	
	for(int x = -in_l, y = -1; x<=-1 ; x++,y--){
	  c.set(x, y);
	  InPoints[listOfEdgesIN[j]].push_back(c); j++;
	}
	
	c.set(0, -in_r);
	InPoints[listOfEdgesIN[j]].push_back(c); j++;
	
	for(int x = 1, y = -in_r; x<=in_r ; x++,y++){
	  c.set(x, y);
	  InPoints[listOfEdgesIN[j]].push_back(c); j++;
	}   
	
	c.set(in_r, 0);
	InPoints[listOfEdgesIN[j]].push_back(c); j++;
	
	assert( j == nbIn ); 
      }
      else{
	for(unsigned int j = 0; j<nbIn; j++)
	  InPoints[listOfEdgesIN[j]].push_back(Coord());
      }
      
      EdgesIN[v] = listOfEdgesIN;
      unsigned s = listOfEdgesOUT.size();
      EdgesOUT[v] = vector<edge>(s);
      for(unsigned int i = 0; i<s; i++)
	EdgesOUT[v][s-(i+1)] = listOfEdgesOUT[i];
      
    }
    
    // mise à jour du contour
    if(k == 0) C = V[0];
    else{
      C.erase(il+1, ir);
      ir = find(C.begin(), C.end(), nr);
      C.insert(ir, V[k].begin(), V[k].end());
    }
  }  
}

//====================================================
void MixedModel::computeCoords(){
  NodeCoords.clear();
  
  nodeSize.setAll(Coord());                   // permet de conserver une taille relative pout les sommets
  Iterator<node> * itn = carte->getNodes();
  while(itn->hasNext()){
    node n = itn->next();
    Coord c = size->getNodeValue(n);
    c[0] += edgeNodeSpacing;
    nodeSize.set(n.id,c);
  }delete itn;
  
  map<node, node> father;  // permet de connaître le noeud de référence
  father.clear();
  assert(V.size() != 0);
  
  vector<node> C;  // chemin courant
  double out_r_moins1;
  
  //-------------  initialisation --------------------------
  for(unsigned int i = 0; i<V[0].size(); i++){
    node v = V[0][i];
    
    double out_l = outl[v], out_r = outr[v];
    Coord c = nodeSize.get((V[0][i]).id);
    float x;
    if(i == 0) x = (out_l < (c.getX()/2.) ? (c.getX()/2.) : out_l) ;
    else  x = out_r_moins1 + (out_l < (c.getX()/2.) ? (c.getX()/2.) : out_l)  + spacing;
    
    NodeCoords[v] = Coord(x, 0, 0);  // y(vi) = 0
    out_r_moins1 = (out_r < (c.getX()/2.) ? (c.getX()/2.) : out_r);
  }
  
  C = V[0];  // initiation du contour C0
  
  unsigned int size_V = V.size();
  
  for(unsigned int k = 1; k< size_V; k++){ // parcours de chaque partition 
    
    unsigned int p  = V[k].size();  // taille de la partition Vk
    node cl = leftV(k), cr = rightV(k); // recherche du left et right de Vk, qui là est cherché à l'aide de EdgesIN
    
    vector<node>::iterator il = find(C.begin(), C.end(), cl);
    assert(il != C.end());
    vector<node>::iterator ir = find(il, C.end(), cr);  // par définition, il<ir
    assert(ir != C.end());
    Coord co = nodeSize.get((*il).id);    
    float max_y = NodeCoords[(*il)].getY() + co.getY()/2. ;
    float somme = 0; // relative to cl
    for(vector<node>::iterator i = il+1; i != ir+1; i++){
      Coord co2 = nodeSize.get((*i).id);   
      float y =  NodeCoords[(*i)].getY() + co2.getY()/2. ; //recherche max des y(ci) dans [cl...cr]
      if(max_y<y) max_y = y;
      // compute x-coords [cl+1.. cr] temporarily
      somme += NodeCoords[(*i)].getX();
      NodeCoords[(*i)].setX(somme);  // somme x(cj) dans [cl+1...ci]
    }
    
    node Z0 = V[k][0];
    co = nodeSize.get(Z0.id);
    int max_y_taille = (int) (inr[Z0]<co.getY()/2. ? co.getY()/2. : inr[Z0]);
    int taille_tmp;
    for(unsigned int i = 0;i<p; i++){
      co = nodeSize.get((V[k][i]).id);
      taille_tmp = (int) (inr[V[k][i]]<co.getY()/2. ? co.getY()/2. : inr[V[k][i]]);
      if(taille_tmp > max_y_taille)
	max_y_taille = taille_tmp;
    }
    
    for(unsigned int i = 0; i<p; i++){
      // assign y-coords
      NodeCoords[V[k][i]] = Coord(0, max_y_taille + max_y + spacing, 0);
    }
    
    // assign x-coords :
    unsigned int n = EdgesIN[V[k][p-1]].size();
    int dxl = (int)(OutPoints[EdgesIN[V[k][0]][0]].getX()),
      dxr = (int)(OutPoints[EdgesIN[V[k][p-1]][n-1]].getX());
    
    if(EdgesIN[Z0].size() >= 3){
      assert(p==1);
      
      int in_l = inl[Z0];
      vector<node>::iterator it = il; int t = 1;
      for(; t<in_l+2 && it != ir; t++){
	do{
	  it++;
	}
	while(!existEdge(Z0, (*it)).isValid());
      }
      assert(t==in_l+2);
      
      edge e = EdgesIN[V[k][0]][t-1];
      float dxt = OutPoints[e].getX(); 
      int out_l = outl[V[k][0]], out_r = outr[V[k][0]];
      
      float tmp = NodeCoords[(*it)].getX()+ dxt;
      float ftmp = (out_l<(nodeSize.get(Z0.id)).getX()/2. ? (nodeSize.get(Z0.id)).getX()/2. : out_l) + dxl;
      if(tmp >= ftmp) NodeCoords[Z0].setX(tmp);
      else NodeCoords[Z0].setX(ftmp);

      float delta = NodeCoords[Z0].getX() - (dxt +  NodeCoords[(*it)].getX());
      if(delta>0)
	for(vector<node>::iterator i = it; i != ir+1; i++)
	  NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() + delta);
      else delta = 0;
      
      tmp = NodeCoords[(*ir)].getX()+ delta - NodeCoords[Z0].getX();
      ftmp = (out_r<(nodeSize.get(Z0.id)).getX()/2. ? (nodeSize.get(Z0.id)).getX()/2. : out_r) - dxr;
      if(tmp >= ftmp) NodeCoords[(*ir)].setX(tmp);
      else NodeCoords[(*ir)].setX(ftmp);
      
      float xZ0 = NodeCoords[Z0].getX();

      for(vector<node>::iterator i = il+1; i != ir ;++i){//it; i++){
	NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() - xZ0);
	father[(*i)] = Z0;
      }
    }
    else{
      int out_l, out_r, out_r_moins1;
      float somme = 0;
      
      // assign x(zi)
      for(unsigned int i = 0; i<p; i++){
	out_l = outl[V[k][i]]; out_r = outr[V[k][i]];
	Coord co2 = nodeSize.get((V[k][i]).id);
	double x;
	if(i == 0) x = (out_l<co2.getX()/2. ? co2.getX()/2. : out_l) + dxl;
	else x = out_r_moins1 + (out_l<co2.getX()/2. ? co2.getX()/2. : out_l)+1;
	
	NodeCoords[V[k][i]].setX(x);
	somme += x;
	
	out_r_moins1 = (int) (out_r<co2.getX()/2. ? co2.getX()/2. : out_r);
      }
      
      //assign x(cr)
      co = nodeSize.get((V[k][p-1]).id);
      float tmp = (out_r<co.getX()/2. ? co.getX()/2. : out_r) - dxr;
      
      float xtmp = NodeCoords[cr].getX();
      double x;
      if(tmp > xtmp - somme) x = tmp;
      else x = xtmp - somme;
      NodeCoords[cr].setX(x);
      
      float xZ0 = NodeCoords[Z0].getX();
      
      for(vector<node>::iterator i = il+1; i != ir; i++){
	NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() - xZ0);
	father[(*i)] = Z0;
      }
    }
    C.erase(il+1, ir);
    ir = find(C.begin(), C.end(), cr);
    C.insert(ir, V[k].begin(), V[k].end());
  }  
  
  float somme = 0;
  for(unsigned int i = 0; i<C.size(); i++){
    somme += NodeCoords[C[i]].getX();
    NodeCoords[C[i]].setX(somme);
  }
  
  for(int k = size_V - 1; k>=0; k--){
    unsigned int p = V[k].size();
    for(unsigned int i = 0; i<p; i++){
      node v = V[k][i];
      if(find(C.begin(), C.end(), v) == C.end())
	if(father.find(v) != father.end())
	  NodeCoords[v].setX(NodeCoords[v].getX() + NodeCoords[father[v]].getX());
    }
  }
}

//====================================================
edge MixedModel::existEdge(node n, node v){
  edge e = carte->existEdge(n, v);
  if(!e.isValid())
    e = carte->existEdge(v, n);
  return e;
}
 
//====================================================
node MixedModel::leftV(unsigned int k){
  assert( (0<k) && (k<V.size()) );
  edge el = EdgesIN[V[k][0]][0];
  return (carte->source(el) == V[k][0])?carte->target(el):carte->source(el);
}

//====================================================
node MixedModel::rightV(unsigned int k){
  assert( (0<k) && (k<V.size()) );
  unsigned int p = V[k].size();
  unsigned int n = EdgesIN[V[k][p-1]].size();
  edge er = EdgesIN[V[k][p-1]][n-1];
  return (carte->source(er) == V[k][p-1])?carte->target(er):carte->source(er);
}
//====================================================
