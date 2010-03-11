//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 25/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLGRAPHSTATICDATA_H
#define Tulip_GLGRAPHSTATICDATA_H

#include <tulip/tulipconf.h>

#include <string>

namespace tlp {

  enum EdgeShape {POLYLINESHAPE = 0, BEZIERSHAPE = 4, SPLINESHAPE = 8, CUBICBSPLINE=16};
  
  class TLP_GL_SCOPE GlGraphStaticData {
  
  public:

    static const int edgeShapesCount;
    static int edgeShapeIds[];
    static std::string labelPositionNames[];
    
    static std::string edgeShapeName(int id);
    static int edgeShapeId(std::string name);
  
    static std::string labelPositionName(int id);
    static int labelPositionId(std::string name);
    
  };
  
}

#endif // Tulip_GLGRAPHSTATICDATA_H
