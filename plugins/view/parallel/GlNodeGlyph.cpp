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
#include <tulip/Glyph.h>
#include <tulip/GlDisplayListManager.h>
#include "ParallelTools.h"

#include <iostream>

namespace tlp {

void cube(GLenum type) {
    //  GLenum type = GL_LINE_LOOP;
    /* front face */
    glBegin(type);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    /* back face */
    glBegin(type);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    /* right face */
    glBegin(type);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
    /* left face */
    glBegin(type);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
   //#include <tulip/GlTLPFeedBackBuilder.h>

 glVertex3f(-0.5f, -0.5f, 0.5f); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    /* top face */
    glBegin(type);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    /* bottom face */
    glBegin(type);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
}

  GlNodeGlyph::GlNodeGlyph(Coord position, GlGraphInputData *dat, node n) : data(dat), glyphPosition(position), nodeId(n) {
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
      glColor4ub(colorSelect2[0], colorSelect2[1],  colorSelect2[2], colorSelect2[3]); 
      cube(GL_LINE_LOOP);
      glPopAttrib();
      GlDisplayListManager::getInst().endNewDisplayList();
    }
    
    Size nodeSize = data->elementSize->getNodeValue(nodeId);
    nodeSize *= 3;

    glPushMatrix();
    glTranslatef(glyphPosition[0], glyphPosition[1], glyphPosition[2]);
    glRotatef(data->elementRotation->getNodeValue(nodeId), 0., 0., 1.);
    glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);
    
    data->glyphs.get(data->elementShape->getNodeValue(nodeId))->draw(nodeId);
    
    if (data->elementSelected->getNodeValue(nodeId)) {
      glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil()-1,0xFFFF);
      GlDisplayListManager::getInst().callDisplayList("selection");
      glStencilFunc(GL_LEQUAL,data->parameters->getNodesStencil(),0xFFFF);
    }
    glPopMatrix(); 
  }
  
}
