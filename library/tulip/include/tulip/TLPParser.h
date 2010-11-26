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
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <iostream>
#include <sstream>
#include <string>
#include <list>

#include <tulip/PluginProgress.h>

namespace tlp {

struct ParserError {
  ParserError(int err=0,int lin=0,int cha=0):errorNumber(err),lineInFile(lin),charInLine(cha){}
  int errorNumber;
  int lineInFile;
  int charInLine;
};

#define LEXICALPARSERERROR 0
#define GRAMMARPARSERERROR 1

struct TLPValue {
  std::string str;
  long integer;
  double real;
  bool boolean;
  std::pair<long, long> range;
};

 enum TLPToken{ BOOLTOKEN,ENDOFSTREAM,STRINGTOKEN,INTTOKEN,DOUBLETOKEN,IDTOKEN,ERRORINFILE,OPENTOKEN,CLOSETOKEN,COMMENTTOKEN,RANGETOKEN};
//=====================================================================================
struct TLPTokenParser {
  int curLine;
  int curChar;
  std::istream &is;
  TLPTokenParser(std::istream &i):curLine(0),curChar(0),is(i){}
  TLPToken nextToken(TLPValue &val,int &curPos) {
    val.str.erase();
    bool endOfStream=false,strGet=false,slashMode=false,started=false,stop=false,strComment=false;
    char ch;
    while ( (!stop) && (endOfStream=is.get(ch))) {
      curPos++;
      curChar++;
      if (strGet)
	switch (ch) {
	case 13 :
	case '\n': curChar=0;curLine++;val.str+=ch; break;
	case '\t': val.str+="    "; break;
	case '\\':if (!slashMode) {slashMode=true;} else {val.str+=ch;slashMode=false;} break;
	case '"':if (!slashMode)  {return STRINGTOKEN;} else {val.str+=ch;slashMode=false;} break;
	case 'n':if (slashMode)   {val.str+='\n'; slashMode=false; break;}
	default:
	  if (!slashMode)
	    val.str+=ch;
	  slashMode=false;
	  break;
	}
      else
	if (strComment)
	  switch (ch) {
	  case 13 :
	  case '\n': curChar=0;curLine++;stop=true;return COMMENTTOKEN; break;
	  default: curChar++;val.str+=ch;break;
	  }
	else
	  switch (ch) {
	  case ' ':
	  case '\t': if (started) stop=true; break;
	  case 13 :
	  case '\n': curChar=0;curLine++;if (started) stop=true; break;
	  case '(': if (!started) return OPENTOKEN; else {--curPos,--curChar;is.unget();stop=true;} break;
	  case ')': if (!started) return CLOSETOKEN; else {--curPos,--curChar;is.unget();stop=true;} break;
	  case '"': strGet=true;if (started) {--curPos,--curChar;is.unget();stop=true;} else started=true;break;
	  case ';': strComment=true;if (started) {--curPos,--curChar;is.unget();stop=true;} else started=true;break;
	  default : val.str+=ch;started=true;break;
	  }
    }
    if (!started && !endOfStream) return ENDOFSTREAM;
    char *endPtr=0;
    const char *cstr = val.str.c_str();
    errno = 0;
    long resultl= strtol(cstr, &endPtr, 10);
    if (errno == ERANGE)
      return ERRORINFILE;
    unsigned long strlength = val.str.length();
    if (endPtr==(cstr+ strlength)) {
      val.integer=resultl;
      return INTTOKEN;
    }
    // check for a range
    if (endPtr > cstr && (cstr + strlength) > (endPtr + 2)) {
      val.range.first = resultl;
      if ((endPtr[0] == '.') && (endPtr[1] == '.')) {
	char* beginPtr = endPtr + 2;
	errno = 0;
	resultl = strtol(beginPtr, &endPtr, 10);
	if (errno == ERANGE)
	  return ERRORINFILE;
	if (endPtr == (cstr + strlength)) {
	  if (resultl < val.range.first)
	    return ERRORINFILE;
	  val.range.second = resultl;
	  return RANGETOKEN;
	}
      }
    }
    endPtr=0;

    double resultd=strtod(cstr, &endPtr);
    if (errno == ERANGE)
      return ERRORINFILE;
    if (endPtr==(cstr + strlength)) {
      val.real=resultd;
      return DOUBLETOKEN;
    }
    if (strcasecmp(cstr, "true")==0) {val.boolean=true;return BOOLTOKEN;}
    if (strcasecmp(cstr, "false")==0) {val.boolean=false;return BOOLTOKEN;}
    if (started) return STRINGTOKEN;
    return ERRORINFILE;
  }
};
//=====================================================================================
struct TLPBuilder {
  virtual ~TLPBuilder() {}
  virtual bool addBool(const bool)=0;
  virtual bool addInt(const int)=0;
  virtual bool addRange(int, int)=0;
  virtual bool addDouble(const double)=0;
  virtual bool addString(const std::string &)=0;
  virtual bool addStruct(const std::string&,TLPBuilder*&)=0;  
  virtual bool close() =0;
  virtual bool canRead() { return false; }
  virtual bool read(std::istream&) { return false; }
};

struct TLPTrue:public TLPBuilder {
  bool addBool(const bool) {return true;}
  bool addInt(const int) {return true;}
  bool addRange(int, int) {return true;}
  bool addDouble(const double) {return true;}
  bool addString(const std::string &) {return true;}
  bool addStruct(const std::string& /*structName*/, TLPBuilder*&newBuilder) {
    newBuilder=new TLPTrue();
    return true;
  }
  bool close(){return true;}
};

struct TLPFalse:public TLPBuilder {
  bool addBool(const bool) {return false;}
  bool addInt(const int) {return false;}
  bool addRange(int, int) {return false;}
  bool addDouble(const double) {return false;}
  bool addString(const std::string &) {return false;}
  bool addStruct(const std::string& /*structName*/, TLPBuilder*&newBuilder) {
    newBuilder=new TLPFalse();
    return false;
  }
  bool close(){return true;}
};

struct TLPWriter:public TLPBuilder {
  bool addBool(const bool boolean)  {std::cout << "bool::" << boolean << std::endl;return true;}
  bool addInt(const int integer)  {std::cout << "int::" << integer << std::endl;return true;}
  bool addRange(int first, int second)  {std::cout << "range::" << first << ".." << second << std::endl;return true;}
  bool addDouble(const double real) {std::cout.flags(std::ios::scientific); std::cout << "real::" << real << std::endl;return true;}
  bool addString(const std::string &str)  {std::cout << "string::" << str << std::endl;return true;}
  bool addStruct(const std::string& structName,TLPBuilder*&newBuilder) 
  {
    std::cout << "struct::" << structName << std::endl;
    newBuilder=new TLPWriter();
    return true;
  }
  bool close() {std::cout << "EndStruct::"<< std::endl;return true;}
};
//=====================================================================================
template <bool displayComment> 
struct TLPParser {
  std::list<TLPBuilder *> builderStack;
  std::istream &inputStream;
  TLPTokenParser *tokenParser;
  PluginProgress *pluginProgress;
  int fileSize,curPos;

