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

#include <QTextCodec>

#include <tulip/CSVParser.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginProgress.h>

#include <fstream>
#include <cassert>

using namespace std;
using namespace tlp;

const string defaultRejectedChars = " \r\n";
const string spaceChars = " \t";
CSVSimpleParser::CSVSimpleParser(const string& fileName,const QString& separator, const bool mergesep, char textDelimiter,const string& fileEncoding,unsigned int firstLine,unsigned int lastLine):_fileName(fileName),_separator(separator),_textDelimiter(textDelimiter),_fileEncoding(fileEncoding),_firstLine(firstLine),_lastLine(lastLine),_mergesep(mergesep) {
}

CSVSimpleParser::~CSVSimpleParser() {
}

string CSVSimpleParser::convertStringEncoding(const std::string& toConvert,QTextCodec* encoder) {
  return string(encoder->toUnicode ( toConvert.c_str() ).toUtf8().data());
}

bool CSVSimpleParser::parse(CSVContentHandler* handler, PluginProgress* progress) {
  if(!handler) {
    return false;
  }

  handler->begin();
  istream *csvFile = tlp::getInputFileStream(_fileName.c_str(),ifstream::in|ifstream::binary);

  if (*csvFile) {
    //Real row number used to
    unsigned int row = 0;
    //Read row number
    unsigned int columnMax = 0;

    csvFile->seekg(0, std::ios_base::end);
    // get position = file size
    unsigned long fileSize = csvFile->tellg(), readSize = 0;
    // reset position
    csvFile->seekg(0, std::ios_base::beg);
    string line;
    vector<string> tokens;

    unsigned int displayProgressEachLineNumber = 200;

    QTextCodec * codec = QTextCodec::codecForName ( _fileEncoding.c_str());

    if(codec == NULL) {
      qWarning() << __PRETTY_FUNCTION__<<":"<<__LINE__<<" Cannot found the convertion codec to convert from "<<_fileEncoding<<" string will be treated as utf8.";
      codec = QTextCodec::codecForName("UTF-8");
    }

    if (progress) {
      progress->progress(0, 100);
    }

    while (multiplatformgetline(*csvFile, line) && row <=_lastLine) {

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

      if(!line.empty() && row >= _firstLine) {
        //Correct the encoding of the line.
        line = convertStringEncoding(line,codec);
        tokens.clear();
        tokenize(line, tokens, _separator,_mergesep, _textDelimiter, 0);
        unsigned int column = 0;

        for (column = 0; column < tokens.size(); ++column) {
          tokens[column]= treatToken(tokens[column], row, column);
        }

        handler->line(row,tokens);
        columnMax = max(columnMax, column);

        //If user want to stop break the import process.
        if (progress) {
          if (progress->state() != TLP_CONTINUE) {
            break;
          }
        }
      }

      ++row;
    }

    handler->end(row, columnMax);
    delete csvFile;
    return true;
  }
  else {
    delete csvFile;
    return false;
  }
}

bool CSVSimpleParser::multiplatformgetline ( istream& is, string& str ) {
  //nothing new to read.
  if(is.eof())
    return false;

  str.clear();
  str.reserve(2048);
  char c;
  bool tdlm = false;

  while(is.get(c)) {
    if (c == _textDelimiter) {
      tdlm = !tdlm;
      str.push_back(c);
      continue;
    }

    //Carriage return Windows and mac
    if(c=='\r') {
      //Check if the next character is \n and remove it.
      if(is.get(c) && c != '\n') {
        is.unget();
      }

      if (!tdlm)
        break;
    }
    else if(c=='\n' && !tdlm) {
      break;
    }

    //Push the character
    str.push_back(c);
  }

  //End of line reading.
  return true;
}

