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

#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tulip/GlTextureManager.h>

#include "GlCylinder.h"

#include <iostream>



namespace tlp {

const int dth = 20;		// no. of angular cylinder subdivsions
const int dz = 20;		// no. of cylinder subdivsions in z direction

void setMaterial(const tlp::Color &c) {
    float colorMat[4];
    colorMat[0] = ((float)c[0])/255.0;
    colorMat[1] = ((float)c[1])/255.0;
    colorMat[2] = ((float)c[2])/255.0;
    colorMat[3] = ((float)c[3])/255.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorMat);
}

  GlCylinder::GlCylinder(const Coord &start, const Coord &end, const float startRad, const float endRad, const Color &c, const bool withCaps) :
    startPoint(start), endPoint(end), startRadius(startRad), endRadius(endRad), displayCaps(withCaps), color(c), textureFile("")   {

    boundingBox.check(Coord(startPoint.getX() - startRad, startPoint.getY() + startRad, startPoint.getZ() + startRad));
    boundingBox.check(Coord(endPoint.getX() + endRad, endPoint.getY() - endRad, endPoint.getZ() - endRad));
    computeVectors();
  }

  GlCylinder::GlCylinder(const Coord &start, const Coord &end, const float startRad, const float endRad, const std::string& textFile, const Color &c, const bool withCaps) : startPoint(start), endPoint(end), startRadius(startRad), endRadius(endRad), displayCaps(withCaps), color(c),  textureFile(textFile)  {

    boundingBox.check(Coord(startPoint.getX() - startRad, startPoint.getY() + startRad, startPoint.getZ() + startRad));
    boundingBox.check(Coord(endPoint.getX() + endRad, endPoint.getY() - endRad, endPoint.getZ() - endRad));
    computeVectors();
  }


  void GlCylinder::computeVectors() {

    // orientation vector
    vx = endPoint.getX() - startPoint.getX() ;
    vy = endPoint.getY() - startPoint.getY();
    vz = endPoint.getZ() - startPoint.getZ();

    // cylinder length
    cylinderLength = sqrt( vx*vx + vy*vy + vz*vz );

    // normalize orientation vector
    vx = vx / cylinderLength;
    vy = vy / cylinderLength;
    vz = vz / cylinderLength;

    // rotation vector
    rx = vx;
    ry = vy;
    rz = 1 + vz;

    if ((rx == 0.0) && (ry == 0.0) && (rz == 0.0) ) // exception!
      rx = 1;
  }

  void GlCylinder::draw(float lod,Camera *camera) {

    GLUquadricObj *cyl;
    cyl  = gluNewQuadric();

    gluQuadricNormals(cyl, GLU_SMOOTH);
    gluQuadricTexture(cyl, GL_TRUE);
    gluQuadricDrawStyle(cyl, GLU_FILL);

    if (textureFile != "") {
      GlTextureManager::getInst().activateTexture(textureFile);
    }

    setMaterial(color);

    glPushMatrix();

    glTranslated(startPoint.getX(), startPoint.getY(), startPoint.getZ());	// translate to point 1
    glRotatef(180,rx,ry,rz);

    if (displayCaps)
      gluDisk( cyl, 0.0, startRadius, dth, 1);		// draw bottom of cylinder

    gluCylinder(cyl, startRadius, endRadius, cylinderLength, dth, dz);	// draw side faces of cylinder

    glPopMatrix();

    if (displayCaps) {
      glPushMatrix();
      glTranslated( endPoint.getX(), endPoint.getY(), endPoint.getZ());	// translate to point 2
      glRotatef(180,rx,ry,rz);
      gluDisk( cyl, 0.0, endRadius, dth, 1);		// draw top of cylinder
      glPopMatrix();
    }

    gluDeleteQuadric( cyl );

    GlTextureManager::getInst().desactivateTexture();

  }


  void GlCylinder::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::createProperty(rootNode, "type", "GlCylinder");

    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"startPosition",startPoint);
    GlXMLTools::getXML(dataNode,"endPosition",endPoint);
    GlXMLTools::getXML(dataNode,"startRadius",startRadius);
    GlXMLTools::getXML(dataNode,"endRadius",endRadius);
    GlXMLTools::getXML(dataNode,"color",color);
    GlXMLTools::getXML(dataNode,"textureFile",textureFile);

  }
  //============================================================
  void GlCylinder::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {

      GlXMLTools::setWithXML(dataNode,"startPosition",startPoint);
      GlXMLTools::setWithXML(dataNode,"endPosition",endPoint);
      GlXMLTools::setWithXML(dataNode,"startRadius",startRadius);
      GlXMLTools::setWithXML(dataNode,"endRadius",endRadius);
      GlXMLTools::setWithXML(dataNode,"color",color);
      GlXMLTools::setWithXML(dataNode,"textureFile",textureFile);

      boundingBox.check(Coord(startPoint.getX() - startRadius, startPoint.getY() + startRadius, startPoint.getZ() + startRadius));
      boundingBox.check(Coord(endPoint.getX() + endRadius, endPoint.getY() - endRadius, endPoint.getZ() - endRadius));
      computeVectors();
    }
  }
}

