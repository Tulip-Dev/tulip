/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <fstream>
#include <string>
#include <vector>
#include <errno.h>
#include <sys/stat.h>

#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/ImportModule.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/MethodFactory.h>
#include <tulip/PropertyTypes.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpTools.h>
#include <tulip/TLPParser.h>

#define TLP "tlp"
#define AUTHOR "author"
#define COMMENTS "comments"
#define TLP_VERSION 2.3
#define NODES "nodes"
#define EDGE "edge"
#define NB_NODES "nb_nodes"
#define NB_EDGES "nb_edges"
#define CLUSTER "cluster"
#define CLUSTERNODES "nodes"
#define CLUSTEREDGES "edges"
#define PROPERTY "property"
#define METRIC "metric"
#define METAGRAPH "metagraph"
#define GRAPH "graph"
#define LAYOUT "layout"
#define SIZE "size"
#define COLOR "color"
#define COORD "coord"
#define INT "int"
#define UINT "uint"
#define DOUBLE "double"
#define FLOAT "float"
#define STRING "string"
#define BOOL "bool"
#define SIZE_VECTOR "vector<size>"
#define COLOR_VECTOR "vector<color>"
#define COORD_VECTOR "vector<coord>"
#define INT_VECTOR "vector<int>"
#define DOUBLE_VECTOR "vector<double>"
#define STRING_VECTOR "vector<string>"
#define BOOL_VECTOR "vector<bool>"
#define DATASET "DataSet"
#define NODEVALUE "node"
#define EDGEVALUE "edge"
#define DEFAULTVALUE "default"

#define DISPLAYING "displaying"
#define GLYPH "glyph"
#define PLUGIN "plugin"
#define OLD_ATTRIBUTES "attributes"
#define ATTRIBUTES "graph_attributes"
#define SCENE "scene"
#define VIEWS "views"
#define CONTROLLER "controller"

using namespace tlp;

#ifndef DOXYGEN_NOTFOR_DEVEL

namespace tlp {
    //=================================================================================
    struct TLPGraphBuilder:public TLPTrue {
        Graph *_graph;
        std::map<int,node> nodeIndex;
        std::map<int,edge> edgeIndex;
        std::map<int,Graph *> clusterIndex;
        DataSet *dataSet;
        bool inTLP;
        double version;

        TLPGraphBuilder(Graph *graph, DataSet *dataSet): _graph(graph),
        dataSet(dataSet) {
            clusterIndex[0]=graph;
            inTLP = false;
            version = 0;
        }

        virtual ~TLPGraphBuilder(){
        }

        bool addString(const std::string& str) {
            // only used to handle the version of tlp file format
            if (!version) {
                version = atof(str.c_str());
                return version <= TLP_VERSION;
            }
            return false;
        }

