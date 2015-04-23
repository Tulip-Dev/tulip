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
#include <tulip/GlXMLTools.h>

#include <string>

#include <tulip/Gl2DRect.h>
#include <tulip/GlComposite.h>
#include <tulip/GlBox.h>
#include <tulip/GlCircle.h>
#include <tulip/GlGrid.h>
#include <tulip/GlLabel.h>
#include <tulip/GlLine.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlRect.h>
#include <tulip/GlSphere.h>
#include <tulip/GlCurve.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlConvexHull.h>
#include <tulip/GlQuad.h>

using namespace std;

unsigned int tlp::GlXMLTools::indentationNumber=0;

namespace tlp {

void GlXMLTools::applyIndentation(string &outString) {
  for(unsigned int i=0; i<indentationNumber; ++i)
    outString.append("  ");
}

void GlXMLTools::goToNextCaracter(const string &inString, unsigned int &currentPosition) {
  while(inString[currentPosition]==' ' || inString[currentPosition]=='\n')
    currentPosition++;
}

void GlXMLTools::beginDataNode(string &outString) {
  applyIndentation(outString);
  outString.append("<data>\n");
  indentationNumber++;
}

void GlXMLTools::endDataNode(string &outString) {
  indentationNumber--;
  applyIndentation(outString);
  outString.append("</data>\n");
}

void GlXMLTools::enterDataNode(const string &inString, unsigned int &currentPosition) {
  goToNextCaracter(inString, currentPosition);
  assert(inString.substr(currentPosition,6)=="<data>");
  currentPosition+=6;
}

void GlXMLTools::leaveDataNode(const string &inString, unsigned int &currentPosition) {
  goToNextCaracter(inString, currentPosition);
  assert(inString.substr(currentPosition,7)=="</data>");
  currentPosition+=7;
}

void GlXMLTools::beginChildNode(string &outString,const string &name) {
  applyIndentation(outString);
  outString.append("<"+name+">\n");
  indentationNumber++;
}

void GlXMLTools::endChildNode(string &outString,const string &name) {
  indentationNumber--;
  applyIndentation(outString);
  outString.append("</"+name+">\n");
}

string GlXMLTools::enterChildNode(const string &inString, unsigned int &currentPosition) {
  goToNextCaracter(inString, currentPosition);
  unsigned int beginPosition=currentPosition+1;
  size_t endPosition = inString.find('>',currentPosition);

  if(inString.substr(beginPosition-1,endPosition-beginPosition).find("</")!=string::npos)
    return "";

  size_t childNameEndPosition = inString.find(' ',currentPosition);
  currentPosition=endPosition+1;

  if(childNameEndPosition<endPosition)
    return inString.substr(beginPosition,childNameEndPosition-beginPosition);
  else
    return inString.substr(beginPosition,endPosition-beginPosition);
}

void GlXMLTools::leaveChildNode(const string &inString, unsigned int &currentPosition,const string &childName) {
  currentPosition=inString.find("</"+childName+">",currentPosition)+3+childName.size();
}

void GlXMLTools::createProperty(string &outString, const string &name, const string &value, const string &parent) {
  if(parent=="") {
    outString=outString.substr(0,outString.size()-2);
    outString.append(" "+name+"=\""+value+"\">\n");
  }
  else {
    size_t beginPosition=outString.rfind("<"+parent);
    beginPosition=outString.find('>',beginPosition);
    string end=outString.substr(beginPosition);
    outString.resize(beginPosition);
    outString.append(" "+name+"=\""+value+"\"");
    outString.append(end);
  }
}

map<string,string> GlXMLTools::getProperties(const string &inString, unsigned int &currentPosition) {
  map<string,string> properties;
  size_t beginPosition=inString.rfind('<',currentPosition);
  size_t propertyPosition=inString.find('=',beginPosition);

  while(propertyPosition<currentPosition) {
    size_t propertyNameBegin=inString.rfind(' ',propertyPosition)+1;
    size_t propertyValueEnd=inString.find('\"',propertyPosition+2);
    string name=inString.substr(propertyNameBegin,propertyPosition-propertyNameBegin);
    string value=inString.substr(propertyPosition+2,propertyValueEnd-propertyPosition-2);
    properties[name]=value;
    propertyPosition=inString.find('=',propertyPosition+1);
  }

  return properties;
}

GlSimpleEntity *GlXMLTools::createEntity(const string &name) {
  if(name=="GlBox") {
    return new GlBox();
  }
  else if(name=="GlCircle") {
    return new GlCircle();
  }
  else if(name=="GlComplexPolygon") {
    return new GlComplexPolygon();
  }
  else if(name=="GlComposite") {
    return new GlComposite();
  }
  else if(name=="GlConvexHull") {
    return new GlConvexHull;
  }
  else if(name=="GlCurve") {
    return new GlCurve();
  }
  else if(name=="GlGraphComposite") {
  }
  else if(name=="GlGrid") {
    return new GlGrid();
  }
  else if(name=="GlLabel") {
    return new GlLabel();
  }
  else if(name=="GlLine") {
    return new GlLine();
  }
  else if(name=="GlPolygon") {
    return new GlPolygon();
  }
  else if(name=="GlQuad") {
    return new GlQuad();
  }
  else if(name=="GlRect") {
    return new GlRect();
  }
  else if(name=="GlRectTextured") {
    return new Gl2DRect();
  }
  else if(name=="Gl2DRect") {
    return new Gl2DRect();
  }
  else if(name=="GlSphere") {
    return new GlSphere();
  }
  else {
    tlp::debug() << "Unknow entity type : " << name << ". Can't create it !" << endl;
  }

  return NULL;
}

}
