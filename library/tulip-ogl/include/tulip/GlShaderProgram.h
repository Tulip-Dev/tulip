/*
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
///@cond DOXYGEN_HIDDEN


#ifndef GL_SHADER_PROGRAM
#define GL_SHADER_PROGRAM

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>
#include <vector>

#include <tulip/tulipconf.h>
#include <tulip/Matrix.h>
#include <tulip/Color.h>

namespace tlp {

enum ShaderType {Vertex, Fragment, Geometry};

/**
 * \brief A class to manage shader objects, components of a shader program
 *
 * This class allow to create and compile OpenGL shader object. Shaders are used to program the graphics processing unit (GPU) rendering pipeline.
 * The three existing types of shaders are managed :
 *
 *  -> Vertex shader : run once for each vertex given to the graphics processor. The purpose is to transform each vertex's 3D position in virtual space
 *     to the 2D coordinate at which it appears on the screen (as well as a depth value for the Z-buffer).
 *     Vertex shaders can manipulate properties such as position, color, and texture coordinate, but cannot create new vertices.
 *     The output of the vertex shader goes to the next stage in the pipeline, which is either a geometry shader if present or the rasterizer otherwise.
 *
 *  -> Geometry shader : can add and remove vertices from a mesh. Geometry shaders can be used to generate geometry procedurally
 *     or to add volumetric detail to existing meshes that would be too costly to process on the CPU. If geometry shaders are being used,
 *     the output is then sent to the rasterizer.
 *
 *  -> Fragment shader (Pixel shader) : calculate the color of individual pixels. The input to this stage comes from the rasterizer,
 *     which fills in the polygons being sent through the graphics pipeline.
 *
 * Shaders source codes have to be written with the "OpenGL Shading Language (GLSL)"
 */
class TLP_GL_SCOPE GlShader {

  friend class GlShaderProgram;

public :

  /**
   * Vertex and Fragment shader constructor
   *
   * Use this constructor to create either a vertex shader or a fragment shader
   *
   * \param shaderType Type of the shader to create, Vertex or Fragment
   */
  GlShader(ShaderType shaderType);

  /**
   * Geometry shader constructor
   *
   * Use this constructor to create a geometry shader
   *
   * \param inputPrimitiveType the type of graphic primitive the geometry shader takes as input.
   *        (must be one from the following list : GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT)
   *
   * \param outputPrimitiveType the type of graphics primitives the geometry shader will output
   *        (must be one of the following list : GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP)
   */
  GlShader(GLenum inputPrimitiveType, GLenum outputPrimitiveType);

  /**
   * GlShader destructor
   */
  ~GlShader();

  /**
   * Return the GL identifier of this shader object
   */
  GLuint getShaderId() const {
    return shaderObjectId;
  }

  /**
   * Return the type of the shader (Vertex, Geometry or Fragment)
   */
  ShaderType getShaderType() const {
    return shaderType;
  }


  /**
   * Method only relevant for geometry shaders. Return the graphic primitive type this geometry shader takes as input.
   */
  GLenum getInputPrimitiveType() const {
    return inputPrimitiveType;
  }

  /**
   * Method only relevant for geometry shaders. Return the graphics primitives type this geometry shader will output.
   */
  GLenum getOutputPrimitiveType() const {
    return outputPrimitiveType;
  }

  /**
   * Method only relevant for geometry shaders. Set the graphic primitive type this geometry shader takes as input.
   * Note that when modifying the input primitive type, the associated shader program (whose object is from type GlShaderProgram)
   * has to be relinked for changes to take effect.
   *
   * \param inputPrimitiveType the type of graphic primitive the geometry shader takes as input.
   *        (must be one from the following list : GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT)
   */
  void setInputPrimitiveType(const GLenum inputPrimitiveType) {
    this->inputPrimitiveType = inputPrimitiveType;
  }

  /**
   * Method only relevant for geometry shaders. Set the graphics primitives type this geometry shader will output.
   * Note that when modifying the output primitive type, the associated shader program (whose object is from type GlShaderProgram)
   * has to be relinked for changes to take effect.
   *
   * \param outputPrimitiveType the type of graphics primitives the geometry shader will output
   *        (must be one of the following list : GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP)
   */
  void setOutputPrimitiveType(const GLenum outputPrimitiveType) {
    this->outputPrimitiveType = outputPrimitiveType;
  }

