/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef Tulip_PREFERENCEMANAGER_H
#define Tulip_PREFERENCEMANAGER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tulip/tulipconf.h"
#include "tulip/Color.h"

namespace tlp {

  /** \brief Class to manage preference
   * Singleton class to load/store preference need by Tulip
   */
  class TLP_SCOPE PreferenceManager {


  public:

    /**
     * Create the preference manager singleton
     */
    static void createInst();
    /**
     * Return the pereference manager singleton, il singleton doesn't exist this function create it
     */
    static PreferenceManager &getInst() {
      if(!inst)
        inst=new PreferenceManager();
      return *inst;
    }

    /**
     * Set if tulip auto connect at startup
     */
    void setNetworkConnection(bool connection){
      networkConnection=connection;
    }

    /**
     * Get if tulip auto connect at startup
     */
    bool getNetworkConnection() {
      return networkConnection;
    }

    /**
     * Set color for selection
     */
    void setSelectionColor(const Color &color) {
      selectionColor=color;
    }

    /**
     * Get color for selection
     */
    const Color& getSelectionColor() {
      return selectionColor;
    }

    /**
     * Set if tulip auto load controller
     */
    void setAutoLoadController(bool autoLoad){
      autoLoadController=autoLoad;
    }

    /**
     * Get if tulip auto load controller
     */
    bool getAutoLoadController() {
      return autoLoadController;
    }

  private:

    /**
     * empty private constructor for singleton
     */
    PreferenceManager() {}

    static PreferenceManager* inst;

    bool networkConnection;
    Color selectionColor;
    bool autoLoadController;

  };

}

#endif // Tulip_GLTEXTUREMANAGER_H
