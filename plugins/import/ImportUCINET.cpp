/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include <algorithm>
#include <cstring>
#include <tulip/TulipPluginHeaders.h>


/** \file
 *  \brief - Import UCINET DL format graph file.
 *  This plugin imports a graph from a file in UCINET DL input format,</br>
 *  as it is described in the UCINET reference manual (http://www.analytictech.com/ucinet/documentation/reference.rtf).
 *  <b>HISTORY</b>
 *
 *  - 12/09/2011 Version 1.0: Initial release
 *
 *  \author Patrick Mary of Tulip Team http://tulip-software.org/
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
*/
using namespace std;
using namespace tlp;

namespace {
const char * paramHelp[] = {
  // filename
  HTML_HELP_OPEN()            \
  HTML_HELP_DEF( "type", "pathname" )       \
  HTML_HELP_BODY()                  \
  "This parameter indicates the pathname of the file in dl format to import."       \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()            \
  HTML_HELP_DEF( "type", "string" )       \
  HTML_HELP_BODY()                  \
  "This parameter indicates the name of the default metric."        \
  HTML_HELP_CLOSE(),
};
}

namespace {
bool getUnsignedInt(unsigned int& i, const string& str) {
  const char* ptr = str.c_str();
  char* endPtr;
  long int value = strtol(ptr, &endPtr, 10);
  i = (unsigned int) value;
  return (value >= 0) && (*endPtr == 0);
}

bool getDouble(double& d, const string& str) {
  const char* ptr = str.c_str();
  char* endPtr;
  d = strtod(ptr, &endPtr);
  return (*endPtr == 0);
}

bool nocasecmp(const string& str1, const char* str2) {
  return strcasecmp(str1.c_str(), str2) == 0;
}

bool skipEqualSign(const string& str, string::size_type& pos) {
  bool equalFound = false;
  string::size_type size = str.size();

  for (; pos < size; ++pos) {
    switch(str[pos]) {
    case '=':
      equalFound = true;

    case ' ':
    case '\r':
    case '\t':
      break;

    default:
      return equalFound;
    }
  }

  return false;
}

bool nextUnsignedInt(const string& str, unsigned int& value,
                     string::size_type& pos) {
  string::size_type lastPos = str.find_first_not_of(" \r\t ,=", pos);
  // Find next separator
  pos = str.find_first_of(" \r\t ,", lastPos);

  if (string::npos != pos || string::npos != lastPos)
    return getUnsignedInt(value, str.substr(lastPos, pos - lastPos));

  return false;
}

bool nextString(const string& str, string& token,
                string::size_type& pos) {
  token.clear();
  // Skip separators at the beginning.
  string::size_type lastPos = str.find_first_not_of(" \r\t", pos);
  string::size_type size = str.size();

  if (string::npos != lastPos && str[lastPos] == '"') {
    // found and open " which marks the beginning of a string description
    // build token until the closing "
    ++lastPos;
    bool bslashFound = false;

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

    return (pos++ != size);
  }

  return false;
}

bool nextToken(const string& str, const string& separators, string& token,
               string::size_type& pos) {
  token.clear();
  // Skip separators at the beginning.
  string::size_type lastPos = str.find_first_not_of(separators, pos);
  // Find next separator
  pos = str.find_first_of(separators, lastPos);
  string::size_type size = str.size();

  if (string::npos != pos || string::npos != lastPos) {
    if (str[lastPos] == '\"') {
      // found and open " which marks the beginning of a string description
      // build token until the closing "
      ++lastPos;
      bool bslashFound = false;

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

      ++pos;
    }
    else
      token.insert(0, str, lastPos, pos - lastPos);
  }

  return true;
}

bool tokenize(const string& str, vector<string>& tokens,
              const string& separators) {
  if (str.empty())
    return true;

  tokens.clear();
  string token;
  string::size_type pos = 0;
  bool result;

  while ((result = nextToken(str, separators, token, pos))
         && !token.empty())
    tokens.push_back(token);

  return result;
}
}

