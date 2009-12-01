#include <tulip/GlewManager.h>

#include <iostream>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
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
class Sphere: public Glyph, public EdgeExtremityGlyphFrom3DGlyph {
public:
	Sphere(GlyphContext *gc = NULL);
	Sphere(EdgeExtremityGlyphContext *gc = NULL);
	virtual ~Sphere();
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);

private:
	void generateBuffers(int space);
	inline void drawGlyph(const Color& glyphColor, const string& texture,
			const string& texturePath, float lod);
};

GLYPHPLUGIN(Sphere, "3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1.0", 2)
;
EEGLYPHPLUGIN(Sphere, "3D - Sphere", "Bertrand Mathieu", "09/07/2002", "Textured sphere", "1.0", 3)
;

//=========================================================================================
Sphere::Sphere(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom3DGlyph(NULL) {
}
Sphere::Sphere(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom3DGlyph(gc) {
}

Sphere::~Sphere() {
}
//=====================================================
void Sphere::getIncludeBoundingBox(BoundingBox &boundingBox) {
	boundingBox.first = Coord(0.15, 0.15, 0.15);
	boundingBox.second = Coord(0.85, 0.85, 0.85);
}

static GLuint buffers[] = { 0, 0, 0 };

static GLfloat *vertex;
static GLfloat *texturesCoord;
static GLushort *indices;
//=====================================================
void Sphere::draw(node n, float lod) {

	//	bool canUseGlew = GlewManager::getInst().canUseGlew();
	//
	//	int space = 9;
	//	int vertexCount = (90 / space) * (360 / space) * 4;
	//
	//	if (canUseGlew) {
	//		if (buffers[0] == 0) {
	//			generateBuffers(space);
	//		}
	//	} else {
	//		if (GlDisplayListManager::getInst().beginNewDisplayList("Sphere_sphere")) {
	//			GLUquadricObj *quadratic;
	//			quadratic = gluNewQuadric();
	//			gluQuadricNormals(quadratic, GLU_SMOOTH);
	//			gluQuadricTexture(quadratic, GL_TRUE);
	//			gluSphere(quadratic, 0.5f, 30, 30);
	//			GlDisplayListManager::getInst().endNewDisplayList();
	//			gluDeleteQuadric(quadratic);
	//		}
	//	}
	//
	//	tlp::setMaterial(glGraphInputData->elementColor->getNodeValue(n));
	//	string texFile = glGraphInputData->elementTexture->getNodeValue(n);
	//
	//	if (texFile.size() != 0) {
	//		string texturePath = glGraphInputData->parameters->getTexturePath();
	//		GlTextureManager::getInst().activateTexture(texturePath + texFile);
	//	}
	//
	//	if (canUseGlew) {
	//		glEnableClientState(GL_VERTEX_ARRAY);
	//		glEnableClientState(GL_NORMAL_ARRAY);
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	//		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	//		glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
	//
	//		if (texFile.size() != 0) {
	//			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	//			glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
	//		}
	//
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	//		glDrawElements(GL_TRIANGLE_STRIP, vertexCount * 2, GL_UNSIGNED_SHORT,
	//				BUFFER_OFFSET(0));
	//
	//		glDisableClientState(GL_VERTEX_ARRAY);
	//		glDisableClientState(GL_NORMAL_ARRAY);
	//		if (texFile.size() != 0)
	//			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//	} else {
	//		GlDisplayListManager::getInst().callDisplayList("Sphere_sphere");
	//	}
	//
	//	GlTextureManager::getInst().desactivateTexture();
	drawGlyph(glGraphInputData->elementColor->getNodeValue(n),
			glGraphInputData->elementTexture->getNodeValue(n),
			glGraphInputData->parameters->getTexturePath(), lod);
}

void Sphere::draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod) {
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->elementTexture->getEdgeValue(e),
			edgeExtGlGraphInputData->parameters->getTexturePath(), lod);
}

