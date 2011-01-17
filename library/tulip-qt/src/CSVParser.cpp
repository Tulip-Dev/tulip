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
#include <tulip/PluginProgress.h>
#include <QtGui/QApplication>

#include <iostream>

#include <fstream>
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;
using namespace tlp;

const string defaultRejectedChars = " \r\n";
const string spaceChars = " \t\r\n";
CSVSimpleParser::CSVSimpleParser(const string& fileName,const string& separator,char textDelimiter,const string& fileEncoding):fileName(fileName),separator(separator),textDelimiter(textDelimiter),fileEncoding(fileEncoding){
}

CSVSimpleParser::~CSVSimpleParser() {
}

bool CSVSimpleParser::parse(CSVContentHandler* handler, PluginProgress* progress) {
    if(!handler){
        return false;
    }
    handler->begin();
    ifstream csvFile(fileName.c_str(),ifstream::in|ifstream::binary);

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

        QTextCodec * codec = QTextCodec::codecForName ( fileEncoding.c_str());
        if(codec == NULL){
            std::cerr << __PRETTY_FUNCTION__<<":"<<__LINE__<<" Cannot found the convertion codec to convert from "<<fileEncoding<<" string will be treated as utf8."<<std::endl;
            codec = QTextCodec::codecForName("UTF-8");
        }

        if (progress) {
            progress->progress(0, 100);
        }
        while (multiplatformgetline(csvFile, line)) {

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
            //If the line is empty continue.
            if(line.empty()){
                continue;
            }
            //Correct the encoding of the line.
            line = convertStringEncoding(line,codec);
            tokens.clear();
            tokenize(line, tokens, separator,textDelimiter, 0);
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
        handler->end(row, columnMax);
        return true;
    }else{
        return false;
    }
}

bool CSVSimpleParser::multiplatformgetline ( istream& is, string& str ){
    //nothing new to read.
    if(is.eof())
        return false;
    str.clear();
    str.reserve(2048);
    char c;
    while(is.get(c)){
        //Carriage return Windows and mac
        if(c=='\r'){
            //Check if the next character is /n and remove it.
            if(is.get(c) && c != '\n'){
                is.unget();
            }
            break;
        }else
            if(c=='\n'){
            break;
        }else{
            //Push the character
            str.push_back(c);
        }
    }
    //End of line reading.
    return true;
}

void CSVSimpleParser::tokenize(const string& str, vector<string>& tokens,
                               const string& delimiters,char textDelimiter, unsigned int numberOfCol) {
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
        //If the next token begin with the text delimiter char search the ending double quote before searching the next delimiter.
        if(str[pos]== textDelimiter){
            pos = str.find_first_of(textDelimiter, pos+1);
            //ensure there is a ending double quote.
            assert(pos != string::npos);
        }
        //Find the next token
        pos = str.find_first_of(delimiters, pos);
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
    //Treat string to remove special characters from it's beginning and its end.
    string rejectedChars = defaultRejectedChars;
    rejectedChars.push_back(textDelimiter);
    return removeQuotesIfAny(currentToken,rejectedChars);
}


string CSVSimpleParser::removeQuotesIfAny(const string &s,const std::string& rejectedChars) {
    string::size_type beginPos = s.find_first_not_of(rejectedChars);
    string::size_type endPos = s.find_last_not_of(rejectedChars);    
    if (beginPos != string::npos && endPos != string::npos)
        return s.substr(beginPos, endPos - beginPos + 1);
    else
        return s;
}

CSVInvertMatrixParser::CSVInvertMatrixParser(CSVParser* parser):parser(parser){
}

CSVInvertMatrixParser::~CSVInvertMatrixParser(){
    delete parser;
}

bool CSVInvertMatrixParser::parse(CSVContentHandler *handler, PluginProgress *progress){
    this->handler = handler;
    return parser->parse(this,progress);
}

void CSVInvertMatrixParser::begin(){
    handler->begin();
}

void CSVInvertMatrixParser::token(unsigned int row, unsigned int column, const string &token){
    handler->token(column,row,token);
}

void CSVInvertMatrixParser::end(unsigned int rowNumber, unsigned int columnNumber){
    handler->end(columnNumber,rowNumber);
}
