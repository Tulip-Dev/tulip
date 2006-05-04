//-*-c++-*-
#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Graph.h"
#include "LayoutProperty.h"
#include "DoubleProperty.h"
#include "StringProperty.h"
#include "BooleanProperty.h"
#include "ColorProperty.h"
#include "IntegerProperty.h"
#include "SizeProperty.h"
#include "GraphProperty.h"
#include "PluginProgress.h"
#include "Reflect.h"
#include "ExtendedClusterOperation.h"
#include "ExportModule.h"
#include "Clustering.h"
#include "ImportModule.h"

namespace tlp {
  extern TLP_SCOPE const char PATH_DELIMITER;
  extern TLP_SCOPE std::string TulipLibDir;
  extern TLP_SCOPE std::string TulipPluginsPath;
  extern TLP_SCOPE std::string TulipDocProfile;
  extern TLP_SCOPE std::string TulipUserHandBookIndex;
  
  extern TLP_SCOPE void initTulipLib(char* appDirPath = 0);
    
  TLP_SCOPE void loadPlugins(PluginLoader *plug=0);
  TLP_SCOPE bool loadPlugin(const std::string & filename, PluginLoader *plug=0);

  /**
   * Load a sg in the tlp format
   * Warning : this function use "tlp" import plugin (must be laoded)
   */
  TLP_SCOPE Graph * loadGraph(const std::string &filename);
  /**
   * Save a sg in tlp format
   * Warning : this function use "tlp" export plugin (must be laoded)
   */
  TLP_SCOPE bool saveGraph(Graph *, const std::string &filename);
  TLP_SCOPE Graph * importGraph(const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0,Graph *newGraph=0);
  TLP_SCOPE bool exportGraph(Graph *sg,std::ostream  &os,const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0);
  TLP_SCOPE bool clusterizeGraph(Graph *sg,std::string &errorMsg, DataSet *dataSet =0,const std::string &alg="hierarchical" , PluginProgress *plugProgress=0);
  /**
   * Return a new sg
   */
  TLP_SCOPE Graph* newGraph();
  /**
   *  Return an empty subsg 
   */
  TLP_SCOPE Graph *newSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  Return a subsg equal to the sg given in parameter (a clone subsg)
   */
  TLP_SCOPE Graph *newCloneSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  find the first node of degree 0, if no node exist return false else true
   */
  TLP_SCOPE bool getSource(Graph *, node &n);
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
   * Append the selected part of the sg inG (properties, nodes & edges) into the sg outG.
   * If no selection is done (inSel=NULL), the whole inG sg is appended.
   * The output selection is used to select the appended nodes & edges
   * \warning The input selection is extended to all selected edge ends.
   */
  TLP_SCOPE void copyToGraph( Graph * outG, Graph *	inG, BooleanProperty* inSelection=0, BooleanProperty* outSelection=0 );
  /**
   * Remove the selected part of the sg ioG (properties, nodes & edges).
   * If no selection is done (inSel=NULL), the whole sg is reseted to default value.
   * \warning The selection is extended to all selected edge ends.
   */
  TLP_SCOPE void removeFromGraph(Graph * ioG, BooleanProperty* inSelection = 0 );
  
};

#endif
