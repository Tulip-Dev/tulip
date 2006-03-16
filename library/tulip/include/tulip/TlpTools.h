//-*-c++-*-
#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SuperGraph.h"
#include "LayoutProxy.h"
#include "MetricProxy.h"
#include "StringProxy.h"
#include "SelectionProxy.h"
#include "ColorsProxy.h"
#include "IntProxy.h"
#include "SizesProxy.h"
#include "MetaGraphProxy.h"
#include "PluginProgress.h"
#include "Reflect.h"
#include "ExtendedClusterOperation.h"

namespace tlp {
  extern TLP_SCOPE const char PATH_DELIMITER;
  extern TLP_SCOPE std::string TulipLibDir;
  extern TLP_SCOPE std::string TulipPluginsPath;
  extern TLP_SCOPE std::string TulipDocProfile;
  extern TLP_SCOPE std::string TulipUserHandBookIndex;
  
  extern TLP_SCOPE void initTulipLib(char* appDirPath = 0);
    
  extern TLP_SCOPE TemplateFactory<ClusteringFactory,Clustering,ClusterContext > clusteringFactory;
  extern TLP_SCOPE TemplateFactory<ImportModuleFactory,ImportModule,ClusterContext > importFactory;
  extern TLP_SCOPE TemplateFactory<ExportModuleFactory,ExportModule,ClusterContext > exportFactory;
  TLP_SCOPE void loadPlugins(PluginLoader *plug=0);
  TLP_SCOPE void loadPlugin(const std::string & filename, PluginLoader *plug=0);

  /**
   * Load a graph in the tlp format
   * Warning : this function use "tlp" import plugin (must be laoded)
   */
  TLP_SCOPE SuperGraph * load(const std::string &filename);
  /**
   * Save a graph in tlp format
   * Warning : this function use "tlp" export plugin (must be laoded)
   */
  TLP_SCOPE bool save(SuperGraph *, const std::string &filename);
  TLP_SCOPE SuperGraph * importGraph(const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0,SuperGraph *newSuperGraph=0);
  TLP_SCOPE bool exportGraph(SuperGraph *sg,std::ostream  &os,const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0);
  TLP_SCOPE bool clusterizeGraph(SuperGraph *sg,std::string &errorMsg, DataSet *dataSet =0,const std::string &alg="hierarchical" , PluginProgress *plugProgress=0);
  /**
   * Return a new graph
   */
  TLP_SCOPE SuperGraph* newSuperGraph();
  /**
   *  Return an empty subgraph 
   */
  TLP_SCOPE SuperGraph *newSubGraph(SuperGraph *, std::string name = "unnamed");
  /**
   *  Return a subgraph equal to the graph given in parameter (a clone subgraph)
   */
  TLP_SCOPE SuperGraph *newCloneSubGraph(SuperGraph *, std::string name = "unnamed");
  /**
   *  find the first node of degree 0, if no node exist return false else true
   */
  TLP_SCOPE bool getSource(SuperGraph *, node &n);
  /**
   * Return an istream from a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  TLP_SCOPE std::istream *getIgzstream(const char *name, int open_mode = std::ios::in);
  /**
   * Return an ostream to write to a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  TLP_SCOPE std::ostream *getOgzstream(const char *name, int open_mode = std::ios::out);
  /**
   * Append the selected part of the graph inG (properties, nodes & edges) into the graph outG.
   * If no selection is done (inSel=NULL), the whole inG graph is appended.
   * The output selection is used to select the appended nodes & edges
   * \warning The input selection is extended to all selected edge ends.
   */
  TLP_SCOPE void copyToGraph( SuperGraph * outG, SuperGraph *	inG, SelectionProxy* inSelection=0, SelectionProxy* outSelection=0 );
  /**
   * Remove the selected part of the graph ioG (properties, nodes & edges).
   * If no selection is done (inSel=NULL), the whole graph is reseted to default value.
   * \warning The selection is extended to all selected edge ends.
   */
  TLP_SCOPE void removeFromGraph(	SuperGraph * ioG, SelectionProxy* inSelection = 0 );
  
};

#endif
