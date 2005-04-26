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
#include "tulip/MetricProxy.h"
#include "tulip/StringProxy.h"
#include "tulip/SelectionProxy.h"
#include "tulip/LayoutProxy.h"
#include "tulip/IntProxy.h"
#include "tulip/ColorsProxy.h"
#include "tulip/SizesProxy.h"
#include "tulip/MetaGraphProxy.h"

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
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizesProxy*)propertyName)->setNodeValue(n,tmpSiz);
      return true;
    }
    else
      return false;
  }

  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((LayoutProxy*)propertyName)->setNodeValue(n,tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorsProxy*)propertyName)->setNodeValue(n,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    ((StringProxy*)propertyName)->setNodeValue(n,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((MetricProxy*)propertyName)->setNodeValue(n,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntProxy*)propertyName)->setNodeValue(n,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((SelectionProxy*)propertyName)->setNodeValue(n,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((SelectionProxy*)propertyName)->setNodeValue(n,false);
      return true;
    }
    return false;
  }
  return false;
}

bool stringToEdgeProperty(PProxy *propertyName,edge e,string &str) {
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizesProxy*)propertyName)->setEdgeValue(e,tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    LineType::RealType tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      ((LayoutProxy*)propertyName)->setEdgeValue(e,tmpLCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorsProxy*)propertyName)->setEdgeValue(e,tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    ((StringProxy*)propertyName)->setEdgeValue(e,str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((MetricProxy*)propertyName)->setEdgeValue(e,result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntProxy*)propertyName)->setEdgeValue(e,(int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((SelectionProxy*)propertyName)->setEdgeValue(e,true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((SelectionProxy*)propertyName)->setEdgeValue(e,false);
      return true;
    }
    return false;
  }
  return false;
}

string nodePropertyToString(PProxy *propertyName,node n) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraphProxy)) {
    if (((MetaGraphProxy*)propertyName)->getNodeValue(n)!=0) 
      s << ((MetaGraphProxy*)propertyName)->getNodeValue(n)->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    s << ((MetricProxy*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    const Coord &tmpCo=((LayoutProxy*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
      return ((StringProxy*)propertyName)->getNodeValue(n);
    }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
    if ( ((SelectionProxy*)propertyName)->getNodeValue(n))
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    s << ((IntProxy*)propertyName)->getNodeValue(n);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    const Color &tmpCo=((ColorsProxy*)propertyName)->getNodeValue(n);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    const Size &tmpCo=((SizesProxy*)propertyName)->getNodeValue(n);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string edgePropertyToString(PProxy *propertyName,edge e) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraphProxy)) {
      if  ( ((MetaGraphProxy*)propertyName)->getEdgeValue(e)!=0 )
	s << ((MetaGraphProxy*)propertyName)->getEdgeValue(e)->getId();
      else
	s << "0";
      return s.str();
    }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    s << ((MetricProxy*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((LayoutProxy*)propertyName)->getEdgeValue(e);
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
  if (typeid((*propertyName)) == typeid(StringProxy)) {
      return ((StringProxy*)propertyName)->getEdgeValue(e);
    }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
      if (((SelectionProxy*)propertyName)->getEdgeValue(e))
	return string("true");
      else
	return string("false");
    }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    s << ((IntProxy*)propertyName)->getEdgeValue(e);
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    Color tmpCo=((ColorsProxy*)propertyName)->getEdgeValue(e);
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    Size tmpCo=((SizesProxy*)propertyName)->getEdgeValue(e);
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyType(PProxy *propertyName) {
  if (typeid((*propertyName)) == typeid(MetaGraphProxy)) return "metagraph";
  if (typeid((*propertyName)) == typeid(MetricProxy)) return "metric";
  if (typeid((*propertyName)) == typeid(LayoutProxy)) return "layout";
  if (typeid((*propertyName)) == typeid(StringProxy)) return "string";
  if (typeid((*propertyName)) == typeid(IntProxy)) return "int";
  if (typeid((*propertyName)) == typeid(ColorsProxy)) return "color";
  if (typeid((*propertyName)) == typeid(SizesProxy)) return "size";
  if (typeid((*propertyName)) == typeid(SelectionProxy)) return "bool";
  return "unknown";
}

string propertyNodeDefaultValue(PProxy *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraphProxy)) {
    if (((MetaGraphProxy*)propertyName)->getNodeDefaultValue()!=0) 
      s << ((MetaGraphProxy*)propertyName)->getNodeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    s << ((MetricProxy*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    const Coord &tmpCo=((LayoutProxy*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    return ((StringProxy*)propertyName)->getNodeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy))  {
    if (((SelectionProxy*)propertyName)->getNodeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    s << ((IntProxy*)propertyName)->getNodeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    const Color &tmpCo=((ColorsProxy*)propertyName)->getNodeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    const Size &tmpCo=((SizesProxy*)propertyName)->getNodeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

string propertyEdgeDefaultValue(PProxy *propertyName) {
  string result;
  ostringstream s;
  if (typeid((*propertyName)) == typeid(MetaGraphProxy)) {
    if (((MetaGraphProxy*)propertyName)->getEdgeDefaultValue()!=0) 
      s << ((MetaGraphProxy*)propertyName)->getEdgeDefaultValue()->getId();
    else
      s << "0";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    s << ((MetricProxy*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    vector<Coord>::iterator lCoordIt,lCoordItEnd;
    vector<Coord> tmpLi=((LayoutProxy*)propertyName)->getEdgeDefaultValue();
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
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    return ((StringProxy*)propertyName)->getEdgeDefaultValue();
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
    if ( ((SelectionProxy*)propertyName)->getEdgeDefaultValue())
      return string("true");
    else
      return string("false");
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    s << ((IntProxy*)propertyName)->getEdgeDefaultValue();
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    const Color &tmpCo=((ColorsProxy*)propertyName)->getEdgeDefaultValue();
    s << "(" << (unsigned int)tmpCo[0] << "," << (unsigned int)tmpCo[1] ;
    s << "," << (unsigned int)tmpCo[2] << "," << (unsigned int)tmpCo[3] << ")";
    return s.str();
  }
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    const Size &tmpCo=((SizesProxy*)propertyName)->getEdgeDefaultValue();
    s << "(" << tmpCo[0] << "," << tmpCo[1] << "," << tmpCo[2] << ")";
    return s.str();
  }
  return result;
}

//== A deplacer dans les types pour augmenter la simplicit� des extensions
bool stringToAllNodeProperty(PProxy *propertyName, string &str) {
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      //cer << "tulip set nodeValue()" ;
      ((SizesProxy*)propertyName)->setAllNodeValue(tmpSiz);
      //cer << "ok" << endl;
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    Coord tmpCoo;
    if (stringToCoord(str,tmpCoo)) {
      ((LayoutProxy*)propertyName)->setAllNodeValue(tmpCoo);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorsProxy*)propertyName)->setAllNodeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    ((StringProxy*)propertyName)->setAllNodeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((MetricProxy*)propertyName)->setAllNodeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntProxy*)propertyName)->setAllNodeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
      if (strcasecmp(str.c_str(),"true")==0) {
	((SelectionProxy*)propertyName)->setAllNodeValue(true);
	return true;
      }
      if (strcasecmp(str.c_str(),"false")==0) {
	((SelectionProxy*)propertyName)->setAllNodeValue(false);
	return true;
      }
      return false;
  }
  return false;
}

bool stringToAllEdgeProperty(PProxy *propertyName, string &str) {
  //cer << "StringToEdge" << endl;
  if (typeid((*propertyName)) == typeid(SizesProxy)) {
    Size tmpSiz;
    if (stringToSize(str,tmpSiz)) {
      ((SizesProxy*)propertyName)->setAllEdgeValue(tmpSiz);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(LayoutProxy)) {
    vector<Coord> tmpLCoo;
    if (stringToLCoord(str,tmpLCoo)) {
      vector<Coord> tmpL;//=((LayoutProxy*)propertyName)->getEdgeValue(e);
      //tmpL.clear();
      for (unsigned int i=0;i<tmpLCoo.size();++i) {
	tmpL.push_back(tmpLCoo[i]);
      }
      tmpLCoo.clear();
      ((LayoutProxy*)propertyName)->setAllEdgeValue(tmpL);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(ColorsProxy)) {
    Color tmpCol;
    if (stringToColor(str,tmpCol)) {
      ((ColorsProxy*)propertyName)->setAllEdgeValue(tmpCol);
      return true;
    }
    else
      return false;
  }
  if (typeid((*propertyName)) == typeid(StringProxy)) {
    ((StringProxy*)propertyName)->setAllEdgeValue(str);
    return true;
  }
  if (typeid((*propertyName)) == typeid(MetricProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    double result=strtod(startPtr,&endPtr);
    if (endPtr==startPtr) return false;
    ((MetricProxy*)propertyName)->setAllEdgeValue(result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(IntProxy)) {
    char *endPtr=0;
    const char *startPtr=str.c_str();
    long result=strtol(startPtr,&endPtr,10);
    if (endPtr==startPtr) return false;
    ((IntProxy*)propertyName)->setAllEdgeValue((int)result);
    return true;
  }
  if (typeid((*propertyName)) == typeid(SelectionProxy)) {
    if (strcasecmp(str.c_str(),"true")==0) {
      ((SelectionProxy*)propertyName)->setAllEdgeValue(true);
      return true;
    }
    if (strcasecmp(str.c_str(),"false")==0) {
      ((SelectionProxy*)propertyName)->setAllEdgeValue(false);
      return true;
    }
    return false;
  }
  return false;
}



