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

void printOGLError(const char *file, int line);
#define printOpenGLError() printOGLError(__FILE__, __LINE__)

namespace tlp {

class GlShaderManager {

public :

	static GlShaderManager *getInstance();

	bool isShaderProgramsSupported() const {return shaderProgramsSupported;}
	bool isGeometryShaderSupported() const {return geometryShaderSupported;}

	bool registerShaderProgramFromFiles(const std::string &programName, const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath,
										const std::string &geometryShaderSourceFilePath = "", GLenum inputPrimitiveType = GL_LINES, GLenum outputPrimitiveType = GL_LINE_STRIP);

	bool registerShaderProgramFromStrings(const std::string &programName, const std::string &vertexShaderSource, const std::string &fragmentShaderSource,
										  const std::string &geometryShaderSource = "", GLenum inputPrimitiveType = GL_LINES, GLenum outputPrimitiveType = GL_LINE_STRIP);

	GLuint getShaderProgramId(const std::string &programName);
	void deleteShaderProgram(const std::string &programName);

	void activateShaderProgram(const std::string &programName);
	void desactivateShaderProgram();

private :

	static GlShaderManager *instance;

	GlShaderManager();
	bool checkShaderProgramsSupport();
	bool checkGeometryShaderSupport();
	bool registerShaderProgram(const std::string &programName, const GLuint vertexShaderObject, const GLuint fragmentShaderObject,
							   const GLuint geometryShaderObject, const GLenum inputPrimitiveType, const GLenum outputPrimitiveType);

	bool shaderProgramsSupported;
	bool geometryShaderSupported;
	std::map<std::string, GLuint> shaderProgramsMap;

};


}

#endif /* GLSHADERMANAGER_H_ */
