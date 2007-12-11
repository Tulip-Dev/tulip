//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLSPHERE_H
#define Tulip_GLSPHERE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  class TLP_GL_SCOPE GlSphere : public GlSimpleEntity{ // : public ObservableScene {
  
  public:
     
    GlSphere(const Coord &position,float radius);

    virtual void draw(float lod);

  private:
    
    Coord position;
    float radius;

  };
  
}

#endif // Tulip_GLSCENE_H