  /**
   * Set the shader source code from a C string and compile it.
   *
   *  \param shaderSrc a C string containing the shader source code
   */
  void compileFromSourceCode(const char *shaderSrc);

  /**
   * Set the shader source code from a C++ string and compile it.
   *
   *  \param shaderSrc a C++ string containing the shader source code
   */
  void compileFromSourceCode(const std::string &shaderSrc);

  /**
   * Set the shader source code from a file and compile it.
   *
   *  \param shaderSrcFilename the absolute path of the file containing the shader source code
   */
  void compileFromSourceFile(const std::string &shaderSrcFilename);

  /**
   * Return true if the shader compilation was successfull, false otherwise
   */
  bool isCompiled() const {
    return shaderCompiled;
  }

  /**
   * Return the log output by the shader compiler
   */
  std::string getCompilationLog() const {
    return compilationLog;
  }

private :

  void setAnonymousCreation(const bool anonymousCreation) {
    this->anonymousCreation = anonymousCreation;
  }
  bool anonymouslyCreated() const {
    return anonymousCreation;
  }

  void compileShaderObject(const char *shaderSrc);

  ShaderType shaderType;
  GLuint shaderObjectId;
  GLenum inputPrimitiveType, outputPrimitiveType;
  bool shaderCompiled;
  std::string compilationLog;
  bool anonymousCreation;

};

/**
 * \brief A class to manage OpenGL shader program.
 *
 * This class allows to create and use shader programs by linking several shader objects. At least one shader object must be
 * provided in order to use the shader program. Multiple shader objects of the same type can be added but exactly one
 * of these shader objects must have a main() function. As in C, in order to use a function defined in a separate shader object
 * from another shader object, this function has to be declared with the same prototype in the source code of the last one.
 *
 * This class also allows to specify uniform and attribute variables values of the shader program.
 */
class TLP_GL_SCOPE GlShaderProgram {

public :

  /**
   * GlShaderProgram constructor
   *
   * \param name An optionnal name can be provided to identify the shader program
   */
  GlShaderProgram(const std::string &name = "");

  /**
   * GlShaderProgram destructor
   */
  ~GlShaderProgram();

  /**
   * A static function which returns true if vertex and fragment shaders are supported by the host graphic card
   */
  static bool shaderProgramsSupported();

  /**
   * A static function which returns true if geometry shaders are supported by the host graphic card
   */
  static bool geometryShaderSupported();

  /**
   * A static function which returns the current active shader if any
   */
  static GlShaderProgram *getCurrentActiveShader();

  /**
   * Return the string identifier of this shader program
   */
  std::string getName() const {
    return programName;
  }

  /**
   * Return the OpenGL identifier of this shader program
   */
  GLuint getShaderProgramId() const {
    return programObjectId;
  }

  /**
   * Add a shader object to this shader program
   *
   * \param shader the shader object to add to this shader program
   */
  void addShader(GlShader *shader);

  /**
   * Remove a shader object from this shader program
   * Note that the shader object will not be  destroyed
   *
   * \param shader the shader object to remove from this shader program
   */
  void removeShader(GlShader *shader);

  /**
   * remove all shaders from this shader program
   */
  void removeAllShaders();

  /**
   * Convenient method to add a shader object (from type Vertex or Fragment) from a source code stored in a C string
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param shaderType the type of the shader object to add (must be Vertex or Fragment)
   * \param shaderSrc the C string containing the shader object source code
   */
  void addShaderFromSourceCode(const ShaderType shaderType, const char *shaderSrc);

  /**
   * Convenient method to add a shader object (from type Vertex or Fragment) from a source code stored in a C++ string
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param shaderType the type of the shader object to add (must be Vertex or Fragment)
   * \param shaderSrc the C++ string containing the shader object source code
   */
  void addShaderFromSourceCode(const ShaderType shaderType, const std::string &shaderSrc);

  /**
   * Convenient method to add a shader object (from type Vertex or Fragment) from a source code stored in a file
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param shaderType the type of the shader object to add (must be Vertex or Fragment)
   * \param shaderSrcFilename the aboslute path to the file containing the shader object source code
   */
  void addShaderFromSourceFile(const ShaderType shaderType, const std::string &shaderSrcFilename);

