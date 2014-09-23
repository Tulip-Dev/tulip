/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#include <tulip/GlShaderProgram.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/TlpTools.h>

using namespace std;

enum ObjectType {SHADER, PROGRAM};

static void getInfoLog(GLuint obj, ObjectType objectType, string &logStr) {
  GLint infoLogLength = 0;
  GLint charsWritten  = 0;
  GLchar *infoLog;

  if (objectType == SHADER) {
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
  }
  else {
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
  }

  if (infoLogLength > 1) {
    infoLog = new GLchar[infoLogLength+1];

    if (objectType == SHADER) {
      glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
    }
    else {
      glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
    }

    infoLog[infoLogLength] = '\0';
    logStr = infoLog;
    delete [] infoLog;
  }
}

static void readShaderSourceFile(const string &vertexShaderSourceFilePath, char **shader) {
  istream *ifs = tlp::getInputFileStream(vertexShaderSourceFilePath.c_str());

  if (!ifs->good()) {
    delete ifs;
    tlp::warning() << "Error opening file : " << vertexShaderSourceFilePath << endl;
    return;
  }

  ifs->seekg (0, ios::end);
  unsigned int length = static_cast<unsigned int>(ifs->tellg());
  ifs->seekg (0, ios::beg);

  *shader = new char[length+1];

  ifs->read ((char *)*shader,length);
  (*shader)[length] = '\0';
  delete ifs;
}

