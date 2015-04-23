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

#include <tulip/TulipPluginHeaders.h>
#include <tulip/MapIterator.h>
#include <tulip/PlanarConMap.h>
#include <tulip/Ordering.h>
#include <tulip/MutableContainer.h>
#include <tulip/Bfs.h>
#include <tulip/GraphTools.h>
#include <tulip/StringCollection.h>
#include <tulip/TulipViewSettings.h>

#include "MixedModel.h"
#include "DatasetTools.h"

PLUGIN(MixedModel)

using namespace std;
using namespace tlp;

float spacing = 2;
float edgeNodeSpacing = 2;

//===============================================================
namespace {
const char * paramHelp[] = {
  //Orientation
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF( "default", "vertical" )   \
  HTML_HELP_BODY() \
  "This parameter enables to choose the orientation of the drawing" \
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
  HTML_HELP_CLOSE(),
  // node shape
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Integer" ) \
  HTML_HELP_DEF( "values", "An existing shape property" ) \
  HTML_HELP_DEF( "default", "viewShape" ) \
  HTML_HELP_BODY() \
  "This parameter defines the property used as node's shape." \
  HTML_HELP_CLOSE(),
};
}
#define ORIENTATION "vertical;horizontal;"
//====================================================
MixedModel::MixedModel(const tlp::PluginContext* context):LayoutAlgorithm(context)  {
  addNodeSizePropertyParameter(this, true /* inout */);
  addInParameter<StringCollection> ("orientation", paramHelp[0], ORIENTATION );
  addInParameter<float> ("y node-node spacing",paramHelp[1],"2");
  addInParameter<float> ("x node-node and edge-node spacing",paramHelp[2],"2");
  addOutParameter<IntegerProperty>("node shape", paramHelp[3],
                                   "viewShape");
  addDependency("Connected Component Packing", "1.0");
}
//====================================================
MixedModel::~MixedModel() {
}
//====================================================
bool MixedModel::run() {
  string orientation = "vertical";
  sizeResult = NULL;
  glyphResult = NULL;

  if (dataSet!=NULL) {
    getNodeSizePropertyParameter(dataSet, sizeResult);
    StringCollection tmp;

    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }

    dataSet->get("y node-node spacing",spacing);
    dataSet->get("x node-node and edge-node spacing",edgeNodeSpacing);
    dataSet->get("node shape", glyphResult);
  }

  if (sizeResult == NULL)
    sizeResult = graph->getProperty<SizeProperty>("viewSize");

  if (glyphResult == NULL)
    glyphResult = graph->getLocalProperty<IntegerProperty>("viewShape");


  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      const Size& tmp = sizeResult->getNodeValue(n);
      sizeResult->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }

  glyphResult->setAllEdgeValue(EdgeShape::Polyline);

  // give some empirical feedback of what we are doing 1 %
  pluginProgress->progress(1, 1000);

  Pere = graph->addCloneSubGraph("Father");
  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(Pere, components);

  for (unsigned int i = 0; i < components.size(); ++i) {
    Pere->inducedSubGraph(components[i]);
  }

  vector<edge> edge_planar;


  int nbConnectedComponent = 0;
  Iterator<Graph *> *it = Pere->getSubGraphs();

  while(it->hasNext()) {
    nbConnectedComponent++;
    currentGraph = it->next();

    //====================================================
    //don't compute the canonical ordering if the nmber of nodes is less than 3

    /* tlp::debug() << currentGraph->numberOfNodes() << " nodes and " << currentGraph->numberOfEdges() << " edges " << endl;
    tlp::debug() << "Create map" << endl; */
    if(currentGraph->numberOfNodes() == 1) {
      node n = currentGraph->getOneNode();
      result->setNodeValue(n, Coord(0,0,0));
      continue;
    }
    else if(currentGraph->numberOfNodes() == 2 || currentGraph->numberOfNodes() == 3) {
      Iterator<node> * itn = currentGraph->getNodes();
      node n = itn->next();
      Coord c(sizeResult->getNodeValue(n));
      result->setNodeValue(n, Coord(0,0,0));
      node n2 = itn->next();
      Coord c2(sizeResult->getNodeValue(n2));
      result->setNodeValue(n2, Coord(spacing + c.getX()/2+c2.getX()/2,0,0));

      if(currentGraph->numberOfNodes() == 3) {
        node n3 = itn->next();
        Coord c3(sizeResult->getNodeValue(n2));
        result->setNodeValue(n3, Coord(2.f * spacing + c.getX()/2.f + c2.getX()+c3.getX()/2.f,0,0));
        edge e = currentGraph->existEdge(n,n3, false);

        if(e.isValid()) {
          vector<Coord> bends(2);
          bends.clear();
          unsigned int max = (unsigned int) (c[1]>c2[1]? c[1]/2 : c2[1]/2);
          max += (unsigned int) edgeNodeSpacing;

          if(currentGraph->source(e) == n) {
            bends.push_back(Coord(0,static_cast<float>(max),0));
            bends.push_back(Coord(2.f * spacing + c.getX()/2.f + c2.getX()+c3.getX()/2.f,static_cast<float>(max),0));
          }
          else {
            bends.push_back(Coord(2.f * spacing + c.getX()/2.f + c2.getX()+c3.getX()/2.f,static_cast<float>(max),0));
            bends.push_back(Coord(0,static_cast<float>(max),0));
          }

          result->setEdgeValue(e, bends);
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

    if(!planar) {
      // tlp::debug() << "Graph is not planar ...";
      BooleanProperty resultatAlgoSelection(currentGraph);
      Bfs sp(currentGraph, &resultatAlgoSelection);
      currentGraph->delSubGraph(sp.graph);
      G = currentGraph->addSubGraph();
      Iterator<edge> * ite = currentGraph->getEdges();

      while(ite->hasNext()) {
        edge e_tmp = ite->next();

        if(resultatAlgoSelection.getEdgeValue(e_tmp)) {
          const pair<node, node>& eEnds = currentGraph->ends(e_tmp);
          G->addNode(eEnds.first);
          G->addNode(eEnds.second);
          G->addEdge(e_tmp);
          edge_planar.push_back(e_tmp);
        }
        else
          unplanar_edges.push_back(e_tmp);
      }

      delete ite;
      //===================================================

      graphMap = computePlanarConMap(G);
      vector<edge> re_added = getPlanarSubGraph(graphMap, unplanar_edges);

      for (unsigned int ui = 0; ui < re_added.size() ; ++ui) {
        edge e = re_added[ui];
        G->addEdge(e);
        resultatAlgoSelection.setEdgeValue(e,true);
        edge_planar.push_back(e);
        vector<edge>::iterator ite = find(unplanar_edges.begin(),unplanar_edges.end(),e);
        unplanar_edges.erase(ite);
      }

      delete graphMap;
      // tlp::debug() << "... Planar subGraph computed" << endl;
    }
    else {
      G = currentGraph->addCloneSubGraph();
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
    carte = computePlanarConMap(G);
    assert(BiconnectedTest::isBiconnected(G));
    assert(BiconnectedTest::isBiconnected(carte));

    // give some empirical feedback of what we are doing 2%
    pluginProgress->progress(1, 100);

    // give some empirical feedback (5%)
    pluginProgress->progress(2, 100);

    // tlp::debug() << "Make the map planar ...";
    // tlp::debug() << "... end" << endl;
    // give some empirical feedback of what we are doing (10%)
    if (pluginProgress->progress(5, 100) !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;

    InPoints.clear();
    OutPoints.clear();
    EdgesIN.clear();
    EdgesOUT.clear();
    outl.clear();
    outr.clear();
    inl.clear();
    inr.clear();

    rank.clear();

    V.clear();

    NodeCoords.clear();
    // Cout << "Partition initialization ...";
    initPartition() ;

    // tlp::debug()<<"... Partition initialized"<<endl;
    if (pluginProgress->state() == TLP_CANCEL)
      return false;

    // tlp::debug() << "InOutPoint computation ..."  ;
    assignInOutPoints();
    // tlp::debug()<<"... InOutPoints computed"<<endl;

    // tlp::debug() << "Coordinate computation ..."  ;
    computeCoords();
    // tlp::debug()<<"... Coordinate computed"<<endl;
    // tlp::debug() << "Drawing edges and nodes ..." ;
    placeNodesEdges();
    // tlp::debug() << "... Edges and Nodes drawn" <<endl;

    vector<edge>::const_iterator ite = dummy.begin();

    for (; ite!=dummy.end(); ++ite)
      currentGraph->delEdge(*ite, true);

    ite = added_edges.begin();
    const vector<Coord>& dv = result->getEdgeDefaultValue();

    for (; ite!=added_edges.end(); ++ite) {
      edge e = *ite;
      currentGraph->delEdge(e, true);
      result->setEdgeValue(e, dv);
    }

    delete carte;
    currentGraph->delAllSubGraphs(G);

  }

  delete it;

  if(nbConnectedComponent != 1) {
    string err ="";
    LayoutProperty layout(graph);
    DataSet tmp;
    tmp.set("coordinates", result);
    graph->applyPropertyAlgorithm(string("Connected Component Packing"),&layout,err,NULL,&tmp);
    Iterator<node> *itN = graph->getNodes();

    while(itN->hasNext()) {
      node n = itN->next();
      result->setNodeValue(n, layout.getNodeValue(n));
    }

    delete itN;
    Iterator<edge> *itE = graph->getEdges();

    while(itE->hasNext()) {
      edge e = itE->next();
      result->setEdgeValue(e, layout.getEdgeValue(e));
    }

    delete itE;
  }

  graph->delAllSubGraphs(Pere);


  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      const Size& tmp = sizeResult->getNodeValue(n);
      sizeResult->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      const Coord& tmpC = result->getNodeValue(n);
      result->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
    edge e;
    forEach(e, graph->getEdges()) {
      LineType::RealType tmp = result->getEdgeValue(e);
      LineType::RealType tmp2;
      LineType::RealType::iterator it;

      for (it = tmp.begin(); it!= tmp.end(); ++it) {
        tmp2.push_back(Coord(-(*it)[1], (*it)[0], (*it)[2]));
      }

      result->setEdgeValue(e, tmp2);
    }
  }

  dataSet->set("planar_edges",edge_planar);

  return true;
}


//====================================================
bool MixedModel::check(std::string &err) {
  bool result = true ;
  err = "The graph must be ";

  if(!SimpleTest::isSimple(graph)) {
    err += "simple and without self-loop ";
    result = false;
  }

  return result;
}


//====================================================
vector<edge> MixedModel::getPlanarSubGraph(tlp::PlanarConMap *sg, std::vector<tlp::edge> unplanar_edges) {
  vector<edge> res;

  for(unsigned int ui = 0; ui < unplanar_edges.size() ; ++ui) {
    edge e = unplanar_edges[ui];
    const pair<node, node>& eEnds = sg->ends(e);
    Face f = sg->sameFace(eEnds.first, eEnds.second);

    if( f != Face()) {
      sg->splitFace(f,e);
      res.push_back(e);
    }
  }

  return res;
}


//====================================================
void MixedModel::placeNodesEdges() {
  float maxX = 0, maxY = 0;

  Iterator<node> *it = carte->getNodes();

  while(it->hasNext()) {
    node n = it->next();
    Coord c = nodeSize.get(n.id);
    c[0] -= edgeNodeSpacing;
    graph->getProperty<SizeProperty>("viewSize")->setNodeValue(n,Size(c[0],c[1],0.3f));
    result->setNodeValue(n, NodeCoords[n]);
  }

  delete it;

  Iterator<edge> *ite = carte->getEdges();

  while(ite->hasNext()) {
    edge e = ite->next();
    const pair<node, node>& eEnds = carte->ends(e);
    node src = eEnds.first;
    node tgt = eEnds.second;
    Coord cs, ct, c;


    unsigned int rs = rank[src], rt = rank[tgt];

    if(rs != rt) {
      vector<Coord> bends;

      if(rs>rt) {
        cs = InPoints[e][0] + NodeCoords[src];
        ct = OutPoints[e] + NodeCoords[tgt];
        c = Coord(ct.getX(),cs.getY(), 0);
      }
      else {
        ct = InPoints[e][0] + NodeCoords[tgt];
        cs = OutPoints[e] + NodeCoords[src];
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

      if((cs != NodeCoords[src]) && (cs != ct))
        bends.push_back(cs);

      if((c != cs) && (c != ct)) bends.push_back(c);

      if((ct != NodeCoords[tgt]) && (ct != cs))
        bends.push_back(ct);



      if(!bends.empty())
        result->setEdgeValue(e, bends);
    }

    // rs == rt, même partition donc pas de points intermédiaire �  calculer
    // en cas de post-processing, il faudra pe y changer
  }

  delete ite;

  if(!planar) {
    float z_size = (maxX+maxY)/3.f;//sqrt(maxX + maxY);
    maxX /= 8;
    maxY /= 8;

    for(unsigned int ui = 0; ui < unplanar_edges.size(); ++ui) {
      edge e = unplanar_edges[ui];
      const pair<node, node>& eEnds = carte->ends(e);
      node n = eEnds.first;
      node v = eEnds.second;
      Coord c_n(-maxX+(NodeCoords[n].getX()+NodeCoords[v].getX())/2.f, -maxY+(NodeCoords[n].getY()+NodeCoords[v].getY())/2.f, -z_size);
      vector<Coord> bends;
      bends.push_back(c_n);
      result->setEdgeValue(e, bends);
      graph->getProperty<IntegerProperty>("viewShape")->setEdgeValue(e, EdgeShape::BezierCurve);
      graph->getProperty<ColorProperty>("viewColor")->setEdgeValue(e,Color(218,218,218));
    }
  }
}

//====================================================
void MixedModel::initPartition() {
  V = tlp::computeCanonicalOrdering(carte, &dummy, pluginProgress);

  if (pluginProgress->state() == TLP_CANCEL)
    return;

  for(unsigned int i = 0; i < V.size(); ++i)
    for(unsigned int j = 0; j < V[i].size(); ++j)
      rank[V[i][j]] = i;
}

//====================================================
void MixedModel::assignInOutPoints() { // on considère qu'il n'y a pas d'arc double
  assert(carte);
  vector<node> C;  // chemin courant

  // empirical feedback (95% -> 99%)
  int minProgress = 950, deltaProgress = 40;

  for(unsigned int k = 0; k < V.size(); ++k) {
    // give pluginProgress feedback
    if (pluginProgress->progress(minProgress +
                                 (deltaProgress * k)/ V.size(),
                                 1000) != TLP_CONTINUE)
      return;

    unsigned int p = V[k].size();

    vector<node>::iterator il, ir;
    node nl = node(), nr = node();

    if(k!=0) {
      // ordonné les arcs "in"
      unsigned int i;

      for(i = 0; !nl.isValid() && (i<C.size()-1); ++i) {
        if(existEdge(C[i], V[k][0]).isValid()) {
          nl = C[i];
        }
      }

      assert(nl.isValid());

      for(i = C.size()-1; !nr.isValid() && (i>0); i--) {
        if(existEdge(C[i], V[k][p-1]).isValid()) {
          nr = C[i];
        }
      }

      assert(nr.isValid());

      il = find(C.begin(), C.end(), nl);
      assert(il != C.end());
      ir = find(il, C.end(), nr);
      assert(ir != C.end());
    }

    for(unsigned int i = 0; i < p; ++i) {
      node v = V[k][i];
      unsigned nbIn = 0, nbOut = 0;
      vector<edge> listOfEdgesIN;
      vector<edge> listOfEdgesOUT;

      listOfEdgesIN.clear();
      listOfEdgesOUT.clear();


      // build in-edge vector and out-edge vector in the good order
      Iterator<edge> *it = carte->getInOutEdges(v);
      vector<edge> tmp;
      char pred = 'p';

      tmp.clear();

      while(it->hasNext()) {
        edge e = it->next();
        const pair<node, node>& eEnds = carte->ends(e);

        node n = (eEnds.first == v)? eEnds.second : eEnds.first;

        bool trouve = false;
        unsigned int r = rank[n];


        if(r < k) {
          ++nbIn;
          trouve = true;

          if(pred == 'o') {
            pred = 'i';
            listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
            tmp.clear();
          }
          else if( pred == 'p') pred = 'i';

          tmp.push_back(e);
        }

        if(r > k && !trouve) {
          ++nbOut;
          trouve = true;

          if(pred == 'i') {
            pred = 'o';
            listOfEdgesIN.insert(listOfEdgesIN.begin(), tmp.begin(), tmp.end());
            tmp.clear();
          }
          else if( pred == 'p') pred = 'o';

          tmp.push_back(e);
        }

        if((i != 0)&&(!trouve)) { // test le voisin de gauche
          if(V[k][i-1] == n) {
            ++nbIn;
            trouve = true;

            if(pred == 'o') {
              pred = 'i';
              listOfEdgesOUT.insert(listOfEdgesOUT.begin(), tmp.begin(), tmp.end());
              tmp.clear();
            }
            else if( pred == 'p') pred = 'i';

            tmp.push_back(e);
          }
        }

        if( (i != V[k].size()-1) && !trouve) { // test le voisin de droite
          if(V[k][i+1] == n) {
            ++nbIn;

            if(pred == 'o') {
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

      if(k!=0) {
        if(i==0) {
          edge e = existEdge(nl, v);
          vector<edge>::iterator eil = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
          assert(eil != listOfEdgesIN.end());

          if(!((*listOfEdgesIN.begin()) == e)) {
            vector<edge> tmp;
            tmp.insert(tmp.begin(), eil, listOfEdgesIN.end());
            tmp.insert(tmp.end(), listOfEdgesIN.begin(), eil);
            listOfEdgesIN = tmp;
          }
        }
        else if(i == p-1) {
          edge e = existEdge(nr, v);
          vector<edge>::iterator eir = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
          assert(eir != listOfEdgesIN.end());

          if(!((*listOfEdgesIN.rbegin()) == e)) {
            vector<edge> tmp;
            tmp.insert(tmp.begin(), eir+1, listOfEdgesIN.end());
            tmp.insert(tmp.end(), listOfEdgesIN.begin(), eir+1);
            listOfEdgesIN = tmp;
          }
        }
        else {
          edge e = existEdge(V[k][i-1], v);

          if(e.isValid()) {
            if(!((*listOfEdgesIN.begin()) == e)) {
              vector<edge> tmp;
              vector<edge>::iterator ei = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
              tmp.insert(tmp.begin(), ei, listOfEdgesIN.end());
              tmp.insert(tmp.end(), listOfEdgesIN.begin(), ei);
              listOfEdgesIN = tmp;
            }
          }
          else {
            e = existEdge(V[k][i+1], v);
            assert(e.isValid());

            if(!((*listOfEdgesIN.rbegin()) == e)) {
              vector<edge> tmp;
              vector<edge>::iterator ei = find(listOfEdgesIN.begin(), listOfEdgesIN.end(), e);
              tmp.insert(tmp.begin(), ei+1, listOfEdgesIN.end());
              tmp.insert(tmp.end(), listOfEdgesIN.begin(), ei+1);
              listOfEdgesIN = tmp;
            }
          }
        }
      }

      int out_l = 0, out_r = 0, dl = 0, dr = 0;
      // determinate the coords out-in points

      float dtmp = (nbOut-1.f)/2.f;
      int outPlus = int(dtmp);

      int outMoins = (0 >= outPlus ? 0 : outPlus);

      if(dtmp != float(outPlus))
        ++outPlus;  // valeur entière supérieure

      if( nbIn >= 2) {
        dl = 1;
        dr = 1;
        out_l = outMoins;
        out_r = outPlus;
      }
      else if(nbIn == 0) {
        dl = dr = 0;
        out_l = outMoins;
        out_r = outPlus;
      }
      else { // cas nbIn == 1
        if( k == 0 ) {
          if( i == 0 ) { // (Z0,Z1), existe forcément par définition
            dl = 0;
            dr = 1;
            out_l = outPlus;
            out_r = outMoins;
          }
          else if( i == V[k].size()-1) {
            dl = 1;
            dr = 0;
            out_l = outMoins;
            out_r = outPlus;
          }
          else {
            bool trouve = false;
            edge e = existEdge(v, V[k][i-1]); // (Zi, Zi-1)

            if(!e.isValid()) {
              trouve = true;
              dl = 0;
              dr = 1;
              out_l = outPlus;
              out_r = outMoins;
            }

            if(!trouve) {
              edge e = existEdge(v, V[k][i+1]); // (Zi, Zi+1)

              if(!e.isValid()) {
                dl = 1;
                dr = 0;
                out_l = outMoins;
                out_r = outPlus;
              }
            }
          }
        }
        else {
          if(i == 0) { // existe un arc entre left(V[k]),Z0
            dl = 1;
            dr = 0;
            out_l = outMoins;
            out_r = outPlus;
          }
          else if(i == V[k].size()-1) {
            dl = 0;
            dr = 1;
            out_l = outPlus;
            out_r = outMoins;
          }
          else {
            bool trouve = false;
            edge e = existEdge(v, V[k][i-1]); // (Zi, Zi-1)

            if(!e.isValid()) {
              trouve = true;
              dl = 0;
              dr = 1;
              out_l = outPlus;
              out_r = outMoins;
            }

            if(!trouve) {
              edge e = existEdge(v, V[k][i+1]); // (Zi, Zi+1)

              if(!e.isValid()) {
                dl = 1;
                dr = 0;
                out_l = outMoins;
                out_r = outPlus;
              }
            }
          }
        }
      }

      outl[v] = out_l;
      outr[v] = out_r;

      //
      if(nbOut>=1) {
        Coord c;

        for(int x = -out_l, y = dl; x<=-1 ; ++x,++y) {
          c.set(static_cast<float>(x), static_cast<float>(y));
          out_points[v].push_back(c);
        }

        c.set(0,(out_l+dl-1 > out_r+dr-1) ? static_cast<float>(out_l+dl-1) : static_cast<float>(out_r+dr-1));
        out_points[v].push_back(c);

        for(int x = 1, y = out_r+dr-1; y>=dr ; ++x,--y) {
          c.set(static_cast<float>(x),static_cast<float>(y));
          out_points[v].push_back(c);
        }
      }

      if(k !=0) {
        for(unsigned int ui = 0; ui < listOfEdgesIN.size(); ++ui) {
          edge e_tmp = listOfEdgesIN[ui];
          const pair<node, node>& eEnds = carte->ends(e_tmp);
          node n_tmp = (eEnds.first == v) ? eEnds.second : eEnds.first;

          if(rank[n_tmp] < k) {
            if( i == 0) {
              unsigned int t = out_points[n_tmp].size();

              if(n_tmp == nl) {
                OutPoints[e_tmp] = out_points[n_tmp][t - 1];
                out_points[n_tmp].erase(out_points[n_tmp].end());
              }

              else {
                OutPoints[e_tmp] = out_points[n_tmp][0];
                out_points[n_tmp].erase(out_points[n_tmp].begin());
              }
            }
            else if(i == p-1) {
              // n_tmp is equal to nr
              OutPoints[e_tmp] = out_points[n_tmp][0];
              out_points[n_tmp].erase(out_points[n_tmp].begin());
            }
          }
        }
      }

      int in_l = (nbIn - 3)/2;
      float ftmp = (nbIn - 3.f)/2.f;
      int in_r = in_l;

      if(0>in_l) in_l = 0;

      if(ftmp != float(in_r)) ++in_r;

      if(0>in_r) in_r = 0;

      inr[v] = in_r;
      inl[v] = in_l;

      if(nbIn > 3) {
        unsigned int j = 0;
        Coord c(-static_cast<float>(in_l),0);
        InPoints[listOfEdgesIN[j]].push_back(c);
        ++j;

        for(int x = -in_l, y = -1; x<=-1 ; ++x,--y) {
          c.set(static_cast<float>(x), static_cast<float>(y));
          InPoints[listOfEdgesIN[j]].push_back(c);
          ++j;
        }

        c.set(0, -static_cast<float>(in_r));
        InPoints[listOfEdgesIN[j]].push_back(c);
        ++j;

        for(int x = 1, y = -in_r; x<=in_r ; ++x,++y) {
          c.set(static_cast<float>(x), static_cast<float>(y));
          InPoints[listOfEdgesIN[j]].push_back(c);
          ++j;
        }

        c.set(static_cast<float>(in_r), 0);
        InPoints[listOfEdgesIN[j]].push_back(c);
        ++j;

        assert( j == nbIn );
      }
      else {
        for(unsigned int j = 0; j<nbIn; ++j)
          InPoints[listOfEdgesIN[j]].push_back(Coord());
      }

      EdgesIN[v] = listOfEdgesIN;
      unsigned s = listOfEdgesOUT.size();
      EdgesOUT[v] = vector<edge>(s);

      for(unsigned int i = 0; i<s; ++i)
        EdgesOUT[v][s-(i+1)] = listOfEdgesOUT[i];

    }

    // mise �  jour du contour
    if(k == 0) C = V[0];
    else {
      C.erase(il+1, ir);
      ir = find(C.begin(), C.end(), nr);
      C.insert(ir, V[k].begin(), V[k].end());
    }
  }
}

//====================================================
void MixedModel::computeCoords() {
  NodeCoords.clear();

  nodeSize.setAll(Coord());                   // permet de conserver une taille relative pout les sommets
  Iterator<node> * itn = carte->getNodes();

  while(itn->hasNext()) {
    node n = itn->next();
    Coord c(sizeResult->getNodeValue(n));
    c[0] += edgeNodeSpacing;
    nodeSize.set(n.id, c);
  }

  delete itn;

  map<node, node> father;  // permet de connaître le noeud de référence
  father.clear();
  assert(!V.empty());

  vector<node> C;  // chemin courant
  double out_r_moins1 = 0.0;

  //-------------  initialisation --------------------------
  for(unsigned int i = 0; i<V[0].size(); ++i) {
    node v = V[0][i];

    double out_l = outl[v], out_r = outr[v];
    Coord c = nodeSize.get((V[0][i]).id);
    float x;

    if(i == 0) x = (out_l < static_cast<double>(c.getX()/2.f)) ? (c.getX()/2.f) : static_cast<float>(out_l) ;
    else  x = static_cast<float>(out_r_moins1) + ((out_l < static_cast<double>(c.getX()/2.f)) ? (c.getX()/2.f) : static_cast<float>(out_l))  + spacing;

    NodeCoords[v] = Coord(x, 0, 0);  // y(vi) = 0
    out_r_moins1 = (out_r < (c.getX()/2.) ? (c.getX()/2.) : out_r);
  }

  C = V[0];  // initiation du contour C0

  unsigned int size_V = V.size();

  for(unsigned int k = 1; k< size_V; ++k) { // parcours de chaque partition

    unsigned int p  = V[k].size();  // taille de la partition Vk
    node cl = leftV(k), cr = rightV(k); // recherche du left et right de Vk, qui l�  est cherché �  l'aide de EdgesIN

    vector<node>::iterator il = find(C.begin(), C.end(), cl);
    assert(il != C.end());
    vector<node>::iterator ir = find(il, C.end(), cr);  // par définition, il<ir
    assert(ir != C.end());
    Coord co = nodeSize.get((*il).id);
    float max_y = NodeCoords[(*il)].getY() + co.getY()/2.f ;
    float somme = 0; // relative to cl

    for(vector<node>::iterator i = il+1; i != ir+1; ++i) {
      Coord co2 = nodeSize.get((*i).id);
      float y =  NodeCoords[(*i)].getY() + co2.getY()/2.f ; //recherche max des y(ci) dans [cl...cr]

      if(max_y<y) max_y = y;

      // compute x-coords [cl+1.. cr] temporarily
      somme += NodeCoords[(*i)].getX();
      NodeCoords[(*i)].setX(somme);  // somme x(cj) dans [cl+1...ci]
    }

    node Z0 = V[k][0];
    co = nodeSize.get(Z0.id);
    int max_y_taille = (int) ((inr[Z0]<co.getY()/2.) ? co.getY()/2. : inr[Z0]);

    for(unsigned int i = 0; i<p; ++i) {
      co = nodeSize.get((V[k][i]).id);
      int taille_tmp = (int) ((inr[V[k][i]]<co.getY()/2.) ? co.getY()/2. : inr[V[k][i]]);

      if(taille_tmp > max_y_taille)
        max_y_taille = taille_tmp;
    }

    for(unsigned int i = 0; i<p; ++i) {
      // assign y-coords
      NodeCoords[V[k][i]] = Coord(0, max_y_taille + max_y + spacing, 0);
    }

    // assign x-coords :
    unsigned int n = EdgesIN[V[k][p-1]].size();
    int dxl = (int)(OutPoints[EdgesIN[V[k][0]][0]].getX()),
        dxr = (int)(OutPoints[EdgesIN[V[k][p-1]][n-1]].getX());

    if(EdgesIN[Z0].size() >= 3) {
      assert(p==1);

      int in_l = inl[Z0];
      vector<node>::iterator it = il;
      int t = 1;

      for(; t<in_l+2 && it != ir; ++t) {
        do {
          ++it;
        }
        while(!existEdge(Z0, (*it)).isValid());
      }

      assert(t==in_l+2);

      edge e = EdgesIN[V[k][0]][t-1];
      float dxt = OutPoints[e].getX();
      int out_l = outl[V[k][0]], out_r = outr[V[k][0]];

      float tmp = NodeCoords[(*it)].getX()+ dxt;
      float ftmp = ((out_l<(nodeSize.get(Z0.id)).getX()/2.f) ? (nodeSize.get(Z0.id)).getX()/2.f : out_l) + dxl;

      if(tmp >= ftmp) NodeCoords[Z0].setX(tmp);
      else NodeCoords[Z0].setX(ftmp);

      float delta = NodeCoords[Z0].getX() - (dxt +  NodeCoords[(*it)].getX());

      if(delta>0)
        for(vector<node>::iterator i = it; i != ir+1; ++i)
          NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() + delta);
      else delta = 0;

      tmp = NodeCoords[(*ir)].getX()+ delta - NodeCoords[Z0].getX();
      ftmp = ((out_r<(nodeSize.get(Z0.id)).getX()/2.f) ? (nodeSize.get(Z0.id)).getX()/2.f : out_r) - dxr;

      if(tmp >= ftmp) NodeCoords[(*ir)].setX(tmp);
      else NodeCoords[(*ir)].setX(ftmp);

      float xZ0 = NodeCoords[Z0].getX();

      for(vector<node>::iterator i = il+1; i != ir ; ++i) { //it; i++){
        NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() - xZ0);
        father[(*i)] = Z0;
      }
    }
    else {
      int out_r = 0, out_r_moins1 = 0;
      float somme = 0;

      // assign x(zi)
      for(unsigned int i = 0; i<p; ++i) {
        int out_l = outl[V[k][i]];
        out_r = outr[V[k][i]];
        Coord co2 = nodeSize.get((V[k][i]).id);
        double x;

        if(i == 0) x = ((out_l<co2.getX()/2.) ? co2.getX()/2. : out_l) + dxl;
        else x = out_r_moins1 + ((out_l<co2.getX()/2.) ? co2.getX()/2. : out_l)+1;

        NodeCoords[V[k][i]].setX(static_cast<float>(x));
        somme += static_cast<float>(x);

        out_r_moins1 = (int) ((out_r<co2.getX()/2.) ? co2.getX()/2. : out_r);
      }

      //assign x(cr)
      co = nodeSize.get((V[k][p-1]).id);
      float tmp = ((out_r<co.getX()/2.f) ? co.getX()/2.f : out_r) - dxr;

      float xtmp = NodeCoords[cr].getX();
      double x;

      if(tmp > xtmp - somme) x = tmp;
      else x = xtmp - somme;

      NodeCoords[cr].setX(static_cast<float>(x));

      float xZ0 = NodeCoords[Z0].getX();

      for(vector<node>::iterator i = il+1; i != ir; ++i) {
        NodeCoords[(*i)].setX(NodeCoords[(*i)].getX() - xZ0);
        father[(*i)] = Z0;
      }
    }

    C.erase(il+1, ir);
    ir = find(C.begin(), C.end(), cr);
    C.insert(ir, V[k].begin(), V[k].end());
  }

  float somme = 0;

  for(unsigned int i = 0; i<C.size(); ++i) {
    somme += NodeCoords[C[i]].getX();
    NodeCoords[C[i]].setX(somme);
  }

  for(int k = size_V - 1; k>=0; k--) {
    unsigned int p = V[k].size();

    for(unsigned int i = 0; i<p; ++i) {
      node v = V[k][i];

      if(find(C.begin(), C.end(), v) == C.end())
        if(father.find(v) != father.end())
          NodeCoords[v].setX(NodeCoords[v].getX() + NodeCoords[father[v]].getX());
    }
  }
}

//====================================================
node MixedModel::leftV(unsigned int k) {
  assert( (0<k) && (k<V.size()) );
  edge el = EdgesIN[V[k][0]][0];
  const pair<node, node>& eEnds = carte->ends(el);
  return (eEnds.first == V[k][0]) ? eEnds.second : eEnds.first;
}

//====================================================
node MixedModel::rightV(unsigned int k) {
  assert( (0<k) && (k<V.size()) );
  unsigned int p = V[k].size();
  unsigned int n = EdgesIN[V[k][p-1]].size();
  edge er = EdgesIN[V[k][p-1]][n-1];
  const pair<node, node>& eEnds = carte->ends(er);
  return (eEnds.first == V[k][p-1]) ? eEnds.second : eEnds.first;
}
//====================================================
