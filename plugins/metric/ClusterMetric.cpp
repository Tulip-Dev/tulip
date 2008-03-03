#include <deque>
#include "ClusterMetric.h"

DOUBLEPLUGINOFGROUP(ClusterMetric,"Cluster","David Auber","26/02/2003","Beta","1.0","Graph");

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "[1 , |G|]" ) \
    HTML_HELP_DEF( "default", "1" ) \
    HTML_HELP_BODY() \
    "This parameter defines the maximal depth of a computed cluster." \
    HTML_HELP_CLOSE()
  };
}
//=================================================
ClusterMetric::ClusterMetric(const PropertyContext &context):DoubleAlgorithm(context) {
  addParameter<unsigned int>("depth",paramHelp[0],"1");
}
//=================================================
bool ClusterMetric::run() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  unsigned int maxDepth=1;
  if (dataSet!=0) dataSet->get("depth",maxDepth);
  return computeClusterMetric(graph, doubleResult, maxDepth, pluginProgress);
}

