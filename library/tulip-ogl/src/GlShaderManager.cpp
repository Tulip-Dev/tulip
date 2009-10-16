/*
	  GlShaderManager.cpp

   Created on: 29 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <GL/glew.h>

#include <iostream>
#include <fstream>

#include "tulip/GlShaderManager.h"

using namespace std;

// Returns 1 if an OpenGL error occurred, 0 otherwise.
void printOGLError(const char *file, int line) {
	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		cerr << "OpenGL error in file " << file << " at line " << line << " : " << gluErrorString(err) << endl;
		err = glGetError();
	}
}

enum ObjectType {SHADER, PROGRAM};

// Print out the information log for a shader object or a program object
static void printInfoLog(GLuint obj, ObjectType objectType)
{
	GLint infoLogLength = 0;
	GLint charsWritten  = 0;
	GLchar *infoLog;

	if (objectType == SHADER) {
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
	} else {
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	if (infoLogLength > 0) {
		infoLog = new GLchar[infoLogLength+1];
		if (objectType == SHADER) {
			glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
		} else {
			glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
		}
		infoLog[infoLogLength] = '\0';
		cout << infoLog << endl;
		delete [] infoLog;
	}
}

static void readShaderSourceFile(const string &vertexShaderSourceFilePath, GLchar **shader) {
	ifstream ifs;
	ifs.open(vertexShaderSourceFilePath.c_str());
	if (!ifs.is_open()) {
		cerr << "Error opening file : " << vertexShaderSourceFilePath << endl;
		return;
	}
	// get length of file:
	ifs.seekg (0, ios::end);
	unsigned int length = ifs.tellg();
	ifs.seekg (0, ios::beg);

	// allocate memory:
	*shader = new GLchar[length+1];

	// read data as a block:
	ifs.read ((char *)*shader,length);
	(*shader)[length] = '\0';
	ifs.close();
}

enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};

static GLuint compileShaderObject(GLchar *shaderSrc, ShaderType shaderType) {
	GLuint shaderObject;
	GLint shaderCompiled;

	// Create a shader object
	if (shaderType == VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	} else if (shaderType == FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	} else {
		shaderObject = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	}

	// Load source code strings into shaders
	glShaderSource(shaderObject, 1,(const GLchar **) &shaderSrc, NULL);

	// Compile the shader, and print out the compiler log file.
	glCompileShader(shaderObject);


	printOpenGLError();


	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &shaderCompiled);

#ifndef NDEBUG
	printInfoLog(shaderObject, SHADER);
#endif
	if (!shaderCompiled) {
		glDeleteShader(shaderObject);
		shaderObject = 0;
	}

	return shaderObject;
}

static GLuint getShaderObjectFromFile(const std::string &shaderSourceFilePath, ShaderType shaderType) {

	if (shaderSourceFilePath == "") {
		return 0;
	}

	GLchar  *shaderSrc = NULL;

	// Read the shader source file
	readShaderSourceFile(shaderSourceFilePath, &shaderSrc);

	// File can't be read
	if(!shaderSrc) {
		return 0;
	}

	GLuint shaderObject = compileShaderObject(shaderSrc, shaderType);

	delete [] shaderSrc;

	return shaderObject;
}

static GLuint getShaderObjectFromString(const std::string &shaderSource, ShaderType shaderType) {

	if (shaderSource == "") {
		return 0;
	}

	GLuint shaderObject = compileShaderObject((GLchar *) shaderSource.c_str(), shaderType);

	return shaderObject;
}

namespace tlp {

#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::GlShaderManager* tlp::GlShaderManager::instance=0;
#endif
#else
tlp::GlShaderManager* tlp::GlShaderManager::instance=0;
#endif

GlShaderManager::GlShaderManager() : currentShaderProgram(NULL) {
	shaderProgramsSupported = checkShaderProgramsSupport();
	geometryShaderSupported = checkGeometryShaderSupport();
}

GlShaderManager* GlShaderManager::getInstance() {
	if (instance == NULL) {
		instance = new GlShaderManager();
	}
	return instance;
}

bool GlShaderManager::checkShaderProgramsSupport() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return false;
	}
	return (GL_ARB_vertex_shader && GL_ARB_fragment_shader);
}

bool GlShaderManager::checkGeometryShaderSupport() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return false;
	}
	return GL_EXT_geometry_shader4;
}

bool GlShaderManager::shadersSupported(bool checkGeometry) {
	if (!shaderProgramsSupported) {
		cerr << "Shader programs are not supported by the graphic card" << endl;
		return false;
	}

	if (checkGeometry) {
		if (!geometryShaderSupported) {
			cerr << "Geometry shaders are not supported by the graphic card" << endl;
			return false;
		}
	}

	return true;
}

GlShaderProgram *GlShaderManager::registerShaderProgram(const std::string &programName, const GLuint vertexShaderObject, const GLuint fragmentShaderObject,
		const GLuint geometryShaderObject, const GLenum inputPrimitiveType, const GLenum outputPrimitiveType) {

	// Create a program object and attach the two compiled shaders
	GLuint programObject = glCreateProgram();

	if (vertexShaderObject != 0) {
		glAttachShader(programObject, vertexShaderObject);
		glDeleteShader(vertexShaderObject);
	}

	if (fragmentShaderObject != 0) {
		glAttachShader(programObject, fragmentShaderObject);
		glDeleteShader(fragmentShaderObject);
	}

	if (geometryShaderObject != 0) {
		glAttachShader(programObject, geometryShaderObject);
		glProgramParameteriEXT(programObject, GL_GEOMETRY_INPUT_TYPE_EXT, inputPrimitiveType);
		glProgramParameteriEXT(programObject, GL_GEOMETRY_OUTPUT_TYPE_EXT, outputPrimitiveType);

		GLint temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
		glProgramParameteriEXT(programObject, GL_GEOMETRY_VERTICES_OUT_EXT, temp);

		glDeleteShader(geometryShaderObject);
	}

	//  Link the program object and print out the info log
	glLinkProgram(programObject);

#ifndef NDEBUG
	printInfoLog(programObject, PROGRAM);
#endif
	GLint linked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (linked) {
		GlShaderProgram *shaderProgram = new GlShaderProgram(programName, programObject);
		shaderProgramsMap[programName] = shaderProgram;
		return shaderProgram;
	} else {
		shaderProgramsMap[programName] = NULL;
		return NULL;
	}
}

GlShaderProgram *GlShaderManager::createVertexShaderFromFile(const string &name, const std::string &vertexShaderSourceFilePath) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}
	GLuint vertexShaderObject = getShaderObjectFromFile(vertexShaderSourceFilePath, VERTEX_SHADER);

	return registerShaderProgram(name, vertexShaderObject, 0, 0);
}

GlShaderProgram *GlShaderManager::createFragmentShaderFromFile(const string &name, const std::string &fragmentShaderSourceFilePath) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}

	GLuint fragmentShaderObject = getShaderObjectFromFile(fragmentShaderSourceFilePath, FRAGMENT_SHADER);

	return registerShaderProgram(name, 0, fragmentShaderObject, 0);
}

GlShaderProgram *GlShaderManager::createVertexAndFragmentShaderFromFiles(const string &name, const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}
	GLuint vertexShaderObject = getShaderObjectFromFile(vertexShaderSourceFilePath, VERTEX_SHADER);
	GLuint fragmentShaderObject = getShaderObjectFromFile(fragmentShaderSourceFilePath, FRAGMENT_SHADER);

	return registerShaderProgram(name, vertexShaderObject, fragmentShaderObject, 0);
}

GlShaderProgram *GlShaderManager::createGeometryShaderFromFile(const string &name, const std::string &geometryShaderSourceFilePath, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {
	if (!shadersSupported(true)) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}
	GLuint geometryShaderObject = getShaderObjectFromFile(geometryShaderSourceFilePath, GEOMETRY_SHADER);

	return registerShaderProgram(name, 0, 0, geometryShaderObject, inputPrimitiveType, outputPrimitiveType);
}

GlShaderProgram *GlShaderManager::createVertexShaderFromString(const string &name, const std::string &vertexShaderSource) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}
	GLuint vertexShaderObject = getShaderObjectFromString(vertexShaderSource, VERTEX_SHADER);

	return registerShaderProgram(name, vertexShaderObject, 0, 0);
}

GlShaderProgram *GlShaderManager::createFragmentShaderFromString(const string &name, const std::string &fragmentShaderSource) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}

	GLuint fragmentShaderObject = getShaderObjectFromString(fragmentShaderSource, FRAGMENT_SHADER);

	return registerShaderProgram(name, 0, fragmentShaderObject, 0);
}

GlShaderProgram *GlShaderManager::createVertexAndFragmentShaderFromStrings(const string &name, const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
	if (!shadersSupported()) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}
	GLuint vertexShaderObject = getShaderObjectFromString(vertexShaderSource, VERTEX_SHADER);
	GLuint fragmentShaderObject = getShaderObjectFromString(fragmentShaderSource, FRAGMENT_SHADER);

	return registerShaderProgram(name, vertexShaderObject, fragmentShaderObject, 0);
}

static const string dummyVertexShader =
"void main()"
"{"
"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"	gl_FrontColor =  gl_Color;"
"	gl_TexCoord[0] = gl_MultiTexCoord0;"
"}";

GlShaderProgram *GlShaderManager::createGeometryShaderFromString(const string &name, const std::string &geometryShaderSource, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {
	if (!shadersSupported(true)) {
		return NULL;
	}
	if (shaderProgramsMap.find(name) != shaderProgramsMap.end()) {
		return shaderProgramsMap[name];
	}

	GLuint vertexShaderObject = getShaderObjectFromString(dummyVertexShader, VERTEX_SHADER);
	GLuint geometryShaderObject = getShaderObjectFromString(geometryShaderSource, GEOMETRY_SHADER);

	return registerShaderProgram(name, vertexShaderObject, 0, geometryShaderObject, inputPrimitiveType, outputPrimitiveType);
}

void GlShaderManager::deleteShaderProgram(GlShaderProgram *shaderProgram) {
	if (shaderProgramsMap.find(shaderProgram->getName()) != shaderProgramsMap.end()) {
		glDeleteProgram(shaderProgram->getShaderProgramId());
		shaderProgramsMap.erase(shaderProgram->getName());
	}
}

void GlShaderManager::activateShaderProgram(GlShaderProgram *shaderProgram) {
	if (shaderProgramsMap.find(shaderProgram->getName()) != shaderProgramsMap.end()) {
		glUseProgram(shaderProgram->getShaderProgramId());
		currentShaderProgram = shaderProgram;
	}
}

void GlShaderManager::desactivateShaderProgram() {
	glUseProgram(0);
	currentShaderProgram = NULL;
}

string GlShaderManager::getCurrentShaderProgramName() const {
	if (currentShaderProgram != NULL) {
		return currentShaderProgram->getName();
	} else {
		return "";
	}
}

GlShaderProgram *GlShaderManager::getCurrentShaderProgram() const {
	return currentShaderProgram;
}

bool GlShaderManager::shaderProgramAlreadyCompiled(const std::string &programName) {
	return shaderProgramsMap.find(programName) != shaderProgramsMap.end();
}

GlShaderProgram *GlShaderManager::getShaderProgram(const std::string &programName) {
	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		return shaderProgramsMap[programName];
	} else {
		return NULL;
	}
}

GlShaderProgram::GlShaderProgram(const std::string &name, const GLuint programObjectId) : name(name), programObjectId(programObjectId) {}

GLint GlShaderProgram::getUniformVariableLocation(const std::string &variableName) {
	return glGetUniformLocation(programObjectId, variableName.c_str());
}

void GlShaderProgram::setUniformFloat(const std::string &variableName, const float f) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform1f(loc, f);
}

void GlShaderProgram::setUniformVec2Float(const std::string &variableName, const Vector<float, 2> &vec2f) {
	setUniformVec2FloatArray(variableName, 1, (float *) &vec2f);
}

void GlShaderProgram::setUniformVec2Float(const std::string &variableName, const float f1, const float f2) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform2f(loc, f1, f2);
}

void GlShaderProgram::setUniformVec3Float(const std::string &variableName, const Vector<float, 3> &vec3f) {
	setUniformVec3FloatArray(variableName, 1, (float *) &vec3f);
}

void GlShaderProgram::setUniformVec3Float(const std::string &variableName, const float f1, const float f2, const float f3) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform3f(loc, f1, f2, f3);
}

void GlShaderProgram::setUniformVec4Float(const std::string &variableName, const Vector<float, 4> &vec4f) {
	setUniformVec4FloatArray(variableName, 1, (float *) &vec4f);
}

void GlShaderProgram::setUniformVec4Float(const std::string &variableName, const float f1, const float f2, const float f3, const float f4) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform4f(loc, f1, f2, f3, f4);
}

template <unsigned int SIZE>
float *getMatrixData(const Matrix<float, SIZE> &matrix) {
	float *matrixData = new float[SIZE*SIZE];
	for (unsigned int i = 0 ; i < SIZE ; ++i) {
		for (unsigned int j = 0 ; j < SIZE ; ++j) {
			matrixData[i * SIZE + j] = matrix[i][j];
		}
	}
	return matrixData;
}

void GlShaderProgram::setUniformMat2Float(const std::string &variableName, const Matrix<float, 2> &mat2f, const bool transpose) {
	float *matrixData = getMatrixData(mat2f);
	setUniformMat2Float(variableName, matrixData, transpose);
	delete [] matrixData;
}

void GlShaderProgram::setUniformMat2Float(const std::string &variableName, const float *f, const bool transpose) {
	setUniformMat2FloatArray(variableName, 1, f, transpose);
}

void GlShaderProgram::setUniformMat3Float(const std::string &variableName, const Matrix<float, 3> &mat3f, const bool transpose) {
	float *matrixData = getMatrixData(mat3f);
	setUniformMat3Float(variableName, matrixData, transpose);
	delete [] matrixData;
}

void GlShaderProgram::setUniformMat3Float(const std::string &variableName, const float *f, const bool transpose) {
	setUniformMat3FloatArray(variableName, 1, f, transpose);
}

void GlShaderProgram::setUniformMat4Float(const std::string &variableName, const Matrix<float, 4> &mat4f, const bool transpose) {
	float *matrixData = getMatrixData(mat4f);
	setUniformMat4Float(variableName, matrixData, transpose);
	delete [] matrixData;
}

void GlShaderProgram::setUniformMat4Float(const std::string &variableName, const float *f, const bool transpose) {
	setUniformMat4FloatArray(variableName, 1, f, transpose);
}

void GlShaderProgram::setUniformInt(const std::string &variableName, const int i) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform1i(loc, i);
}

void GlShaderProgram::setUniformVec2Int(const std::string &variableName, const Vector<int, 2> &vec2i) {
	setUniformVec2IntArray(variableName, 1, (int *) &vec2i);
}

void GlShaderProgram::setUniformVec2Int(const std::string &variableName, const float i1, const float i2) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform2i(loc, i1, i2);
}

void GlShaderProgram::setUniformVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i) {
	setUniformVec3IntArray(variableName, 1, (int *) &vec3i);
}

void GlShaderProgram::setUniformVec3Int(const std::string &variableName, const int i1, const int i2, const int i3) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform3i(loc, i1, i2, i3);
}

void GlShaderProgram::setUniformVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i) {
	setUniformVec4IntArray(variableName, 1, (int *) &vec4i);
}

void GlShaderProgram::setUniformVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform4i(loc, i1, i2, i3, i4);
}

void GlShaderProgram::setUniformBool(const std::string &variableName, const bool b) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform1i(loc, b);
}

void GlShaderProgram::setUniformVec2Bool(const std::string &variableName, const Vector<bool, 2> &vec2b) {
	setUniformVec2IntArray(variableName, 1, (int *) &vec2b);
}

void GlShaderProgram::setUniformVec2Bool(const std::string &variableName, const bool b1, const bool b2) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform2i(loc, b1, b2);
}

void GlShaderProgram::setUniformVec3Bool(const std::string &variableName, const Vector<bool, 3> &vec3b) {
	setUniformVec3IntArray(variableName, 1, (int *) &vec3b);
}

void GlShaderProgram::setUniformVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform3i(loc, b1, b2, b3);
}

void GlShaderProgram::setUniformVec4Bool(const std::string &variableName, const Vector<bool, 4> &vec4b) {
	setUniformVec4IntArray(variableName, 1, (int *) &vec4b);
}

void GlShaderProgram::setUniformVec4Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3, const bool b4) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform4i(loc, b1, b2, b3, b4);
}

GLint GlShaderProgram::getAttributeVariableLocation(const std::string &variableName) {
	return glGetAttribLocation(programObjectId, variableName.c_str());
}

void GlShaderProgram::setAttributeFloat(const std::string &variableName, const float f) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib1f(loc, f);
}

void GlShaderProgram::setAttributeVec2Float(const std::string &variableName, const Vector<float, 2> &vec2f) {
	setAttributeVec2Float(variableName, vec2f[0], vec2f[1]);
}

void GlShaderProgram::setAttributeVec2Float(const std::string &variableName, const float f1, const float f2) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib2f(loc, f1, f2);
}

void GlShaderProgram::setAttributeVec3Float(const std::string &variableName, const Vector<float, 3> &vec3f) {
	setAttributeVec3Float(variableName, vec3f[0], vec3f[1], vec3f[2]);
}

void GlShaderProgram::setAttributeVec3Float(const std::string &variableName, const float f1, const float f2, const float f3) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib3f(loc, f1, f2, f3);
}

void GlShaderProgram::setAttributeVec4Float(const std::string &variableName, const Vector<float, 4> &vec4f) {
	setAttributeVec4Float(variableName, vec4f[0], vec4f[1], vec4f[2], vec4f[3]);
}

void GlShaderProgram::setAttributeVec4Float(const std::string &variableName, const float f1, const float f2, const float f3, const float f4) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib4f(loc, f1, f2, f3, f4);
}

void GlShaderProgram::setAttributeInt(const std::string &variableName, const int i) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib1s(loc, i);
}

void GlShaderProgram::setAttributeVec2Int(const std::string &variableName, const Vector<int, 2> &vec2i) {
	setAttributeVec2Int(variableName, vec2i[0], vec2i[1]);
}

void GlShaderProgram::setAttributeVec2Int(const std::string &variableName, const float i1, const float i2) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib2s(loc, i1, i2);
}

void GlShaderProgram::setAttributeVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i) {
	setAttributeVec3Int(variableName, vec3i[0], vec3i[1], vec3i[2]);
}

void GlShaderProgram::setAttributeVec3Int(const std::string &variableName, const int i1, const int i2, const int i3) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib3s(loc, i1, i2, i3);
}

void GlShaderProgram::setAttributeVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i) {
	setAttributeVec4Int(variableName, vec4i[0], vec4i[1], vec4i[2], vec4i[3]);
}

void GlShaderProgram::setAttributeVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4) {
	GLint loc = getAttributeVariableLocation(variableName);
	glVertexAttrib4s(loc, i1, i2, i3, i4);
}

void GlShaderProgram::setAttributeBool(const std::string &variableName, const bool b) {
	setAttributeInt(variableName, b);
}

void GlShaderProgram::setAttributeVec2Bool(const std::string &variableName, const Vector<bool, 2> &vec2b) {
	setAttributeVec2Bool(variableName, vec2b[0], vec2b[1]);
}

void GlShaderProgram::setAttributeVec2Bool(const std::string &variableName, const bool b1, const bool b2) {
	setAttributeVec2Int(variableName, b1, b2);
}

void GlShaderProgram::setAttributeVec3Bool(const std::string &variableName, const Vector<bool, 3> &vec3b) {
	setAttributeVec3Bool(variableName, vec3b[0], vec3b[1], vec3b[2]);
}

void GlShaderProgram::setAttributeVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3) {
	setAttributeVec3Int(variableName, b1, b2, b3);
}

void GlShaderProgram::setAttributeVec4Bool(const std::string &variableName, const Vector<bool, 4> &vec4b) {
	setAttributeVec4Bool(variableName, vec4b[0], vec4b[1], vec4b[2], vec4b[3]);
}

void GlShaderProgram::setAttributeVec4Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3, const bool b4) {
	setAttributeVec4Int(variableName, b1, b2, b3, b4);
}


void GlShaderProgram::setUniformTextureSampler(const std::string &samplerVariableName, const int samplerId) {
	setUniformInt(samplerVariableName, samplerId);
}

void GlShaderProgram::setUniformColor(const std::string &variableName, const Color &color) {
	float *glColor = color.getGL();
	setUniformVec4Float(variableName, glColor[0], glColor[1], glColor[2], glColor[3]);
	delete [] glColor;
}

void GlShaderProgram::setAttributeColor(const std::string &variableName, const Color &color) {
	float *glColor = color.getGL();
	setAttributeVec4Float(variableName, glColor[0], glColor[1], glColor[2], glColor[3]);
	delete [] glColor;
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformFloatArray(const std::string &variableName, const Vector<float, SIZE> &vecf) {
	setUniformFloatArray(variableName, SIZE, (float *)vecf);
}

void GlShaderProgram::setUniformFloatArray(const std::string &variableName, const unsigned int fCount, const float *f) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform1fv(loc, fCount, f);
}

template <typename T, unsigned int SIZE, unsigned int SIZE2>
T *getVectorOfVectorData(const Vector<Vector<T, SIZE>, SIZE2> &vv) {
	T *vvData = new T[SIZE*SIZE2];
	for (unsigned int i = 0 ; i < SIZE2 ; ++i) {
		for (unsigned int j = 0 ; j < SIZE ; ++j) {
			vvData[i * SIZE + j] = vv[i][j];
		}
	}
	return vvData;
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec2FloatArray(const std::string &variableName, const Vector<Vector<float, 2>, SIZE> &vecvec2f) {
	float *vvData = getVectorOfVectorData(vecvec2f);
	setUniformVec2FloatArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec2FloatArray(const std::string &variableName, const unsigned int vec2fCount, const float *f) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform2fv(loc, vec2fCount, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3FloatArray(const std::string &variableName, const Vector<Vector<float, 3>, SIZE> &vecvec3f) {
	float *vvData = getVectorOfVectorData(vecvec3f);
	setUniformVec3FloatArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec3FloatArray(const std::string &variableName, const unsigned int vec3fCount, const float *f) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform3fv(loc, vec3fCount, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4FloatArray(const std::string &variableName, const Vector<Vector<float, 4>, SIZE> &vecvec4f) {
	float *vvData = getVectorOfVectorData(vecvec4f);
	setUniformVec4FloatArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec4FloatArray(const std::string &variableName, const unsigned int vec4fCount, const float *f) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform4fv(loc, vec4fCount, f);
}

template <unsigned int SIZE, unsigned int SIZE2>
float *getVectorOfMatrixData(const Vector<Matrix<float, SIZE>, SIZE2> &vm) {
	float *vmData = new float[SIZE*SIZE*SIZE2];
	for (unsigned int i = 0 ; i < SIZE2 ; ++i) {
		for (unsigned int j = 0 ; j < SIZE ; ++j) {
			for (unsigned int k = 0 ; k < SIZE ; ++k) {
				vmData[i * (SIZE * SIZE) + j * SIZE + k] = vm[i][j][k];
			}
		}
	}
	return vmData;
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformMat2FloatArray(const std::string &variableName, const Vector<Matrix<float, 2>, SIZE> &vecmat2f, const bool transpose) {
	float *vmData = getVectorOfMatrixData(vecmat2f);
	setUniformMat2FloatArray(variableName, SIZE, vmData, transpose);
	delete [] vmData;
}

void GlShaderProgram::setUniformMat2FloatArray(const std::string &variableName, const unsigned int mat2fCount, const float *f, const bool transpose) {
	GLint loc = getUniformVariableLocation(variableName);
	GLboolean transposeGL = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix2fv(loc, mat2fCount, transposeGL, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformMat3FloatArray(const std::string &variableName, const Vector<Matrix<float, 3>, SIZE> &vecmat3f, const bool transpose) {
	float *vmData = getVectorOfMatrixData(vecmat3f);
	setUniformMat3FloatArray(variableName, SIZE, vmData, transpose);
	delete [] vmData;
}

void GlShaderProgram::setUniformMat3FloatArray(const std::string &variableName, const unsigned int mat3fCount, const float *f, const bool transpose) {
	GLint loc = getUniformVariableLocation(variableName);
	GLboolean transposeGL = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix3fv(loc, mat3fCount, transposeGL, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformMat4FloatArray(const std::string &variableName, const Vector<Matrix<float, 4>, SIZE> &vecmat4f, const bool transpose) {
	float *vmData = getVectorOfMatrixData(vecmat4f);
	setUniformMat4FloatArray(variableName, SIZE, vmData, transpose);
	delete [] vmData;
}

void GlShaderProgram::setUniformMat4FloatArray(const std::string &variableName, const unsigned int mat4fCount, const float *f, const bool transpose) {
	GLint loc = getUniformVariableLocation(variableName);
	GLboolean transposeGL = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(loc, mat4fCount, transposeGL, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformIntArray(const std::string &variableName, const Vector<int, SIZE> &veci) {
	setUniformIntArray(variableName, SIZE, (int *)&veci);
}

void GlShaderProgram::setUniformIntArray(const std::string &variableName, const unsigned int iCount, const int *i) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform1iv(loc, iCount, (const GLint*)i);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec2IntArray(const std::string &variableName, const Vector<Vector<int, 2>, SIZE> &vecvec2i) {
	int *vvData = getVectorOfVectorData(vecvec2i);
	setUniformVec2IntArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec2IntArray(const std::string &variableName, const unsigned int vec2iCount, const int *i) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform2iv(loc, vec2iCount,(const GLint*)i);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3IntArray(const std::string &variableName, const Vector<Vector<int, 3>, SIZE> &vecvec3i) {
	int *vvData = getVectorOfVectorData(vecvec3i);
	setUniformVec3IntArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec3IntArray(const std::string &variableName, const unsigned int vec3iCount, const int *i) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform3iv(loc, vec3iCount, (const GLint*)i);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4IntArray(const std::string &variableName, const Vector<Vector<int, 4>, SIZE> &vecvec4i) {
	int *vvData = getVectorOfVectorData(vecvec4i);
	setUniformVec4IntArray(variableName, SIZE, vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec4IntArray(const std::string &variableName, const unsigned int vec4iCount, const int *i) {
	GLint loc = getUniformVariableLocation(variableName);
	glUniform4iv(loc, vec4iCount, (const GLint*)i);
}


template <unsigned int SIZE>
void GlShaderProgram::setUniformBoolArray(const std::string &variableName, const Vector<bool, SIZE> &vecb) {
	setUniformIntArray(variableName, SIZE, (int *) &vecb);
}

void GlShaderProgram::setUniformBoolArray(const std::string &variableName, const unsigned int bCount, const bool *b) {
	setUniformIntArray(variableName, bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec2BoolArray(const std::string &variableName, const Vector<Vector<bool, 2>, SIZE> &vecvec2b) {
	bool *vvData = getVectorOfVectorData(vecvec2b);
	setUniformVec2IntArray(variableName, SIZE, (int *) vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec2BoolArray(const std::string &variableName, const unsigned int vec2bCount, const bool *b) {
	setUniformVec2IntArray(variableName, vec2bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3BoolArray(const std::string &variableName, const Vector<Vector<bool, 3>, SIZE> &vecvec3b) {
	bool *vvData = getVectorOfVectorData(vecvec3b);
	setUniformVec3IntArray(variableName, SIZE, (int *) vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec3BoolArray(const std::string &variableName, const unsigned int vec3bCount, const bool *b) {
	setUniformVec3IntArray(variableName, vec3bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4BoolArray(const std::string &variableName, const Vector<Vector<bool, 4>, SIZE> &vecvec4b) {
	bool *vvData = getVectorOfVectorData(vecvec4b);
	setUniformVec4IntArray(variableName, SIZE, (int *) vvData);
	delete [] vvData;
}

void GlShaderProgram::setUniformVec4BoolArray(const std::string &variableName, const unsigned int vec4bCount, const bool *b) {
	setUniformVec4IntArray(variableName, vec4bCount, (int *) b);
}

void GlShaderProgram::getUniformFloatVariableValue(const std::string &variableName, float *value) {
	GLint loc = getUniformVariableLocation(variableName);
	glGetUniformfv(programObjectId, loc, value);
}

void GlShaderProgram::getUniformIntVariableValue(const std::string &variableName, int *value) {
	GLint loc = getUniformVariableLocation(variableName);
	glGetUniformiv(programObjectId, loc, (GLint *)value);
}

void GlShaderProgram::getUniformBoolVariableValue(const std::string &variableName, bool *value) {
	int valueInt;
	getUniformIntVariableValue(variableName, &valueInt);
	*value = (valueInt > 0);
}

void GlShaderProgram::getUniformVec2BoolVariableValue(const std::string &variableName, bool *value) {
	int valueInt[2];
	getUniformIntVariableValue(variableName, valueInt);
	for (unsigned int i = 0 ; i < 2 ; ++i) {
		value[i] = (valueInt[i] > 0);
	}
}

void GlShaderProgram::getUniformVec3BoolVariableValue(const std::string &variableName, bool *value) {
	int valueInt[3];
	getUniformIntVariableValue(variableName, valueInt);
	for (unsigned int i = 0 ; i < 3 ; ++i) {
		value[i] = (valueInt[i] > 0);
	}
}

void GlShaderProgram::getUniformVec4BoolVariableValue(const std::string &variableName, bool *value) {
	int valueInt[4];
	getUniformIntVariableValue(variableName, valueInt);
	for (unsigned int i = 0 ; i < 4 ; ++i) {
		value[i] = (valueInt[i] > 0);
	}
}

}

