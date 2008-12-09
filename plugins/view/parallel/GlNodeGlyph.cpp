//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include "GlNodeGlyph.h"

#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Glyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTools.h>
#include "ParallelTools.h"

#include <iostream>

using namespace std;

namespace tlp {

  GlNodeGlyph::GlNodeGlyph(const Coord &position, const Size &glyphSize, GlGraphInputData *dat, node n) : data(dat), glyphPosition(position), glyphSize(glyphSize), nodeId(n) {
    const Size &nodeSize = data->elementSize->getNodeValue(nodeId);
    boundingBox.check(Coord(position[0] - nodeSize[0] / 2., position[1] - nodeSize[1] / 2., position[2] - nodeSize[2] / 2.));
    boundingBox.check(Coord(position[0] + nodeSize[0] / 2., position[1] + nodeSize[1] / 2., position[2] + nodeSize[2] / 2.));
  }

  void GlNodeGlyph::draw(float lod,Camera *camera) {

    if (GlDisplayListManager::getInst().beginNewDisplayList("selection")) {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDepthFunc(GL_LEQUAL);
      glLineWidth(3);
      setColor(COLOR_SELECT);
      cube(GL_LINE_LOOP);
      glPopAttrib();
      GlDisplayListManager::getInst().endNewDisplayList();
    }

    glPushMatrix();
    glTranslatef(glyphPosition[0], glyphPosition[1], glyphPosition[2]);
    glRotatef(data->elementRotation->getNodeValue(nodeId), 0., 0., 1.);
    glScalef(glyphSize[0], glyphSize[1], glyphSize[2]);

    data->glyphs.get(data->elementShape->getNodeValue(nodeId))->draw(nodeId,lod);

    if (data->elementSelected->getNodeValue(nodeId)) {
    	glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil()-1,0xFFFF);
    	GlDisplayListManager::getInst().callDisplayList("selection");
    	glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
    }
    glPopMatrix();
  }

}
