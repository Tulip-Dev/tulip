#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "tulip/SuperGraph.h"
#include "tulip/TypeConverter.h"
#include "tulip/PropertyManager.h"
#include "tulip/PropertyProxy.h"
#include "tulip/Metric.h"
#include "tulip/String.h"
#include "tulip/Selection.h"
#include "tulip/Layout.h"
#include "tulip/Int.h"
#include "tulip/Colors.h"
#include "tulip/Sizes.h"
#include "tulip/MetaGraph.h"

using namespace std;

bool stringToColor(string &str,Color &col) {
  char strC[512];
  strcpy(strC,str.c_str());
  char *endPtr;
  long h,w,d,x;
  unsigned int i,j;
  i=0;
  while (i<str.length()) {
    if (str[i]=='(') {i++;break;}
    i++;
  }
  if (i>=str.length()) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  h=strtol(strC+j,&endPtr,10);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  w=strtol(strC+j,&endPtr,10);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  x=strtol(strC+j,&endPtr,10);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==')') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  d=strtol(strC+j,&endPtr,10);
  if (endPtr==(strC+j)) {return false;}
  col.set(h,w,x,d);
  return true;
}

bool stringToSize(string &str,Size &siz) {
  //  cerr << "stringtosize str:\""<<str<<"\"" << endl;
  char strC[512];
  //  strC=(char *)malloc(sizeof(char)*str.length());
  //  if (strC==0) cerr << "pointeur null";
  strcpy(strC,str.c_str());
  char *endPtr;
  double h,w,d;
  unsigned int i,j;
  i=0;
  while (i<str.length()) {
    if (str[i]=='(') {i++;break;}
    i++;
  }
  if (i>=str.length()) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  w=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  h=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==')') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  d=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  siz.set(w,h,d);
  //  free(strC);
  //  cerr << "..ok" << endl;
  return true;
}

bool stringToCoord(string &str,Coord &coo) {
  char strC[512];
  strcpy(strC,str.c_str());
  char *endPtr;
  double h,w,d;
  unsigned int i,j;
  i=0;
  while (i<str.length()) {
    if (str[i]=='(') {i++;break;}
    i++;
  }
  if (i>=str.length()) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  h=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==',') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  w=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  j=i;
  while (i<str.length()) {
    if (str[i]==')') {i++;break;}
    i++;
  }
  endPtr=strC+(i-2);
  d=strtod(strC+j,&endPtr);
  if (endPtr==(strC+j)) {return false;}
  coo[0]=h;
  coo[1]=w;
  coo[2]=d;
  return true;
}


bool stringToLCoord(string &str,LineType::RealType &lcoo) {
  //cer << "String to L coord" << endl;
  char strC[1024];
  strcpy(strC,str.c_str());
  unsigned int i,j;
  i=0;
  while (i<str.length()) {
      if (str[i]=='(') {i++;break;}
      i++;
    }
  if (i>=str.length()) {return false;}
  j=i;
  while (i<str.length()) {
      if (str[i]=='(') 
	{
	  //	  cerr << "Start Coord :" << i << "," << str[i] << endl;
	  unsigned int start=i;
	  i++;
	  while (i<str.length()) {
	      if (str[i]==')') {i++;break;}
	      i++;
	    }
	  //	  cerr << "End Coord :" << i << "," << str[i] << endl; 
	  if (i>=str.length()) {return false;}
	  char tmpStr[512];
	  strncpy(tmpStr,strC+start,i-start);
	  tmpStr[i-start+1]=0;
	  //	  cerr << "Find coord :" << tmpStr;
	  Coord tmpCoord;
	  string tmpString=tmpStr;
	  if (stringToCoord(tmpString,tmpCoord))
	    lcoo.push_back(tmpCoord);
	  else {
	      return false;
	    }
	  i--;
	}
      else
	if (str[i]==')') {return true;}
      i++;
    }
  if (i>=str.length()) {return false;}
  return true;
}

