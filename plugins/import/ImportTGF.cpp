/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <tulip/TulipPluginHeaders.h>

/** \file
 *  \brief - Import a graph file in Trivial Graph format
 *  This plugin imports a graph from a file in Trivial Graph input format,</br>
 *  as it is described https://en.wikipedia.org/wiki/Trivial_Graph_Format
 *
 *
 */
using namespace std;
using namespace tlp;

bool getUnsignedInt(unsigned int &i, const string &str) {
  const char *ptr = str.c_str();
  char *endPtr;
  long int value = strtol(ptr, &endPtr, 10);
  i = uint(value);
  return (value >= 0) && (*endPtr == 0);
}

bool nextToken(const string &str, string &token, string::size_type &pos, bool lastToken) {
  token.clear();
  auto size = str.size();
  if (pos == size)
    return false;

  // look for the beginning of the next token
  pos = str.find_first_not_of(' ', pos);
  if (pos == string::npos)
    return false;

  // look for the end of the next token
  auto endPos = lastToken ? str.find_last_not_of(' ') : str.find(' ', pos);
  if (endPos == string::npos)
    endPos = size;
  else if (lastToken)
    ++endPos;

  // copy found token into token
  token.reserve(endPos - pos);
  token.append(str.data() + pos, endPos - pos);

  // advance pos for the next lookup
  pos = endPos;

  return true;
}

bool tokenize(const string &str, vector<string> &tokens, unsigned int nbMax) {
  if (str.empty())
    return true;

  tokens.clear();
  tokens.reserve(nbMax);
  string token;
  string::size_type pos = 0;

  // collect nbMax tokens
  while (nbMax && nextToken(str, token, pos, nbMax == 1)) {
    --nbMax;
    tokens.push_back(token);
  }

  return tokens.size() > 0;
}

class ImportTGF : public ImportFileModule {

public:
  PLUGININFORMATION(
      "TGF", "Patrick Mary", "30/11/2023",
      "<p>File extension: tgf</p><p>Imports a new graph from a text file in Trivial Graph Format<br/>as it is described in <a href=\"https://en.wikipedia.org/wiki/Trivial_Graph_Format\">https://en.wikipedia.org/wiki/Trivial_Graph_Format</a></p>",
      "1.0", "File")

  ImportTGF(const tlp::PluginContext *context) : ImportFileModule(context, {"tgf"}) {}

  ~ImportTGF() override {}

  bool importFile() override {
    std::istream *in = tlp::getInputFileStream(filename);
    // check for open stream failure
    if (in->fail()) {
      std::stringstream ess;
      ess << filename << ": " << tlp::getStrError();
      pluginProgress->setError(ess.str());
      delete in;
      return false;
    }

    stringstream errors;
    size_t lineNumber = 0;

    if (pluginProgress)
      pluginProgress->showPreview(false);

    tlp::StringProperty *labels = graph->getProperty<StringProperty>("viewLabel");
    tlp_hash_map<unsigned int, tlp::node> nodes;
    vector<std::string> tokens;
    std::string line;
    bool ok = true, inNodes = true;

    while (!in->eof() && std::getline(*in, line)) {
      ++lineNumber;
      ok = tokenize(line, tokens, inNodes ? 2 : 3);
      if (!ok) {
        errors << "empty line";
        break;
      }

      if (inNodes) {
        if (tokens.size() == 1 && tokens[0] == "#") {
          // found separator between nodes and edges declaration
          inNodes = false;
          continue;
        }
        unsigned int id;
        // get node id
        if (!getUnsignedInt(id, tokens[0]) || (nodes.find(id) != nodes.end())) {
          errors << "invalid node id";
          ok = false;
          break;
        }
        // add node
        node n = graph->addNode();
        nodes.emplace(id, n);
        // assign it a label if any
        if (tokens.size() == 2)
          labels->setNodeValue(n, tokens[1]);
      } else {
        if (tokens.size() < 2) {
          errors << "invalid node id";
          ok = false;
          break;
        }
        unsigned int sid, tid;
        // get source and target nodes
        if (!getUnsignedInt(sid, tokens[0]) || (nodes.find(sid) == nodes.end()) ||
            !getUnsignedInt(tid, tokens[1]) || (nodes.find(tid) == nodes.end())) {
          errors << "invalid node id";
          ok = false;
          break;
        }
        // add edge
        edge e = graph->addEdge(nodes[sid], nodes[tid]);
        // assign it a label if any
        if (tokens.size() == 3)
          labels->setEdgeValue(e, tokens[2]);
      }
    }
    if (!ok && pluginProgress) {
      errors << " found while parsing file: " << filename << endl;
      errors << "at line " << lineNumber << endl;

      if (pluginProgress) {
        pluginProgress->setError(errors.str());
      }
    }

    delete in;
    return ok;
  }
};

PLUGIN(ImportTGF)
