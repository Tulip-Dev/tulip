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
#include <fstream>
#include <cassert>
using namespace std;
using namespace tlp;

CSVParser::CSVParser() :
  removeQuotes(true) {
}

CSVParser::~CSVParser() {
}

const vector<vector<string> > & CSVParser::parse(const string& fileName, const string& separator,
    unsigned int numberOfRows, unsigned int numberOfCol) {
  tokens.clear();
  ifstream csvFile(fileName.c_str());
  string line;
  vector<string> lines;
  while (getline(csvFile, line) && lines.size() < numberOfRows) {
    lines.push_back(line);
  }
  tokens.resize(lines.size());
  size_t maxNumberOfCol = 0;
  for (int i = 0; i < lines.size(); ++i) {
    tokenize(lines[i], tokens[i], separator, numberOfCol);
    maxNumberOfCol = max(maxNumberOfCol, tokens[i].size());
  }

  //Ensure lines have the right col number
  for (int i = 0; i < tokens.size(); ++i) {
    tokens[i].resize(maxNumberOfCol);
  }

  for (int i = 0; i < tokens.size(); ++i) {
    for (int j = 0; j < tokens[i].size(); ++j) {
      tokens[i][j] = treatToken(tokens[i][j], i, j);
    }
  }
  return tokens;
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

vector<string> CSVParser::extractRow(unsigned int rowNumber, unsigned int colBegin, unsigned int colNumber) {
  assert(tokens.size() >= rowNumber);
  assert(tokens[rowNumber].size() >= colBegin + colNumber);
  vector<string> data;

  for (int i = 0; i < colNumber; ++i) {
    data .push_back(tokens[rowNumber][colBegin + i]);
  }
  return data;
}
vector<string> CSVParser::extractColumn(unsigned int colNumber, unsigned int rowBegin, unsigned int rowNumber) {
  assert(tokens.size() >= rowBegin + rowNumber);
  assert(tokens[0].size() >= colNumber);
  vector<string> data;

  for (int i = 0; i < rowNumber; ++i) {
    data.push_back(tokens[rowBegin + i][colNumber]);
  }
  return data;
}