  TLPParser(std::istream &inputStream,TLPBuilder *builder,PluginProgress *pluginProgress,int size): inputStream(inputStream),
       pluginProgress(pluginProgress),
       fileSize(size),
       curPos(0) {
    builderStack.push_front(builder);
  }

  ~TLPParser() {
    while (!builderStack.empty()) {
      TLPBuilder *builder = builderStack.front();
      builderStack.pop_front();
      if (!builderStack.empty() && builder != builderStack.front())
	delete builder;
    }
  }

  bool formatError() {
    std::stringstream ess;
    ess << "Error when parsing char " << tokenParser->curChar
	<< " at line " << tokenParser->curLine + 1;
    if (errno)
      ess << std::endl << strerror(errno);
    pluginProgress->setError(ess.str());
    return false;
  }

  bool parse() {
    TLPTokenParser tParser(inputStream);
    tokenParser= &tParser;
    TLPToken currentToken;
    TLPValue currentValue;
    while ((currentToken=tokenParser->nextToken(currentValue,curPos))!=ENDOFSTREAM) {
	if (curPos%2000==1) if (pluginProgress->progress(curPos,fileSize)!=TLP_CONTINUE) 
	  return pluginProgress->state()!=TLP_CANCEL;
	switch (currentToken) {
	  case OPENTOKEN:
	    currentToken=tokenParser->nextToken(currentValue,curPos);
	    if (currentToken!=STRINGTOKEN)
	      return formatError();//we can throw an exeption
	    TLPBuilder *newBuilder;
	    if (builderStack.front()->addStruct(currentValue.str, newBuilder)) {	      
		builderStack.push_front(newBuilder);
		if (newBuilder->canRead())
		  if (!newBuilder->read(inputStream))
		    return formatError();
	      }
	    else
	      return formatError();
	    break;
	  case BOOLTOKEN:
	    if (!builderStack.front()->addBool(currentValue.boolean))
	      return formatError();
	    break;
	  case INTTOKEN:
	    if (!builderStack.front()->addInt(currentValue.integer))
	      return formatError();
	    break;
	  case RANGETOKEN:
	    if (!builderStack.front()->addRange(currentValue.range.first,
						currentValue.range.second))
	      return formatError();
	    break;
	  case DOUBLETOKEN:
	    if (!builderStack.front()->addDouble(currentValue.real))
	      return formatError();
	    break;
	  case STRINGTOKEN:
	    if (!builderStack.front()->addString(currentValue.str))
	      return formatError();
	    break;
	  case CLOSETOKEN:
	    if (builderStack.front()->close()) {
	      TLPBuilder * builder = builderStack.front();
	      builderStack.pop_front();
	      if (builder != builderStack.front())
		delete builder;
	    } else
	      return formatError();
	    break;
	  case ERRORINFILE:
	    return formatError();
	  case ENDOFSTREAM:return true;
	  case COMMENTTOKEN:if (displayComment) std::cout << "Comment line:" << tokenParser->curLine << "->" <<  currentValue.str << std::endl;break;
	  default:break;
	  }
      }
    return true;
  }
};

}
//=====================================================================================

#endif // DOXYGEN_NOTFOR_DEVEL
