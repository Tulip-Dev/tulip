#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlPentagon.h>

#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph
/**
 * This glyph draws a textured pentagone using the "viewTexture"
 * node property value. If this property has no value, the pentagone
 * is then colored using the "viewColor" node property value.
 */
class Pentagone: public Glyph, public EdgeExtremityGlyphFrom2DGlyph {
public:
	Pentagone(GlyphContext *gc = NULL);
	Pentagone(EdgeExtremityGlyphContext *gc);
	virtual ~Pentagone();
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);
  virtual void draw(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod);

protected:

  static GlPentagon *pentagon;

};

#ifdef _WIN32
#ifdef DLL_EXPORT
GlPentagon* Pentagone::pentagon=0;
#endif
#else
GlPentagon* Pentagone::pentagon=0;
#endif

//=====================================================
GLYPHPLUGIN(Pentagone, "2D - Pentagone", "David Auber", "09/07/2002", "Textured Pentagone", "1.0", 12)
;
EEGLYPHPLUGIN(Pentagone, "2D - Pentagone", "David Auber", "09/07/2002", "Textured Pentagone", "1.0", 13)
;
//===================================================================================
Pentagone::Pentagone(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
  if(!pentagon)
    pentagon=new GlPentagon(Coord(0,-0.05,0),Size(.5,.5,0));
}
Pentagone::Pentagone(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
  if(!pentagon)
    pentagon=new GlPentagon(Coord(0,-0.05,0),Size(.5,.5,0));
}
//=====================================================
Pentagone::~Pentagone() {
}
//=====================================================
void Pentagone::getIncludeBoundingBox(BoundingBox &boundingBox) {
        boundingBox[0] = Coord(0.20, 0.15, 0);
        boundingBox[1] = Coord(0.80, 0.85, 0);
}
//=====================================================
void Pentagone::draw(node n, float lod) {
  string textureName=glGraphInputData->elementTexture->getNodeValue(n);
  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  draw(glGraphInputData->elementColor->getNodeValue(n),
       glGraphInputData->elementBorderColor->getNodeValue(n),
       glGraphInputData->elementBorderWidth->getNodeValue(n),
       textureName,
       lod);
}

void Pentagone::draw(edge e, node, const Color& glyphColor, const Color &borderColor, float lod) {
  string textureName=edgeExtGlGraphInputData->elementTexture->getEdgeValue(e);
  if(textureName!="")
    textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

  draw(glyphColor,
       borderColor,
       edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
       textureName,
       lod);
	glDisable(GL_LIGHTING);
}

//=====================================================
void Pentagone::draw(const Color &fillColor,
                     const Color &borderColor,
                     float borderWidth,
                     const std::string &textureName,
                     float lod){
  if(borderWidth<1e-6)
    borderWidth=1e-6;

  pentagon->setFillColor(fillColor);
  pentagon->setOutlineColor(borderColor);
  pentagon->setOutlineSize(borderWidth);
  pentagon->setTextureName(textureName);
  pentagon->draw(lod,NULL);
}
