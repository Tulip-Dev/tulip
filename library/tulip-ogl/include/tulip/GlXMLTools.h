/*
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
///@cond DOXYGEN_HIDDEN

#ifndef DOXYGEN_NOTFOR_DEVEL

#ifndef Tulip_GLXMLTOOLS_H
#define Tulip_GLXMLTOOLS_H

#include <sstream>
#include <vector>
#include <map>
#include <cassert>

#include <tulip/tulipconf.h>

namespace tlp {

/**
 * static tools class use to store/load xml data
 */
class GlSimpleEntity;

class TLP_GL_SCOPE GlXMLTools {

public :

  /**
   * Create and enter into a new data node
   */
  static void beginDataNode(std::string &outString);

  /**
   * End last data node
   */
  static void endDataNode(std::string &outString);

  /**
   * Enter into a data node and set the currentPosition integer to the position next the data tag
   */
  static void enterDataNode(const std::string &inString, unsigned int &currentPosition);

  /**
   * Leave a data node and set the currentPosition integer to the position next the data end tag
   */
  static void leaveDataNode(const std::string &inString, unsigned int &currentPosition);

  /**
   * Create and enter into a new children node with name : name
   */
  static void beginChildNode(std::string &outString, const std::string &name="children");

  /**
   * End last children node with name : name
   */
  static void endChildNode(std::string &outString, const std::string &name="children");

  /**
   * Enter into a child node and set the currentPosition integer to the position next the child tag
   * \return child name
   */
  static std::string enterChildNode(const std::string &inString, unsigned int &currentPosition);

  /**
   * Leave into a child node and set the currentPosition integer to the position next the child end tag
   * \return child name
   */
  static void leaveChildNode(const std::string &inString, unsigned int &currentPosition, const std::string &childName);

  /**
   * Create a property with name and value in rootNode
   */
  static void createProperty(std::string &outString, const std::string &name, const std::string &value, const std::string &parent="");

  /**
   * Get properties associated with the last opened child
   * These properties is returned into a map of string/string
   */
  static std::map<std::string,std::string> getProperties(const std::string &inString, unsigned int &currentPosition);

  /**
   * Create a GlEntity with the given name
   */
  static GlSimpleEntity *createEntity(const std::string &name);

  /**
   * Get the XML output for a vector of Object
   */
  template <typename Obj>
  static void getXML(std::string &outString,const std::string &name,const typename std::vector<Obj> &vect) {

    std::stringstream str;
    str << "(" ;
    typename std::vector<Obj>::const_iterator it=vect.begin();
    assert(it != vect.end());
    str << *it ;
    ++it;

    for(; it!=vect.end(); ++it) {
      str << "," << *it ;
    }

    str << ")" ;
    outString.append("<"+name+">"+str.str()+"</"+name+">\n");
  }

  /**
   * Set vector of Object with the given XML
   */
  template <typename Obj>
  static void setWithXML(const std::string &inString, unsigned int &currentPosition,const std::string &name,std::vector<Obj> &vect) {

    goToNextCaracter(inString,currentPosition);

    std::string nameTag=inString.substr(currentPosition,name.size()+2);
    assert(nameTag=="<"+name+">");
    currentPosition+=name.size()+2;

    size_t endValuePosition=inString.find("</"+name+">",currentPosition);
    assert(endValuePosition!=std::string::npos);

    std::istringstream is(inString.substr(currentPosition,endValuePosition-currentPosition));
    Obj data;
    char c=is.get();

    while(c!=')') {
      is >> data ;
      vect.push_back(data);
      c=is.get();
    }

    currentPosition=endValuePosition+name.size()+3;

  }

  /**
   * Get the XML output for an Object
   */
  template <typename Obj>
  static void getXML(std::string &outString, const std::string &name, const Obj &value) {
    std::stringstream str;
    str << value ;
    applyIndentation(outString);
    outString.append("<"+name+">"+str.str()+"</"+name+">\n");
  }


  static bool checkNextXMLtag(const std::string &inString, unsigned int &currentPosition, const std::string &name) {
    unsigned int tmp=currentPosition;
    goToNextCaracter(inString,tmp);
    std::string nameTag=inString.substr(tmp,name.size()+2);
    return (nameTag=="<"+name+">");
  }

  /**
   * Set an Object with the given XML
   */
  template <typename Obj>
  static void setWithXML(const std::string &inString, unsigned int &currentPosition, const std::string &name, Obj &value) {

    goToNextCaracter(inString,currentPosition);

    std::string nameTag=inString.substr(currentPosition,name.size()+2);
    assert(nameTag=="<"+name+">");
    currentPosition+=name.size()+2;

    size_t endValuePosition=inString.find("</"+name+">",currentPosition);
    assert(endValuePosition!=std::string::npos);

    std::stringstream str(inString.substr(currentPosition,endValuePosition-currentPosition));
    str >> value;
    currentPosition=endValuePosition+name.size()+3;
  }

  /**
   * Set an Object with the given XML and default value
   */
  template <typename Obj>
  static void setWithXML(const std::string &inString, unsigned int &currentPosition, const std::string &name, Obj &value,const Obj &defValue) {
    goToNextCaracter(inString,currentPosition);

    std::string nameTag=inString.substr(currentPosition,name.size()+2);

    if(nameTag=="<"+name+">") {
      currentPosition+=name.size()+2;

      size_t endValuePosition=inString.find("</"+name+">",currentPosition);
      assert(endValuePosition!=std::string::npos);

      std::stringstream str(inString.substr(currentPosition,endValuePosition-currentPosition));
      str >> value;
      currentPosition=endValuePosition+name.size()+3;
    }
    else {
      value=defValue;
    }
  }

private :

  static void applyIndentation(std::string &outString);
  static void goToNextCaracter(const std::string &inString, unsigned int &currentPosition);

  static unsigned int indentationNumber;

};

}
#endif

#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
