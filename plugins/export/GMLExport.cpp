/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <iostream>
#include <iomanip>

#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

void printFloat(ostream &os, const string &name, float f) {
  /*float a=floor(f);
  float b=(f-a)*1000;
  if (b<0) b*=-1;*/
  os << name << f << endl;
}

void printCoord(ostream &os, const Coord &v) {
  printFloat(os, "x ", v.getX());
  printFloat(os, "y ", v.getY());
  printFloat(os, "z ", v.getZ());
}
void printPoint(ostream &os, const Coord &v) {
  os << "point [" << endl;
  printCoord(os, v);
  os << "]" << endl;
}
void printSize(ostream &os, const Size &v) {
  printFloat(os, "h ", v.getW());
  printFloat(os, "w ", v.getH());
  printFloat(os, "d ", v.getD());
}

/**
 * This plugin records a Tulip graph structure using the GML File format.
 * This format is the file format used by Graphlet.
 * See www.infosun.fim.uni-passau.de/Graphlet/GML/ for details.
 */
class GMLExport : public tlp::ExportModule {
public:
  PLUGININFORMATION("GML Export", "Auber David", "31/07/2001",
                    "<p>Supported extensions: gml</p><p>Exports a Tulip graph in a file using the "
                    "GML format (used by Graphlet).<br/>See: "
                    "<a "
                    "href=\"http://www.infosun.fim.uni-passau.de/Graphlet/GML/\">http://"
                    "www.infosun.fim.uni-passau.de/Graphlet/GML/</a> for details.</p>",
                    "1.0", "File")
  std::string fileExtension() const override {
    return "gml";
  }

  GMLExport(tlp::PluginContext *context) : tlp::ExportModule(context) {}

  ~GMLExport() override {}

  string convert(const string &tmp) {
    string newStr;

    for (unsigned int i = 0; i < tmp.length(); i++) {
      if (tmp[i] == '\"')
        newStr += "\\\"";
      else
        newStr += tmp[i];
    }

    return newStr;
  }

  bool exportGraph(ostream &os) override {

    os << "graph [" << endl;
    os << "directed 1" << endl;
    os << "version 2" << endl;

    LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
    StringProperty *label = graph->getProperty<StringProperty>("viewLabel");
    //    IntegerProperty *shape
    //    =getProperty<IntegerProperty>(graph->getPropertyManager(),"viewShape");
    ColorProperty *colors = graph->getProperty<ColorProperty>("viewColor");
    SizeProperty *sizes = graph->getProperty<SizeProperty>("viewSize");
    // Save Nodes

    for (auto n : graph->nodes()) {
      os << "node [" << endl;
      os << "id " << n.id << endl;
      os << "label \"" << convert(label->getNodeValue(n)) << "\"" << endl;
      os << "graphics [" << endl;
      printCoord(os, layout->getNodeValue(n));
      printSize(os, sizes->getNodeValue(n));
      os << "type \"rectangle\"" << endl;
      os << "width 0.12" << endl;
      os << "fill \"#" << hex << setfill('0') << setw(2) << int(colors->getNodeValue(n).getR())
         << hex << setfill('0') << setw(2) << int(colors->getNodeValue(n).getG()) << hex
         << setfill('0') << setw(2) << int(colors->getNodeValue(n).getB()) << "\"" << endl;

      //      os << "outline \"#"<< hex << setfill('0') << setw(2)
      //      <<(int)colors->getNodeValue(n).getR()
      //         << hex << setfill('0') << setw(2) <<(int)colors->getNodeValue(n).getG()
      //         << hex << setfill('0') << setw(2) <<(int)colors->getNodeValue(n).getB() <<
      //         "\""<< endl;

      os << "outline \"#000000\"" << endl;
      os << dec << setfill(' ') << setw(6) << "]" << endl;
      os << ']' << endl;
    }

    // Save edges
    for (auto e : graph->edges()) {
      auto ends = graph->ends(e);
      os << "edge [" << endl;
      os << "source " << ends.first.id << endl;
      os << "target " << ends.second.id << endl;
      os << "id " << e.id << endl;
      os << "label \"" << label->getEdgeValue(e) << "\"" << endl;
      os << "graphics [" << endl;
      os << "type \"line\"" << endl;
      os << "arrow \"last\"" << endl;
      os << "width 0.1" << endl;
      os << "Line [" << endl;
      vector<Coord> lcoord;
      vector<Coord>::const_iterator it;
      lcoord = layout->getEdgeValue(e);

      if (!lcoord.empty()) {
        printPoint(os, layout->getNodeValue(ends.first));
      }

      for (it = lcoord.begin(); it != lcoord.end(); ++it) {
        printPoint(os, *it);
      }

      if (!lcoord.empty()) {
        printPoint(os, layout->getNodeValue(ends.second));
      }

      os << "]" << endl;
      os << "]" << endl;
      os << "]" << endl;
    }

    os << "]" << endl;
    return true;
  }
};

PLUGIN(GMLExport)
