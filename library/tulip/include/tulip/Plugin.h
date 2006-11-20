//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
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
  /**Return the release number of the plug-in*/ 
  virtual  std::string getRelease() const=0;
  /**Return the version number of the plug-in*/ 
  virtual  std::string getVersion() const=0;
};

}
#endif
