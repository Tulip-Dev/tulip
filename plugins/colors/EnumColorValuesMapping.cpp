#include <stdio.h>
#include <sstream>

#include <tulip/Graph.h>
#include <tulip/hash_string.h>
#include <tulip/StableIterator.h>
#include <tulip/TulipPlugin.h>
#include <tulip/Color.h>
#include <tulip/TlpQtTools.h>
#include <QtGui/qmessagebox.h>

#include <tulip/GlRect.h>
#include <tulip/GlLabel.h>

//================================================================================
using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "PropertyInterface*" ) \
    HTML_HELP_BODY() \
    "Specify the property that will be used to partition the graph" \
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF("Values", "nodes <BR> edges") \
    HTML_HELP_DEF( "default", "nodes" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of graph elements to partition"	\
    HTML_HELP_CLOSE()
  };
}
#define ELT_TYPE "Type"
#define ELT_TYPES "nodes;edges;"
#define NODE_ELT 0
#define EDGE_ELT 1

char *defColors[12] = {
  "(255,0,0,255)", "(0,255,0,255)", "(0,0,255,255)", "(255,255,0,255)",
  "(0,255,255,255)", "(255,0,255,255)", "(96,159,191,255)", "(0,191,112,255)",
  "(255,106,0,255)",  "(238,255,170,255)", "(255,205,205,255)", "(191,143,0,255)"};

class EnumColorValuesMapping: public ColorAlgorithm {
  // the property
  PropertyInterface *property;
  // flag for element type
  bool onNodes;
  // user choosen colors
  stdext::hash_map<string, Color> colors;
  
public:
  //================================================================================
  EnumColorValuesMapping(const PropertyContext &context):ColorAlgorithm(context) {
    addParameter<PropertyInterface*>("Property", paramHelp[0], "viewMetric");
    addParameter<StringCollection>(ELT_TYPE, paramHelp[1], ELT_TYPES);
  }
  //==============================================================================
  bool check(string &err) {
    string tmp1,tmp2;
    property=0;
    StringCollection eltTypes(ELT_TYPES);
    eltTypes.setCurrent(0);
    if (dataSet!=0) {
      dataSet->get("Property", property);  
      dataSet->get(ELT_TYPE, eltTypes);
    }

    if (property == 0)
      property = graph->getProperty("viewMetric");

    onNodes = eltTypes.getCurrent() == NODE_ELT;

    // check the number of different values
    int step = 0, maxSteps;
    if (pluginProgress) {
      pluginProgress->showPreview(false);
      pluginProgress->setComment(onNodes ? "Partitioning nodes..." : "Partitioning edges");
    }

    set<string> partitions;
    if (onNodes) {
      StableIterator<node> itN(graph->getNodes());
      maxSteps = graph->numberOfNodes();
      if (maxSteps < 100)
	maxSteps = 100;
      while (itN.hasNext()) {
	string tmp = property->getNodeStringValue(itN.next());
	if (partitions.find(tmp) == partitions.end()) {
	  partitions.insert(tmp);
	  if (partitions.size() > 12) {
	    err = "too much different values found (> 12)";
	    return false;
	  }
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE) {
	      err = "stopped by user";
	      return false;
	    }
	  }
	}
      }
    } else {
      StableIterator<edge> itE(graph->getEdges());
      maxSteps = graph->numberOfEdges();
      if (maxSteps < 100)
	maxSteps = 100;
      while(itE.hasNext()) {
	edge ite = itE.next();
	string tmp = property->getEdgeStringValue(itE.next());
	if (partitions.find(tmp) == partitions.end()) {
	  partitions.insert(tmp);
	  if (partitions.size() > 12) {
	    err = "too much different values found (> 12)";
	    return false;
	  }
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE) {
	      err = "stopped by user";
	      return false;
	    }
	  }
	}
      }
    }

    // build parameters
    DataSet dSet;
    StructDef dParams;
    set<string>::const_iterator it;
    unsigned int i = 0;
    for (it = partitions.begin(); it != partitions.end(); ++it, ++i) {
      stringstream sstr;
      sstr << "Color #" << i;
      string label(sstr.str());
      sstr << " will be used for elements whose value is " << (*it).c_str() << '.';
      dParams.add<Color>(label.c_str(), sstr.str().c_str(), defColors[i], true);
    }
    dParams.buildDefaultDataSet(dSet);
    if (tlp::openDataSetDialog(dSet, 0, &dParams,
			       &dSet, "Enter Color values")) {
      // get user choosen colors
      for (i = 0, it = partitions.begin(); it != partitions.end(); ++it, ++i) {
	stringstream sstr;
	sstr << "Color #" << i;
	Color color;
	if (!dSet.get(sstr.str(), color))
	  cerr << sstr.str().c_str() << " not found" << endl;
	colors[*it] = color;
      }
      return true;
    }
    err = "stopped by user";
    return false;
  }

  bool run() {
    if (onNodes) {
      node n;
      forEach(n, graph->getNodes())
	colorResult->setNodeValue(n, colors[property->getNodeStringValue(n)]);
    } else {
      edge e;
      forEach(e, graph->getEdges())
	colorResult->setEdgeValue(e, colors[property->getEdgeStringValue(e)]);
    }
    
    *dataSet = DataSet();
    GlComposite *composite=new GlComposite();
    DataSet mainDataSet;
    DataSet entityDataSet;
    vector<GlRect *> rectVector;
    vector<GlLabel *> labelVector;
    int i=0;
    int xMax=0;
    for(stdext::hash_map<string, Color>::iterator it=colors.begin();it!=colors.end();++it) {
      GlRect *rect=new GlRect(Coord(15,25+i*15,0),Coord(25,15+i*15,0),(*it).second,(*it).second);
      GlLabel *label=new GlLabel(TulipBitmapDir,Coord(30,20+i*15,0),Coord(5+12*(*it).first.size(),15,0),Color(0,0,0,255),true);
      label->setText((*it).first);
      if(30+label->getSize()[0]>xMax)
	xMax=30+label->getSize()[0];
      
      rectVector.push_back(rect);
      labelVector.push_back(label);
      i++;
    }
    GlRect *rect=new GlRect(Coord(10,15+i*15,0),Coord(xMax+5,10,0),Color(0,0,0,50),Color(0,0,0,50));
    composite->addGlEntity(rect,"rect");
    i=0;
    for(stdext::hash_map<string, Color>::iterator it=colors.begin();it!=colors.end();++it) {
      stringstream sstr;
      sstr << i ;
      composite->addGlEntity(rectVector[i],"color"+sstr.str());
      composite->addGlEntity(labelVector[i],"label"+sstr.str());
      i++;
    }
    mainDataSet.set<int>("caption",(int)composite);
    entityDataSet.set<DataSet>("Foreground",mainDataSet);
    dataSet->set<DataSet>("entities",entityDataSet);

    return true;
  }
  //================================================================================
};


COLORPLUGIN(EnumColorValuesMapping,"Enumerated Values Mapping", "P.Mary", "27/06/2007", "Color enumerated values mapping plugin","1.0");
