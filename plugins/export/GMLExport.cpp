/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <cmath>
#include <iostream>
#include <iomanip>

#include <tulip/TulipPlugin.h>
#include <tulip/AbstractProperty.h>

using namespace std;
using namespace tlp;

void printFloat(ostream &os, const string &name , float f){
  /*float a=floor(f);
  float b=(f-a)*1000;
  if (b<0) b*=-1;*/
  os << name << f << endl; 
}

void printCoord(ostream &os,const Coord &v){
  printFloat(os,"x ",v.getX());
  printFloat(os,"y ",v.getY());
  printFloat(os,"z ",v.getZ());
}
void printPoint(ostream &os,const Coord &v){ 
  os << "point [" << endl;
  printCoord(os,v);
  os << "]" << endl;
}
void printSize(ostream &os,const Size &v){
  printFloat(os,"h ",v.getW());
  printFloat(os,"w ",v.getH());
  printFloat(os,"d ",v.getD());
}

/** \addtogroup export */
/*@{*/
/// Export plugin for GML format.
/**
 * This plugin records a Tulip graph structure using the GML File format.
 * This format is the file format used by Graphlet.
 * See www.infosun.fmi.uni-passau.de/Graphlet/GML/ for details.
 */
class GDFExport:public ExportModule {
public:
  GDFExport(AlgorithmContext context):ExportModule(context)
  {}

  ~GDFExport(){}

  string convert(string tmp) {
    string newStr;
    for (unsigned int i=0;i<tmp.length();i++) {
      if (tmp[i]=='\"')
	newStr+="\\\"";
      else
	newStr+=tmp[i];
    }
    return newStr;
  }

  bool exportGraph(ostream &os,Graph *currentGraph) {
  os << "nodedef> name,x,y" << endl; //jobfun VARCHAR(100),dept VARCHAR(32)

  LayoutProperty *layout = currentGraph->getProperty<LayoutProperty>("viewLayout");
  node n;
  forEach(n, currentGraph->getNodes()) {
  os << "v" << n.id << "," << layout->getNodeValue(n).getX() << "," << layout->getNodeValue(n).getY() << endl;
  }
  os << "edgedef> node1,node2" << endl;
  edge e;
  forEach (e, currentGraph->getEdges()) {
    os << "v" << currentGraph->source(e).id << ",v" << currentGraph->target(e).id << endl;
  }
  os << endl;
    return true;
  }
};
/*@}*/
EXPORTPLUGIN(GDFExport,"GDF","Auber David","31/07/2001","GDF Export plugin","1.0")
