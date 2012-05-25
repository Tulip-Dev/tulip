/*

    Copyright (C) 2010 Antoine Lambert

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <tulip/ImportModule.h>
#include <tulip/TulipPlugin.h>
#include <tulip/ForEach.h>

#include <QtXml/QXmlStreamReader>
#include <QtCore/QFile>


using namespace tlp;
using namespace std;

/*
   Tulip GEXF format import plugin
   This plugin allow to import graph saved in GEXF format (proper to Gephi software).
   GEXF is a xml graph format, a sample file describing a graph with two nodes and one edge is given below :

<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.1draft" version="1.1" xmlns:viz="http://www.gexf.net/1.1draft/viz" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.gexf.net/1.1draft http://www.gexf.net/1.1draft/gexf.xsd">
  <meta lastmodifieddate="2010-10-22">
    <creator>Gephi 0.7</creator>
    <description></description>
  </meta>
  <graph defaultedgetype="undirected" timeformat="double" mode="dynamic">
  <attributes class="node" mode="static">
      <attribute id="string_attribute" title="string_attribute" type="string"></attribute>
      <attribute id="float_attribute" title="float_attribute" type="float"></attribute>
  </attributes>
  <attributes class="edge" mode="static">
      <attribute id="weight" title="weight" type="float"></attribute>
  </attributes>
    <nodes>
      <node id="1" label="node 1">
        <attvalues>
          <attvalue for="string_attribute" value="foo"></attvalue>
          <attvalue for="float_attribute" value="0.0"></attvalue>
	</attvalues>
        <viz:size value="10.0"></viz:size>
        <viz:position x="-173.14468" y="170.76477"></viz:position>
        <viz:color r="93" g="172" b="194"></viz:color>
      </node>
      <node id="2" label="node2">
        <attvalues>
          <attvalue for="string_attribute" value="bar"></attvalue>
          <attvalue for="float_attribute" value="1.0"></attvalue>
	</attvalues>
        <viz:size value="10.0"></viz:size>
        <viz:position x="-7.442688" y="61.904907"></viz:position>
        <viz:color r="93" g="172" b="194"></viz:color>
      </node>
    </nodes>
    <edges>
      <edge source="1" target="2">
        <attvalues>
          <attvalue for="weight" value="1.0"></attvalue>
        </attvalues>
      </edge>
    </edges>
  </graph>
</gexf>

*/

class GEXFImport : public ImportModule { // import plugins must inherit class tlp::ImportModule

public :

	GEXFImport(AlgorithmContext context):ImportModule(context) {
		// add a file parameter for the pugin
		// when the plugin will be called, a file chooser will be automatically created by Tulip
		addInParameter<string>("file::filename","");
		addInParameter<bool>("Curved edges","", "false");
	}

	~GEXFImport() {}

	// import plugins must implement bool import(const string &)
	bool import(const string&) {
		string filename;
		bool curvedEdges = false;
		// get the filename choosed by the user
		dataSet->get<string>("file::filename", filename);
		dataSet->get<bool>("Curved edges", curvedEdges);

		QString qfilename(filename.c_str());
		// if wrong extension, abort
		if (!qfilename.endsWith(".gexf")) {
			return false;
		}

		// get Tulip visual attributes properties associated to the empty graph we want to fill
		viewLayout = graph->getProperty<LayoutProperty>("viewLayout");
		viewLabel = graph->getProperty<StringProperty>("viewLabel");
		viewSize = graph->getProperty<SizeProperty>("viewSize");
		viewColor = graph->getProperty<ColorProperty>("viewColor");
		viewShape = graph->getProperty<IntegerProperty>("viewShape");

		nodesHaveCoordinates = false;

		// Open the GEXF file choosed by the user
		QFile *xmlFile = new QFile(qfilename);
		xmlFile->open(QIODevice::ReadOnly | QIODevice::Text);

		// Instantiate a QXmlStreamReader to parse the file (GEXF is xml)
		QXmlStreamReader xmlReader(xmlFile);
		// Parse each line of the file
		while (!xmlReader.atEnd()) {
			// create Tulip Properties from Gephi attributes
			if (xmlReader.isStartElement() && xmlReader.name() == "attributes") {
				createPropertiesFromAttributes(xmlReader);

				// parse graph node data
			} else if (xmlReader.isStartElement() && xmlReader.name() == "node") {
				parseNode(xmlReader);

				// parse graph edge data
			} else if (xmlReader.isStartElement() && xmlReader.name() == "edge") {
				parseEdge(xmlReader);
			}

			xmlReader.readNext();
		}
		delete xmlFile;

		// Special case : some GEXF files declare edges before nodes
		// so we have to add edges once nodes have been parsed
		if (edgesTmp.size() > 0) {
			for (size_t i = 0 ; i < edgesTmp.size() ; ++i) {
				graph->addEdge(nodesMap[edgesTmp[i].first], nodesMap[edgesTmp[i].second]);
			}
		}

		viewShape->setAllNodeValue(14);

		// Set edges to be rendered as Cubic Bézier curves and
		// compute curve control points for each edge
		if (curvedEdges && nodesHaveCoordinates) {
			viewShape->setAllEdgeValue(4);
			curveGraphEdges();
		}

		return true;
	}

