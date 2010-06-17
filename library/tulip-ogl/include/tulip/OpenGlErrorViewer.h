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
#ifndef Tulip_OPENGLERRORVIEWER_H
#define Tulip_OPENGLERRORVIEWER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <iostream>


namespace tlp {

  /** \brief Class used to display OpenGl errors
   *
   * Class used to display OpenGl errors
   */
  class TLP_GL_SCOPE OpenGlErrorViewer {

  public :

    /**
     * Simple error display with cerr output
     */
    virtual void displayError(const std::string &title,const std::string &errorMsg){
      std::cerr << title << " : " << errorMsg << std::endl;
    }

    /**
     * Simple error display with cerr output
     */
    virtual void displayErrorWithAskAgain(const std::string &title,const std::string &errorMsg){
      displayError(title,errorMsg);
    }

  };

}

#endif
