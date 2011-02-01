#include <GL/glew.h>

#include <tulip/Glyph.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlTextureManager.h>

using namespace tlp;
using namespace std;

const string roundedBoxFragmentShaderSrc =
		"uniform float boxWidth;"
		"uniform float boxHeight;"

		"uniform bool textureActivated;"
		"uniform sampler2D texture;"

		"void main() {"
		"	float radius = min(boxWidth / 4.0, boxHeight / 4.0);"
		"	float sRadius = radius / boxWidth;"
		"	float tRadius = radius / boxHeight;"
		"	if (gl_TexCoord[0].s < sRadius && gl_TexCoord[0].t < tRadius) {"
		"		float dist = distance(vec2(sRadius*boxWidth, tRadius*boxHeight), gl_TexCoord[0].st * vec2(boxWidth, boxHeight));"
		"		if (dist > radius)"
		"			discard;"
		"	}"
		"	if (gl_TexCoord[0].s > (1.0 - sRadius) && gl_TexCoord[0].t > (1.0 - tRadius)) {"
		"		float dist = distance(vec2((1.0 - sRadius)*boxWidth, (1.0 - tRadius)*boxHeight), gl_TexCoord[0].st * vec2(boxWidth, boxHeight));"
		"		if (dist > radius)"
		"			discard;"
		"	}"
		"	if (gl_TexCoord[0].s < sRadius && gl_TexCoord[0].t > (1.0 - tRadius)) {"
		"		float dist = distance(vec2(sRadius*boxWidth, (1.0 - tRadius)*boxHeight), gl_TexCoord[0].st * vec2(boxWidth, boxHeight));"
		"		if (dist > radius)"
		"			discard;"
		"	}"
		"	if (gl_TexCoord[0].s > (1.0 - sRadius) && gl_TexCoord[0].t < tRadius) {"
		"		float dist = distance(vec2((1.0 - sRadius)*boxWidth, tRadius*boxHeight), gl_TexCoord[0].st * vec2(boxWidth, boxHeight));"
		"		if (dist > radius)"
		"			discard;"
		"	} "
		"	gl_FragColor = gl_Color;"
		"	if (textureActivated)"
		"		gl_FragColor *= texture2D(texture, gl_TexCoord[0].st);"
		"}"
		;

const string roundedBoxOutlineVertexShaderSrc =
		"#version 120\n"
		"void main() {"
		"	gl_Position = gl_Vertex;"
		"	gl_FrontColor = gl_Color;"
		"}"
		;

const string roundedBoxOutlineGeometryShaderSrc =
		"#version 120\n"
		"#extension GL_EXT_geometry_shader4 : enable\n"
		"#define M_PI 3.141592653589793238462643\n"

		"uniform float boxWidth;"
		"uniform float boxHeight;"

		"const int steps = 20;"
		"const float delta = (M_PI/2.0) / float(steps);"

		"void main() {"
		"	float radius = min(boxWidth / 4.0, boxHeight / 4.0);"
		"	float radiusL = radius / boxWidth;"
		"	float radiusH = radius / boxHeight;"
		"	float wi = 1.0 - 2*radiusL;"
		"	float hi = 1.0 - 2*radiusH;"
		"	vec3 P1 = gl_PositionIn[0].xyz + vec3(radiusL, -radiusH, 0.0);"
		"	vec3 P2 = P1 + vec3(wi, 0.0, 0.0);"
		"	vec3 P3 = P2 + vec3(0.0, -hi, 0.0);"
		"	vec3 P4 = P1 + vec3(0.0, -hi, 0.0);"

		"	float w = 0.0;"
		"	float x = 0.0;"
		"	float y = 0.0;"
		"	vec3 p = vec3(0.0);"

		"	gl_FrontColor = gl_FrontColorIn[0];"
		"	for (int i = 0 ; i < steps; ++i ) {"
		"		w = delta + float(i) * delta;"
		"		x = -cos(w);"
		"		y = sin(w);"
		"		p = P1 + vec3(x, y, 0.0) * vec3(radiusL, radiusH, 0.0);"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.0);"
		"		EmitVertex();"
		"	}"

		"	gl_FrontColor = gl_FrontColorIn[1];"
		"	for (int i = 0 ; i < steps; ++i ) {"
		"		w = delta + float(steps - i - 1) * delta;"
		"		x = cos(w);"
		"		y = sin(w);"
		"		p = P2 + vec3(x, y, 0.0) * vec3(radiusL, radiusH, 0.0);"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.0);"
		"		EmitVertex();"
		"	}"

		"	gl_FrontColor = gl_FrontColorIn[2];"
		"	for (int i = 0 ; i < steps; ++i ) {"
		"		w = delta + float(i) * delta;"
		"		x = cos(w);"
		"		y = -sin(w);"
		"		p = P3 + vec3(x, y, 0.0) * vec3(radiusL, radiusH, 0.0);"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.0);"
		"		EmitVertex();"
		"	}"

		"	gl_FrontColor = gl_FrontColorIn[3];"
		"	for (int i = 0 ; i < steps; ++i ) {"
		"		float w = delta + float(steps - i - 1) * delta;"
		"		float x = -cos(w);"
		"		float y = -sin(w);"
		"		p = P4 + vec3(x, y, 0.0) * vec3(radiusL, radiusH, 0.0);"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.0);"
		"		EmitVertex();"
		"	}"

		"	w = delta;"
		"	x = -cos(w);"
		"	y = sin(w);"

		"	p = P1 + vec3(x, y, 0.0) * vec3(radiusL, radiusH, 0.0);"
		"	gl_Position = gl_ModelViewProjectionMatrix * vec4(p, 1.0);"
		"	EmitVertex();"
		"}"
		;