	// Create a set of Tulip Properties from the attributes declared in the GEXF file
	// according to data types
	void createPropertiesFromAttributes(QXmlStreamReader &xmlReader) {
		bool nodeProperties = xmlReader.attributes().value("class") == "node";
		map<string, PropertyInterface *> *propertiesMap = NULL;
		if (nodeProperties) {
			propertiesMap = &nodePropertiesMap;
		} else {
			propertiesMap = &edgePropertiesMap;
		}
		while (!(xmlReader.isEndElement() && xmlReader.name() == "attributes")) {
			xmlReader.readNext();
			// create a Tulip property and store mapping between attribute id and property
			if (xmlReader.isStartElement() && xmlReader.name() == "attribute") {
				string attributeId = xmlReader.attributes().value("id").toString().toStdString();
				string attributeName = xmlReader.attributes().value("title").toString().toStdString();
				string attributeType = xmlReader.attributes().value("type").toString().toStdString();
				if (attributeType == "string") {
					(*propertiesMap)[attributeId] = graph->getProperty<StringProperty>(attributeName);
				} else if (attributeType == "float" || attributeType == "double") {
					(*propertiesMap)[attributeId] = graph->getProperty<DoubleProperty>(attributeName);
				} else if (attributeType == "integer") {
					(*propertiesMap)[attributeId] = graph->getProperty<IntegerProperty>(attributeName);
				} else if (attributeType == "boolean") {
					(*propertiesMap)[attributeId] = graph->getProperty<BooleanProperty>(attributeName);
				}
			}
		}
	}

