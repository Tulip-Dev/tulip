
#include <iostream>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/GlTools.h>


using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a textured sphere using the "viewTexture" node
 * property value. If this property has no value, the sphere
 * is then colored using the "viewColor" node property value.
 */
class Sphere : public Glyph {
public:
  Sphere(GlyphContext *gc=NULL);
  virtual ~Sphere();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
  virtual void draw(node n,float lod);

private:
};

GLYPHPLUGIN(Sphere, "3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1.0", 2);

//=========================================================================================
Sphere::Sphere(GlyphContext *gc): Glyph(gc) {
}

Sphere::~Sphere(){
}
//=====================================================
void Sphere::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox.first=Coord(0.15,0.15,0.15);
  boundingBox.second=Coord(0.85,0.85,0.85);
}
//=====================================================
void Sphere::draw(node n,float lod) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  if(GlDisplayListManager::getInst().beginNewDisplayList("Sphere_sphere")) {
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);  
    gluSphere(quadratic, 0.5f, 30, 30);
    GlDisplayListManager::getInst().endNewDisplayList();
    gluDeleteQuadric(quadratic);
  }

  tlp::setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
  
  GlDisplayListManager::getInst().callDisplayList("Sphere_sphere");

  GlTextureManager::getInst().desactivateTexture();
}
/*@}*/
