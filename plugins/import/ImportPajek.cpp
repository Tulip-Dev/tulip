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
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/DoubleProperty.h>


/** \file
 *  \brief - Import Pajek format graph file.
 *  This plugin imports a graph from a file (.net) in Pajek input format,</br>
 *  as it is described in the Pajek manual (http://pajek.imfm.si/lib/exe/fetch.php?media=dl:pajekman203.pdf) from the Pajek wiki page http://pajek.imfm.si/doku.php?id=download.
 *  Warning: the description of the edges with *Matrix (adjacency lists)</br>
 *  is not yet supported.
 *  <b>HISTORY</b>
 *
 *  - 09/05/2011 Version 1.0: Initial release
 *
 *  \author Patrick Mary of Tulip Team http://tulip.labri.fr/
 *
 *
*/
using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
  // filename
  "This parameter indicates the pathname of the Pajek file (.net or .paj) to import.",
};


namespace {
bool tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
  if (str.empty())
    return true;

  tokens.clear();
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos = str.find_first_of(delimiters, lastPos);
  string::size_type size = str.size();

  while (string::npos != pos || string::npos != lastPos) {
    if (str[lastPos] == '\"') {
      // found and open " which marks the beginning of a string description
      // build token until the closing "
      ++lastPos;
      bool bslashFound = false;
      string token;

      for (pos = lastPos; pos < size; ++pos) {
        char c = str[pos];

        if (bslashFound) {
          token.push_back(c);
          bslashFound = false;
        }
        else {
          if (c == '\\')
            bslashFound = true;
          else {
            if (c == '"')
              break;

            token.push_back(c);
          }
        }
      }

      if (pos == size)
        return false;

      tokens.push_back(token);
      ++pos;
    }
    else
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }

  return true;
}
}

class ImportPajek : public ImportModule {

public :
  PLUGININFORMATION("Pajek","Patrick Mary","09/05/2011",
                    "<p>Supported extensions: net, paj</p><p>Imports a new graph from a file (.net) in Pajek input format<br/>as it is described in the Pajek manual (<b>http://pajek.imfm.si/lib/exe/fetch.php?media=dl:pajekman203.pdf</b>)<br/>from the Pajek wiki page <b>http://pajek.imfm.si/doku.php?id=download</b>.<br/>Warning: the description of the edges with Matrix (adjacency lists)<br/>is not yet supported.</p>",
                    "1.0","File")
  std::list<std::string> fileExtensions() const {
    std::list<std::string> l;
    l.push_back("net");
    l.push_back("paj");
    return l;
  }

  ImportPajek(const tlp::PluginContext* context):
    ImportModule(context), nbNodes(0),
    weights(NULL), labels(NULL),
    layout(NULL), sizes(NULL), expectedLine(NET_UNKNOWN), partition(NULL),
    curNodeId(0), vectorProp(NULL) {
    addInParameter<string>("file::filename", paramHelp[0],"");
  }