	// Parse node data
	void parseNode(QXmlStreamReader &xmlReader) {
		string nodeId = xmlReader.attributes().value("id").toString().toStdString();
		// add a node in the graph we are building
		node n = graph->addNode();
		// save mapping between gexf node id and created Tulip node
		nodesMap[nodeId] = n;
		// parse node label
		if (xmlReader.attributes().hasAttribute("label")) {
			string nodeLabel = xmlReader.attributes().value("label").toString().toStdString();
			viewLabel->setNodeValue(n, nodeLabel);
		}
		xmlReader.readNext();
		while (!(xmlReader.isEndElement() && xmlReader.name() == "node")) {
			// parse node color
			if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:color") {
				unsigned int r = xmlReader.attributes().value("r").toString().toUInt();
				unsigned int g = xmlReader.attributes().value("g").toString().toUInt();
				unsigned int b = xmlReader.attributes().value("b").toString().toUInt();
				viewColor->setNodeValue(n, Color(r, g, b));
			// parse node coordinate
			} else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:position") {
				nodesHaveCoordinates = true;
				float x = xmlReader.attributes().value("x").toString().toFloat();
				float y = xmlReader.attributes().value("y").toString().toFloat();
				float z = xmlReader.attributes().value("z").toString().toFloat();
				viewLayout->setNodeValue(n, Coord(x, y, z));
			// parse node size
			} else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "viz:size") {
				float size = xmlReader.attributes().value("value").toString().toFloat();
				viewSize->setNodeValue(n, Size(size, size, size));
			// parse node attributes
			} else if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "attvalue") {
				string attributeId = "";
				if (xmlReader.attributes().hasAttribute("id")) {	
					attributeId = xmlReader.attributes().value("id").toString().toStdString();
				} else if (xmlReader.attributes().hasAttribute("for")) {
					attributeId = xmlReader.attributes().value("for").toString().toStdString();
				}
				string attributeStrValue = xmlReader.attributes().value("value").toString().toStdString();
				if (nodePropertiesMap.find(attributeId) != nodePropertiesMap.end()) {
					nodePropertiesMap[attributeId]->setNodeStringValue(n, attributeStrValue);
				}
			}
			xmlReader.readNext();
		}
	}

	// Parse edge data
	void parseEdge(QXmlStreamReader &xmlReader) {
		// parse the source node id and target node id
		string srcId = xmlReader.attributes().value("source").toString().toStdString();
		string tgtId = xmlReader.attributes().value("target").toString().toStdString();
		// Check if nodes have been parsed
		if (nodesMap.size() > 0) {
			// add an edge in the graph we are building
			edge e = graph->addEdge(nodesMap[srcId], nodesMap[tgtId]);
			if (xmlReader.attributes().hasAttribute("label")) {
				string edgeLabel = xmlReader.attributes().value("label").toString().toStdString();
				viewLabel->setEdgeValue(e, edgeLabel);
			}
			xmlReader.readNext();
			while (!(xmlReader.isEndElement() && xmlReader.name() == "edge")) {
				// parse edge attribute
				if (xmlReader.isStartElement() && xmlReader.qualifiedName() == "attvalue") {
					string attributeId = "";
					if (xmlReader.attributes().hasAttribute("id")) {
						attributeId = xmlReader.attributes().value("id").toString().toStdString();
					} else if (xmlReader.attributes().hasAttribute("for")) {
						attributeId = xmlReader.attributes().value("for").toString().toStdString();
					}
					string attributeStrValue = xmlReader.attributes().value("value").toString().toStdString();
					if (edgePropertiesMap.find(attributeId) != edgePropertiesMap.end()) {
						edgePropertiesMap[attributeId]->setEdgeStringValue(e, attributeStrValue);
					}
				}
				xmlReader.readNext();
			}
		// Store edge extremities informations to add them to the graph once nodes will be parsed
		} else {
			edgesTmp.push_back(make_pair(srcId, tgtId));
		}
	}

	// Methods which compute Cubic Bézier control points for each edge
	void curveGraphEdges() {
		edge e;
		forEach(e, graph->getEdges()) {
			node src = graph->source(e);
			node tgt = graph->target(e);
			Coord srcCoord = viewLayout->getNodeValue(src);
			Coord tgtCoord = viewLayout->getNodeValue(tgt);
			Coord dir = tgtCoord - srcCoord;
			dir /= dir.norm();
			float length = srcCoord.dist(tgtCoord);
			float factor = 0.2f * length;
			Coord normal = Coord(dir[1], -dir[0]);
			normal *= factor;

			Coord p1 = dir;
			p1 *= factor;
			p1 += srcCoord;
			p1 += normal;

			Coord p2 = dir;
			p2 *= -factor;
			p2 += tgtCoord;
			p2 += normal;

			// Set the second and third Cubic Bézier curve control points as edge bends
			vector<Coord> bends;
			bends.push_back(p1);
			bends.push_back(p2);

			viewLayout->setEdgeValue(e, bends);
		}
	}

private :

	// maps associating attribute id to Tulip property
	map<string, PropertyInterface *> nodePropertiesMap;
	map<string, PropertyInterface *> edgePropertiesMap;

	// map associating GEXF node id to Tulip node
	map<string, node> nodesMap;

	// vector to store edge informations in case edges are declared before nodes in GEXF file
	vector<pair<string, string> > edgesTmp;

	// Viusal attributes properties pointers to the graph we are building
	LayoutProperty *viewLayout;
	SizeProperty *viewSize;
	ColorProperty *viewColor;
	StringProperty *viewLabel;
	IntegerProperty *viewShape;

	bool nodesHaveCoordinates;

};

// Macro for declaring import plugin in Tulip, it will then be accessible since the File -> Import -> File menu entry in Tulip
IMPORTPLUGINOFGROUP(GEXFImport,"GEXF Import","Antoine LAMBERT","05/05/2010","GEXF Import Plugin","1.0","File")