//=====================================================
void Sphere::generateBuffers(int space) {
	int vertexCount = (90 / space) * (360 / space) * 4;

	glGenBuffers(3, buffers);

	double PI = 3.1415926535897;
	vertex = new GLfloat[vertexCount * 3* 2 ];
	texturesCoord= new GLfloat[vertexCount*2*2];
	indices = new GLushort[vertexCount*2*2];

	int n;

	n = 0;

	for(float j = 0; j <= 90 - space; j+=space) {
		for(float i = 0; i <= 360 - space; i+=space) {
			indices[n]=n;
			indices[n+1]=n+1;
			indices[n+2]=n+2;
			indices[n+3]=n+3;
			indices[vertexCount*2-n]=n+vertexCount;
			indices[vertexCount*2-n-1]=n+vertexCount+1;
			indices[vertexCount*2-n-2]=n+vertexCount+2;
			indices[vertexCount*2-n-3]=n+vertexCount+3;

			vertex[n*3] = sin((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
			vertex[n*3+1] = cos((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
			vertex[n*3+2] = -cos((j) / 180 * PI) /2.;
			vertex[(vertexCount+n)*3]=vertex[n*3];
			vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
			vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
			texturesCoord[n*2] = 1-((i) / 360);
			texturesCoord[n*2+1] = (2 * j) / 360;
			texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
			texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
			n++;

			vertex[n*3] = sin((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j + space) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i) / 360;
      texturesCoord[n*2+1] = (2 * (j + space)) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;

      vertex[n*3] = sin((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i + space) / 360;
      texturesCoord[n*2+1] = (2 * j) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;

      vertex[n*3] = sin((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+1] = cos((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
      vertex[n*3+2] = -cos((j + space) / 180 * PI) /2.;
      vertex[(vertexCount+n)*3]=vertex[n*3];
      vertex[(vertexCount+n)*3+1]=vertex[n*3+1];
      vertex[(vertexCount+n)*3+2]=-vertex[n*3+2];
      texturesCoord[n*2] = 1-(i + space) / 360;
      texturesCoord[n*2+1] = (2 * (j + space)) / 360;
      texturesCoord[(vertexCount+n)*2] = texturesCoord[n*2];
      texturesCoord[(vertexCount+n)*2+1] = -texturesCoord[n*2+1];
      n++;
    }
  }


  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, vertexCount*3*2*sizeof(GLfloat),vertex, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, vertexCount*2*2*sizeof(GLfloat),texturesCoord, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount*2*sizeof(GLushort), indices, GL_STATIC_DRAW);
}
	/*@}*/
void Sphere::drawGlyph(const Color& glyphColor, const string& texture,
		const string& texturePath, float lod) {
	bool canUseGlew = GlewManager::getInst().canUseGlew();

	int space = 9;
	int vertexCount = (90 / space) * (360 / space) * 4;

	if (canUseGlew) {
		if (buffers[0] == 0) {
			generateBuffers(space);
		}
	} else {
		if (GlDisplayListManager::getInst().beginNewDisplayList("Sphere_sphere")) {
			GLUquadricObj *quadratic;
			quadratic = gluNewQuadric();
			gluQuadricNormals(quadratic, GLU_SMOOTH);
			gluQuadricTexture(quadratic, GL_TRUE);
			gluSphere(quadratic, 0.5f, 30, 30);
			GlDisplayListManager::getInst().endNewDisplayList();
			gluDeleteQuadric(quadratic);
		}
	}

	tlp::setMaterial(glyphColor);
	if (texture.size() != 0) {
		GlTextureManager::getInst().activateTexture(texturePath + texture);
	}

	if (canUseGlew) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

		if (texture.size() != 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
			glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
		glDrawElements(GL_TRIANGLE_STRIP, vertexCount * 2, GL_UNSIGNED_SHORT,
				BUFFER_OFFSET(0));

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if (texture.size() != 0)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	} else {
		GlDisplayListManager::getInst().callDisplayList("Sphere_sphere");
	}

	GlTextureManager::getInst().desactivateTexture();
}
