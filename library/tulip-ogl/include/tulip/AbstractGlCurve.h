#ifndef ABSTRACTGLCURVE_H
#define ABSTRACTGLCURVE_H

#include <tulip/GlSimpleEntity.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlShaderProgram.h>

namespace tlp {

class TLP_GL_SCOPE AbstractGlCurve : public GlSimpleEntity {

public :

	AbstractGlCurve(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode, const std::vector<Coord> &controlPoints,
				    const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints,
				    const bool outlined, const Color &outlineColor, const std::string &texture);

	virtual ~AbstractGlCurve();

	void draw(float lod, Camera *camera);

	void translate(const Coord &move);

	virtual void setTexture(const std::string &texture) {this->texture = texture;}

	virtual void setOutlined(const bool outlined) {this->outlined = outlined;}

	virtual void setOutlineColor(const Color &outlineColor) {this->outlineColor = outlineColor;}

	void getXML(xmlNodePtr rootNode);

	void setWithXML(xmlNodePtr rootNode);

protected:

	virtual void setCurveVertexShaderRenderingSpecificParameters() {};

	virtual void cleanupAfterCurveVertexShaderRendering() {};

	virtual void computeCurvePointsOnCPU(std::vector<Coord> &curvePoints) = 0;

	static void buildCurveVertexBuffers(const unsigned int nbCurvePoints, bool vboOk);

	static std::map<unsigned int, GLfloat *> curveVertexBuffersData;
	static std::map<unsigned int, std::vector<GLushort *> > curveVertexBuffersIndices;
	static std::map<unsigned int, GLuint* > curveVertexBuffersObject;
	static GLint MAX_SHADER_CONTROL_POINTS;	
	static std::map<std::string, GlShaderProgram *> curvesShadersMap;
	static GlShader *curveVertexShaderMain;
	static GlShader *fisheyeDistortionVertexShader;

	std::string shaderProgramName;
	GlShaderProgram *curveShaderProgram;

	std::vector<Coord> controlPoints;
	unsigned int nbControlPoints;
	float *glControlPoints;
	Color startColor;
	Color endColor;
	float startSize;
	float endSize;
	unsigned int nbCurvePoints;
	bool outlined;
	Color outlineColor;
	std::string texture;
	bool vboOk;

};

}

#endif

