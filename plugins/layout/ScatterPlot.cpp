#include <map>
#include <cmath>
#include <qmessagebox.h>
#include "ScatterPlot.h"

LAYOUTPLUGINOFGROUP(ScatterPlot, "Scatter Plot", "M.Delorme", "25/07/2005", "Alpha", "1.0", "Misc");

using namespace std;
using namespace tlp;

namespace 
{
  const char * paramHelp[] = 
    {
    // Used Metrics 1 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Metric" ) \
    HTML_HELP_DEF( "values", "An existing metric property" ) \
    HTML_HELP_DEF( "default", "viewMetric" ) \
    HTML_HELP_BODY() \
    "This parameter defines the first metric used to compute the scatter plot." \
    HTML_HELP_CLOSE(),

    // Used Metrics 2 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Metric" ) \
    HTML_HELP_DEF( "values", "An existing metric property" ) \
    HTML_HELP_DEF( "default", "viewMetric" ) \
    HTML_HELP_BODY() \
    "This parameter defines the second metric used to compute the scatter plot." \
    HTML_HELP_CLOSE(),

    // Used Metrics 3 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Metric" ) \
    HTML_HELP_DEF( "values", "An existing metric property" ) \
    HTML_HELP_DEF( "default", "viewMetric" ) \
    HTML_HELP_BODY() \
    "This parameter defines the third metric used to compute the scatter plot." \
    HTML_HELP_CLOSE(),

    // Discretization Step 1 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "double" ) \
    HTML_HELP_DEF( "values", "]0; +infinite[" ) \
    HTML_HELP_DEF( "default", "1" ) \
    HTML_HELP_BODY() \
    "This parameter defines the discretization step used for discretization of the first metric of the scatter plot" \
    HTML_HELP_CLOSE(),

    // Discretization Step 2 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "double" ) \
    HTML_HELP_DEF( "values", "]0; +infinite[" ) \
    HTML_HELP_DEF( "default", "1" ) \
    HTML_HELP_BODY() \
    "This parameter defines the discretization step used for discretization of the second metric of the scatter plot" \
    HTML_HELP_CLOSE(),

    // Discretization Step 3 :
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "double" ) \
    HTML_HELP_DEF( "values", "]0; +infinite[" ) \
    HTML_HELP_DEF( "default", "1" ) \
    HTML_HELP_BODY() \
    "This parameter defines the discretization step used for discretization of the third metric of the scatter plot" \
    HTML_HELP_CLOSE(),

    // nMetrics
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "[1; 3]" ) \
    HTML_HELP_DEF( "default", "3" ) \
    HTML_HELP_BODY() \
    "This parameter defines the number of metrics used to compute the scatter plot." \
    HTML_HELP_CLOSE(),

    // Shape Conversion
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter enables the shape conversion of the scatterplot so that the final graph is only composed of cubes." \
    HTML_HELP_CLOSE(),
  };
}

ScatterPlot::ScatterPlot(const PropertyContext &context) : LayoutAlgorithm(context) 
{
  addParameter<DoubleProperty>("usedMetric1", paramHelp[0], "viewMetric");
  addParameter<DoubleProperty>("usedMetric2", paramHelp[1], "viewMetric");
  addParameter<DoubleProperty>("usedMetric3", paramHelp[2], "viewMetric");

  addParameter<double>("discretizationStep1", paramHelp[3], "1.0");
  addParameter<double>("discretizationStep2", paramHelp[4], "1.0");
  addParameter<double>("discretizationStep3", paramHelp[5], "1.0");

  addParameter<int>("nMetrics", paramHelp[6], "3");
  addParameter<bool>("shapeConversion", paramHelp[7], "true");
}

ScatterPlot::~ScatterPlot() 
{
}

bool ScatterPlot::run() 
{
  Iterator<node> *itN = graph->getNodes();

  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  
  if (shapeConversion)
    graph->getLocalProperty<IntegerProperty>("viewShape")->setAllNodeValue(1);
  
  // The histogram if we only have 1 metric
  std::map<int, int> histogram;
  
  int histoMax = 0;
  
  while (itN->hasNext())
    {
      node n = itN->next();
	
      float nodeVal[3];
      
      for(int i=0; i < nMetrics; i++)
	nodeVal[i] = metric[i]->getNodeValue(n);
      
      int histoVal[3];

      for(int i=0; i < 3; i++)
	histoVal[i] = 0;
      
      for(int i=0; i < nMetrics; i++)
	histoVal[i] = (int)(nodeVal[i] / disc[i]);
      
      if (nMetrics == 1)
	{
	  histoVal[1] = histogram[histoVal[0]];
	  
	  if (histoVal[1] > histoMax)
	    histoMax = histoVal[1];
	  
	  histogram[histoVal[0]]++;
	}
      
      // We have to revert Y axis to make the histogram looks good
      layoutResult->setNodeValue(n, Coord((double)histoVal[0], (double)-histoVal[1], (double)histoVal[2]));      
    }
  
  delete itN;
  
  layoutResult->setAllEdgeValue(vector<Coord>(0));

  dataSet->set("histoMax", histoMax);

  return true;
}

bool ScatterPlot::check(string &erreurMsg) 
{
  if (dataSet == 0 || !dataSet->get("nMetrics", nMetrics)) 
    nMetrics = 3;

  else if (nMetrics < 1 || nMetrics > 3)
    {
      erreurMsg = "Invalid number of metric";
      return false;
    }

  for(int i=0; i < 3; i++)
    {
      char n = '1' + i;

      char dtxt[20] = "discretizationStep1";

      dtxt[18] = n;
      
      if (dataSet == 0 || !dataSet->get(dtxt, disc[i]))
	disc[i] = 1;

      else if (disc[i] <= 0)
	{
	  erreurMsg = "Invalid discretization step (must be in the interval : ]0; +infinite[)";
	  return false;
	}

      char mtxt[12] = "usedMetric1";
      mtxt[10] = n;

      if (dataSet == 0 || !dataSet->get(mtxt, metric[i]))
	metric[i] = graph->getProperty<DoubleProperty>("viewMetric");
    }

  if (dataSet == 0 || !dataSet->get("shapeConversion", shapeConversion))
    shapeConversion = true;

  return true;
}
