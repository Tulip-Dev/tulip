/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

void drawTree() {
  if (GlDisplayListManager::getInst().beginNewDisplayList(
        "ChristmasTree_tree")) {
    GLUquadricObj *quadratic1;
    GLUquadricObj *quadratic2;
    GLUquadricObj *quadratic3;
    GLUquadricObj *quadratic4;
    setMaterial(Color(125, 80, 0, 255));
    quadratic1 = gluNewQuadric();
    gluQuadricNormals(quadratic1, GLU_SMOOTH);
    gluQuadricTexture(quadratic1, GL_TRUE);
    glTranslatef(0.0f, 0.0f, -0.5f);
    gluQuadricOrientation(quadratic1, GLU_INSIDE);
    gluDisk(quadratic1, 0.0f, 0.1f, 10, 10);
    gluQuadricOrientation(quadratic1, GLU_OUTSIDE);
    gluCylinder(quadratic1, 0.1f, 0.1f, 0.2f, 10, 10);
    glTranslatef(0.0f, 0.0f, 0.5f);

    setMaterial(Color(26, 121, 0, 255));
    quadratic2 = gluNewQuadric();
    gluQuadricNormals(quadratic2, GLU_SMOOTH);
    gluQuadricTexture(quadratic2, GL_TRUE);
    glTranslatef(0.0f, 0.0f, -0.3f);
    gluQuadricOrientation(quadratic2, GLU_OUTSIDE);
    gluCylinder(quadratic2, 0.4f, 0.0f, 0.5f, 10, 10);
    gluQuadricOrientation(quadratic2, GLU_INSIDE);
    gluDisk(quadratic2, 0.0f, 0.4f, 10, 10);
    glTranslatef(0.0f, 0.0f, 0.3f);

    quadratic3 = gluNewQuadric();
    gluQuadricNormals(quadratic3, GLU_SMOOTH);
    gluQuadricTexture(quadratic3, GL_TRUE);
    glTranslatef(0.0f, 0.0f, -0.1f);
    gluQuadricOrientation(quadratic3, GLU_OUTSIDE);
    gluCylinder(quadratic3, 0.3f, 0.0f, 0.4f, 10, 10);
    gluQuadricOrientation(quadratic3, GLU_INSIDE);
    gluDisk(quadratic3, 0.0f, 0.3f, 10, 10);
    glTranslatef(0.0f, 0.0f, 0.1f);

    quadratic4 = gluNewQuadric();
    gluQuadricNormals(quadratic4, GLU_SMOOTH);
    gluQuadricTexture(quadratic4, GL_TRUE);
    glTranslatef(0.0f, 0.0f, 0.1f);
    gluQuadricOrientation(quadratic4, GLU_OUTSIDE);
    gluCylinder(quadratic4, 0.2f, 0.0f, 0.3f, 10, 10);
    gluQuadricOrientation(quadratic4, GLU_INSIDE);
    gluDisk(quadratic4, 0.0f, 0.2f, 10, 10);
    glTranslatef(0.0f, 0.0f, -0.1f);

    GlDisplayListManager::getInst().endNewDisplayList();
    gluDeleteQuadric(quadratic1);
    gluDeleteQuadric(quadratic2);
    gluDeleteQuadric(quadratic3);
    gluDeleteQuadric(quadratic4);
  }

  if (GlDisplayListManager::getInst().beginNewDisplayList(
        "ChristmasTree_sphere")) {
    GLUquadricObj *quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);
    glTranslatef(0.0f, 0.0f, 0.45f);
    gluSphere(quadratic, 0.05f, 30, 30);
    glTranslatef(0.0f, 0.0f, -0.45f);
    GlDisplayListManager::getInst().endNewDisplayList();
    gluDeleteQuadric(quadratic);
  }

  if (GlDisplayListManager::getInst().beginNewDisplayList(
        "ChristmasTree_outsphere")) {
    GLUquadricObj *quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);
    gluSphere(quadratic, 0.5f, 30, 30);
    GlDisplayListManager::getInst().endNewDisplayList();
    gluDeleteQuadric(quadratic);
  }
}

/** \addtogroup glyph */

/// A 3D glyph.
/** This glyph draws a christmas tree special for christmas release of 2008
 */
class ChristmasTree: public Glyph {
public:
  GLYPHINFORMATION("3D - ChristmasTree", "Morgan Mathiaut", "16/12/2008", "Christmas tree", "1.0" , NodeShape::ChristmasTree)
  ChristmasTree(const tlp::PluginContext* context = NULL);
  virtual void draw(node n, float lod);
};
PLUGIN(ChristmasTree)

ChristmasTree::ChristmasTree(const tlp::PluginContext* context) :
  Glyph(context) {
}
void ChristmasTree::draw(node n, float) {
  drawTree();
  glRotatef(-90., 1., 0., 0.);
  GlDisplayListManager::getInst().callDisplayList("ChristmasTree_tree");
  setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
  GlDisplayListManager::getInst().callDisplayList("ChristmasTree_sphere");
  setMaterial(Color(255, 255, 255, 50));
  GlDisplayListManager::getInst().callDisplayList("ChristmasTree_outsphere");
}




