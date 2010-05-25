#include "tulip/GlTextureManager.h"
#include "tulip/GlTools.h"

#include "tulip/GlPolyQuad.h"
#include "tulip/Vector.h"

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
        boundingBox.insert(startEdge);
        boundingBox.insert(endEdge);
	polyQuadEdgesColors.push_back(edgeColor);
}

void GlPolyQuad::draw(float lod, Camera *camera) {

	assert(polyQuadEdges.size() % 2 == 0 && polyQuadEdgesColors.size() == (polyQuadEdges.size() / 2));

	if (textureName != "") {
		GlTextureManager::getInst().activateTexture(textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	glDisable(GL_CULL_FACE);

	const unsigned int nbSubdivisionsPerSegment = 20;
	GLfloat *vertexArray = new GLfloat[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2 * 3];
	GLfloat *texCoordsArray = new GLfloat[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2 * 2];
	GLfloat *colorsArray = new GLfloat[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2 * 4];
	GLushort *quadIndices = new GLushort[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2];
	GLushort *outlineIndices = new GLushort[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2];

	for (unsigned int i = 0 ; i < (polyQuadEdges.size() / 2) - 1 ; ++i) {
		Vector<float, 4> startColor;
		Vector<float, 4> endColor;
		startColor[0] = polyQuadEdgesColors[i].getRGL();
		startColor[1] = polyQuadEdgesColors[i].getGGL();
		startColor[2] = polyQuadEdgesColors[i].getBGL();
		startColor[3] = polyQuadEdgesColors[i].getAGL();
		endColor[0] = polyQuadEdgesColors[i+1].getRGL();
		endColor[1] = polyQuadEdgesColors[i+1].getGGL();
		endColor[2] = polyQuadEdgesColors[i+1].getBGL();
		endColor[3] = polyQuadEdgesColors[i+1].getAGL();
		for (unsigned int j = 0 ; j < nbSubdivisionsPerSegment ; ++j) {

			unsigned int n = i * nbSubdivisionsPerSegment + j;
			Coord v1 = polyQuadEdges[2*i] + (j / static_cast<float>(nbSubdivisionsPerSegment - 1)) * (polyQuadEdges[2*(i+1)] - polyQuadEdges[2*i]);
			Coord v2 = polyQuadEdges[2*i+1] + (j / static_cast<float>(nbSubdivisionsPerSegment - 1)) * (polyQuadEdges[2*(i+1)+1] - polyQuadEdges[2*i+1]);
			Vector<float, 4> color = startColor + (j / static_cast<float>(nbSubdivisionsPerSegment - 1)) * (endColor - startColor);
			float texCoordFactor = ((polyQuadEdges[2*i].dist(polyQuadEdges[2*i+2])) / (nbSubdivisionsPerSegment - 1)) / (polyQuadEdges[2*i].dist(polyQuadEdges[2*i+1]));

			vertexArray[6*n] = v1[0];
			vertexArray[6*n+1] = v1[1];
			vertexArray[6*n+2] = v1[2];
			vertexArray[6*n+3] = v2[0];
			vertexArray[6*n+4] = v2[1];
			vertexArray[6*n+5] = v2[2];

			texCoordsArray[4*n] = static_cast<GLfloat>(j) * texCoordFactor;
			texCoordsArray[4*n+1] = 0.0f;
			texCoordsArray[4*n+2] = static_cast<GLfloat>(j) * texCoordFactor;
			texCoordsArray[4*n+3] = 1.0f;

			colorsArray[8*n] = color[0];
			colorsArray[8*n+1] = color[1];
			colorsArray[8*n+2] = color[2];
			colorsArray[8*n+3] = color[3];
			colorsArray[8*n+4] = color[0];
			colorsArray[8*n+5] = color[1];
			colorsArray[8*n+6] = color[2];
			colorsArray[8*n+7] = color[3];

			quadIndices[2*n] = 2*n;
			quadIndices[2*n+1] = 2*n+1;

			outlineIndices[n] = 2*n;
			outlineIndices[((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2 - (n+1)] = 2*n+1;
		}
	}

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vertexArray);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(float), texCoordsArray);
	glColorPointer(4, GL_FLOAT, 4 * sizeof(float), colorsArray);

	glDrawElements(GL_QUAD_STRIP, ((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2, GL_UNSIGNED_SHORT, quadIndices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if (textureName != "") {
		GlTextureManager::getInst().desactivateTexture();
	}

	if (outlined && textureName == "") {
		glLineWidth(outlineWidth);
		setMaterial(outlineColor);
		glDrawElements(GL_LINE_LOOP, ((polyQuadEdges.size() / 2) - 1) * nbSubdivisionsPerSegment * 2, GL_UNSIGNED_SHORT, outlineIndices);
		if (outlineWidth != 1) {
			glLineWidth(1);
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	delete [] vertexArray;
	delete [] texCoordsArray;
	delete [] colorsArray;
	delete [] quadIndices;
	delete [] outlineIndices;

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

}

void GlPolyQuad::translate(const Coord& move) {
        boundingBox.translate(move);

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
                boundingBox.insert(*it);
	}
}

}