        bool addNode(int id) {
            if (version < 2.1)
                nodeIndex[id]=_graph->addNode();
            else
                _graph->addNode();
            return true;
        }
        bool addNodes(int first, int last) {
	     std::vector<node> nodes;
	    _graph->addNodes(last - first + 1, nodes);
            if (version < 2.1) {
	        std::vector<node>::iterator it = nodes.begin();
                while (first <= last) {
		  nodeIndex[first]=(*it);
		  ++first, ++it;
                }
            }
            return true;
        }
        bool reserveNodes(int nbNodes) {
            _graph->reserveNodes(nbNodes);
            return true;
        }
        bool reserveEdges(int nbEdges) {
            _graph->reserveEdges(nbEdges);
            return true;
        }
        bool addClusterNode(int id, int nodeId) {
            node n(nodeId);
            if (version < 2.1)
                n = nodeIndex[nodeId];
            if (_graph->isElement(n) && clusterIndex[id]) {
                clusterIndex[id]->addNode(n);
                return true;
            }
            return false;
        }
        bool addClusterEdge(int id, int edgeId)  {
            edge e(edgeId);
            if (version < 2.1)
                e = edgeIndex[edgeId];
            if (_graph->isElement(e) && clusterIndex[id]) {
                clusterIndex[id]->addEdge(e);
            }
            return true;
        }
        bool addEdge(int id, int idSource,int idTarget) {
            node src(idSource), tgt(idTarget);
            if (version < 2.1) {
                src = nodeIndex[idSource];
                tgt = nodeIndex[idTarget];
            }

            if (_graph->isElement(src) && _graph->isElement(tgt)) {
                edgeIndex[id]=_graph->addEdge(src, tgt);
                return true;
            }
            return false;
        }
        bool setNodeValue(int nodeId, int clusterId, const std::string& propertyType, const std::string& propertyName, std::string& value) {
            node n(nodeId);
            if (version < 2.1)
                n = nodeIndex[nodeId];
            if (_graph->isElement(n) && clusterIndex[clusterId]) {
                //cerr << "set node value ....." ;
                if (propertyType==GRAPH || propertyType==METAGRAPH) { // METAGRAPH was used in Tulip 2
                    //      cerr << ".";
                    char *endPtr=0;
                    const char *startPtr=value.c_str();
                    int result=strtol(startPtr,&endPtr,10);
                    if (endPtr==startPtr) return false;
                    if (clusterIndex.find(result)==clusterIndex.end()) return false;
                    if (result==0)
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setNodeValue(n, 0);
                    else
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setNodeValue(n, clusterIndex[result]);
                    return true;
                }
                if (propertyType==STRING) {
                    if (propertyName == std::string("viewFont") ||
			propertyName == std::string("viewTexture")) {
                        // if needed replace symbolic path by real path
                        size_t pos = value.find("TulipBitmapDir/");
                        if (pos!=std::string::npos)
                            value.replace(pos, 15, TulipBitmapDir);
                    }
		}
		return clusterIndex[clusterId]->getProperty(propertyName)->setNodeStringValue(n, value );
            }
            return false;
            //cer << "..ok" << endl;
        }

        bool setEdgeValue(int edgeId, int clusterId, const std::string& propertyType, const std::string& propertyName, std::string& value) {
            edge e(edgeId);
            if (version < 2.1)
                e = edgeIndex[edgeId];
            if (_graph->isElement(e) && clusterIndex[clusterId]) {
                //cerr << "setEdgeValue...." << "edge:" << edgeId << " cluster " << clusterId << " " << propertyName << " " << propertyType << " value=\""<< value<<"\"  ";
                bool result=false;

                if (propertyType==GRAPH || propertyType==METAGRAPH)  { // METAGRAPH was used in Tulip 2
                    std::set<edge> v;
                    result = EdgeSetType::fromString(v, value);
                    if (result)
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setEdgeValue(e, v);
                    return result;
                }
                if (propertyType==INT){
                  //If we are in the old edge extremities id system we need to convert the ids in the file.
                  if(version < 2.2){
                    if(propertyName==std::string("viewSrcAnchorShape") || propertyName==std::string("viewTgtAnchorShape")){
                      return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setEdgeStringValue(e, convertOldEdgeExtremitiesValueToNew(value) );
                    }
                  }
                }
                if (propertyType==STRING) {
                    if (propertyName == std::string("viewFont") ||
			propertyName == std::string("viewTexture")) {
                        // if needed replace symbolic path by real path
                        size_t pos = value.find("TulipBitmapDir/");
                        if (pos!=std::string::npos)
                            value.replace(pos, 15, TulipBitmapDir);
                    }
		}
		return clusterIndex[clusterId]->getProperty(propertyName)->setEdgeStringValue(e, value );
            }
            return false;
        }

        /**
         * @brief Convert the id of a edge extremity glyph from the old numeraotation system to the new numerotation system.
         *
         * We cannot set the new value automatically as one edge extremity id doesn't change.
         * @param oldValue The old glyph value.
         * @return The new glyph value or the old value if no change are needed.
         */
        std::string convertOldEdgeExtremitiesValueToNew(const std::string& oldValue){
          if(oldValue == std::string("0")){
            return "-1";
          }else if(oldValue == std::string("1")){
            return "0";
          }else if(oldValue == std::string("3")){
            return "2";
          }else if(oldValue == std::string("4")){
            return "3";
          }else if(oldValue == std::string("5")){
            return "4";
          }else if(oldValue == std::string("7")){
            return "6";
          }else if(oldValue == std::string("10")){
            return "9";
          }else if(oldValue == std::string("13")){
            return "12";
          }else if(oldValue == std::string("14")){
            return "13";
          }else if(oldValue == std::string("15")){
            return "14";
          }else if(oldValue == std::string("16")){
            return "15";
          }else if(oldValue == std::string("29")){
            return "28";
          }else{
            return oldValue;
          }
        }

