#include "tulip/GlLabel.h"

#include <GL/gl.h>

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GraphProperty.h>

#include "tulip/GlTools.h"
#include "tulip/GlyphManager.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/OcclusionTest.h"
#include "tulip/TextRenderer.h"
#include "tulip/GlTLPFeedBackBuilder.h"

#include <iostream>

using namespace std;

namespace tlp {

  GlLabel::GlLabel(const string& fontPath,Coord centerPosition,Coord size,Color fontColor):centerPosition(centerPosition),size(size) {
    renderer.setContext(fontPath + "font.ttf", 20, 0, 0, 255);
    renderer.setMode(TLP_POLYGON);
    renderer.setColor(fontColor[0], fontColor[1], fontColor[2]);
  }

  void GlLabel::setText(const string& text) {
    renderer.setString(text, VERBATIM);
  }

  BoundingBox GlLabel::getBoundingBox() {
    return BoundingBox(centerPosition-size/2,centerPosition+size/2);
  }

  void GlLabel::draw(float lod) {
    glPolygonMode(GL_FRONT, GL_FILL);
    bool lightingOn=glIsEnabled(GL_LIGHTING);
    bool blendOn=glIsEnabled(GL_BLEND);
    bool stencilTestOn=glIsEnabled(GL_STENCIL_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);

    float w_max = 300;
    float w,h;
    float div_w, div_h;

    renderer.getBoundingBox(w_max, h, w);

    glPushMatrix();
    glTranslatef(centerPosition[0],centerPosition[1], centerPosition[2]);
    div_w = size[0]/w;
    div_h = size[1]/h;
    if(div_h > div_w) 
      glScalef(div_w, div_w, 1);
    else
      glScalef(div_h, div_h, 1);  
    renderer.draw(w,w, 0);
    glPopMatrix();

    if(lightingOn)
      glEnable(GL_LIGHTING);
    if(blendOn)
      glEnable(GL_BLEND);
    if(stencilTestOn)
      glEnable(GL_STENCIL_TEST);
  }
}
