#include "GlNodeGlyph.h"
#include "ParallelTools.h"

#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Glyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTools.h>

#include <iostream>

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
      glColor4ub(COLOR_SELECT[0], COLOR_SELECT[1],  COLOR_SELECT[2], COLOR_SELECT[3]);
      cube(GL_LINE_LOOP);
      glPopAttrib();
      GlDisplayListManager::getInst().endNewDisplayList();
    }

    glPushMatrix();
    glTranslatef(glyphPosition[0], glyphPosition[1], glyphPosition[2]);
    glRotatef(data->elementRotation->getNodeValue(nodeId), 0., 0., 1.);
    glScalef(glyphSize[0], glyphSize[1], glyphSize[2]);

    if(lod>=4){
      data->glyphs.get(data->elementShape->getNodeValue(nodeId))->draw(nodeId, lod);

      if (data->elementSelected->getNodeValue(nodeId)) {
        glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil()-1,0xFFFF);
        GlDisplayListManager::getInst().callDisplayList("selection");
        glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
      }
    }else{
      glDisable(GL_LIGHTING);

      Color color=data->elementColor->getNodeValue(nodeId);
      if(data->elementSelected->getNodeValue(nodeId))
        color=COLOR_SELECT;

      setColor(color);
      glPointSize(lod);
      glBegin(GL_POINTS);
      glVertex3f(0,0,0.1);
      glEnd();

      glEnable(GL_LIGHTING);
    }

    glPopMatrix();
  }

}