        bool setAllNodeValue(int clusterId, const std::string& propertyType, const std::string& propertyName, std::string& value) {
            if (clusterIndex[clusterId]) {
                //cerr << "set all node value ....." ;
                if (propertyType==GRAPH || propertyType==METAGRAPH) { // METAGRAPH was used in Tulip 2
                    char *endPtr=0;
                    const char *startPtr=value.c_str();
                    int result=strtol(startPtr,&endPtr,10);
                    if (endPtr==startPtr) result =0; //return false;
                    if (clusterIndex.find(result)==clusterIndex.end()) return false;
                    if (result==0)
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllNodeValue(0);
                    else
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllNodeValue(clusterIndex[result]);
                    return true;
                }
                if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
                    return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==LAYOUT)
                    return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==SIZE)
                    return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==COLOR)
                    return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==INT)
                    return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==BOOL)
                    return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==STRING) {
                    if (propertyName == std::string("viewFont") ||
			propertyName == std::string("viewTexture")) {
                        // if needed replace symbolic path by real path
                        size_t pos = value.find("TulipBitmapDir/");
                        if (pos!=std::string::npos)
                            value.replace(pos, 15, TulipBitmapDir);
                    }
                    return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setAllNodeStringValue( value );
                }
                if (propertyType==SIZE_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==COLOR_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==COORD_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==DOUBLE_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==INT_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==BOOL_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setAllNodeStringValue( value );
                if (propertyType==STRING_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setAllNodeStringValue( value );
            }
            return false;
            //cer << "..ok" << endl;
        }
        bool setAllEdgeValue(int clusterId, const std::string& propertyType, const std::string& propertyName, std::string& value) {
            if (clusterIndex[clusterId]) {
                //cerr << "setAllEdgeValue.." << endl;
                bool result=false;

                if (propertyType==GRAPH|| propertyType==METAGRAPH)  { // METAGRAPH was used in Tulip 2
                    std::set<edge> v;
                    result = EdgeSetType::fromString(v, value);
                    if (result)
                        clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllEdgeValue(v);
                    return result;
                }
                if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
                    return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==LAYOUT)
                    return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==SIZE)
                    return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==COLOR)
                    return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==INT){
                  //If we are in the old edge extremities id system we need to convert the ids in the file.
                  if(version < 2.2){
                    if(propertyName==std::string("viewSrcAnchorShape") || propertyName==std::string("viewTgtAnchorShape")){
                      return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setAllEdgeStringValue( convertOldEdgeExtremitiesValueToNew(value) );
                    }
                  }
                   return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setAllEdgeStringValue( value );
                }
                if (propertyType==BOOL)
                    return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==STRING) {
                    if (propertyName == std::string("viewFont") ||
			propertyName == std::string("viewTexture")) {
                        // if needed replace symbolic path by real path
                        size_t pos = value.find("TulipBitmapDir/");
                        if (pos!=std::string::npos)
                            value.replace(pos, 15, TulipBitmapDir);
                    }
                    return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setAllEdgeStringValue( value );
                }
                if (propertyType==SIZE_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==COLOR_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==COORD_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==DOUBLE_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==INT_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==BOOL_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setAllEdgeStringValue( value );
                if (propertyType==STRING_VECTOR)
                    return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setAllEdgeStringValue( value );
            }
            return false;
        }
        bool addCluster(int id, const std::string& name, int supergraphId=0) {
            if (clusterIndex[supergraphId]) {
                BooleanProperty sel(clusterIndex[supergraphId]);
                sel.setAllNodeValue(false);
                sel.setAllEdgeValue(false);
                clusterIndex[id] = clusterIndex[supergraphId]->addSubGraph(&sel, id);
                if (name.size())
		  clusterIndex[id]->setAttribute("name", name);
                return true;
            }
            return false;
        }
        bool addStruct(const std::string& structName,TLPBuilder*&newBuilder) ;
    };
    //=================================================================================
    struct TLPNodeBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        TLPNodeBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder){}
        bool addInt(const int id) {return graphBuilder->addNode(id);}
        bool addRange(int first, int second) {
            return graphBuilder->addNodes(first, second);
        }
        bool close(){return true;}
    };
    //=================================================================================
    struct TLPNodesBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        TLPNodesBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder){}
        bool addInt(const int nbNodes) {return graphBuilder->reserveNodes(nbNodes);}
        bool close()  { return true; }
    };
    //=================================================================================
    struct TLPEdgeBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        int parameter[3];
        int nbParameter;
        TLPEdgeBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),nbParameter(0){}
        bool addInt(const int id)  {
            if (nbParameter<3) {
                parameter[nbParameter]=id;
                nbParameter++;
                return true;
            }
            else
                return false;
        }
        bool close()  {
            if (nbParameter==3) {
                return graphBuilder->addEdge(parameter[0],parameter[1],parameter[2]);
            }
            else
                return false;
        }
    };
    //=================================================================================
    struct TLPEdgesBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        TLPEdgesBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder){}
        bool addInt(const int nbEdges) {return graphBuilder->reserveEdges(nbEdges);}
        bool close()  { return true; }
    };
    //=================================================================================
    struct TLPClusterBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        int clusterId, supergraphId;
        TLPClusterBuilder(TLPGraphBuilder *graphBuilder, int supergraph=0):graphBuilder(graphBuilder), supergraphId(supergraph){}
        bool addInt(const int id) {
	  clusterId = id;
	  if (graphBuilder->version > 2.2)
	    return graphBuilder->addCluster(id, std::string(), supergraphId);
	  return true;
        }
        bool addString(const std::string& str) {
	  if (graphBuilder->version < 2.3)
	    return graphBuilder->addCluster(clusterId, str, supergraphId);
	  return true;
        }
        bool addStruct(const std::string& structName, TLPBuilder*&newBuilder);
        bool addNode (int nodeId) {
            return graphBuilder->addClusterNode(clusterId,nodeId);
        }
        bool addEdge (int edgeId) {
            return graphBuilder->addClusterEdge(clusterId,edgeId);
        }
        bool close(){
            return true;
        }
    };
    //=================================================================================
    struct TLPClusterNodeBuilder:public TLPFalse {
        TLPClusterBuilder *clusterBuilder;
        TLPClusterNodeBuilder(TLPClusterBuilder *clusterBuilder):clusterBuilder(clusterBuilder){}
        bool addInt(const int id) {return clusterBuilder->addNode(id);}
        bool addRange(int first, int second) {
            while (first <= second) {
                clusterBuilder->addNode(first);
                ++first;
            }
            return true;
        }
        bool close(){return true;}
    };
    //=================================================================================
    struct TLPClusterEdgeBuilder:public TLPFalse {
        TLPClusterBuilder *clusterBuilder;
        TLPClusterEdgeBuilder(TLPClusterBuilder *clusterBuilder):clusterBuilder(clusterBuilder){}
        bool addInt(const int id) {return clusterBuilder->addEdge(id);}
        bool addRange(int first, int second) {
            while (first <= second) {
                clusterBuilder->addEdge(first);
                ++first;
            }
            return true;
        }
        bool close(){return true;}
    };
    //================================================================================
    struct TLPAttributesBuilder: public TLPFalse {
        TLPGraphBuilder *graphBuilder;

        TLPAttributesBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder){}
        virtual ~TLPAttributesBuilder(){
        }
        bool close() {return true;}
        bool canRead() { return true; }
        bool read(std::istream& is) {
	  char c = ' ';
	  // go to the first non space char
	  while((is >> c) && isspace(c)) {}
	  is.unget();
	  // to read the id of the graph
	  unsigned int id;
	  if( !(is >> id) )
	    return false;
	  Graph* subgraph = id ? graphBuilder->_graph->getSubGraph(id) :
	    graphBuilder->_graph;
	  if (subgraph == NULL)
	    return false;
	  return DataSet::read(is, const_cast<DataSet &>(subgraph->getAttributes()));
      }
    };
    //================================================================================
    struct TLPDataSetBuilder: public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        DataSet dataSet;
        DataSet* currentDataSet;
        char* dataSetName;

        TLPDataSetBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),currentDataSet((DataSet *) &(graphBuilder->_graph->getAttributes())){
            dataSetName = (char *) 0;
	}
        TLPDataSetBuilder(TLPGraphBuilder *graphBuilder, char* name):graphBuilder(graphBuilder),currentDataSet(graphBuilder->dataSet){
            dataSetName = name;
            graphBuilder->dataSet->get(dataSetName, dataSet);
	    currentDataSet = &dataSet;
        }
        TLPDataSetBuilder(TLPGraphBuilder *graphBuilder,DataSet *currentDataSet):graphBuilder(graphBuilder),currentDataSet(currentDataSet){
        }
        virtual ~TLPDataSetBuilder(){
        }
        bool close() {
            if (dataSetName) {
                // handle old displaying properties
                // to ensure compatibility
                bool b = false;
                if (dataSet.get<bool>("_viewArrow", b))
                    dataSet.set("arrow", b);
                if (dataSet.get<bool>("_viewLabel", b)) {
                    dataSet.set("nodeLabel", b);
                    dataSet.set("edgeLabel", b);
                }
                if (dataSet.get<bool>("_viewNodeLabel", b))
                    dataSet.set("nodeLabel", b);
                if (dataSet.get<bool>("_viewEdgeLabel", b))
                    dataSet.set("edgeLabel", b);
                if (dataSet.get<bool>("_viewMetaLabel", b))
                    dataSet.set("metaLabel", b);
                if (dataSet.get<bool>("_viewElementOrdered", b))
                    dataSet.set("elementOrdered", b);
                if (dataSet.get<bool>("_viewStrahler", b))
                    dataSet.set("elementOrdered", b);
                if (dataSet.get<bool>("_viewAutoScale", b))
                    dataSet.set("autoScale", b);
                if (dataSet.get<bool>("_incrementalRendering", b))
                    dataSet.set("incrementalRendering", b);
                if (dataSet.get<bool>("_edgeColorInterpolate", b))
                    dataSet.set("edgeColorInterpolation", b);
                if (dataSet.get<bool>("_edgeSizeInterpolate", b))
                    dataSet.set("edgeSizeInterpolation", b);
                if (dataSet.get<bool>("_edge3D", b))
                    dataSet.set("edge3D", b);
                unsigned int ui = 0;
                if (dataSet.get<unsigned int>("_viewOrtho", ui))
                    dataSet.set("orthogonalProjection", ui ? true : false);
                if (dataSet.get<unsigned int>("_FontsType", ui))
                    dataSet.set("fontType", ui);
                graphBuilder->dataSet->set<DataSet>(dataSetName, dataSet);
            }
            return true;
        }
      bool canRead() { return true; }
      bool read(std::istream& is) {
	return DataSet::read(is, *currentDataSet);
      }
    };
    //================================================================================
    struct TLPFileInfoBuilder: public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        std::string name;

        TLPFileInfoBuilder(TLPGraphBuilder *graphBuilder, std::string infoName):
                graphBuilder(graphBuilder), name(infoName){
        }
        virtual ~TLPFileInfoBuilder(){
        }

        bool addString(const std::string &str) {
            if (name == AUTHOR)
                graphBuilder->dataSet->set<std::string>(AUTHOR, str);
            else if (name == COMMENTS)
                graphBuilder->dataSet->set<std::string>("text::comments", str);
            return true;
        }
        bool close(){
            return true;
        }
    };
    //================================================================================
    struct TLPSceneBuilder: public TLPFalse {
        TLPGraphBuilder *graphBuilder;

        TLPSceneBuilder(TLPGraphBuilder *graphBuilder):
                graphBuilder(graphBuilder){
        }
        virtual ~TLPSceneBuilder(){
        }

        bool addString(const std::string &str) {
            graphBuilder->dataSet->set<std::string>(SCENE, str);
            return true;
        }
        bool close(){
            return true;
        }
    };
    //=================================================================================
    bool TLPClusterBuilder::addStruct(const std::string& structName, TLPBuilder*&newBuilder)   {
        if (structName==CLUSTERNODES) {
            newBuilder=new TLPClusterNodeBuilder(this);
        }
        else
            if (structName==CLUSTEREDGES) {
            newBuilder=new TLPClusterEdgeBuilder(this);
        }
        else
            if (structName==CLUSTER) {
            newBuilder = new TLPClusterBuilder(graphBuilder, clusterId);
	}
	else {
            newBuilder=new TLPFalse();
            return false;
	}
        return true;
    }
    //=================================================================================
    struct TLPPropertyBuilder:public TLPFalse {
        TLPGraphBuilder *graphBuilder;
        int clusterId;
        std::string propertyType,propertyName;
        bool typeOk,nameOk;
        virtual ~TLPPropertyBuilder() {}
        TLPPropertyBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),typeOk(false),nameOk(false){}
        bool addInt(const int id)  {
            clusterId = id;
            return true;
        }
        bool addString(const std::string &str)
        {
            if (!typeOk)
            {propertyType=str;typeOk=true;}
            else
                if (!nameOk)
                {propertyName=str;nameOk=true;}
            else
                return false;
            return true;
        }
        bool setNodeValue(int nodeId, const std::string& value)  {
            return graphBuilder->setNodeValue(nodeId,clusterId,propertyType,propertyName, (std::string&) value);
        }
        bool setEdgeValue(int edgeId, const std::string& value)  {
            return graphBuilder->setEdgeValue(edgeId,clusterId,propertyType,propertyName, (std::string&) value);
        }
        bool setAllNodeValue(const std::string& value)  {
            return graphBuilder->setAllNodeValue(clusterId,propertyType,propertyName, (std::string&) value);
        }
        bool setAllEdgeValue(const std::string value)  {
            return graphBuilder->setAllEdgeValue(clusterId,propertyType,propertyName, (std::string&) value);
        }
        bool addStruct(const std::string& structName,TLPBuilder*&newBuilder);
        bool close(){return true;}
    };
    //=================================================================================
    struct TLPNodePropertyBuilder:public TLPFalse {
        TLPPropertyBuilder *propertyBuilder;
        int nodeId;
        TLPNodePropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder){}
        bool addInt(const int id)
        {nodeId=id;return true;}
        bool addString(const std::string&val)
        {return propertyBuilder->setNodeValue(nodeId,val);}
        bool close(){return true;}
    };
    //=================================================================================
    struct TLPEdgePropertyBuilder:public TLPFalse {
        TLPPropertyBuilder *propertyBuilder;
        int edgeId;
        std::string nodeValue;
        TLPEdgePropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder){}
        virtual ~TLPEdgePropertyBuilder(){}
        bool addInt(const int id)
        {edgeId=id;return true;}
        bool addString(const std::string &val)
        {return propertyBuilder->setEdgeValue(edgeId,val);}
        bool close(){return true;}
    };
    struct TLPDefaultPropertyBuilder:public TLPFalse {
        TLPPropertyBuilder *propertyBuilder;
        int edgeId;
        std::string nodeValue;
        int i;
        TLPDefaultPropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder),i(0){}
        virtual ~TLPDefaultPropertyBuilder(){}
        bool addString(const std::string &val)
        {
            if (i==0) {i++;return propertyBuilder->setAllNodeValue(val);}
            if (i==1) {i++;return propertyBuilder->setAllEdgeValue(val);}
            return false;
        }
        bool close(){return true;}
    };

} // namespace tlp
//=================================================================================
bool TLPPropertyBuilder::addStruct(const std::string& structName,TLPBuilder*&newBuilder) {
    if (structName==DEFAULTVALUE) {
        newBuilder= new TLPDefaultPropertyBuilder(this);
        return true;
    }
    else
        if (structName==NODEVALUE) {
        newBuilder=new TLPNodePropertyBuilder(this);
        return true;
    }
    else
        if (structName==EDGEVALUE) {
	newBuilder=new TLPEdgePropertyBuilder(this);
	return true;
    }
    return false;
}
//=================================================================================
bool TLPGraphBuilder::addStruct(const std::string& structName,TLPBuilder*&newBuilder) {
    if (structName==TLP) {
        inTLP = true;
        newBuilder=this;
    }
    else if (structName==NODES) {
        newBuilder=new TLPNodeBuilder(this);
    }
    else if (structName==NB_NODES) {
        newBuilder=new TLPNodesBuilder(this);
    }
    else if (structName==NB_EDGES) {
        newBuilder=new TLPEdgesBuilder(this);
    }
    else if (structName==EDGE) {
        newBuilder=new TLPEdgeBuilder(this);
    }
    else if (structName==CLUSTER) {
        newBuilder=new TLPClusterBuilder(this);
    }
    else if (structName==PROPERTY) {
        newBuilder=new TLPPropertyBuilder(this);
    }
    else if (structName==DISPLAYING) {
        newBuilder=new TLPDataSetBuilder(this, (char *) DISPLAYING);
    } else if (structName==OLD_ATTRIBUTES) {
        newBuilder=new TLPDataSetBuilder(this);
    } else if (structName==ATTRIBUTES) {
        newBuilder=new TLPAttributesBuilder(this);
    } else if (structName==SCENE) {
        newBuilder=new TLPSceneBuilder(this);
    } else if (structName==VIEWS) {
        newBuilder=new TLPDataSetBuilder(this, (char *) VIEWS);
    } else if (structName==CONTROLLER) {
        newBuilder=new TLPDataSetBuilder(this, (char *) CONTROLLER);
    }
    else
        newBuilder=new TLPFileInfoBuilder(this, structName);
    return true;
}
//================================================================================