  /**
   * Convenient method to add a geometry shader object from a source code stored in a C string
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param geometryShaderSrc the C string containing the geometry shader object source code
   * \param inputPrimitiveType the type of graphic primitive the geometry shader takes as input.
   *        (must be one from the following list : GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT)
   *
   * \param outputPrimitiveType the type of graphics primitives the geometry shader will output
   *        (must be one of the following list : GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP)
   */
  void addGeometryShaderFromSourceCode(const char *geometryShaderSrc, GLenum inputPrimitiveType, GLenum outputPrimitiveType);

  /**
   * Convenient method to add a geometry shader object from a source code stored in a C++ string
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param geometryShaderSrc the C++ string containing the geometry shader object source code
   * \param inputPrimitiveType the type of graphic primitive the geometry shader takes as input.
   *        (must be one from the following list : GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT)
   *
   * \param outputPrimitiveType the type of graphics primitives the geometry shader will output
   *        (must be one of the following list : GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP)
   */
  void addGeometryShaderFromSourceCode(const std::string &geometryShaderSrc, GLenum inputPrimitiveType, GLenum outputPrimitiveType);

  /**
   * Convenient method to add a geometry shader object from a source code stored in a file
   * The created shader object will be automatically destroyed when removing all attached shader objects
   * or destroying the shader program
   *
   * \param geometryShaderSrcFilename the absolute path to the file containing the geometry shader object source code
   * \param inputPrimitiveType the type of graphic primitive the geometry shader takes as input.
   *        (must be one from the following list : GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT)
   *
   * \param outputPrimitiveType the type of graphics primitives the geometry shader will output
   *        (must be one of the following list : GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP)
   */
  void addGeometryShaderFromSourceFile(const std::string &geometryShaderSrcFilename, GLenum inputPrimitiveType, GLenum outputPrimitiveType);

  /**
   * Link the shader program.
   */
  void link();

  /**
   * return true if the shader program has been successfully linked, false otherwise
   */
  bool isLinked() const {
    return programLinked;
  }

  /**
   * Print the info log containing errors and warnings related to shader objects compilation and shader program linkage
   */
  void printInfoLog();

  /**
   * Activate the shader program. If the shader program has not been linked, the link method will be called.
   */
  void activate();

