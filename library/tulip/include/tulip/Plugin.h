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

#ifndef TULIPPLUGIN_H
#define TULIPPLUGIN_H
#include <string>

namespace tlp {

/** Common interface for plug-ins.*/
class Plugin {
public:
  virtual ~Plugin(){}
  /**Return the name of the plug-in, be careful, 
     this name must be unique, if two plug-ins have the same name
     only the latest loaded will be available*/
  virtual  std::string getName() const=0;
  /* Returns the group name of the plug-in,
     a string with subgroup names separated by ::
     ex: trees::planar trees */
  virtual std::string getGroup() const=0;
  /**Return the author of the plug-in*/
  virtual  std::string getAuthor() const=0;
  /**Return the date of creation of the plug-in*/
  virtual  std::string getDate() const=0;
  /**Return free information about the plug-in*/  
  virtual  std::string getInfo() const=0;
  /**Return the whole release number of the plug-in*/ 
  virtual  std::string getRelease() const=0;
  /**Return the major release number of the plug-in*/ 
  virtual  std::string getMajor() const=0;
  /**Return the minor release number of the plug-in*/ 
  virtual  std::string getMinor() const=0;
  /**Return the whole Tulip release number the plug-in is build with*/ 
  virtual  std::string getTulipRelease() const=0;
  /**Return the major Tulip release number the plug-in is build with*/ 
  virtual  std::string getTulipMajor() const=0;
  /**Return the major Tulip release number the plug-in is build with*/ 
  virtual  std::string getTulipMinor() const=0;
};

}
#endif
