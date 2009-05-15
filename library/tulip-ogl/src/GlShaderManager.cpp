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
  int infoLogLength = 0;
  int charsWritten  = 0;
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
      cout << infoLog << "\n" << endl;
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

GlShaderManager* GlShaderManager::instance(NULL);

GlShaderManager::GlShaderManager() {
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

bool GlShaderManager::registerShaderProgram(const std::string &programName, const GLuint vertexShaderObject, const GLuint fragmentShaderObject,
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
		shaderProgramsMap[programName] = programObject;
		return true;
	} else {
		return false;
	}
}

bool GlShaderManager::registerShaderProgramFromFiles(const std::string &programName, const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath,
													 const std::string &geometryShaderSourceFilePath, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {

	if (!shaderProgramsSupported) {
		cerr << "Shader programs are not supported by the graphic card" << endl;
		return false;
	}

	if (geometryShaderSourceFilePath != "" && !geometryShaderSupported) {
		cerr << "Geometry shader are not supported by the graphic card" << endl;
		return false;
	}

	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		return true;
	}

	// Get the vertex/fragment shaders
	GLuint vertexShaderObject = getShaderObjectFromFile(vertexShaderSourceFilePath, VERTEX_SHADER);
	GLuint fragmentShaderObject = getShaderObjectFromFile(fragmentShaderSourceFilePath, FRAGMENT_SHADER);
	GLuint geometryShaderObject = getShaderObjectFromFile(geometryShaderSourceFilePath, GEOMETRY_SHADER);


	return registerShaderProgram(programName, vertexShaderObject, fragmentShaderObject, geometryShaderObject, inputPrimitiveType, outputPrimitiveType);
}

bool GlShaderManager::registerShaderProgramFromStrings(const std::string &programName, const std::string &vertexShaderSource, const std::string &fragmentShaderSource,
													   const std::string &geometryShaderSource, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {

	if (!shaderProgramsSupported) {
		cerr << "Shader programs are not supported by the graphic card" << endl;
		return false;
	}

	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		return true;
	}

	if (geometryShaderSource != "" && !geometryShaderSupported) {
		cerr << "Geometry shader are not supported by the graphic card" << endl;
		return false;
	}

	// Get the vertex/fragment shaders
	GLuint vertexShaderObject = getShaderObjectFromString(vertexShaderSource, VERTEX_SHADER);
	GLuint fragmentShaderObject = getShaderObjectFromString(fragmentShaderSource, FRAGMENT_SHADER);
	GLuint geometryShaderObject = getShaderObjectFromString(geometryShaderSource, GEOMETRY_SHADER);

	return registerShaderProgram(programName, vertexShaderObject, fragmentShaderObject, geometryShaderObject, inputPrimitiveType, outputPrimitiveType);
}

GLuint GlShaderManager::getShaderProgramId(const std::string &programName) {
	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		return shaderProgramsMap[programName];
	} else {
		return 0;
	}
}

void GlShaderManager::deleteShaderProgram(const std::string &programName) {
	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		glDeleteProgram(shaderProgramsMap[programName]);
		shaderProgramsMap.erase(programName);
	}
}

void GlShaderManager::activateShaderProgram(const std::string &programName) {
	if (shaderProgramsMap.find(programName) != shaderProgramsMap.end()) {
		glUseProgram(shaderProgramsMap[programName]);
	}
}

void GlShaderManager::desactivateShaderProgram() {
	glUseProgram(0);
}

}

