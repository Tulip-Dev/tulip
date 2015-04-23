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

#ifndef Tulip_GLDISPLAYLISTMANAGER_H
#define Tulip_GLDISPLAYLISTMANAGER_H

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <map>
#include <string>

#include <stdint.h>

#include <tulip/tulipconf.h>

namespace tlp {

/** \brief Singleton used to manage OpenGl display list
 *
 * Singleton used to manage OpenGl display list.
 * First createInst must be call.
 */
class TLP_GL_SCOPE GlDisplayListManager {

  typedef std::map<std::string,GLuint> DisplayListMap;
  typedef std::map<uintptr_t, DisplayListMap> ContextAndDisplayListMap;

public:

  /**
   * Return the current instance. If instance doesn't exist, create it.
   */
  static GlDisplayListManager &getInst() {
    if(!inst)
      inst=new GlDisplayListManager();

    return *inst;
  }

  /**
   * Change OpenGl context because display list can't be shared in different context
   */
  void changeContext(uintptr_t context);

  /**
   * remove context
   */
  void removeContext(uintptr_t context);

  /**
   * Begin to record a new display list with name : name
   */
  bool beginNewDisplayList(const std::string& name);
  /**
   * End the current record of display list
   */
  void endNewDisplayList();

  /**
   * Call display list with name : name
   */
  bool callDisplayList(const std::string& name);

private:

  /**
   * Private constructor for singleton
   */
  GlDisplayListManager() {}

  uintptr_t currentContext;

  static GlDisplayListManager* inst;

  ContextAndDisplayListMap displayListMap;

};

}

#endif // Tulip_GLDISPLAYLISTMANAGER_H
///@endcond
