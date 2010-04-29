//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 17/2/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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

    virtual void displayError(const std::string &fileName,const std::string &errorMsg){
      std::cerr << errorMsg << std::endl;
    }

  };

}

#endif
