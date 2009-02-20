#include "tulip/GlTextureManager.h"
#include "tulip/GlTools.h"

#include "tulip/GlPolyQuad.h"

using namespace std;

namespace tlp {

GlPolyQuad::GlPolyQuad(const string &textureName, const bool outlined, const int outlineWidth, const Color &outlineColor)
	: textureName(textureName), outlined(outlined), outlineWidth(outlineWidth), outlineColor(outlineColor) {}

GlPolyQuad::GlPolyQuad(const vector<Coord> &polyQuadEdges, const vector<Color> &polyQuadEdgesColors, const string &textureName,
		               const bool outlined, const int outlineWidth, const Color &outlineColor)
	: textureName(textureName), outlined(outlined), outlineWidth(outlineWidth), outlineColor(outlineColor) {

	assert(polyQuadEdges.size() % 2 == 0 && polyQuadEdgesColors.size() == (polyQuadEdges.size() / 2));

	for (unsigned int i = 0 ; i < (polyQuadEdges.size() / 2) ; ++i) {
		addQuadEdge(polyQuadEdges[2*i], polyQuadEdges[2*i + 1], polyQuadEdgesColors[i]);
	}
}

GlPolyQuad::GlPolyQuad(const std::vector<Coord> &polyQuadEdges, const Color &polyQuadColor, const std::string &textureName,
					   const bool outlined, const int outlineWidth, const Color &outlineColor)
	: textureName(textureName), outlined(outlined), outlineWidth(outlineWidth), outlineColor(outlineColor) {

	assert(polyQuadEdges.size() % 2 == 0);

	for (unsigned int i = 0 ; i < (polyQuadEdges.size() / 2) ; ++i) {
		addQuadEdge(polyQuadEdges[2*i], polyQuadEdges[2*i + 1], polyQuadColor);
	}
}

void GlPolyQuad::addQuadEdge(const Coord &startEdge, const Coord &endEdge, const Color &edgeColor) {
	polyQuadEdges.push_back(startEdge);
	polyQuadEdges.push_back(endEdge);
	boundingBox.check(startEdge);
	boundingBox.check(endEdge);
	polyQuadEdgesColors.push_back(edgeColor);
}

void GlPolyQuad::draw(float lod, Camera *camera) {

	assert(polyQuadEdges.size() % 2 == 0 && polyQuadEdgesColors.size() == (polyQuadEdges.size() / 2));

	if (textureName != "") {
		GlTextureManager::getInst().activateTexture(textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBegin( GL_QUAD_STRIP);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < (polyQuadEdges.size() / 2) ; ++i) {
		setMaterial(polyQuadEdgesColors[i]);
		glTexCoord2f(1.0*i, 0.);
		glVertex3fv((float *) &polyQuadEdges[2*i]);
		glTexCoord2f(1.0*i, 1.);
		glVertex3fv((float *) &polyQuadEdges[2*i+1]);
	}
	glEnd();


	if (outlined) {
		glLineWidth(outlineWidth);
		glBegin(GL_LINE_LOOP);
		setMaterial(outlineColor);
		for(unsigned int i = 0 ; i < (polyQuadEdges.size() / 2) ; ++i) {
	        glVertex3fv((float *)&polyQuadEdges[2*i]);
		}
		for(unsigned int i = (polyQuadEdges.size() / 2) ; i > 0 ; --i) {
			glVertex3fv((float *)&polyQuadEdges[2*i-1]);
		}
		glEnd();
		if (outlineWidth != 1) {
			glLineWidth(1);
		}
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(textureName != "") {
		GlTextureManager::getInst().desactivateTexture();
	}
}

void GlPolyQuad::translate(const Coord& move) {
  boundingBox.first+=move;
  boundingBox.second+=move;

  for (unsigned int i = 0 ; i < polyQuadEdges.size() ; ++i) {
	  polyQuadEdges[i]+=move;
  }
}

void GlPolyQuad::setColor(const Color &color) {
	for (unsigned int i = 0 ; i < polyQuadEdgesColors.size() ; ++i) {
		polyQuadEdgesColors[i] = color;
	}
}

void GlPolyQuad::getXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;
	GlXMLTools::createProperty(rootNode, "type", "GlPolyQuad");
	GlXMLTools::getDataNode(rootNode,dataNode);
	GlXMLTools::getXML(dataNode,"polyQuadEdges", polyQuadEdges);
	GlXMLTools::getXML(dataNode,"polyQuadEdgesColors", polyQuadEdgesColors);
	GlXMLTools::getXML(dataNode,"textureName", textureName);
}

void GlPolyQuad::setWithXML(xmlNodePtr rootNode) {
	 xmlNodePtr dataNode=NULL;
	 GlXMLTools::getDataNode(rootNode,dataNode);

	 // Parse Data
	 if(dataNode) {
		 GlXMLTools::setWithXML(dataNode,"polyQuadEdges", polyQuadEdges);
	     GlXMLTools::setWithXML(dataNode,"polyQuadEdgesColors", polyQuadEdgesColors);
	     GlXMLTools::setWithXML(dataNode,"textureName", textureName);
	 }

	 vector<Coord>::iterator it;
	 for (it = polyQuadEdges.begin() ; it != polyQuadEdges.end() ; ++it) {
		boundingBox.check(*it);
	 }
}

}
