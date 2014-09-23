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
#include <string>
#include <list>
#include <cstring>

/*
  Il faut ajouter la gestion correcte des erreurs et cest nikel.
 */

struct ParserError {
  ParserError(int err=0,int lin=0,int cha=0):errorNumber(err),lineInFile(lin),charInLine(cha) {}
  int errorNumber;
  int lineInFile;
  int charInLine;
};

struct GMLValue {
  std::string str;
  long integer;
  double real;
  bool boolean;
};

enum GMLToken { BOOLTOKEN,ENDOFSTREAM,STRINGTOKEN,INTTOKEN,DOUBLETOKEN,ERRORINFILE,OPENTOKEN,CLOSETOKEN };
//=====================================================================================
struct GMLTokenParser {
  int curLine;
  int curChar;
  std::istream &is;
  GMLTokenParser(std::istream &i):curLine(0),curChar(0),is(i) {}
  GMLToken nextToken(GMLValue &val) {
    val.str.erase();
    bool endOfStream=false,strGet=false,slashMode=false,started=false,stop=false;
    char ch;

    while ( (!stop) && (endOfStream=bool(is.get(ch)))) {
      curChar++;

      if (strGet)
        switch (ch) {
        case 13 :
          break;

        case '\n':
          curChar=0;
          curLine++;
          val.str+=ch;
          break;

        case '\\':

          if (!slashMode) {
            slashMode=true;
          }
          else {
            val.str+=ch;
            slashMode=false;
          }

          break;

        case '"':

          if (!slashMode) {
            return STRINGTOKEN;
          }
          else {
            val.str+=ch;
            slashMode=false;
          }

          break;

        default:
          slashMode=false;
          val.str+=ch;
          break;
        }
      else
        switch (ch) {
        case 13 :
          break;

        case ' ' :

          if (started) stop=true;

          break;

        case '\t':

          if (started) stop=true;

          break;

        case '\n':
          curChar=0;
          curLine++;

          if (started) stop=true;

          break;

        case '[':

          if (!started) return OPENTOKEN;
          else {
            is.unget();
            stop=true;
          }

          break;

        case ']':

          if (!started) return CLOSETOKEN;
          else {
            is.unget();
            stop=true;
          }

          break;

        case '"':
          strGet=true;

          if (started) {
            is.unget();
            stop=true;
          }
          else started=true;

          break;

        default :
          val.str+=ch;
          started=true;
          break;
        }
    }

    if (!started && !endOfStream) return ENDOFSTREAM;

    char *endPtr=NULL;
    long resultl=strtol(val.str.c_str(),&endPtr,10);

    if (endPtr==(val.str.c_str()+val.str.length())) {
      val.integer=resultl;
      return INTTOKEN;
    }

    endPtr=NULL;
    double resultd=strtod(val.str.c_str(),&endPtr);

    if (endPtr==(val.str.c_str()+val.str.length())) {
      val.real=resultd;
      return DOUBLETOKEN;
    }

    if (strcasecmp(val.str.c_str(),"true")==0) {
      val.boolean=true;
      return BOOLTOKEN;
    }

    if (strcasecmp(val.str.c_str(),"false")==0) {
      val.boolean=false;
      return BOOLTOKEN;
    }

    if (started) return STRINGTOKEN;

    return ERRORINFILE;
  }
};
//=====================================================================================
struct GMLBuilder {
  virtual ~GMLBuilder() {}
  virtual bool addBool(const std::string &,const bool)=0;
  virtual bool addInt(const std::string &,const int)=0;
  virtual bool addDouble(const std::string &,const double)=0;
  virtual bool addString(const std::string &,const std::string &)=0;
  virtual bool addStruct(const std::string&,GMLBuilder*&)=0;
  virtual bool close() =0;
};

struct GMLTrue:public GMLBuilder {
  bool addBool(const std::string &,const bool) {
    return true;
  }
  bool addInt(const std::string &,const int) {
    return true;
  }
  bool addDouble(const std::string &,const double) {
    return true;
  }
  bool addString(const std::string &,const std::string &) {
    return true;
  }
  bool addStruct(const std::string&,GMLBuilder*&newBuilder) {
    newBuilder=new GMLTrue();
    return true;
  }
  bool close() {
    return true;
  }
};

