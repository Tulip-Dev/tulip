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
#ifndef Tulip_GLMETANODERENDERER_H
#define Tulip_GLMETANODERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/Graph.h>

#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
namespace tlp {

  class GlGraphInputData;
  class Camera;

  /**
   * Class use to render a meta node, this version render nothing
   */
  class TLP_GL_SCOPE GlMetaNodeRenderer {

  public:

    virtual ~GlMetaNodeRenderer() {}

    virtual void prerender(node,float,Camera*) {}
    virtual void render(node,float,Camera*) {}
    virtual bool havePrerender() {return false;}
    virtual bool glMetaNodeHaveToRenderLabels(){return true;}

    virtual void setInputData(GlGraphInputData *) {}
    virtual GlGraphInputData *getInputData() {return NULL;}

  };

}

#endif // Tulip_GLMETANODERENDERER_H