namespace {
    const char * paramHelp[] = {
        // filename
        HTML_HELP_OPEN() \
                HTML_HELP_DEF( "type", "pathname" ) \
                HTML_HELP_BODY() \
                "This parameter defines the file pathname to import." \
                HTML_HELP_CLOSE()
            };
}
#endif //DOXYGEN_NOTFOR_DEVEL
namespace tlp {

    /** \addtogroup import */
    /*@{*/
    /// Import plugin for TLP format.
    /**
 * This plugin imports a Tulip graph structure recorded in a file
 * using the TLP format.
 * TLP is the Tulip Software Graph Format.
 * See 'Tulip-Software.org->Docs->TLP File Format' for description.
 * Note: When using the Tulip graphical user interface,
 * choosing "File->Import->TLP" menu item is the same that using
 * "File->Open" menu item.
 */
    class TLPImport:public ImportModule {
    public:
        TLPImport(AlgorithmContext context):ImportModule(context) {
            addParameter<std::string>("file::filename", paramHelp[0]);
            addParameter<DataSet>(DISPLAYING);
        }
        ~TLPImport(){}
        bool import(const std::string&) {
            std::string filename;
            std::string data;
            std::stringstream *tmpss=NULL;
            int size;
            std::istream *input;
            bool result;

            if(dataSet->exist("file::filename")){
                dataSet->get<std::string>("file::filename", filename);
                struct stat infoEntry;
                result = (stat(filename.c_str(),&infoEntry) == 0);
                if (!result) {
                    std::stringstream ess;
                    ess << filename.c_str() << ": " << strerror(errno);
                    pluginProgress->setError(ess.str());
                    std::cerr << pluginProgress->getError() << std::endl;
                    return false;
                }
                size=infoEntry.st_size;
                if (filename.rfind(".gz") == (filename.length() - 3)) {
                    input = tlp::getIgzstream(filename.c_str());
                    size *= 4;
                }
                else
		  input = new std::ifstream(filename.c_str(),
					    std::ifstream::in |
					    // consider file has binary
					    // to avoid pb using tellg
					    // on the input stream
					    std::ifstream::binary );
            }else{
                dataSet->get<std::string>("file::data", data);
                size=data.size();
                std::stringstream *tmpss=new std::stringstream;
                (*tmpss) << data;
                input = tmpss;
            }

            pluginProgress->showPreview(false);
            pluginProgress->setComment(std::string("Loading ") + filename + "...");
            TLPParser<false> myParser(*input, new TLPGraphBuilder(graph, dataSet), pluginProgress, size);
            result = myParser.parse();
            if (!result) {
                pluginProgress->setError(filename + ": " + pluginProgress->getError());
                std::cerr << pluginProgress->getError() << std::endl;
            }
            if(tmpss)
                delete tmpss;
            delete input;
            return result;
        }
    };
    IMPORTPLUGINOFGROUP(TLPImport,"tlp","Auber","16/02/2001","Tlp Import plugin","1.0","File");
    /*@}*/
}