void CSVSimpleParser::tokenize(const string& str, vector<string>& tokens,
                               const QString& delimiters, const bool mergedelim, char textDelimiter, unsigned int) {
  // Skip delimiters at beginning.
  string::size_type lastPos = 0;
  string::size_type pos = 0;
  bool quit = false;

  string delim(delimiters.toStdString());

  while(!quit) {
    //Don't search tokens in chars sourrounded by text delimiters.
    assert(pos!=string::npos);
    assert(pos < str.size());

    if(str[pos]==textDelimiter) {
      //Go the the next text delimiter .
      pos = str.find_first_of(textDelimiter, pos+1);
    }

    //Find the delimiter
    pos = str.find(delim, pos);

    //if merge delimiter, skip the next char if it is a delimiter
    if(mergedelim) {
      while((pos<str.length()-delim.size())&&(str.substr(pos+1, delim.length())==delim))
        pos+=delim.length();
    }

    //Extracting tokens.
    assert(lastPos!=string::npos);
    size_t nbExtractedChars=string::npos;

    if(pos == string::npos) {
      //If no delimiter found extract the rest of the line.
      nbExtractedChars = pos;
    }
    else {
      //Compute the extracted char number
      nbExtractedChars = pos - lastPos;
    }

    try {
      tokens.push_back(str.substr(lastPos, nbExtractedChars));
    }
    catch (...) {
      //An error occur quit the line parsing.
      break;
    }

    //Go to the begin of the next token.
    if(pos != string::npos && pos +1 < str.size()) {
      //Skip the delimiter.
      ++pos;
      //Store the begin position of the next token
      lastPos=pos;
    }
    else {
      //End of line found quit
      quit = true;
    }
  }
}


string CSVSimpleParser::treatToken(const string& token, int, int) {
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
    }
    else {
      if (endPos == string::npos) {
        // erase space chars at the end
        currentToken.erase(beginPos);
        break;
      }

      // replace multiple space chars
      if (endPos - beginPos > 1)
        currentToken.replace(beginPos, endPos - beginPos, 1, ' ');

      beginPos = currentToken.find_first_of(spaceChars, beginPos + 1);
    }
  }

  //Treat string to remove special characters from its beginning and its end.
  string rejectedChars = defaultRejectedChars;
  rejectedChars.push_back(_textDelimiter);
  return removeQuotesIfAny(currentToken,rejectedChars);
}


string CSVSimpleParser::removeQuotesIfAny(const string &s,const std::string& rejectedChars) {
  string::size_type beginPos = s.find_first_not_of(rejectedChars);
  string::size_type endPos = s.find_last_not_of(rejectedChars);

  if (beginPos != string::npos && endPos != string::npos) {
    try {
      return s.substr(beginPos, endPos - beginPos + 1);
    }
    catch (...) {
      return s;
    }
  }
  else {
    return s;
  }
}

CSVInvertMatrixParser::CSVInvertMatrixParser(CSVParser* parser):parser(parser) {
}

CSVInvertMatrixParser::~CSVInvertMatrixParser() {
  delete parser;
}

bool CSVInvertMatrixParser::parse(CSVContentHandler *handler, PluginProgress *progress) {
  this->handler = handler;
  return parser->parse(this,progress);
}

void CSVInvertMatrixParser::begin() {
  maxLineSize = 0;
}

void CSVInvertMatrixParser::line(unsigned int ,const std::vector<std::string>& lineTokens) {
  maxLineSize = max(maxLineSize,static_cast<unsigned int>(lineTokens.size()));
  columns.push_back(lineTokens);
}

void CSVInvertMatrixParser::end(unsigned int , unsigned int ) {
  handler->begin();
  vector<string> tokens(columns.size());

  //Fill the line wiht
  for(unsigned int line = 0 ; line < maxLineSize ; ++line) {
    for(unsigned int i = 0 ; i < columns.size() ; ++i ) {
      //Check if the column is large enough
      tokens[i]=columns[i].size() > line ? columns[i][line] : string();
    }

    handler->line(line,tokens);
  }

  handler->end(maxLineSize,columns.size());
}
