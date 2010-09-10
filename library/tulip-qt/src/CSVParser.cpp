/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/CSVParser.h"
#include "tulip/CSVContentHandler.h"
#include <tulip/PluginProgress.h>
#include <QtGui/QApplication>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;
using namespace tlp;

CSVParser::CSVParser() :
  removeQuotes(true) {
}

CSVParser::~CSVParser() {
}

void CSVParser::parse(const string& fileName, const string& separator,
		      CSVContentHandler* handler, PluginProgress* progress) {
  assert(handler);
  handler->begin();
  ifstream csvFile(fileName.c_str());

  unsigned int row = 0;
  unsigned int columnMax = 0;
  if (csvFile) {
    csvFile.seekg(0, std::ios_base::end);
    // get position = file size
    unsigned long fileSize = csvFile.tellg(), readSize = 0;
    // reset position
    csvFile.seekg(0, std::ios_base::beg);
    string line;
    vector<string> tokens;

    unsigned int displayProgressEachLineNumber = 10;

    if (progress) {
      progress->progress(0, 100);
    }
    while (getline(csvFile, line)) {
      if (progress) {
	readSize += line.size();
        if (progress->state() != TLP_CONTINUE) {
          break;
        }
        //Each displayProgressEachLineNumber display progression
        if (row % displayProgressEachLineNumber == 0) {
          // compute progression in function of read size and file size.
          progress->progress(readSize, fileSize);
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
void CSVParser::tokenize(const string& str, vector<string>& tokens,
			 const string& delimiters, unsigned int numberOfCol) {
  // Skip delimiters at beginning.
  string::size_type lastPos = 0;
  string::size_type pos = 0;
  //If the next token begin with a double quote search the ending double quote before searching the next delimiter.
  if(str[pos]=='"'){
    pos = str.find_first_of('"', pos+1);
    //ensure there is a ending double quote.
    assert(pos != string::npos);
  }
  // Find first "non-delimiter".
   pos = str.find_first_of(delimiters, pos);
  while (string::npos != pos || string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    if (tokens.size() == numberOfCol) {
      break;
    }
    if (pos != string::npos){
      pos = pos +1;
      lastPos = pos;
    }
    else{
      lastPos = string::npos;
    }
    // Find next "non-delimiter"
    //If the next token begin with a double quote search the ending double quote before searching the next delimiter.
    if(str[pos]=='"'){
        pos = str.find_first_of('"', pos+1);
        //ensure there is a ending double quote.
        assert(pos != string::npos);
      }
    //Find the next token
    pos = str.find_first_of(delimiters, pos);
  }
}

const string spaceChars = " \t\r\n";
string CSVParser::treatToken(const string& token, int, int) {
  string currentToken = token;
  // erase space chars at the beginning/end of the value
  // and replace multiple occurences of space chars by a blank
  string::size_type beginPos = currentToken.find_first_of(spaceChars);
  while (beginPos != string::npos) {
    string::size_type endPos =
      currentToken.find_first_not_of(spaceChars, beginPos);
    if (beginPos == 0) {
      // erase space chars at the beginning
      if (endPos != string::npos)
	currentToken.erase(beginPos, endPos - beginPos);
      else
	// only space chars in currentToken
	currentToken.clear();
      beginPos = currentToken.find_first_of(spaceChars);
    } else {
      if (endPos == string::npos) {
	// erase space chars at the end
	currentToken.erase(beginPos);
	break;
      }
      // replace multiple space chars
      currentToken.replace(beginPos, endPos - beginPos, 1, ' ');
      beginPos = currentToken.find_first_of(spaceChars, beginPos + 1);
    }
  }

  if (removeQuotes) {
    return removeQuotesIfAny(currentToken);
  }
  else {
    return currentToken;
  }
}

const string rejectedChars = "\"\r";
string CSVParser::removeQuotesIfAny(const string &s) {
  string::size_type beginPos = s.find_first_not_of(rejectedChars);
  string::size_type endPos = s.find_last_not_of(rejectedChars);
  if (beginPos != string::npos && endPos != string::npos)
    return s.substr(beginPos, endPos - beginPos + 1);
  else
    return s;
}