class ImportUCINET : public ImportModule {

public :
  PLUGININFORMATION("UCINET","Patrick Mary","12/09/2011","Imports a new graph from a text file (.txt) in UCINET DL input format.","1.0","File")
  std::list<std::string> fileExtensions() const {
    std::list<std::string> l;
    l.push_back("txt");
    return l;
  }
  ImportUCINET(const tlp::PluginContext* context):ImportModule(context),
    nbNodes(0), defaultMetric("weight"),
    n(0), nr(0), nc(0), nm(0), current(0),
    dl_found(false), diagonal(true),
    diagonal_found(false),
    labels_known(false), title_found(false),
    expectedLine(DL_HEADER),
    embedding(DL_NONE), dataFormat(DL_FM) {
    addInParameter<string>("file::filename", paramHelp[0],"");
    addInParameter<string>("Default metric", paramHelp[1], "weight");
  }

  ~ImportUCINET() {}

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/import_ucinet.png";
  }

  vector<node> nodes;
  unsigned int nbNodes;
  string defaultMetric;
  vector<DoubleProperty*> metrics;
  // n indicates the number of nodes if the graph is not bipartite
  // if it is, nr indicates the number of nodes in the part 1
  // of the graph, nc the number of nodes in the part 2 of the graph
  // nm indicates the number of matrix (describing relationships present
  // in the file)
  // current indicates the current index
  unsigned int n, nr, nc, nm, current;
  // dl_found indicates that 'dl' marker has been found
  // diagonal indicates the presence/absence of the diagonal
  // in the matrix data
  // diagonal_found indicates that the 'diagonal' token has been found
  // labels_known indicates the labels of nodes are known before reading data
  bool dl_found, diagonal, diagonal_found, labels_known, title_found;
  enum TypeOfLine {DL_HEADER = 0, DL_ROW_LABELS, DL_COL_LABELS,
                   DL_LABELS, DL_MATRIX_LABELS, DL_DATA
                  };
  // indicates what kind of line is expected
  TypeOfLine expectedLine;
  enum TypeOfLabelEmbedding {DL_NONE = 0, DL_ROWS = 1, DL_COLS = 2, DL_ALL = 4};
  // indicates what labels are embedded in the data to be read
  unsigned int embedding;
  enum TypeOfData {DL_FM, DL_UH, DL_LH, DL_NL1, DL_NL2, DL_NL1B, DL_EL1, DL_EL2,
                   DL_BM
                  };
  // indicates the current format for the data to be read
  TypeOfData dataFormat;
  TLP_HASH_MAP<std::string, node> labelToNode, colLabelToNode, rowLabelToNode;

  bool readHeader(string& str, stringstream& error) {
    string token;
    string::size_type pos = 0;
    bool result;

    while((result = nextToken(str, " \r\t,=", token, pos))
          && !token.empty()) {
      // check for dl
      if (dl_found == false) {
        if (nocasecmp(token, "dl")) {
          dl_found = true;
          continue;
        }

        error << "dl tag not found";
        return false;
      }

      if (nocasecmp(token, "title")) {
        // no existing title
        if (title_found) {
          error << "TITLE already specified";
          return false;
        }

        if (!skipEqualSign(str, pos) ||
            !nextString(str, token, pos) ||
            token.empty()) {
          error << "invalid specification for parameter TITLE";
          return false;
        }

        graph->setName(token);
        title_found = true;
        continue;
      }

      if (nocasecmp(token, "n")) {
        // we must know nothing about the size of matrices
        if (n || nr || nc) {
          error << "invalid specification for parameter N";
          return false;
        }

        // get n
        if (!nextUnsignedInt(str, n, pos)) {
          error << "invalid value for parameter N";
          return false;
        }

        // add nodes
        graph->addNodes(nbNodes = n, nodes);
        continue;
      }

      if (nocasecmp(token, "nr")) {
        // nr found
        if (n || nr || (dataFormat == DL_NL1) || (dataFormat == DL_NL1B)) {
          error << "invalid specification for parameter NR";
          return false;
        }

        // get nr
        if (!nextUnsignedInt(str, nr, pos)) {
          error << "invalid value for parameter NR";
          return false;
        }

        if (nc)
          // add nodes
          graph->addNodes(nbNodes = nc + nr, nodes);

        continue;
      }

      if (nocasecmp(token, "nc")) {
        // nc found
        if (n || nc || (dataFormat == DL_NL1) || (dataFormat == DL_NL1B)) {
          error << "invalid specification for parameter NC";
          return false;
        }

        // get nc
        if (!nextUnsignedInt(str, nc, pos)) {
          error << "invalid value for parameter NC";
          return false;
        }

        if (nr)
          // add nodes
          graph->addNodes(nbNodes = nc + nr, nodes);

        continue;
      }

      if (nocasecmp(token, "nm")) {
        // nm found
        if (nm) {
          error << "invalid specification for parameter NM";
          return false;
        }

        // get nm
        if (!nextUnsignedInt(str, nm, pos)) {
          error << "invalid value for parameter NM";
          return false;
        }

        continue;
      }

      if (nocasecmp(token, "format")) {
        // format found
        if (!skipEqualSign(str, pos) ||
            !nextToken(str, " \r\t,", token, pos)
            || token.empty()) {
          error << "invalid specification for parameter FORMAT";
          return false;
        }

        // check dataFormat
        if (nocasecmp(token, "fullmatrix") ||
            nocasecmp(token, "fm")) {
          dataFormat = DL_FM;
          continue;
        }

        if (nocasecmp(token, "upperhalf") ||
            nocasecmp(token, "uh")) {
          dataFormat = DL_UH;
          continue;
        }

        if (nocasecmp(token, "lowerhalf") ||
            nocasecmp(token, "lh")) {
          dataFormat = DL_LH;
          continue;
        }

        if (nocasecmp(token, "nodelist1") ||
            nocasecmp(token, "nl1")) {
          if (nr || nc) {
            error << "specification of parameter FORMAT applies only to 1-mode matrix";
            return false;
          }

          dataFormat = DL_NL1;
          continue;
        }

        if (nocasecmp(token, "nodelist2") ||
            nocasecmp(token, "nl2")) {
          dataFormat = DL_NL2;
          continue;
        }

        if (nocasecmp(token, "nodelist1b") ||
            nocasecmp(token, "nl1b")) {
          if (nr || nc) {
            error << "specification of parameter FORMAT applies only to 1-mode matrix";
            return false;
          }

          dataFormat = DL_NL1B;
          continue;
        }

        if (nocasecmp(token, "edgelist1") ||
            nocasecmp(token, "el1")) {
          dataFormat = DL_EL1;
          continue;
        }

        if (nocasecmp(token, "edgelist2") ||
            nocasecmp(token, "el2")) {
          dataFormat = DL_EL2;
          continue;
        }

        if (nocasecmp(token, "blockmatrix") ||
            nocasecmp(token, "bm")) {
          dataFormat = DL_BM;
          continue;
        }

        error << "invalid value for parameter FORMAT";
        return false;
      }

      if (nocasecmp(token, "diagonal")) {
        // diagonal found
        if (diagonal_found) {
          error << "invalid specification for parameter DIAGONAL";
          return false;
        }

        diagonal_found = true;

        if (!nextToken(str, " \r\t,", token, pos)
            || token.empty()) {
          error << "invalid specification for parameter DIAGONAL";
          return false;
        }

        // specification says that DIAGONAL = PRESENT|ABSENT
        // but = may not exist
        if ((token == "=") && (!nextToken(str, " \r\t,", token, pos)
                               || token.empty())) {
          error << "invalid specification for parameter DIAGONAL";
          return false;
        }

        if (nocasecmp(token, "present")) {
          diagonal = true;
          continue;
        }

        if (nocasecmp(token, "absent")) {
          diagonal = false;
          continue;
        }

        error << "invalid value for parameter DIAGONAL";
        return false;
      }

      if (nocasecmp(token, "row")) {
        // 'row' found
        if (embedding & (unsigned int) DL_ROWS) {
          error << "invalid specification for parameter ROWS";
          return false;
        }

        if (!nextToken(str, " \r\t,", token, pos)
            || token.empty())  {
          error << "invalid specification for parameter ROWS";
          return false;
        }

        // next token must be 'labels'
        if (nocasecmp(token, "labels")) {
          // next token must be 'embedded'
          if (!nextToken(str, " \r\t,", token, pos)
              || token.empty()
              || !nocasecmp(token, "embedded")) {
            error << "invalid specification for parameter ROWS";
            return false;
          }

          embedding += (unsigned int) DL_ROWS;
          continue;
        }

        // or 'labels:'
        if (nocasecmp(token, "labels:")) {
          if (n == 0 && nr == 0) {
            error << "specification of ROW LABELS applies only to 2-mode matrix";
            return false;
          }

          expectedLine = DL_ROW_LABELS;
          current = 0;
          return true;
        }

        error << "invalid specification for parameter ROWS";
        return false;
      }

      if (nocasecmp(token, "col") ||
          nocasecmp(token, "column")) {
        // 'col' or 'column' found
        if (embedding & (unsigned int) DL_COLS)
          return false;

        if (!nextToken(str, " \r\t,", token, pos)
            || token.empty()) {
          error << "invalid specification for parameter COLUMNS";
          return false;
        }

        // next token must be 'labels'
        if (nocasecmp(token, "labels")) {
          // next token must be 'embedded'
          if (!nextToken(str, " \r\t,", token, pos)
              || token.empty()
              || !nocasecmp(token, "embedded")) {
            error << "invalid specification for parameter COLUMNS";
            return false;
          }

          embedding += DL_COLS;
          continue;
        }

        // or 'labels:'
        if (nocasecmp(token, "labels:")) {
          if (n == 0 && nc == 0) {
            error << "specification of COLUMN LABELS applies only to 2-mode matrix";
            return false;
          }

          expectedLine = DL_COL_LABELS;
          current = 0;
          return true;
        }

        error << "invalid specification for parameter COLUMNS";
        return false;
      }

      if (nocasecmp(token, "matrix")) {
        // matrix found
        if (!nextToken(str, " \r\t,", token, pos)
            || token.empty()) {
          error << "invalid specification for parameter MATRIX";
          return false;
        }

        // next token must be labels:
        if (nocasecmp(token, "labels:")) {
          if (nm == 0) {
            error << "specification of MATRIX LABELS cannot apply if NM is not defined";
            return false;
          }

          expectedLine = DL_MATRIX_LABELS;
          current = 0;
          return true;
        }

        error << "invalid specification for parameter MATRIX";
        return false;
      }

      if (nocasecmp(token, "labels")) {
        // labels found
        if (embedding) {
          error << "invalid specification for parameter LABELS";
          return false;
        }

        // next token must be 'embedded'
        if (!nextToken(str, " \r\t,", token, pos)
            || token.empty()
            || !nocasecmp(token, "embedded")) {
          error << "invalid specification for parameter LABELS";
          return false;
        }

        embedding = DL_ALL;
        continue;
      }

      if (nocasecmp(token, "labels:")) {
        // labels: found
        if (nbNodes == 0) {
          error << "specification of LABELS applis only to 1-mode matrix";
          return false;
        }

        expectedLine = DL_LABELS;
        current = 0;
        return true;
      }

      if (nocasecmp(token, "data:")) {
        // data: found
        // check if matrix size is known
        if (n == 0 && (nr == 0 || nc == 0)) {
          error << "matrix size unknown";
          return false;
        }

        // set default metric
        if (nm == 0) {
          metrics.push_back(graph->getProperty<DoubleProperty>(defaultMetric));
        }
        else {
          if (metrics.empty()) {
            // create metrics with default name
            for (unsigned int i = 0; i < nm; ++i) {
              stringstream sstr;
              sstr << defaultMetric << (i + 1);
              DoubleProperty* metric =
                graph->getProperty<DoubleProperty>(sstr.str());
              metrics.push_back(metric);
            }
          }
        }

        current = 0; // used to check row label
        expectedLine = DL_DATA;
        return true;
      }

      error << token << " is not a valid keyword";
      return false;
    }

    return result;
  }

  bool readLabels(const string& str, stringstream& error,
                  TLP_HASH_MAP<std::string, node>& labelsHMap,
                  unsigned int nbLabels, unsigned int offset) {
    vector<std::string> labels;
    StringProperty* label = graph->getProperty<StringProperty>("viewLabel");

    if (!tokenize(str, labels, " \r\t,"))
      return false;

    // check the number of read labels
    if ((current + labels.size()) > nbLabels) {
      error << "too much labels specified";
      return false;
    }

    for (unsigned int i = 0; i < labels.size(); ++i, ++current) {
      // assign label to each corresponding node
      label->setNodeValue(nodes[offset + current], labels[i]);
      // and memorize the corresponding uppercase label for each node
      std::transform(labels[i].begin(), labels[i].end(),
                     labels[i].begin(), (int (*) (int)) std::toupper);
      labelsHMap[labels[i]] = nodes[offset + current];
    }

    // check the end of the labels to read
    if (current == nbLabels)
      expectedLine = DL_HEADER;

    return true;
  }

  void checkColumnLabels(vector<std::string>& tokens, unsigned int &ir,
                         unsigned int &ic, unsigned int &i) {
    if (ir == 0 && embedding & (unsigned int) DL_COLS) {
      StringProperty* label = graph->getProperty<StringProperty>("viewLabel");

      // first nc tokens are for labels of the part 1 of the graph
      for (; ic < nc && i < tokens.size(); ++i, ++ic) {
        label->setNodeValue(nodes[ic], tokens[i]);
      }

      if (ic == nc) {
        // all columns labels have been read
        embedding -= (unsigned int) DL_COLS;
        ic = 0;
      }
    }
  }

  node getNodeFromInfo(string& token, unsigned int& i, bool findCol) {
    if (embedding == DL_NONE ||
        (embedding != DL_ALL && !(embedding & (findCol ? DL_COLS : DL_ROWS)))) {
      // token is row index (first is 1)
      unsigned int row;

      if (!getUnsignedInt(row, token) || row > nbNodes)
        return node();

      return nodes[row - 1];
    }

    string upcasetoken(token);
    transform(token.begin(), token.end(),
              upcasetoken.begin(), (int (*) (int)) std::toupper);

    if (n/*embedding == DL_ALL*/) { // 1-mode
      TLP_HASH_MAP<std::string, node>::iterator it =
        labelToNode.find(upcasetoken);

      if (it != labelToNode.end())
        return (*it).second;

      if (labels_known || i == nbNodes)
        // should already exist
        return node();

      ++i;
      graph->getProperty<StringProperty>("viewLabel")->setNodeValue(nodes[i -1], token);
      return labelToNode[upcasetoken] = nodes[i - 1];
    }

    if (findCol) {
      TLP_HASH_MAP<std::string, node>::iterator it =
        colLabelToNode.find(upcasetoken);

      if (it != colLabelToNode.end())
        return (*it).second;

      if (labels_known || i == nc)
        // should already exist
        return node();

      ++i;
      graph->getProperty<StringProperty>("viewLabel")->setNodeValue(nodes[i -1], token);
      return colLabelToNode[upcasetoken] = nodes[i - 1];
    }
    else {
      TLP_HASH_MAP<std::string, node>::iterator it =
        rowLabelToNode.find(upcasetoken);

      if (it != rowLabelToNode.end())
        return (*it).second;

      if (labels_known || i == nr)
        // should already exist
        return node();

      ++i;
      graph->getProperty<StringProperty>("viewLabel")->setNodeValue(nodes[nc + i -1], token);
      return rowLabelToNode[upcasetoken] = nodes[nc + i - 1];
    }
  }

  bool readData(vector<std::string>& tokens,
                stringstream& error,
                unsigned int &ir, unsigned int &ic,
                DoubleProperty* metric) {
    // index of current token
    unsigned int i = 0;

    switch(dataFormat) {
    case DL_FM:
    case DL_LH:
    case DL_UH: {
      // check if column labels are in first line
      checkColumnLabels(tokens, ir, ic, i);

      // read row data
      for (; i < tokens.size(); ++i) {
        // check current node
        if (nc + ir == nbNodes) {
          error << "invalid row";
          return false;
        }

        node src = nodes[nc + ir];

        // check if we first have row label
        if ((embedding & DL_ROWS) && (ic == 0) && (current == 0)) {
          graph->getProperty<StringProperty>("viewLabel")->setNodeValue(src, tokens[i]);

          if (ir == 0 && nc == 1 && diagonal == false)
            ++ir;
          else
            current = 1;

          continue;
        }

        if (dataFormat == DL_UH && ic == 0)
          // nothing exist before the diagonal
          ic = ir;

        // check diagonal
        if (ir == ic && diagonal == false) {
          if (dataFormat == DL_LH) {
            ir = 1;
            src = nodes[nc + 1];
          }
          else {
            if (ir == 0 && nc == 1) {
              // nothing to read in this row
              ++ir, current = 0;
              continue;
            }

            ++ic;
          }
        }

        double value = 0;

        if (getDouble(value, tokens[i])) {
          // addEdge if needed
          if (value) {
            edge e = graph->addEdge(src, nodes[ic]);
            metric->setEdgeValue(e, value);

            if (dataFormat != DL_FM) {
              // matrix is symetric
              e = graph->addEdge(nodes[ic], src);
              metric->setEdgeValue(e, value);
            }
          }
        }
        // ? indicates an unspecified value
        else if (tokens[i] != "?") {
          error << "invalid value";
          return false;
        }

        ++ic;

        // check end of row
        if ((dataFormat == DL_LH && (diagonal ? (ic > ir) : (ic == ir))) ||
            (ic == (nc ? nc : nbNodes)))
          ++ir, ic = 0, current = 0;
      }

      return true;
    }

    case DL_NL1:
    case DL_NL2: {
      // first token indicates the source
      node src = getNodeFromInfo(tokens[0],
                                 dataFormat == DL_NL1 ? ic : ir, false);

      if (!src.isValid()) {
        error << "invalid row";
        return false;
      }

      // read row data
      for (i = 1; i < tokens.size(); ++i) {
        node tgt = getNodeFromInfo(tokens[i], ic, true);

        if (!tgt.isValid()) {
          error << "invalid column";
          return false;
        }

        // add edge
        metric->setEdgeValue(graph->addEdge(src, tgt), 1.0);
      }

      return true;
    }

    case DL_NL1B: {
      node src = nodes[nc + ir];
      // first token indicates the number of cols in that row
      unsigned int nbCols;

      if (!getUnsignedInt(nbCols, tokens[0]) ||
          // nbCols must equal to the number of remaining tokens
          (nbCols != tokens.size() - 1)) {
        error << "invalid number of columns";
        return false;
      }

      for (i = 1; i < tokens.size(); ++i) {
        // each subsequent token is a col index (first is 1)
        unsigned int col;

        if (!getUnsignedInt(col, tokens[i])) {
          error << "invalid column";
          return false;
        }

        // add edge
        graph->addEdge(src, nodes[col - 1]);
      }

      return true;
    }

    case DL_EL1:
    case DL_EL2: {
      if (tokens.size() < 2) {
        error << "missing info";
        return false;
      }

      if (tokens.size() > 3) {
        error << "too much info";
        return false;
      }

      // first two token indicates the source and target of the edge
      node src = getNodeFromInfo(tokens[0],
                                 (dataFormat == DL_EL1) ? ic : ir, false);

      if (!src.isValid()) {
        error << "invalid row";
        return false;
      }

      node tgt = getNodeFromInfo(tokens[1], ic, true);
      edge e = graph->addEdge(src, tgt);
      double value = 1.0;

      if ((tokens.size() == 3) && !getDouble(value, tokens[2])) {
        // non numeric values are missing
        value = 0;
      }

      metric->setEdgeValue(e, value);
      return true;
    }

    default:
      error << "current format is not supported";
    }

    return false;
  }

  bool importGraph() {
    string filename;

    dataSet->get<string>("file::filename", filename);
    dataSet->get<string>("Default metric", defaultMetric);

    if (filename.empty()) {
      pluginProgress->setError("Filename is empty.");
      return false;
    }

    std::istream *in = tlp::getInputFileStream(filename.c_str());

    stringstream errors;
    size_t lineNumber = 0;

    if (pluginProgress)
      pluginProgress->showPreview(false);

    // the current metric
    DoubleProperty* metric = NULL;

    // index of row, column and metric when reading data
    unsigned int ic, ir, im;
    ic = ir = im = 0;

    std::string line;

    while (!in->eof() && std::getline(*in, line)) {
      bool result = false;

      ++lineNumber;

      switch(expectedLine) {
      case DL_HEADER:
        result = readHeader(line, errors);
        break;

      case DL_ROW_LABELS:
        result = readLabels(line, errors, rowLabelToNode,
                            nr ? nr : n, nc);
        break;

      case DL_COL_LABELS:
        result = readLabels(line, errors, colLabelToNode, nc ? nc : n, 0);
        break;

      case DL_LABELS:
        labels_known = true;
        result = readLabels(line, errors, labelToNode, nbNodes, 0);
        break;

      case DL_MATRIX_LABELS: {
        vector<std::string> labels;
        result = tokenize(line, labels, " \r\t,");

        if (labels.size() != nm) {
          errors << "too much matrix labels";
          result = false;
        }

        if (result) {
          for (; current < labels.size(); ++current)
            metrics.push_back(graph->getProperty<DoubleProperty>(labels[current]));

          // check if all matrix labels have been read
          if (current == nm)
            expectedLine = DL_HEADER;
        }

        break;
      }

      case DL_DATA: {
        if (ir == 0) {
          // set current metric
          if (nm) {
            // multi matrices case
            if (im == nm)
              return false;

            metric = metrics[im];
          }
          else
            // default
            metric = metrics[0];
        }

        vector<std::string> tokens;

        if (!(result = tokenize(line, tokens, " \r\t,")))
          break;

        // check for empty line
        if (tokens.empty())
          break;

        // check for line separation between matrices
        // if dataFormat == DL_EL1 | DL_EL2
        // ! indicates a new matrix
        if ((tokens.size() == 1) && (tokens[0] == "!") && (nm > 1) &&
            (im < nm) && ((dataFormat == DL_EL1) || (dataFormat == DL_EL2))) {
          ++im;
          break;
        }

        result = readData(tokens, errors, ir, ic, metric);

        // check for new matrix
        if (result && (nm > 1) && (ir == (nr ? nr : nbNodes))) {
          ir = 0, ic = 0, ++im;
        }
      }

      default:
        break;
      }

      if (result == false) {
        errors << endl;
        errors << "error found while parsing file : " << filename << endl;
        errors << "at line " << lineNumber << endl;

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

PLUGIN(ImportUCINET)