class RoundedBox : public Glyph {
public:
	RoundedBox(GlyphContext *gc = NULL);
	~RoundedBox() {}
	void draw(node n, float lod);
	Coord getAnchor(const Coord &vector) const;
	void getIncludeBoundingBox(BoundingBox &boundingBox, node);

private:

	void initRoundedSquare();
	GlPolygon *createRoundedRect(const Size &size);

	static GlPolygon *roundedSquare;
};

GlPolygon* RoundedBox::roundedSquare=0;

RoundedBox::RoundedBox(GlyphContext *gc) : Glyph(gc) {}

void RoundedBox::initRoundedSquare() {
	roundedSquare = createRoundedRect(Size(1,1,1));
}

void RoundedBox::getIncludeBoundingBox(BoundingBox &boundingBox,node ) {
        boundingBox[0] = Coord(-0.35, -0.35, 0);
        boundingBox[1] = Coord(0.35, 0.35, 0);
}

GlPolygon *RoundedBox::createRoundedRect(const Size &size) {
	float radius = min(size[0] / 4, size[1] / 4);
	float radiusL = radius / size[0];
	float radiusH = radius / size[1];

	float wi = 1.0 - 2*radiusL;
	float hi = 1.0 - 2*radiusH;

	Coord P1 = Coord(-0.5, 0.5) + Coord(radiusL, -radiusH);
	Coord P2 = P1 + Coord(wi, 0);
	Coord P3 = P2 + Coord(0, -hi);
	Coord P4 = P1 + Coord(0, -hi);

	int steps = 20;
	float delta = (M_PI/2) / steps;

	vector<Coord> boxPoints;
	boxPoints.resize(steps * 4);

#pragma omp parallel for
	for (int i = 0 ; i < steps; ++i ) {
		float w = delta + i * delta;
		float x = -cos(w);
		float y = sin(w);
		Coord p = P1 + Coord(x, y) * Coord(radiusL, radiusH);
		boxPoints[i] = p;

		w = delta + (steps - i - 1) * delta;
		x = cos(w);
		y = sin(w);
		p = P2 + Coord(x, y) * Coord(radiusL, radiusH);
		boxPoints[steps + i] = p;

		w = delta +  i * delta;
		x = cos(w);
		y = -sin(w);
		p = P3 + Coord(x, y) * Coord(radiusL, radiusH);
		boxPoints[2 * steps + i] = p;

		w = delta + (steps - i - 1) * delta;
		x = -cos(w);
		y = -sin(w);
		p = P4 + Coord(x, y) * Coord(radiusL, radiusH);
		boxPoints[3 * steps + i] = p;
	}

	vector<Color> fillColors;
	vector<Color> outlineColors;

	fillColors.push_back(Color(255,255,255));
	outlineColors.push_back(Color(0,0,0));

	return new GlPolygon(boxPoints, fillColors, outlineColors, true, true);
}

const float squareVerticesData[56] = {
		/* front face (vvttnnn) */
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		/* back face (vvttnnn) */
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f
};