namespace tlp {

GlShader::GlShader(ShaderType shaderType) : shaderType(shaderType), shaderObjectId(0), shaderCompiled(false), anonymousCreation(false) {
  if (shaderType == Vertex) {
    shaderObjectId = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (shaderType == Fragment) {
    shaderObjectId = glCreateShader(GL_FRAGMENT_SHADER);
  }
}

GlShader::GlShader(GLenum inputPrimitiveType, GLenum outputPrimitiveType) : shaderType(Geometry), shaderObjectId(0),
  inputPrimitiveType(inputPrimitiveType), outputPrimitiveType(outputPrimitiveType),
  shaderCompiled(false), anonymousCreation(false) {
  shaderObjectId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
}

GlShader::~GlShader() {
  if (shaderObjectId != 0) {
    glDeleteShader(shaderObjectId);
  }
}

void GlShader::compileFromSourceCode(const char *shaderSrc) {
  compileShaderObject(shaderSrc);
}

void GlShader::compileFromSourceCode(const std::string &shaderSrc) {
  compileShaderObject(shaderSrc.c_str());
}

void GlShader::compileFromSourceFile(const std::string &shaderSrcFilename) {
  char *shaderSrcCode = NULL;
  readShaderSourceFile(shaderSrcFilename, &shaderSrcCode);

  if (shaderSrcCode) {
    compileShaderObject(shaderSrcCode);
    delete [] shaderSrcCode;
  }
}

void GlShader::compileShaderObject(const char *shaderSrc) {
  glShaderSource(shaderObjectId, 1,(const char **) &shaderSrc, NULL);
  glCompileShader(shaderObjectId);
  GLint compileStatus;
  glGetShaderiv(shaderObjectId, GL_COMPILE_STATUS, &compileStatus);
  shaderCompiled = compileStatus > 0;
  getInfoLog(shaderObjectId, SHADER, compilationLog);
}

GlShaderProgram *GlShaderProgram::currentActiveShaderProgram(NULL);

GlShaderProgram::GlShaderProgram(const std::string &name) :
  programName(name), programObjectId(0), programLinked(false), maxGeometryShaderOutputVertices(0) {
  programObjectId = glCreateProgram();
}

GlShaderProgram::~GlShaderProgram() {
  removeAllShaders();
  glDeleteProgram(programObjectId);
}

void GlShaderProgram::addShaderFromSourceCode(const ShaderType shaderType, const char *shaderSrc) {
  GlShader *shader = new GlShader(shaderType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceCode(shaderSrc);
  addShader(shader);

}

void GlShaderProgram::addShaderFromSourceCode(const ShaderType shaderType, const std::string &shaderSrc) {
  GlShader *shader = new GlShader(shaderType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceCode(shaderSrc);
  addShader(shader);

}

void GlShaderProgram::addShaderFromSourceFile(const ShaderType shaderType, const std::string &shaderSrcFilename) {
  GlShader *shader = new GlShader(shaderType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceFile(shaderSrcFilename);
  addShader(shader);

}

void GlShaderProgram::addGeometryShaderFromSourceCode(const char *geometryShaderSrc, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {
  GlShader *shader = new GlShader(inputPrimitiveType, outputPrimitiveType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceCode(geometryShaderSrc);
  addShader(shader);
}

void GlShaderProgram::addGeometryShaderFromSourceCode(const std::string &geometryShaderSrc, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {
  GlShader *shader = new GlShader(inputPrimitiveType, outputPrimitiveType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceCode(geometryShaderSrc);
  addShader(shader);
}

void GlShaderProgram::addGeometryShaderFromSourceFile(const std::string &geometryShaderSrcFilename, GLenum inputPrimitiveType, GLenum outputPrimitiveType) {
  GlShader *shader = new GlShader(inputPrimitiveType, outputPrimitiveType);
  shader->setAnonymousCreation(true);
  shader->compileFromSourceFile(geometryShaderSrcFilename);
  addShader(shader);
}

void GlShaderProgram::addShader(GlShader *shader) {
  if (std::find(attachedShaders.begin(), attachedShaders.end(), shader) == attachedShaders.end()) {
    if (shader->isCompiled()) {
      glAttachShader(programObjectId, shader->getShaderId());
    }

    attachedShaders.push_back(shader);
    programLinked = false;
  }
}

void GlShaderProgram::removeShader(GlShader *shader) {
  if (std::find(attachedShaders.begin(), attachedShaders.end(), shader) != attachedShaders.end()) {
    if (shader->isCompiled()) {
      glDetachShader(programObjectId, shader->getShaderId());
    }

    attachedShaders.erase(remove(attachedShaders.begin(), attachedShaders.end(), shader), attachedShaders.end());
    programLinked = false;
  }
}

void GlShaderProgram::removeAllShaders() {
  for (size_t i = 0 ; i < attachedShaders.size() ; ++i) {
    removeShader(attachedShaders[i]);

    if (attachedShaders[i]->anonymouslyCreated()) {
      delete attachedShaders[i];
    }
  }
}

void GlShaderProgram::link() {
  bool allShaderCompiled = true;

  for (size_t i = 0 ; i < attachedShaders.size() ; ++i) {
    if (!attachedShaders[i]->isCompiled()) {
      allShaderCompiled = false;
    }

    if (attachedShaders[i]->getShaderType() == Geometry) {
      glProgramParameteriEXT(programObjectId, GL_GEOMETRY_INPUT_TYPE_EXT, attachedShaders[i]->getInputPrimitiveType());
      glProgramParameteriEXT(programObjectId, GL_GEOMETRY_OUTPUT_TYPE_EXT, attachedShaders[i]->getOutputPrimitiveType());

      GLint maxOutputVertices = maxGeometryShaderOutputVertices;

      if (maxOutputVertices == 0)
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxOutputVertices);

      glProgramParameteriEXT(programObjectId, GL_GEOMETRY_VERTICES_OUT_EXT, maxOutputVertices);
    }
  }

  glLinkProgram(programObjectId);

  getInfoLog(programObjectId, PROGRAM, programLinkLog);

  GLint linked;
  glGetProgramiv(programObjectId, GL_LINK_STATUS, &linked);
  programLinked = allShaderCompiled && linked > 0;
}

void GlShaderProgram::printInfoLog() {
  for (size_t i = 0 ; i < attachedShaders.size() ; ++i) {
    string shaderCompilationlog = attachedShaders[i]->getCompilationLog();

    if (shaderCompilationlog != "") {
      tlp::debug() << shaderCompilationlog << endl;
    }
  }

  if (programLinkLog != "") {
    tlp::debug() << programLinkLog << endl;
  }
}

void GlShaderProgram::activate() {
  if (!programLinked) {
    link();
  }

  if (programLinked) {
    glUseProgram(programObjectId);
    currentActiveShaderProgram = this;
  }
}

void GlShaderProgram::desactivate() {
  glUseProgram(0);
  currentActiveShaderProgram = NULL;
}

bool GlShaderProgram::shaderProgramsSupported() {

  static bool vertexShaderExtOk = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_vertex_shader");
  static bool fragmentShaderExtOk = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_fragment_shader");
  return (vertexShaderExtOk && fragmentShaderExtOk);
}

bool GlShaderProgram::geometryShaderSupported() {
  static bool geometryShaderExtOk = OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_geometry_shader4");
  return geometryShaderExtOk;
}

GlShaderProgram *GlShaderProgram::getCurrentActiveShader() {
  return currentActiveShaderProgram;
}

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

void GlShaderProgram::setUniformVec2Int(const std::string &variableName, const int i1, const int i2) {
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

void GlShaderProgram::setUniformVec2Bool(const std::string &variableName, const Array<bool, 2> &vec2b) {
  setUniformVec2IntArray(variableName, 1, (int *) &vec2b);
}

void GlShaderProgram::setUniformVec2Bool(const std::string &variableName, const bool b1, const bool b2) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform2i(loc, b1, b2);
}

void GlShaderProgram::setUniformVec3Bool(const std::string &variableName, const Array<bool, 3> &vec3b) {
  setUniformVec3IntArray(variableName, 1, (int *) &vec3b);
}

void GlShaderProgram::setUniformVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform3i(loc, b1, b2, b3);
}

void GlShaderProgram::setUniformVec4Bool(const std::string &variableName, const Array<bool, 4> &vec4b) {
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

void GlShaderProgram::setAttributeVec2Int(const std::string &variableName, const int i1, const int i2) {
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

void GlShaderProgram::setAttributeVec2Bool(const std::string &variableName, const Array<bool, 2> &vec2b) {
  setAttributeVec2Bool(variableName, vec2b[0], vec2b[1]);
}

void GlShaderProgram::setAttributeVec2Bool(const std::string &variableName, const bool b1, const bool b2) {
  setAttributeVec2Int(variableName, b1, b2);
}

void GlShaderProgram::setAttributeVec3Bool(const std::string &variableName, const Array<bool, 3> &vec3b) {
  setAttributeVec3Bool(variableName, vec3b[0], vec3b[1], vec3b[2]);
}

void GlShaderProgram::setAttributeVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3) {
  setAttributeVec3Int(variableName, b1, b2, b3);
}

void GlShaderProgram::setAttributeVec4Bool(const std::string &variableName, const Array<bool, 4> &vec4b) {
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
T *getVectorOfVectorData(const Array<Vector<T, SIZE>, SIZE2> &vv) {
  T *vvData = new T[SIZE*SIZE2];

  for (unsigned int i = 0 ; i < SIZE2 ; ++i) {
    for (unsigned int j = 0 ; j < SIZE ; ++j) {
      vvData[i * SIZE + j] = vv[i][j];
    }
  }

  return vvData;
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec2FloatArray(const std::string &variableName, const Array<Vector<float, 2>, SIZE> &vecvec2f) {
  float *vvData = getVectorOfVectorData(vecvec2f);
  setUniformVec2FloatArray(variableName, SIZE, vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec2FloatArray(const std::string &variableName, const unsigned int vec2fCount, const float *f) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform2fv(loc, vec2fCount, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3FloatArray(const std::string &variableName, const Array<Vector<float, 3>, SIZE> &vecvec3f) {
  float *vvData = getVectorOfVectorData(vecvec3f);
  setUniformVec3FloatArray(variableName, SIZE, vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec3FloatArray(const std::string &variableName, const unsigned int vec3fCount, const float *f) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform3fv(loc, vec3fCount, f);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4FloatArray(const std::string &variableName, const Array<Vector<float, 4>, SIZE> &vecvec4f) {
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
void GlShaderProgram::setUniformVec2IntArray(const std::string &variableName, const Array<Vector<int, 2>, SIZE> &vecvec2i) {
  int *vvData = getVectorOfVectorData(vecvec2i);
  setUniformVec2IntArray(variableName, SIZE, vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec2IntArray(const std::string &variableName, const unsigned int vec2iCount, const int *i) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform2iv(loc, vec2iCount,(const GLint*)i);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3IntArray(const std::string &variableName, const Array<Vector<int, 3>, SIZE> &vecvec3i) {
  int *vvData = getVectorOfVectorData(vecvec3i);
  setUniformVec3IntArray(variableName, SIZE, vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec3IntArray(const std::string &variableName, const unsigned int vec3iCount, const int *i) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform3iv(loc, vec3iCount, (const GLint*)i);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4IntArray(const std::string &variableName, const Array<Vector<int, 4>, SIZE> &vecvec4i) {
  int *vvData = getVectorOfVectorData(vecvec4i);
  setUniformVec4IntArray(variableName, SIZE, vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec4IntArray(const std::string &variableName, const unsigned int vec4iCount, const int *i) {
  GLint loc = getUniformVariableLocation(variableName);
  glUniform4iv(loc, vec4iCount, (const GLint*)i);
}


template <unsigned int SIZE>
void GlShaderProgram::setUniformBoolArray(const std::string &variableName, const Array<bool, SIZE> &vecb) {
  setUniformIntArray(variableName, SIZE, (int *) &vecb);
}

void GlShaderProgram::setUniformBoolArray(const std::string &variableName, const unsigned int bCount, const bool *b) {
  setUniformIntArray(variableName, bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec2BoolArray(const std::string &variableName, const Array<Array<bool, 2>, SIZE> &vecvec2b) {
  bool *vvData = getVectorOfVectorData(vecvec2b);
  setUniformVec2IntArray(variableName, SIZE, (int *) vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec2BoolArray(const std::string &variableName, const unsigned int vec2bCount, const bool *b) {
  setUniformVec2IntArray(variableName, vec2bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec3BoolArray(const std::string &variableName, const Array<Array<bool, 3>, SIZE> &vecvec3b) {
  bool *vvData = getVectorOfVectorData(vecvec3b);
  setUniformVec3IntArray(variableName, SIZE, (int *) vvData);
  delete [] vvData;
}

void GlShaderProgram::setUniformVec3BoolArray(const std::string &variableName, const unsigned int vec3bCount, const bool *b) {
  setUniformVec3IntArray(variableName, vec3bCount, (int *) b);
}

template <unsigned int SIZE>
void GlShaderProgram::setUniformVec4BoolArray(const std::string &variableName, const Array<Array<bool, 4>, SIZE> &vecvec4b) {
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

void GlShaderProgram::setMaxGeometryShaderOutputVertices(const int maxOutputVertices) {
  maxGeometryShaderOutputVertices = maxOutputVertices;
}

}

