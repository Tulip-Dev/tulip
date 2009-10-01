/*
	  GlShaderManager.h

   Created on: 29 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef GLSHADERMANAGER_H_
#define GLSHADERMANAGER_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>
#include <map>

#include <tulip/tulipconf.h>
#include <tulip/Vector.h>
#include <tulip/Matrix.h>
#include <tulip/Color.h>

void printOGLError(const char *file, int line);
#define printOpenGLError() printOGLError(__FILE__, __LINE__)

namespace tlp {

class TLP_GL_SCOPE GlShaderProgram;

class TLP_GL_SCOPE GlShaderManager {

public :

	static GlShaderManager *getInstance();

	bool isShaderProgramsSupported() const {return shaderProgramsSupported;}
	bool isGeometryShaderSupported() const {return geometryShaderSupported;}

	GlShaderProgram *createVertexShaderFromFile(const std::string &name, const std::string &vertexShaderSourceFilePath);
	GlShaderProgram *createFragmentShaderFromFile(const std::string &name, const std::string &fragmentShaderSourceFilePath);
	GlShaderProgram *createVertexAndFragmentShaderFromFiles(const std::string &name, const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath);
	GlShaderProgram *createGeometryShaderFromFile(const std::string &name, const std::string &geometryShaderSourceFilePath = "", GLenum inputPrimitiveType = GL_LINES, GLenum outputPrimitiveType = GL_LINE_STRIP);

	GlShaderProgram *createVertexShaderFromString(const std::string &name, const std::string &vertexShaderSource);
	GlShaderProgram *createFragmentShaderFromString(const std::string &name, const std::string &fragmentShaderSource);
	GlShaderProgram *createVertexAndFragmentShaderFromStrings(const std::string &name, const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
	GlShaderProgram *createGeometryShaderFromString(const std::string &name, const std::string &geometryShaderSource = "", GLenum inputPrimitiveType = GL_LINES, GLenum outputPrimitiveType = GL_LINE_STRIP);

	void activateShaderProgram(GlShaderProgram *shaderProgram);
	void desactivateShaderProgram();

	void deleteShaderProgram(GlShaderProgram *shaderProgram);

	std::string getCurrentShaderProgramName() const;
	GlShaderProgram *getCurrentShaderProgram() const;

	bool shaderProgramAlreadyCompiled(const std::string &programName);
	GlShaderProgram *getShaderProgram(const std::string &programName);

private :

	static GlShaderManager *instance;

	GlShaderManager();
	bool checkShaderProgramsSupport();
	bool checkGeometryShaderSupport();
	bool shadersSupported(bool checkGeometry=false);
	GlShaderProgram *registerShaderProgram(const std::string &name, const GLuint vertexShaderObject, const GLuint fragmentShaderObject,
										   const GLuint geometryShaderObject, const GLenum inputPrimitiveType = GL_LINES, const GLenum outputPrimitiveType = GL_LINE_STRIP);

	bool shaderProgramsSupported;
	bool geometryShaderSupported;
	std::map<std::string, GlShaderProgram *> shaderProgramsMap;
	GlShaderProgram *currentShaderProgram;

};

class TLP_GL_SCOPE GlShaderProgram {

public :

	GlShaderProgram(const std::string &name, const GLuint programObjectId);

	std::string getName() const {return name;}
	GLuint getShaderProgramId() const {return programObjectId;}

	void setUniformFloat(const std::string &variateName, const float f);
	void setUniformVec2Float(const std::string &variableName, const Vector<float, 2> &vec2f);
	void setUniformVec2Float(const std::string &variableName, const float f1, const float f2);
	void setUniformVec3Float(const std::string &variableName, const Vector<float, 3> &vec3f);
	void setUniformVec3Float(const std::string &variableName, const float f1, const float f2, const float f3);
	void setUniformVec4Float(const std::string &variableName, const Vector<float, 4> &vec4f);
	void setUniformVec4Float(const std::string &variableName, const float f1, const float f2, const float f3, const float f4);
	void setUniformMat2Float(const std::string &variableName, const Matrix<float, 2> &mat2f, const bool transpose = false);
	void setUniformMat2Float(const std::string &variableName, const float *f, const bool transpose = false);
	void setUniformMat3Float(const std::string &variableName, const Matrix<float, 3> &mat3f, const bool transpose = false);
	void setUniformMat3Float(const std::string &variableName, const float *f, const bool transpose = false);
	void setUniformMat4Float(const std::string &variableName, const Matrix<float, 4> &mat4f, const bool transpose = false);
	void setUniformMat4Float(const std::string &variableName, const float *f, const bool transpose = false);

	void setUniformInt(const std::string &variableName, const int f);
	void setUniformVec2Int(const std::string &variableName, const Vector<int, 2> &vec2i);
	void setUniformVec2Int(const std::string &variableName, const float i1, const float i2);
	void setUniformVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i);
	void setUniformVec3Int(const std::string &variableName, const int i1, const int i2, const int i3);
	void setUniformVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i);
	void setUniformVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4);

	void setUniformBool(const std::string &variableName, const bool b);
	void setUniformVec2Bool(const std::string &variableName, const Vector<bool, 2> &vec2b);
	void setUniformVec2Bool(const std::string &variableName, const bool b1, const bool b2);
	void setUniformVec3Bool(const std::string &variableName, const Vector<bool, 3> &vec3b);
	void setUniformVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3);
	void setUniformVec4Bool(const std::string &variableName, const Vector<bool, 4> &vec4b);
	void setUniformVec4Bool(const std::string &variableName, const bool i1, const bool i2, const bool i3, const bool i4);

	void setAttributeFloat(const std::string &variableName, const float f);
	void setAttributeVec2Float(const std::string &variableName, const Vector<float, 2> &vec2f);
	void setAttributeVec2Float(const std::string &variableName, const float f1, const float f2);
	void setAttributeVec3Float(const std::string &variableName, const Vector<float, 3> &vec3f);
	void setAttributeVec3Float(const std::string &variableName, const float f1, const float f2, const float f3);
	void setAttributeVec4Float(const std::string &variableName, const Vector<float, 4> &vec4f);
	void setAttributeVec4Float(const std::string &variableName, const float f1, const float f2, const float f3, const float f4);

	void setAttributeInt(const std::string &variableName, const int f);
	void setAttributeVec2Int(const std::string &variableName, const Vector<int, 2> &vec2i);
	void setAttributeVec2Int(const std::string &variableName, const float i1, const float i2);
	void setAttributeVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i);
	void setAttributeVec3Int(const std::string &variableName, const int i1, const int i2, const int i3);
	void setAttributeVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i);
	void setAttributeVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4);

	void setAttributeBool(const std::string &variableName, const bool b);
	void setAttributeVec2Bool(const std::string &variableName, const Vector<bool, 2> &vec2b);
	void setAttributeVec2Bool(const std::string &variableName, const bool b1, const bool b2);
	void setAttributeVec3Bool(const std::string &variableName, const Vector<bool, 3> &vec3b);
	void setAttributeVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3);
	void setAttributeVec4Bool(const std::string &variableName, const Vector<bool, 4> &vec4b);
	void setAttributeVec4Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3, const bool b4);

	void setUniformTextureSampler(const std::string &samplerVariateName, const int samplerId);
	void setUniformColor(const std::string &variableName, const Color &color);
	void setAttributeColor(const std::string &variableName, const Color &color);

	template <unsigned int SIZE>
	void setUniformFloatArray(const std::string &variableName, const Vector<float, SIZE> &vecf);
	void setUniformFloatArray(const std::string &variableName, const unsigned int fCount, const float *f);

	template <unsigned int SIZE>
	void setUniformVec2FloatArray(const std::string &variableName, const Vector<Vector<float, 2>, SIZE> &vecvec2f);
	void setUniformVec2FloatArray(const std::string &variableName, const unsigned int vec2fCount, const float *f);

	template <unsigned int SIZE>
	void setUniformVec3FloatArray(const std::string &variableName, const Vector<Vector<float, 3>, SIZE> &vecvec3f);
	void setUniformVec3FloatArray(const std::string &variableName, const unsigned int vec3fCount, const float *f);

	template <unsigned int SIZE>
	void setUniformVec4FloatArray(const std::string &variableName, const Vector<Vector<float, 4>, SIZE> &vecvec4f);
	void setUniformVec4FloatArray(const std::string &variableName, const unsigned int vec4fCount, const float *f);

	template <unsigned int SIZE>
	void setUniformMat2FloatArray(const std::string &variableName, const Vector<Matrix<float, 2>, SIZE> &vecmat2f, const bool transpose = false);
	void setUniformMat2FloatArray(const std::string &variableName, const unsigned int mat2fCount, const float *f, const bool transpose = false);

	template <unsigned int SIZE>
	void setUniformMat3FloatArray(const std::string &variableName, const Vector<Matrix<float, 3>, SIZE> &vecmat3f, const bool transpose = false);
	void setUniformMat3FloatArray(const std::string &variableName, const unsigned int mat3fCount, const float *f, const bool transpose = false);

	template <unsigned int SIZE>
	void setUniformMat4FloatArray(const std::string &variableName, const Vector<Matrix<float, 4>, SIZE> &vecmat4f, const bool transpose = false);
	void setUniformMat4FloatArray(const std::string &variableName, const unsigned int mat4fCount, const float *f, const bool transpose = false);



	template <unsigned int SIZE>
	void setUniformIntArray(const std::string &variableName, const Vector<int, SIZE> &veci);
	void setUniformIntArray(const std::string &variableName, const unsigned int iCount, const int *i);

	template <unsigned int SIZE>
	void setUniformVec2IntArray(const std::string &variableName, const Vector<Vector<int, 2>, SIZE> &vecvec2i);
	void setUniformVec2IntArray(const std::string &variableName, const unsigned int vec2iCount, const int *i);

	template <unsigned int SIZE>
	void setUniformVec3IntArray(const std::string &variableName, const Vector<Vector<int, 3>, SIZE> &vecvec3i);
	void setUniformVec3IntArray(const std::string &variableName, const unsigned int vec3iCount, const int *i);

	template <unsigned int SIZE>
	void setUniformVec4IntArray(const std::string &variableName, const Vector<Vector<int, 4>, SIZE> &vecvec4i);
	void setUniformVec4IntArray(const std::string &variableName, const unsigned int vec4iCount, const int *i);


	template <unsigned int SIZE>
	void setUniformBoolArray(const std::string &variableName, const Vector<bool, SIZE> &vecb);
	void setUniformBoolArray(const std::string &variableName, const unsigned int bCount, const bool *b);

	template <unsigned int SIZE>
	void setUniformVec2BoolArray(const std::string &variableName, const Vector<Vector<bool, 2>, SIZE> &vecvec2b);
	void setUniformVec2BoolArray(const std::string &variableName, const unsigned int vec2bCount, const bool *b);

	template <unsigned int SIZE>
	void setUniformVec3BoolArray(const std::string &variableName, const Vector<Vector<bool, 3>, SIZE> &vecvec3b);
	void setUniformVec3BoolArray(const std::string &variableName, const unsigned int vec3bCount, const bool *b);

	template <unsigned int SIZE>
	void setUniformVec4BoolArray(const std::string &variableName, const Vector<Vector<bool, 4>, SIZE> &vecvec4b);
	void setUniformVec4BoolArray(const std::string &variableName, const unsigned int vec4bCount, const bool *b);

	void getUniformFloatVariableValue(const std::string &variableName, float *value);
	void getUniformIntVariableValue(const std::string &variableName, int *value);
	void getUniformBoolVariableValue(const std::string &variableName, bool *value);
	void getUniformVec2BoolVariableValue(const std::string &variableName, bool *value);
	void getUniformVec3BoolVariableValue(const std::string &variableName, bool *value);
	void getUniformVec4BoolVariableValue(const std::string &variableName, bool *value);


private :

	GLint getUniformVariableLocation(const std::string &variableName);
	GLint getAttributeVariableLocation(const std::string &variableName);

	std::string name;
	GLuint programObjectId;

};




}

#endif /* GLSHADERMANAGER_H_ */