  /**
   * Deactivate the shader program.
   */
  void desactivate();

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
  void setUniformVec2Int(const std::string &variableName, const int i1, const int i2);
  void setUniformVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i);
  void setUniformVec3Int(const std::string &variableName, const int i1, const int i2, const int i3);
  void setUniformVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i);
  void setUniformVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4);

  void setUniformBool(const std::string &variableName, const bool b);
  void setUniformVec2Bool(const std::string &variableName, const Array<bool, 2> &vec2b);
  void setUniformVec2Bool(const std::string &variableName, const bool b1, const bool b2);
  void setUniformVec3Bool(const std::string &variableName, const Array<bool, 3> &vec3b);
  void setUniformVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3);
  void setUniformVec4Bool(const std::string &variableName, const Array<bool, 4> &vec4b);
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
  void setAttributeVec2Int(const std::string &variableName, const int i1, const int i2);
  void setAttributeVec3Int(const std::string &variableName, const Vector<int, 3> &vec3i);
  void setAttributeVec3Int(const std::string &variableName, const int i1, const int i2, const int i3);
  void setAttributeVec4Int(const std::string &variableName, const Vector<int, 4> &vec4i);
  void setAttributeVec4Int(const std::string &variableName, const int i1, const int i2, const int i3, const int i4);

  void setAttributeBool(const std::string &variableName, const bool b);
  void setAttributeVec2Bool(const std::string &variableName, const Array<bool, 2> &vec2b);
  void setAttributeVec2Bool(const std::string &variableName, const bool b1, const bool b2);
  void setAttributeVec3Bool(const std::string &variableName, const Array<bool, 3> &vec3b);
  void setAttributeVec3Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3);
  void setAttributeVec4Bool(const std::string &variableName, const Array<bool, 4> &vec4b);
  void setAttributeVec4Bool(const std::string &variableName, const bool b1, const bool b2, const bool b3, const bool b4);

  void setUniformTextureSampler(const std::string &samplerVariateName, const int samplerId);
  void setUniformColor(const std::string &variableName, const Color &color);
  void setAttributeColor(const std::string &variableName, const Color &color);

  template <unsigned int SIZE>
  void setUniformFloatArray(const std::string &variableName, const Vector<float, SIZE> &vecf);
  void setUniformFloatArray(const std::string &variableName, const unsigned int fCount, const float *f);

  template <unsigned int SIZE>
  void setUniformVec2FloatArray(const std::string &variableName, const Array<Vector<float, 2>, SIZE> &vecvec2f);
  void setUniformVec2FloatArray(const std::string &variableName, const unsigned int vec2fCount, const float *f);

  template <unsigned int SIZE>
  void setUniformVec3FloatArray(const std::string &variableName, const Array<Vector<float, 3>, SIZE> &vecvec3f);
  void setUniformVec3FloatArray(const std::string &variableName, const unsigned int vec3fCount, const float *f);

  template <unsigned int SIZE>
  void setUniformVec4FloatArray(const std::string &variableName, const Array<Vector<float, 4>, SIZE> &vecvec4f);
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
  void setUniformVec2IntArray(const std::string &variableName, const Array<Vector<int, 2>, SIZE> &vecvec2i);
  void setUniformVec2IntArray(const std::string &variableName, const unsigned int vec2iCount, const int *i);

  template <unsigned int SIZE>
  void setUniformVec3IntArray(const std::string &variableName, const Array<Vector<int, 3>, SIZE> &vecvec3i);
  void setUniformVec3IntArray(const std::string &variableName, const unsigned int vec3iCount, const int *i);

  template <unsigned int SIZE>
  void setUniformVec4IntArray(const std::string &variableName, const Array<Vector<int, 4>, SIZE> &vecvec4i);
  void setUniformVec4IntArray(const std::string &variableName, const unsigned int vec4iCount, const int *i);

  template <unsigned int SIZE>
  void setUniformBoolArray(const std::string &variableName, const Array<bool, SIZE> &vecb);
  void setUniformBoolArray(const std::string &variableName, const unsigned int bCount, const bool *b);

  template <unsigned int SIZE>
  void setUniformVec2BoolArray(const std::string &variableName, const Array<Array<bool, 2>, SIZE> &vecvec2b);
  void setUniformVec2BoolArray(const std::string &variableName, const unsigned int vec2bCount, const bool *b);

  template <unsigned int SIZE>
  void setUniformVec3BoolArray(const std::string &variableName, const Array<Array<bool, 3>, SIZE> &vecvec3b);
  void setUniformVec3BoolArray(const std::string &variableName, const unsigned int vec3bCount, const bool *b);

  template <unsigned int SIZE>
  void setUniformVec4BoolArray(const std::string &variableName, const Array<Array<bool, 4>, SIZE> &vecvec4b);
  void setUniformVec4BoolArray(const std::string &variableName, const unsigned int vec4bCount, const bool *b);

  void getUniformFloatVariableValue(const std::string &variableName, float *value);
  void getUniformIntVariableValue(const std::string &variableName, int *value);
  void getUniformBoolVariableValue(const std::string &variableName, bool *value);
  void getUniformVec2BoolVariableValue(const std::string &variableName, bool *value);
  void getUniformVec3BoolVariableValue(const std::string &variableName, bool *value);
  void getUniformVec4BoolVariableValue(const std::string &variableName, bool *value);

  // This method must be called before calling the link method to
  // set the max number of vertices a geometry shader can output
  // If not called, the maximum value is set when linking the shader program (not recommended for performance).
  void setMaxGeometryShaderOutputVertices(const int maxOutputVertices);

private :

  GLint getUniformVariableLocation(const std::string &variableName);
  GLint getAttributeVariableLocation(const std::string &variableName);

  std::string programName;
  GLuint programObjectId;

  std::string programLinkLog;
  bool programLinked;

  std::vector<GlShader *> attachedShaders;
  int maxGeometryShaderOutputVertices;

  static GlShaderProgram *currentActiveShaderProgram;



};

}

#endif // GL_SHADER_PROGRAM
///@endcond