const float outlineVeticesData[8] = {
		-0.5f, 0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
};

void RoundedBox::draw(node n, float lod) {

	static string glVendor(((const char*)glGetString(GL_VENDOR)));
	static bool glVendorOk = (glVendor.find("NVIDIA")!=string::npos) || (glVendor.find("ATI")!=string::npos);

	static GlShaderProgram *roundedBoxShader = NULL;
	static GlShaderProgram *roundedBoxOutlineShader = NULL;
	if (roundedBoxShader == NULL && glVendorOk && GlShaderProgram::shaderProgramsSupported() && GlShaderProgram::geometryShaderSupported()) {
		roundedBoxShader = new GlShaderProgram();
		roundedBoxShader->addShaderFromSourceCode(Fragment, roundedBoxFragmentShaderSrc);
		roundedBoxShader->link();
		roundedBoxShader->printInfoLog();

		roundedBoxOutlineShader = new GlShaderProgram();
		roundedBoxOutlineShader->addShaderFromSourceCode(Vertex, roundedBoxOutlineVertexShaderSrc);
		roundedBoxOutlineShader->addGeometryShaderFromSourceCode(roundedBoxOutlineGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_LINE_STRIP);
		roundedBoxOutlineShader->link();
		roundedBoxOutlineShader->printInfoLog();
	}


	const Size &size = glGraphInputData->getElementSize()->getNodeValue(n);
	float outlineWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
	const string &texture = glGraphInputData->getElementTexture()->getNodeValue(n);

	if (roundedBoxShader == NULL || !roundedBoxShader->isLinked() || !roundedBoxOutlineShader->isLinked() || (GlShaderProgram::getCurrentActiveShader() && GlShaderProgram::getCurrentActiveShader()->getName() == "fisheye")) {
		if (roundedSquare == NULL)
			initRoundedSquare();
		GlPolygon *polygon = roundedSquare;
		if (size[0] != size[1]) {
			polygon = createRoundedRect(size);
		}
		polygon->setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));
		polygon->setOutlineColor(glGraphInputData->getElementBorderColor()->getNodeValue(n));
		polygon->setOutlineSize(outlineWidth);
		polygon->setTextureName(texture);
		polygon->draw(lod,NULL);
	} else {


		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(2, GL_FLOAT, 7 * sizeof(float), &squareVerticesData[0]);
		glTexCoordPointer(2, GL_FLOAT, 7 * sizeof(float), &squareVerticesData[2]);
		glNormalPointer(GL_FLOAT, 7 * sizeof(float), &squareVerticesData[4]);

		setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
		if (texture != "") {
			GlTextureManager::getInst().activateTexture(texture);
		}
		roundedBoxShader->activate();
		roundedBoxShader->setUniformFloat("boxWidth", size[0]);
		roundedBoxShader->setUniformFloat("boxHeight", size[1]);
		roundedBoxShader->setUniformBool("textureActivated", texture != "");
		roundedBoxShader->setUniformTextureSampler("texture", 0);
		glDrawArrays(GL_QUADS, 0, 8);
		roundedBoxShader->desactivate();

		if (texture != "") {
			GlTextureManager::getInst().desactivateTexture();
		}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		if (outlineWidth == 0) {
			glLineWidth(1.0);
			setMaterial(glGraphInputData->getElementColor()->getNodeValue(n));
		} else {
			glLineWidth(outlineWidth);
			setMaterial(glGraphInputData->getElementBorderColor()->getNodeValue(n));
		}

		glVertexPointer(2, GL_FLOAT, 2 * sizeof(float), outlineVeticesData);

		OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();
		roundedBoxOutlineShader->activate();
		roundedBoxOutlineShader->setUniformFloat("boxWidth", size[0]);
		roundedBoxOutlineShader->setUniformFloat("boxHeight", size[1]);
		glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, 4);
		roundedBoxOutlineShader->desactivate();
		OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

Coord RoundedBox::getAnchor(const Coord &vector) const {
	Coord v(vector);
	float x, y, z, fmax;
	v.get(x, y, z);
	v.setZ(0.0f);
	fmax = std::max(fabsf(x), fabsf(y));
	if (fmax > 0.0f)
		return v * (0.5f / fmax);
	else
		return v;
}

GLYPHPLUGIN(RoundedBox, "2D - Rounded Box", "Antoine LAMBERT", "02/11/2010", "Rounded Box", "1.0", 18)