//== A deplacer dans les types pour augmenter la simplicit� des extensions
bool stringToNodeProperty(PProxy *propertyName,node n,string &str) {
  if (typeid((*propertyName)) == typeid(Sizes)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((Sizes*)propertyName)->setNodeValue(n,tmpSiz);
      return true;
    }
    else
      return false;
  }

  if (typeid((*propertyName)) == typeid(Layout)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((Layout*)propertyName)->setNodeValue(n,tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((Colors*)propertyName)->setNodeValue(n,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(String)) {
    ((String*)propertyName)->setNodeValue(n,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((Metric*)propertyName)->setNodeValue(n,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((Int*)propertyName)->setNodeValue(n,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Selection)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((Selection*)propertyName)->setNodeValue(n,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((Selection*)propertyName)->setNodeValue(n,false);
      return true;
    }
    return false;
  }
  return false;
}

bool stringToEdgeProperty(PProxy *propertyName,edge e,string &str) {
  if (typeid((*propertyName)) == typeid(Sizes)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((Sizes*)propertyName)->setEdgeValue(e,tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    LineType::RealType tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      ((Layout*)propertyName)->setEdgeValue(e,tmpLCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((Colors*)propertyName)->setEdgeValue(e,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(String)) {
    ((String*)propertyName)->setEdgeValue(e,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((Metric*)propertyName)->setEdgeValue(e,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((Int*)propertyName)->setEdgeValue(e,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Selection)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((Selection*)propertyName)->setEdgeValue(e,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((Selection*)propertyName)->setEdgeValue(e,false);
      return true;
    }
    return false;
  }
  return false;
}

string nodePropertyToString(PProxy *propertyName,node n) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraph)) {
    if (((MetaGraph*)propertyName)->getNodeValue(n)!=0) 
      s << ((MetaGraph*)propertyName)->getNodeValue(n)->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    s << ((Metric*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    const Coord &tmpCo=((Layout*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(String)) {
      return ((String*)propertyName)->getNodeValue(n);
    }
  if (typeid((*propertyName)) == typeid(Selection)) {
    if ( ((Selection*)propertyName)->getNodeValue(n))
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    s << ((Int*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    const Color &tmpCo=((Colors*)propertyName)->getNodeValue(n);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Sizes)) {
    const Size &tmpCo=((Sizes*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string edgePropertyToString(PProxy *propertyName,edge e) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraph)) {
      if  ( ((MetaGraph*)propertyName)->getEdgeValue(e)!=0 )
	s << ((MetaGraph*)propertyName)->getEdgeValue(e)->getId();
      else
	s << "0";
      return s.str();
    }
  if (typeid((*propertyName)) == typeid(Metric)) {
    s << ((Metric*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((Layout*)propertyName)->getEdgeValue(e);
    lCoordIt = tmpLi.begin();
    lCoordItEnd = tmpLi.end();
    s << "(" ;
    if (lCoordIt!=lCoordItEnd) {
      for (;lCoordIt!=lCoordItEnd;++lCoordIt) {
	s << *lCoordIt ; //"(" << lCoordIt->getX() << "," << lCoordIt->getY() << "," << lCoordIt->getZ() << ")" ;
      }
    }
    s << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(String)) {
      return ((String*)propertyName)->getEdgeValue(e);
    }
  if (typeid((*propertyName)) == typeid(Selection)) {
      if (((Selection*)propertyName)->getEdgeValue(e))
	return string("true");
      else
	return string("false");
    }
  if (typeid((*propertyName)) == typeid(Int)) {
    s << ((Int*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    Color tmpCo=((Colors*)propertyName)->getEdgeValue(e);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Sizes)) {
    Size tmpCo=((Sizes*)propertyName)->getEdgeValue(e);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyType(PProxy *propertyName) {
  if (typeid((*propertyName)) == typeid(MetaGraph)) return "metagraph";
  if (typeid((*propertyName)) == typeid(Metric)) return "metric";
  if (typeid((*propertyName)) == typeid(Layout)) return "layout";
  if (typeid((*propertyName)) == typeid(String)) return "string";
  if (typeid((*propertyName)) == typeid(Int)) return "int";
  if (typeid((*propertyName)) == typeid(Colors)) return "color";
  if (typeid((*propertyName)) == typeid(Sizes)) return "size";
  if (typeid((*propertyName)) == typeid(Selection)) return "bool";
  return "unknown";
}

string propertyNodeDefaultValue(PProxy *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraph)) {
    if (((MetaGraph*)propertyName)->getNodeDefaultValue()!=0) 
      s << ((MetaGraph*)propertyName)->getNodeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    s << ((Metric*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    const Coord &tmpCo=((Layout*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(String)) {
    return ((String*)propertyName)->getNodeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(Selection))  {
    if (((Selection*)propertyName)->getNodeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    s << ((Int*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    const Color &tmpCo=((Colors*)propertyName)->getNodeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Sizes)) {
    const Size &tmpCo=((Sizes*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyEdgeDefaultValue(PProxy *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraph)) {
    if (((MetaGraph*)propertyName)->getEdgeDefaultValue()!=0) 
      s << ((MetaGraph*)propertyName)->getEdgeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    s << ((Metric*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((Layout*)propertyName)->getEdgeDefaultValue();
    lCoordIt = tmpLi.begin();
    lCoordItEnd = tmpLi.end();
    s << "(";
    if (lCoordIt!=lCoordItEnd) {
      for (;lCoordIt!=lCoordItEnd;++lCoordIt) {
	s << *lCoordIt; // "(" << lCoordIt->getX() << "," << lCoordIt->getY() << "," << lCoordIt->getZ() << ")" ;
      }
    }
    s << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(String)) {
    return ((String*)propertyName)->getEdgeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(Selection)) {
    if ( ((Selection*)propertyName)->getEdgeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    s << ((Int*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    const Color &tmpCo=((Colors*)propertyName)->getEdgeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(Sizes)) {
    const Size &tmpCo=((Sizes*)propertyName)->getEdgeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

//== A deplacer dans les types pour augmenter la simplicit� des extensions
bool stringToAllNodeProperty(PProxy *propertyName, string &str) {
  if (typeid((*propertyName)) == typeid(Sizes)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      //cer << "tulip set nodeValue()" ;
      ((Sizes*)propertyName)->setAllNodeValue(tmpSiz);
      //cer << "ok" << endl;
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((Layout*)propertyName)->setAllNodeValue(tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((Colors*)propertyName)->setAllNodeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(String)) {
    ((String*)propertyName)->setAllNodeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((Metric*)propertyName)->setAllNodeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((Int*)propertyName)->setAllNodeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Selection)) {
      if (strcasecmp(str.c_str(),"true")==0) {
	((Selection*)propertyName)->setAllNodeValue(true);
	return true;
      }
      if (strcasecmp(str.c_str(),"false")==0) {
	((Selection*)propertyName)->setAllNodeValue(false);
	return true;
      }
      return false;
  }
  return false;
}

bool stringToAllEdgeProperty(PProxy *propertyName, string &str) {
  //cer << "StringToEdge" << endl;
  if (typeid((*propertyName)) == typeid(Sizes)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((Sizes*)propertyName)->setAllEdgeValue(tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Layout)) {
    vector<Coord> tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      vector<Coord> tmpL;//=((Layout*)propertyName)->getEdgeValue(e);
      //tmpL.clear();
      for (unsigned int i=0;i<tmpLCoo.size();++i) {
	tmpL.push_back(tmpLCoo[i]);
      }
      tmpLCoo.clear();
      ((Layout*)propertyName)->setAllEdgeValue(tmpL);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(Colors)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((Colors*)propertyName)->setAllEdgeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(String)) {
    ((String*)propertyName)->setAllEdgeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Metric)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((Metric*)propertyName)->setAllEdgeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Int)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((Int*)propertyName)->setAllEdgeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(Selection)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((Selection*)propertyName)->setAllEdgeValue(true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((Selection*)propertyName)->setAllEdgeValue(false);
      return true;
    }
    return false;
  }
  return false;
}



