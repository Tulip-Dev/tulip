#include <math.h>
#include <sstream>
#include <list>

#include <tulip/SuperGraph.h>
#include <tulip/StableIterator.h>

#include "PlaneClustering.h"

#define EPSILON 1.0e-8

using namespace std;
using namespace tlp;

CLUSTERINGPLUGIN(PlaneClustering, "Plane Clustering", "Maxime Delorme", "27/07/2005", "Alpha", "0", "1");

namespace 
{
  const char * paramHelp[] = 
    {
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "values", "A float value" ) \
    HTML_HELP_DEF( "default", "1.0" ) \
    HTML_HELP_BODY() \
    "This parameter defines the first coordinate of the clustering plane." \
    HTML_HELP_CLOSE(),

    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "values", "A float value" ) \
    HTML_HELP_DEF( "default", "1.0" ) \
    HTML_HELP_BODY() \
    "This parameter defines the second coordinate of the clustering plane." \
    HTML_HELP_CLOSE(),

    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "values", "A float value" ) \
    HTML_HELP_DEF( "default", "1.0" ) \
    HTML_HELP_BODY() \
    "This parameter defines the third coordinate of the clustering plane." \
    HTML_HELP_CLOSE(),

    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "values", "A float value" ) \
    HTML_HELP_DEF( "default", "1.0" ) \
    HTML_HELP_BODY() \
    "This parameter defines the last coordinate of the clustering plane." \
    HTML_HELP_CLOSE()
    };
};

PlaneClustering::PlaneClustering(ClusterContext context):Clustering(context)
{
  addParameter<float>("CoordA", paramHelp[0], "1.0f");
  addParameter<float>("CoordB", paramHelp[1], "1.0f");
  addParameter<float>("CoordC", paramHelp[2], "1.0f");
  addParameter<float>("CoordD", paramHelp[3], "1.0f");
}

PlaneClustering::~PlaneClustering()
{
}

bool PlaneClustering::run() 
{
  LayoutProxy *layout = superGraph->getProperty<LayoutProxy>("viewLayout");

  SuperGraph* sup = superGraph->addSubGraph();
  SuperGraph* inf = superGraph->addSubGraph();
  
  sup->setAttribute("name", string("sup"));
  inf->setAttribute("name", string("inf"));
  

  StableIterator<node> itN(superGraph->getNodes());
  
  while (itN.hasNext())
    {
      node n         = itN.next();
      Coord pos      = layout->getNodeValue(n);
      float planeval = plane.planePointValue(pos);

      if (planeval < 0)
	inf->addNode(n);
      else
	sup->addNode(n);
    }

  StableIterator<edge> itE(superGraph->getEdges());

  while (itE.hasNext())
    {
      edge e = itE.next();
      Coord start, end;
      
      start = layout->getNodeValue(superGraph->source(e));
      end   = layout->getNodeValue(superGraph->target(e));

      float sval, eval;

      sval = plane.planePointValue(start);
      eval = plane.planePointValue(end);

      if (sval * eval >= 0)
	{
	  if (sval < 0)
	    inf->addEdge(e);
	  else
	    sup->addEdge(e);
	}
    }

  return true;
}

bool PlaneClustering::check(string &erreurMsg)
{
  if (dataSet == 0 || !dataSet->get("CoordA", plane.a))
    plane.a = 1.0;

  if (dataSet == 0 || !dataSet->get("CoordB", plane.b))
    plane.b = 1.0;

  if (dataSet == 0 || !dataSet->get("CoordC", plane.c))
    plane.c = 1.0;

  if (dataSet == 0 || !dataSet->get("CoordD", plane.d))
    plane.d = 1.0; 

  float da, db, dc, dd;

  da = fabs(plane.a);
  db = fabs(plane.b);
  dc = fabs(plane.c);
  dd = fabs(plane.d);

  if (da < EPSILON && db < EPSILON && dc < EPSILON && dd < EPSILON)
    {
      erreurMsg = "This is not a valid plane !";
      return false;
    }

  erreurMsg = "";
  return true;
}

void PlaneClustering::reset()
{
}