struct GMLFalse:public GMLBuilder {
  bool addBool(const std::string &,const bool) {
    return false;
  }
  bool addInt(const std::string &,const int) {
    return false;
  }
  bool addDouble(const std::string &,const double) {
    return false;
  }
  bool addString(const std::string &,const std::string &) {
    return false;
  }
  bool addStruct(const std::string&,GMLBuilder*&newBuilder) {
    newBuilder=new GMLFalse();
    return false;
  }
  bool close() {
    return true;
  }
};

struct GMLWriter:public GMLBuilder {
  bool addBool(const std::string &st,const bool boolean)  {
    std::cout << st << " ==> " << "bool::" << boolean << std::endl;
    return true;
  }
  bool addInt(const std::string &st,const int integer)  {
    std::cout << st << " ==> " << "int::" << integer << std::endl;
    return true;
  }
  bool addDouble(const std::string &st,const double real) {
    std::cout.flags(std::ios::scientific);
    std::cout << st << " ==> " << "real::" << real << std::endl;
    return true;
  }
  bool addString(const std::string &st,const std::string &str)  {
    std::cout << st << " ==> " << "string::" << str << std::endl;
    return true;
  }
  bool addStruct(const std::string& structName,GMLBuilder*&newBuilder) {
    std::cout << "struct::" << structName << std::endl;
    newBuilder=new GMLWriter();
    return true;
  }
  bool close() {
    std::cout << "EndStruct::"<< std::endl;
    return true;
  }
};
//=====================================================================================
template <bool displayComment>
struct GMLParser {
  std::list<GMLBuilder *> builderStack;
  std::istream &inputStream;

  GMLParser(std::istream &inputStream,GMLBuilder *builder):inputStream(inputStream) {
    builderStack.push_front(builder);
  }
  ~GMLParser() {
    while (!builderStack.empty()) {
      delete builderStack.front();
      builderStack.pop_front();
    }
  }
  bool parse() {
    GMLTokenParser tokenParser(inputStream);
    GMLToken currentToken;
    GMLToken nextToken;
    GMLValue currentValue;
    GMLValue nextValue;

    while ((currentToken=tokenParser.nextToken(currentValue))!=ENDOFSTREAM) {
      switch (currentToken) {
      case STRINGTOKEN:
        nextToken=tokenParser.nextToken(nextValue);

        switch (nextToken) {
        case OPENTOKEN:
          GMLBuilder *newBuilder;

          if (builderStack.front()->addStruct(currentValue.str,newBuilder)) {
            builderStack.push_front(newBuilder);
          }
          else {
            return false;
          }

          break;

        case BOOLTOKEN:

          if (!builderStack.front()->addBool(currentValue.str,nextValue.boolean)) {
            std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
            return false;
          }

          break;

        case INTTOKEN:

          if (!builderStack.front()->addInt(currentValue.str,nextValue.integer)) {
            std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
            return false;
          }

          break;

        case DOUBLETOKEN:

          if (!builderStack.front()->addDouble(currentValue.str,nextValue.real)) {
            std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
            return false;
          }

          break;

        case STRINGTOKEN:

          if (!builderStack.front()->addString(currentValue.str,nextValue.str)) {
            std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
            return false;
          }

          break;

        case ERRORINFILE:
          return false;
          break;

        case ENDOFSTREAM:
          return true;
          break;

        default:
          break;
        }

        break;

      case CLOSETOKEN:

        if (builderStack.front()->close())
          delete builderStack.front();
        else {
          std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
          return false;
        }

        builderStack.pop_front();
        break;

      default:
        std::cerr << "Error parsing stream line :" << tokenParser.curLine << " char : " << tokenParser.curChar << std::endl;
        return false;
      }
    }

    return true;
  }
};
//=====================================================================================
