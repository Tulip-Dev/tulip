//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 29/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLGRAPHINPUTDATA_H
#define Tulip_GLGRAPHINPUTDATA_H

#include "tulip/tulipconf.h"

#include <tulip/MutableContainer.h>

#include "tulip/GlGraphRenderingParameters.h"


namespace tlp {

  class LayoutProperty;
  class DoubleProperty;
  class StringProperty;
  class IntegerProperty;
  class BooleanProperty;
  class SizeProperty;
  class ColorProperty;
  class GraphProperty;
  class PropertyManager;
  class Graph;
  class Glyph;
 
  class TLP_GL_SCOPE GlGraphInputData {
    
  public:

    GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters);

    Graph* getGraph() {return graph;}
    
  public:

    ColorProperty    *elementColor;
    ColorProperty    *elementLabelColor;
    SizeProperty     *elementSize;
    IntegerProperty  *elementLabelPosition;
    IntegerProperty  *elementShape;
    DoubleProperty   *elementRotation;
    BooleanProperty  *elementSelected;
    StringProperty   *elementLabel;
    LayoutProperty   *elementLayout;
    GraphProperty    *elementGraph;
    StringProperty   *elementTexture;
    ColorProperty    *elementBorderColor;
    DoubleProperty   *elementBorderWidth;

    Graph* graph;

    GlGraphRenderingParameters* parameters;

    MutableContainer<Glyph *> glyphs;

  };
}

#endif