  ~ImportPajek() {}

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/import_pajek.png";
  }

  unsigned int nbNodes;
  DoubleProperty *weights;
  StringProperty *labels;
  LayoutProperty *layout;
  SizeProperty *sizes;
  enum TypeOfLine {NET_UNKNOWN = 0, NET_NODE, NET_EDGE, NET_EDGESLIST, NET_PARTITION, NET_VECTOR, NET_MATRIX};
  TypeOfLine expectedLine;
  Graph *partition;
  unsigned int curNodeId;
  map<string, vector<node> > parts;
  DoubleProperty *vectorProp;

  bool getUnsignedInt(unsigned int& i, const string& str) {
    const char* ptr = str.c_str();
    char* endPtr;
    long int value = strtol(ptr, &endPtr, 10);
    i = uint(value);
    return (value >= 0) && (*endPtr == 0);
  }

  bool getFloat(float& f, const string& str) {
    const char* ptr = str.c_str();
    char* endPtr;
    double d = strtod(ptr, &endPtr);
    f = float(d);
    return (*endPtr == 0);
  }

  bool getDouble(double& d, const string& str) {
    const char* ptr = str.c_str();
    char* endPtr;
    d = strtod(ptr, &endPtr);
    return (*endPtr == 0);
  }

  bool treatLine(string& str) {
    // empty line or comment line found
    if (str.empty() || str[0] == '%')
      return true;

    vector<string> tokens;

    if (!tokenize(str, tokens, " \r\t"))
      return false;

    if (tokens.empty())
      return true;

    unsigned int nbTokens = tokens.size();
    char c = tokens[0][0];
    string lineWoFirstToken;

    for (size_t i = 1 ; i < nbTokens ; ++i) {
      lineWoFirstToken += tokens[i];

      if (i != nbTokens - 1) {
        lineWoFirstToken += " ";
      }
    }

    if (c == '*') {

      if (tokens[0] == "*Network" || tokens[0] == "*network") {
        if (nbTokens > 1) {
          graph->setName(lineWoFirstToken);
        }

        return true;
      }

      if (tokens[0] == "*Vertices" || tokens[0] == "*vertices") {

        if (expectedLine == NET_PARTITION || expectedLine == NET_VECTOR) {
          // currently parsing a partition or a vector, don't treat that line
          return true;
        }

        // next token is the number of vertices
        if (nbTokens < 2)
          return false;

        if (!getUnsignedInt(nbNodes, tokens[1]))
          return false;

        // add nodes
        graph->addNodes(nbNodes);
        // next lines should be for node
        expectedLine = NET_NODE;
        // other remaining tokens are ignored
        return true;
      }

      if (tokens[0] == "*Arcslist" || tokens[0] == "*arcslist" ||
          tokens[0] == "*Edgeslist" || tokens[0] == "*edgeslist") {
        expectedLine = NET_EDGESLIST;
        // no more token for this line
        return true;
      }

      if (tokens[0] == "*Arcs" || tokens[0] == "*arcs" ||
          tokens[0] == "*Edges" || tokens[0] == "*edges") {
        expectedLine = NET_EDGE;
        // no more token for this line
        return true;
      }

      if (tokens[0] == "*Partition" || tokens[0] == "*partition") {

        if (nbTokens < 2)
          return false;

        expectedLine = NET_PARTITION;
        curNodeId = 0;

        partition = graph->addCloneSubGraph(lineWoFirstToken);
        parts.clear();

        // no more token for this line
        return true;
      }

      if (tokens[0] == "*Vector" || tokens[0] == "*vector" || tokens[0] == "*Permutation" || tokens[0] == "*permutation") {

        if (nbTokens < 2)
          return false;

        expectedLine = NET_VECTOR;
        curNodeId = 0;
        vectorProp = graph->getProperty<DoubleProperty>(lineWoFirstToken);

        return true;
      }

      if (tokens[0] == "*Matrix" || tokens[0] == "*matrix") {
        expectedLine = NET_MATRIX;
        curNodeId = 0;
        return true;
      }

      return false;
    }

    if (expectedLine == NET_UNKNOWN)
      return false;

    const vector<node>& nodes = graph->nodes();

    if (expectedLine == NET_MATRIX) {

      for (size_t i = 0 ; i < tokens.size() ; ++i) {
        double val = 0;
        getDouble(val, tokens[i]);

        if (val > 0) {
          edge e = graph->addEdge(nodes[curNodeId], nodes[i]);
          weights->setEdgeValue(e, val);
        }
      }

      ++curNodeId;

      return true;
    }

    if (expectedLine == NET_PARTITION) {
      parts[tokens[0]].push_back(nodes[curNodeId++]);

      if (curNodeId == graph->numberOfNodes()) {
        map<string, vector<node> >::iterator it = parts.begin();

        for (; it != parts.end() ; ++it) {
          Graph *part = partition->inducedSubGraph(it->second);
          part->setName(it->first);
        }
      }

      return true;
    }

    if (expectedLine == NET_VECTOR) {
      double val = 0;

      if (!getDouble(val, tokens[0])) {
        return false;
      }
      else {
        vectorProp->setNodeValue(nodes[curNodeId++], val);
        return true;
      }
    }

    // first token is always the # of a vertex
    unsigned int first;

    if (!getUnsignedInt(first, tokens[0]))
      return false;

    if (first > nbNodes)
      return false;

    /* in NET format vertex # begins to 1
       but nodes index begins to 0 */
    first -= 1;

    if (expectedLine == NET_NODE) {
      node n = nodes[first];

      // next token must be the label of the node
      if (nbTokens == 1)
        return false;

      labels->setNodeValue(n, tokens[1]);

      // check if node coordinates are present
      if (nbTokens == 2)
        return true;

      // get coordinates if any
      unsigned int i = 2;
      Coord coord;

      if (getFloat(coord[0], tokens[2])) {
        ++i;

        // we have x
        // check for y
        if (nbTokens > 3) {
          if (getFloat(coord[1], tokens[3])) {
            ++i;

            // we have y check for z
            if (nbTokens > 4) {
              if (getFloat(coord[2], tokens[4]))
                ++i;
            }
          }
        }

        layout->setNodeValue(n, coord);
      }

      // check other parameters
      Size nSize(0.1f, 0.1f, 0);

      for(; i < nbTokens; ++i) {
        if (tokens[i] == "x_fact") {
          // next token must be a float
          float x_fact;

          if ((nbTokens == i + 1) ||
              !getFloat(x_fact, tokens[i + 1]))
            return false;

          nSize[0] *= x_fact;
          ++i;
          continue;
        }

        if (tokens[i] == "y_fact") {
          // next token must be a float
          float y_fact;

          if ((nbTokens == i + 1) ||
              !getFloat(y_fact, tokens[i + 1]))
            return false;

          nSize[1] *= y_fact;
          ++i;
          continue;
        }

        // colors will be handled later
      }

      // set node size
      sizes->setNodeValue(n, nSize);
      return true;
    }

    /* we expect to find the edge's target */
    if (nbTokens < 2)
      return false;

    // loop on edges list
    unsigned int i = 1;

    while (i < nbTokens) {
      // next token is the index of the edge's target
      unsigned int second;

      if (!getUnsignedInt(second, tokens[i]) || second > nodes.size())
        return false;

      /* in NET format vertex # begins to 1
      but nodes index begins to 0 */
      second -= 1;
      edge e = graph->addEdge(nodes[first], nodes[second]);

      if (expectedLine == NET_EDGE) {
        if (nbTokens  > 2) {
          // if it exists the next token is the edge weight
          double weight;

          if (!getDouble(weight, tokens[2]))
            return false;

          // negative weight is for dot line
          // so ensure weight is positive
          if (weight < 0.0)
            weight = -weight;

          // set weight
          weights->setEdgeValue(e, weight);

          // looking for edge's label if any
          for (first = 3; first < nbTokens; first += 2) {
            if (tokens[first] == "l") {
              // next token is the label
              if (nbTokens == first)
                return false;

              labels->setEdgeValue(e, tokens[first]);
              break;
            }
          }
        }
        else
          // default edge weight is 1
          weights->setEdgeValue(e, 1.0);

        return true;
      }

      ++i;
    }

    return true;
  }

  bool importGraph() {
    string filename;

    dataSet->get<string>("file::filename", filename);

    if (filename.empty()) {
      pluginProgress->setError("Filename is empty.");
      return false;
    }

    std::istream *in = tlp::getInputFileStream(filename.c_str());

    labels   = graph->getProperty<StringProperty>("viewLabel");
    weights  = graph->getProperty<DoubleProperty>("weights");
    layout   = graph->getProperty<LayoutProperty>("viewLayout");
    sizes    = graph->getProperty<SizeProperty>("viewSize");

    // because when node's layout is provided
    // x,y,z coordinates are restricted to [0.0, 1.0]
    // arbitrary set default size for nodes to (0.01, 0.01, 0)
    sizes->setAllNodeValue(Size(0.01f, 0.01f, 0.0f));

    stringstream errors;
    size_t lineNumber = 0;

    if (pluginProgress)
      pluginProgress->showPreview(false);

    nbNodes = 0;

    string line;

    while (!in->eof() && std::getline(*in, line)) {

      ++lineNumber;

      if(!treatLine(line)) {
        errors << "An error occurs while parsing file : " << filename << endl;
        errors << "[ERROR] at line " << lineNumber << endl;

        if (pluginProgress) {
          pluginProgress->setError(errors.str());
        }

        delete in;
        return false;
      }

      if (pluginProgress && ((lineNumber % 100) == 0) &&
          (pluginProgress->progress(lineNumber, 3 * nbNodes) != TLP_CONTINUE)) {
        delete in;
        return false;
      }
    }

    delete in;
    return true;
  }
};

PLUGIN(ImportPajek)
