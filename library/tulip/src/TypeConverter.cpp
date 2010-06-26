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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "tulip/Graph.h"
#include "tulip/TypeConverter.h"
#include "tulip/AbstractProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/GraphProperty.h"

using namespace std;
using namespace tlp;

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
bool stringToNodeProperty(PropertyInterface *propertyName,node n,string &str) {
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizeProperty*)propertyName)->setNodeValue(n,tmpSiz);
      return true;
    }
    else
      return false;
  }

  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((LayoutProperty*)propertyName)->setNodeValue(n,tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorProperty*)propertyName)->setNodeValue(n,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    ((StringProperty*)propertyName)->setNodeValue(n,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((DoubleProperty*)propertyName)->setNodeValue(n,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntegerProperty*)propertyName)->setNodeValue(n,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((BooleanProperty*)propertyName)->setNodeValue(n,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((BooleanProperty*)propertyName)->setNodeValue(n,false);
      return true;
    }
    return false;
  }
  return false;
}

bool stringToEdgeProperty(PropertyInterface *propertyName,edge e,string &str) {
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizeProperty*)propertyName)->setEdgeValue(e,tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    LineType::RealType tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      ((LayoutProperty*)propertyName)->setEdgeValue(e,tmpLCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorProperty*)propertyName)->setEdgeValue(e,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    ((StringProperty*)propertyName)->setEdgeValue(e,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((DoubleProperty*)propertyName)->setEdgeValue(e,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntegerProperty*)propertyName)->setEdgeValue(e,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((BooleanProperty*)propertyName)->setEdgeValue(e,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((BooleanProperty*)propertyName)->setEdgeValue(e,false);
      return true;
    }
    return false;
  }
  return false;
}

string nodePropertyToString(PropertyInterface *propertyName,node n) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(GraphProperty)) {
    if (((GraphProperty*)propertyName)->getNodeValue(n)!=0) 
      s << ((GraphProperty*)propertyName)->getNodeValue(n)->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    s << ((DoubleProperty*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    const Coord &tmpCo=((LayoutProperty*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
      return ((StringProperty*)propertyName)->getNodeValue(n);
    }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
    if ( ((BooleanProperty*)propertyName)->getNodeValue(n))
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    s << ((IntegerProperty*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    const Color &tmpCo=((ColorProperty*)propertyName)->getNodeValue(n);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    const Size &tmpCo=((SizeProperty*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string edgePropertyToString(PropertyInterface *propertyName,edge e) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(GraphProperty)) {
      if  ( ((GraphProperty*)propertyName)->getEdgeValue(e)!=0 )
	s << ((GraphProperty*)propertyName)->getEdgeValue(e)->getId();
      else
	s << "0";
      return s.str();
    }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    s << ((DoubleProperty*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((LayoutProperty*)propertyName)->getEdgeValue(e);
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
  if (typeid((*propertyName)) == typeid(StringProperty)) {
      return ((StringProperty*)propertyName)->getEdgeValue(e);
    }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
      if (((BooleanProperty*)propertyName)->getEdgeValue(e))
	return string("true");
      else
	return string("false");
    }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    s << ((IntegerProperty*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    Color tmpCo=((ColorProperty*)propertyName)->getEdgeValue(e);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    Size tmpCo=((SizeProperty*)propertyName)->getEdgeValue(e);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyType(PropertyInterface *propertyName) {
  if (typeid((*propertyName)) == typeid(GraphProperty)) return "graph";
  if (typeid((*propertyName)) == typeid(DoubleProperty)) return "double";
  if (typeid((*propertyName)) == typeid(LayoutProperty)) return "layout";
  if (typeid((*propertyName)) == typeid(StringProperty)) return "string";
  if (typeid((*propertyName)) == typeid(IntegerProperty)) return "int";
  if (typeid((*propertyName)) == typeid(ColorProperty)) return "color";
  if (typeid((*propertyName)) == typeid(SizeProperty)) return "size";
  if (typeid((*propertyName)) == typeid(BooleanProperty)) return "bool";
  return "unknown";
}

string propertyNodeDefaultValue(PropertyInterface *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(GraphProperty)) {
    if (((GraphProperty*)propertyName)->getNodeDefaultValue()!=0) 
      s << ((GraphProperty*)propertyName)->getNodeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    s << ((DoubleProperty*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    const Coord &tmpCo=((LayoutProperty*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    return ((StringProperty*)propertyName)->getNodeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty))  {
    if (((BooleanProperty*)propertyName)->getNodeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    s << ((IntegerProperty*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    const Color &tmpCo=((ColorProperty*)propertyName)->getNodeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    const Size &tmpCo=((SizeProperty*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyEdgeDefaultValue(PropertyInterface *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(GraphProperty)) {
    if (((GraphProperty*)propertyName)->getEdgeDefaultValue()!=0) 
      s << ((GraphProperty*)propertyName)->getEdgeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    s << ((DoubleProperty*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((LayoutProperty*)propertyName)->getEdgeDefaultValue();
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
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    return ((StringProperty*)propertyName)->getEdgeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
    if ( ((BooleanProperty*)propertyName)->getEdgeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    s << ((IntegerProperty*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    const Color &tmpCo=((ColorProperty*)propertyName)->getEdgeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    const Size &tmpCo=((SizeProperty*)propertyName)->getEdgeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

//== A deplacer dans les types pour augmenter la simplicit� des extensions
bool stringToAllNodeProperty(PropertyInterface *propertyName, string &str) {
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      //cer << "tulip set nodeValue()" ;
      ((SizeProperty*)propertyName)->setAllNodeValue(tmpSiz);
      //cer << "ok" << endl;
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((LayoutProperty*)propertyName)->setAllNodeValue(tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorProperty*)propertyName)->setAllNodeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    ((StringProperty*)propertyName)->setAllNodeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((DoubleProperty*)propertyName)->setAllNodeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntegerProperty*)propertyName)->setAllNodeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
      if (strcasecmp(str.c_str(),"true")==0) {
	((BooleanProperty*)propertyName)->setAllNodeValue(true);
	return true;
      }
      if (strcasecmp(str.c_str(),"false")==0) {
	((BooleanProperty*)propertyName)->setAllNodeValue(false);
	return true;
      }
      return false;
  }
  return false;
}

bool stringToAllEdgeProperty(PropertyInterface *propertyName, string &str) {
  //cer << "StringToEdge" << endl;
  if (typeid((*propertyName)) == typeid(SizeProperty)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizeProperty*)propertyName)->setAllEdgeValue(tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProperty)) {
    vector<Coord> tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      vector<Coord> tmpL;//=((LayoutProperty*)propertyName)->getEdgeValue(e);
      //tmpL.clear();
      for (unsigned int i=0;i<tmpLCoo.size();++i) {
	tmpL.push_back(tmpLCoo[i]);
      }
      tmpLCoo.clear();
      ((LayoutProperty*)propertyName)->setAllEdgeValue(tmpL);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorProperty)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorProperty*)propertyName)->setAllEdgeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProperty)) {
    ((StringProperty*)propertyName)->setAllEdgeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(DoubleProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((DoubleProperty*)propertyName)->setAllEdgeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntegerProperty)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntegerProperty*)propertyName)->setAllEdgeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(BooleanProperty)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((BooleanProperty*)propertyName)->setAllEdgeValue(true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((BooleanProperty*)propertyName)->setAllEdgeValue(false);
      return true;
    }
    return false;
  }
  return false;
}
