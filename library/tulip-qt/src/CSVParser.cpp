/*
 * CSVDataLoader.cpp
 *
 *  Created on: 16 févr. 2010
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU Lesser General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */
#include "tulip/CSVParser.h"
#include "tulip/CSVContentHandler.h"
#include <tulip/PluginProgress.h>
#include <QtGui/QProgressBar>
#include <QtGui/QApplication>
#include <fstream>
#include <algorithm>
#include <cassert>
using namespace std;
using namespace tlp;

CSVParser::CSVParser() :
  removeQuotes(true) {
}

CSVParser::~CSVParser() {
}

void CSVParser::parse(const string& fileName, const string& separator, CSVContentHandler* handler,
    PluginProgress* progress) {
  assert(handler);
  handler->begin();
  ifstream csvFile(fileName.c_str());

  unsigned int row = 0;
  unsigned int columnMax = 0;
  if (csvFile) {
    long fileSize = computeFileSize(fileName);
    string line;
    vector<string> tokens;

    unsigned int displayProgressEachLineNumber = 10;

    if (progress) {
      progress->progress(0, 100);
    }
    while (getline(csvFile, line)) {
      if (progress) {
        if (progress->state() != TLP_CONTINUE) {
          break;
        }
        //Each displayProgressEachLineNumber display progression
        if (row % displayProgressEachLineNumber == 0) {
          // compute progression in function of read size (current position) and max file size.
          progress->progress((csvFile.tellg()*100)/fileSize, 100);
          QApplication::processEvents();
        }
      }
      tokens.clear();
      tokenize(line, tokens, separator, 0);
      unsigned int column = 0;
      for (column = 0; column < tokens.size(); ++column) {
        if (progress) {
          if (progress->state() != TLP_CONTINUE) {
            break;
          }
        }
        handler->token(row, column, treatToken(tokens[column], row, column));
      }
      columnMax = max(columnMax, column);
      ++row;
    }
  }
  handler->end(row, columnMax);
}
void CSVParser::tokenize(const string& str, vector<string>& tokens, const string& delimiters, unsigned int numberOfCol) {
  // Skip delimiters at beginning.
  string::size_type lastPos = 0;
  // Find first "non-delimiter".
  string::size_type pos = str.find_first_of(delimiters, lastPos);
  while (string::npos != pos || string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    if (tokens.size() == numberOfCol) {
      break;
    }
    if (pos != string::npos)
      lastPos = pos + 1;
    else
      lastPos = string::npos;
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

string CSVParser::treatToken(const string& token, int row, int column) {
  if (removeQuotes) {
    return removeQuotesIfAny(token);
  }
  else {
    return token;
  }
}

string CSVParser::removeQuotesIfAny(const string &s) {
  if (s[0] == '"' && s[s.length() - 1] == '"') {
    return s.substr(1, s.length() - 2);
  }
  else {
    return s;
  }
}

long CSVParser::computeFileSize(const std::string& fileName) {
  ifstream csvFile(fileName.c_str());
  // save the current position
  long pos = csvFile.tellg();
  // go to the end of the file
  csvFile.seekg(0, std::ios_base::end);
  // get position = file size
  long size = csvFile.tellg();
  return size;

}
