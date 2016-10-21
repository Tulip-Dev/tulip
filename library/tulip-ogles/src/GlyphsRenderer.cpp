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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <GL/glew.h>

#include <tulip/GlyphsRenderer.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlBuffer.h>
#include <tulip/GlyphsManager.h>
#include <tulip/GlUtils.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Camera.h>
#include <tulip/Light.h>
#include <tulip/ShaderManager.h>

using namespace std;
using namespace tlp;

static std::string glyphVertexShaderSrcCommonCode = R"(
  uniform mat4 u_modelviewMatrix;
  uniform mat4 u_projectionMatrix;
  uniform mat4 u_normalMatrix;
  uniform bool u_textureActivated;
  uniform bool u_flatShading;
  uniform bool u_swapYZ;
  uniform bool u_billboarding;
  uniform bool u_rotationAxisAndAngle;

  uniform vec3 u_eyePosition;
  uniform vec4 u_lightPosition;
  uniform float u_lightConstantAttenuation;
  uniform float u_lightLinearAttenuation;
  uniform float u_lightQuadraticAttenuation;
  uniform vec4 u_lightModelAmbientColor;
  uniform vec4 u_lightAmbientColor;
  uniform vec4 u_lightDiffuseColor;

  uniform vec4 u_materialAmbientColor;

  attribute vec4 a_position;
  attribute vec2 a_texCoord;
  attribute vec3 a_normal;

  varying vec4 v_diffuseColor;
  varying vec4 v_ambientGlobalColor;
  varying vec4 v_ambientColor;

  varying vec3 v_normal;
  varying vec3 v_lightDir;
  varying vec3 v_halfVector;
  varying float v_attenuation;

  varying vec4 v_texData;

  mat4 scaleMatrix(vec3 scale) {
     mat4 ret = mat4(1.0);
     ret[0][0] = scale[0];
     ret[1][1] = scale[1];
     ret[2][2] = scale[2];
     return ret;
  }

  mat4 translationMatrix(vec3 center) {
     mat4 ret = mat4(1.0);
     ret[3][0] = center[0];
     ret[3][1] = center[1];
     ret[3][2] = center[2];
     return ret;
  }

  mat4 rotationMatrixFromAxisAndAngle(vec3 rotationVector, float rotationAngle) {
     mat4 ret = mat4(1.0);
     float c = cos(rotationAngle);
     float s = sin(rotationAngle);
     ret[0][0] = rotationVector[0]*rotationVector[0]*(1.0 - c) + c;
     ret[1][0] = rotationVector[0]*rotationVector[1]*(1.0 - c) - rotationVector[2]*s;
     ret[2][0] = rotationVector[0]*rotationVector[2]*(1.0 - c) + rotationVector[1]*s;
     ret[0][1] = rotationVector[1]*rotationVector[0]*(1.0 - c) + rotationVector[2]*s;
     ret[1][1] = rotationVector[1]*rotationVector[1]*(1.0 - c) + c;
     ret[2][1] = rotationVector[1]*rotationVector[2]*(1.0 - c) - rotationVector[0]*s;
     ret[0][2] = rotationVector[0]*rotationVector[2]*(1.0 - c) - rotationVector[1]*s;
     ret[1][2] = rotationVector[1]*rotationVector[2]*(1.0 - c) + rotationVector[0]*s;
     ret[2][2] = rotationVector[2]*rotationVector[2]*(1.0 - c) + c;
     return ret;
  }

  mat4 rotationMatrix(vec4 rotationData) {
    if (u_rotationAxisAndAngle) {
      return rotationMatrixFromAxisAndAngle(rotationData.xyz, rotationData.w);
    } else {
      mat4 ret = mat4(1.0);
      ret *= rotationMatrixFromAxisAndAngle(vec3(1.0, 0.0, 0.0), rotationData.x);
      ret *= rotationMatrixFromAxisAndAngle(vec3(0.0, 1.0, 0.0), rotationData.y);
      ret *= rotationMatrixFromAxisAndAngle(vec3(0.0, 0.0, 1.0), rotationData.z);
      return ret;
    }
  }

  mat4 billboardMatrix(mat4 originalMdvMatrix, vec3 size) {
    mat4 ret = originalMdvMatrix;
    ret[0][0] = size.x;
    ret[1][1] = size.y;
    ret[2][2] = size.z;
    ret[0][1] = 0.0;
    ret[0][2] = 0.0;
    ret[1][0] = 0.0;
    ret[1][2] = 0.0;
    ret[2][0] = 0.0;
    ret[2][1] = 0.0;
    return ret;
  }
)";

static std::string genGlyphsVertexShaderSrc(const unsigned int maxNbGlyphsByRenderingBatch) {
  std::string shaderSrc = ShaderManager::getShaderSrcPrefix() + "const int MAX_NB_GLYPHS = " + toString(maxNbGlyphsByRenderingBatch) + ";\n" +
                          glyphVertexShaderSrcCommonCode + R"(
       uniform vec3 u_center[MAX_NB_GLYPHS];
       uniform vec3 u_scale[MAX_NB_GLYPHS];
       uniform vec4 u_rotation[MAX_NB_GLYPHS];
       uniform vec4 u_color[MAX_NB_GLYPHS];
       uniform float u_textureUnit[MAX_NB_GLYPHS];
       uniform vec4 u_texCoordOffsets[MAX_NB_GLYPHS];

       void main() {
         int id = int(a_position.w);
         mat4 mdv = u_modelviewMatrix * translationMatrix(u_center[id]) * rotationMatrix(u_rotation[id]) * scaleMatrix(u_scale[id]);
         if (u_billboarding) {
           mdv = billboardMatrix(mdv, u_scale[id]) * rotationMatrix(u_rotation[id]);
         }
         vec4 pos;
         if (u_swapYZ) {
           pos = mdv * vec4(a_position.xzy, 1.0);
         } else {
           pos = mdv * vec4(a_position.xyz, 1.0);
         }
         gl_Position = u_projectionMatrix * pos;

         if (u_textureActivated) {
           v_texData.xy = vec2(u_texCoordOffsets[id].x + a_texCoord.x * (u_texCoordOffsets[id].z - u_texCoordOffsets[id].x),
                             u_texCoordOffsets[id].y + a_texCoord.y * (u_texCoordOffsets[id].w - u_texCoordOffsets[id].y));
           v_texData.z = u_textureUnit[id];
         }

         if (u_flatShading) {
            v_diffuseColor = u_color[id];
            return;
         }

         if (u_swapYZ) {
           v_normal = normalize(mat3(u_normalMatrix) * a_normal.xzy);
         } else {
           v_normal = normalize(mat3(u_normalMatrix) * a_normal);
         }
         vec3 lightVec = u_lightPosition.xyz-pos.xyz;
         if (u_lightPosition.w == 0.0) {
           lightVec = -u_lightPosition.xyz;
         }
         v_lightDir = normalize(lightVec);
         float dist = length(lightVec);
         v_attenuation = 1.0 / (u_lightConstantAttenuation +
                         u_lightLinearAttenuation * dist +
                         u_lightQuadraticAttenuation * dist * dist);

         v_halfVector = u_eyePosition + u_lightPosition.xyz;
         v_halfVector = normalize(v_halfVector);

         v_diffuseColor = u_lightDiffuseColor * u_color[id];

         v_ambientColor = u_lightAmbientColor * u_materialAmbientColor;
         v_ambientGlobalColor = u_lightModelAmbientColor * u_materialAmbientColor;
       }
    )";
  return shaderSrc;
}

static std::string glyphsVertexShaderHardwareInstancingSrc() {
  return ShaderManager::getShaderSrcPrefix() + glyphVertexShaderSrcCommonCode + R"(
      attribute vec3 a_center;
      attribute vec3 a_scale;
      attribute vec4 a_rotation;
      attribute vec4 a_color;
      attribute float a_textureUnit;
      attribute vec4 a_texCoordOffsets;

      void main() {
        mat4 mdv = u_modelviewMatrix * translationMatrix(a_center) * rotationMatrix(a_rotation) * scaleMatrix(a_scale);
        if (u_billboarding) {
          mdv = billboardMatrix(mdv, a_scale) * rotationMatrix(a_rotation);
        }
        vec4 pos;
        if (u_swapYZ) {
          pos = mdv * vec4(a_position.xzy, 1.0);
        } else {
          pos = mdv * vec4(a_position.xyz, 1.0);
        }

        gl_Position = u_projectionMatrix * pos;

        if (u_textureActivated) {
          v_texData.xy = vec2(a_texCoordOffsets.x + a_texCoord.x * (a_texCoordOffsets.z - a_texCoordOffsets.x),
                              a_texCoordOffsets.y + a_texCoord.y * (a_texCoordOffsets.w - a_texCoordOffsets.y));
          v_texData.z = a_textureUnit;
        }

        if (u_flatShading) {
          v_diffuseColor = a_color;
          return;
        }

        if (u_swapYZ) {
          v_normal = normalize(mat3(u_normalMatrix) * a_normal.xzy);
        } else {
          v_normal = normalize(mat3(u_normalMatrix) * a_normal);
        }

        vec3 lightVec = u_lightPosition.xyz-pos.xyz;
        if (u_lightPosition.w == 0.0) {
          lightVec = -u_lightPosition.xyz;
        }
        v_lightDir = normalize(lightVec);
        float dist = length(lightVec);
        v_attenuation = 1.0 / (u_lightConstantAttenuation +
                        u_lightLinearAttenuation * dist +
                        u_lightQuadraticAttenuation * dist * dist);

        v_halfVector = u_eyePosition + u_lightPosition.xyz;
        v_halfVector = normalize(v_halfVector);

        v_diffuseColor = u_lightDiffuseColor * a_color;

        v_ambientColor = u_lightAmbientColor * u_materialAmbientColor;
        v_ambientGlobalColor = u_lightModelAmbientColor * u_materialAmbientColor;
      }
    )";
}

static std::string glyphsFragmentShaderSrc() {
  return ShaderManager::getShaderSrcPrefix() + R"(
    uniform bool u_flatShading;
    uniform bool u_textureActivated;
    uniform sampler2D u_textures[4];

    uniform vec4 u_lightSpecularColor;
    uniform vec4 u_materialSpecularColor;
    uniform float u_materialShininess;

    varying vec4 v_diffuseColor;
    varying vec4 v_ambientGlobalColor;
    varying vec4 v_ambientColor;

    varying vec3 v_normal;
    varying vec3 v_lightDir;
    varying vec3 v_halfVector;
    varying float v_attenuation;

    varying vec4 v_texData;

    vec4 getTexel(int samplerId) {
      vec2 v_texCoord = v_texData.xy;
      vec4 texel = vec4(0.0);
      if (samplerId == 0) {
        texel = texture2D(u_textures[0], v_texCoord);
      } else if (samplerId == 1) {
        texel = texture2D(u_textures[1], v_texCoord);
      } else if (samplerId == 2) {
        texel = texture2D(u_textures[2], v_texCoord);
      } else if (samplerId == 3) {
        texel = texture2D(u_textures[3], v_texCoord);
      }
      return texel;
    }

    void main() {
      int samplerId = int(v_texData.z);
      if (u_flatShading) {
        gl_FragColor = v_diffuseColor;
        if (u_textureActivated && samplerId != -1) {
          gl_FragColor *= getTexel(samplerId);
        }
        return;
      }
      vec3 normal = normalize(v_normal);
      vec4 color = v_ambientGlobalColor + v_ambientColor;
      //   float NdotL = max(dot(normal,normalize(v_lightDir)),0.0);
      float NdotL = abs(dot(normal,normalize(v_lightDir)));
      if (NdotL > 0.0) {
        color += (v_attenuation * v_diffuseColor * NdotL);
        vec3 halfV = normalize(v_halfVector);
        float NdotHV = max(dot(normal, halfV), 0.0);
        color += v_attenuation * u_materialSpecularColor * u_lightSpecularColor * pow(NdotHV, u_materialShininess);
      }

      if (u_textureActivated && samplerId != -1) {
        color *= getTexel(samplerId);
      }

      gl_FragColor = color;
    }
  )";
}

static const unsigned int ushortMax = 65535;

static int maxNbGlyphsByRenderingBatch = 0;

map<string, GlyphsRenderer *> GlyphsRenderer::_instances;
string GlyphsRenderer::_currentCanvasId("");

GlyphsRenderer *GlyphsRenderer::instance() {
  return instance(_currentCanvasId);
}

GlyphsRenderer *GlyphsRenderer::instance(const string &canvasId) {
  if (_instances.find(canvasId) == _instances.end()) {
    _instances[canvasId] = new GlyphsRenderer();
  }
  return _instances[canvasId];
}

GlyphsRenderer::GlyphsRenderer() : _billboardMode(false) {

  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxNbGlyphsByRenderingBatch);
  if (maxNbGlyphsByRenderingBatch > 1024) {
    maxNbGlyphsByRenderingBatch /= 4;
  }
  maxNbGlyphsByRenderingBatch = (maxNbGlyphsByRenderingBatch * 160) / 1024;

#ifdef __EMSCRIPTEN__
  std::string webglExtensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
  _canUseUIntIndices = webglExtensions.find("OES_element_index_uint") != std::string::npos;
  _canUseHardwareInstancing = webglExtensions.find("ANGLE_instanced_arrays") != std::string::npos;
#else
  std::string glVendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  _canUseHardwareInstancing = glewIsSupported("GL_ARB_instanced_arrays") && glVendor.find("Intel") == string::npos;
#endif

  bool glyphShaderOk = false;
  while (!glyphShaderOk) {
    _glyphShader = new GlShaderProgram();
    if (_canUseHardwareInstancing) {
      _glyphShader->addShaderFromSourceCode(GlShader::Vertex, glyphsVertexShaderHardwareInstancingSrc());
    } else {
      _glyphShader->addShaderFromSourceCode(GlShader::Vertex, genGlyphsVertexShaderSrc(maxNbGlyphsByRenderingBatch));
    }
    _glyphShader->addShaderFromSourceCode(GlShader::Fragment, glyphsFragmentShaderSrc());

    _glyphShader->link();
    _glyphShader->printInfoLog();
    if (!_glyphShader->isLinked()) {
      --maxNbGlyphsByRenderingBatch;
      delete _glyphShader;
    } else {
      glyphShaderOk = true;
    }
  }

  _glyphsInstanceAttributesDataBuffer = new GlBuffer(GlBuffer::VertexBuffer, GlBuffer::DynamicDraw);
}

void GlyphsRenderer::prepareGlyphDataPseudoInstancing(int glyphId) {

  Glyph *glyph = GlyphsManager::instance()->getGlyph(glyphId);

  if (_glyphsDataStride.find(glyph) != _glyphsDataStride.end()) {
    return;
  }

  if (_canUseUIntIndices) {
    _maxGlyphInstanceByRenderingBatch[glyph] = maxNbGlyphsByRenderingBatch;
  } else {
    if ((glyph->getGlyphVerticesIndices().size() * maxNbGlyphsByRenderingBatch) < ushortMax) {
      _maxGlyphInstanceByRenderingBatch[glyph] = maxNbGlyphsByRenderingBatch;
    } else {
      _maxGlyphInstanceByRenderingBatch[glyph] = ushortMax / glyph->getGlyphVerticesIndices().size();
    }
  }

  std::vector<float> glyphsData;
  std::vector<unsigned short> glyphsIndicesUShort;
  std::vector<unsigned int> glyphsIndicesUInt;

  const vector<Coord> &glyphVertices = glyph->getGlyphVertices();
  const vector<Vec2f> &glyphTexCoords = glyph->getGlyphTexCoords();
  const vector<unsigned short> &glyphVerticesIndices = glyph->getGlyphVerticesIndices();
  const vector<vector<unsigned short>> &glyphOutlinesIndices = glyph->getGlyphOutlineIndices();
  const vector<Coord> &glyphNormals = glyph->getGlyphNormals();
  unsigned short nbVertices = glyphVertices.size();

  _glyphsDataStride[glyph] = 4;
  if (!glyphTexCoords.empty()) {
    _glyphsDataStride[glyph] += 2;
  }
  if (!glyphNormals.empty()) {
    _glyphsDataStride[glyph] += 3;
  }

  for (unsigned short i = 0; i < _maxGlyphInstanceByRenderingBatch[glyph]; ++i) {
    for (unsigned short j = 0; j < nbVertices; ++j) {
      addTlpVecToVecFloat(Vec4f(glyphVertices[j][0], glyphVertices[j][1], glyphVertices[j][2], float(i)), glyphsData);
      if (!glyphTexCoords.empty()) {
        addTlpVecToVecFloat(glyphTexCoords[j], glyphsData);
      }
      if (!glyphNormals.empty()) {
        addTlpVecToVecFloat(glyphNormals[j], glyphsData);
      }
    }
    for (unsigned short j = 0; j < glyphVerticesIndices.size(); ++j) {
      if (_canUseUIntIndices) {
        glyphsIndicesUInt.push_back(i * nbVertices + glyphVerticesIndices[j]);
      } else {
        glyphsIndicesUShort.push_back(i * nbVertices + glyphVerticesIndices[j]);
      }
    }
  }

  vector<unsigned int> glyphOutlinesNbIndices;
  vector<unsigned int> glyphOutlinesIndicesOffsets;
  unsigned int offset = 0;
  for (unsigned int i = 0; i < glyphOutlinesIndices.size(); ++i) {
    glyphOutlinesNbIndices.push_back(glyphOutlinesIndices[i].size());
    if (_canUseUIntIndices) {
      glyphOutlinesIndicesOffsets.push_back((glyphsIndicesUInt.size() + offset) * sizeof(unsigned int));
    } else {
      glyphOutlinesIndicesOffsets.push_back((glyphsIndicesUShort.size() + offset) * sizeof(unsigned short));
    }
    offset += glyphOutlinesIndices[i].size();
  }

  for (unsigned short i = 0; i < _maxGlyphInstanceByRenderingBatch[glyph]; ++i) {
    for (unsigned int j = 0; j < glyphOutlinesIndices.size(); ++j) {
      for (unsigned int k = 0; k < glyphOutlinesIndices[j].size(); ++k) {
        if (_canUseUIntIndices) {
          glyphsIndicesUInt.push_back(i * nbVertices + glyphOutlinesIndices[j][k]);
        } else {
          glyphsIndicesUShort.push_back(i * nbVertices + glyphOutlinesIndices[j][k]);
        }
      }
    }
  }

  _glyphsOutlinesNbIndices[glyph] = glyphOutlinesNbIndices;
  _glyphsOutlinesIndicesOffset[glyph] = glyphOutlinesIndicesOffsets;

  _glyphsDataBuffer[glyph] = new GlBuffer(GlBuffer::VertexBuffer);
  _glyphsDataBuffer[glyph]->allocate(glyphsData);

  _glyphsIndicesBuffer[glyph] = new GlBuffer(GlBuffer::IndexBuffer);
  if (_canUseUIntIndices) {
    _glyphsIndicesBuffer[glyph]->allocate(glyphsIndicesUInt);
  } else {
    _glyphsIndicesBuffer[glyph]->allocate(glyphsIndicesUShort);
  }
}

void GlyphsRenderer::prepareGlyphDataHardwareInstancing(int glyphId) {

  Glyph *glyph = GlyphsManager::instance()->getGlyph(glyphId);

  if (_glyphsDataStride.find(glyph) != _glyphsDataStride.end()) {
    return;
  }

  std::vector<float> glyphsData;
  std::vector<float> glyphsOutlineData;
  std::vector<unsigned short> glyphsIndicesUShort;

  const vector<Coord> &glyphVertices = glyph->getGlyphVertices();
  const vector<Vec2f> &glyphTexCoords = glyph->getGlyphTexCoords();
  const vector<unsigned short> &glyphVerticesIndices = glyph->getGlyphVerticesIndices();
  const vector<vector<unsigned short>> &glyphOutlinesIndices = glyph->getGlyphOutlineIndices();
  const vector<Coord> &glyphNormals = glyph->getGlyphNormals();
  unsigned int nbVertices = glyphVertices.size();

  _glyphsDataStride[glyph] = 3;
  if (!glyphTexCoords.empty()) {
    _glyphsDataStride[glyph] += 2;
  }
  if (!glyphNormals.empty()) {
    _glyphsDataStride[glyph] += 3;
  }

  for (unsigned int i = 0; i < nbVertices; ++i) {
    addTlpVecToVecFloat(glyphVertices[i], glyphsData);
    if (!glyphTexCoords.empty()) {
      addTlpVecToVecFloat(glyphTexCoords[i], glyphsData);
    }
    if (!glyphNormals.empty()) {
      addTlpVecToVecFloat(glyphNormals[i], glyphsData);
    }
  }

  for (size_t i = 0; i < glyphVerticesIndices.size(); ++i) {
    glyphsIndicesUShort.push_back(glyphVerticesIndices[i]);
  }

  vector<unsigned int> glyphOutlinesNbIndices;
  vector<unsigned int> glyphOutlinesIndicesOffsets;

  for (size_t i = 0; i < glyphOutlinesIndices.size(); ++i) {
    glyphOutlinesNbIndices.push_back(glyphOutlinesIndices[i].size());
    glyphOutlinesIndicesOffsets.push_back(glyphsIndicesUShort.size() * sizeof(unsigned short));
    for (size_t j = 0; j < glyphOutlinesIndices[i].size(); ++j) {
      glyphsIndicesUShort.push_back(glyphOutlinesIndices[i][j]);
    }
  }

  _glyphsOutlinesNbIndices[glyph] = glyphOutlinesNbIndices;
  _glyphsOutlinesIndicesOffset[glyph] = glyphOutlinesIndicesOffsets;

  _glyphsDataBuffer[glyph] = new GlBuffer(GlBuffer::VertexBuffer);
  _glyphsDataBuffer[glyph]->allocate(glyphsData);

  _glyphsIndicesBuffer[glyph] = new GlBuffer(GlBuffer::IndexBuffer);
  _glyphsIndicesBuffer[glyph]->allocate(glyphsIndicesUShort);
}

void GlyphsRenderer::renderGlyphs(const Camera &camera, const Light &light, int glyphId, const std::vector<tlp::Coord> &centers,
                                  const std::vector<tlp::Size> &sizes, const std::vector<tlp::Color> &colors,
                                  const std::vector<std::string> &textures, const std::vector<float> &borderWidths,
                                  const std::vector<tlp::Color> &borderColors, const std::vector<Vec4f> &rotationData, bool forceFlatShading,
                                  bool swapYZ, bool rotationAxisAndAngle) {

  if (centers.empty()) {
    return;
  }

  assert(centers.size() == sizes.size() && centers.size() == colors.size());
  assert(textures.empty() || centers.size() == textures.size());
  assert(borderWidths.empty() || centers.size() == borderWidths.size());
  assert(borderColors.empty() || centers.size() == borderColors.size());
  assert(rotationData.empty() || centers.size() == rotationData.size());

  GlTextureManager::instance()->bindTexturesAtlas();
  setupGlyphsShader(camera, light);
  _glyphShader->setUniformBool("u_rotationAxisAndAngle", rotationAxisAndAngle);
  if (_canUseHardwareInstancing) {
    renderGlyphsHardwareInstancing(glyphId, centers, sizes, colors, textures, borderWidths, borderColors, rotationData, forceFlatShading,
                                   swapYZ);
  } else {
    renderGlyphsPseudoInstancing(glyphId, centers, sizes, colors, textures, borderWidths, borderColors, rotationData, forceFlatShading,
                                 swapYZ);
  }
  GlTextureManager::instance()->unbindTexturesAtlas();
}

template <typename T> vector<T> putValInVector(const T &val) {
  vector<T> ret;
  ret.push_back(val);
  return ret;
}

void GlyphsRenderer::renderGlyph(const Camera &camera, const Light &light, int glyphId, const tlp::Coord &center, const tlp::Size &size,
                                 const tlp::Color &color, const std::string &texture, const float &borderWidth, const tlp::Color &borderColor,
                                 const tlp::Vec4f &rotationData, bool forceFlatShading, bool swapYZ, bool rotationAxisAndAngle) {
  renderGlyphs(camera, light, glyphId, putValInVector(center), putValInVector(size), putValInVector(color), putValInVector(texture),
               putValInVector(borderWidth), putValInVector(borderColor), putValInVector(rotationData), forceFlatShading, swapYZ,
               rotationAxisAndAngle);
}

void GlyphsRenderer::setupGlyphsShader(const Camera &camera, const Light &light) {

  _glyphShader->activate();
  _glyphShader->setUniformMat4Float("u_modelviewMatrix", camera.modelviewMatrix());
  _glyphShader->setUniformMat4Float("u_projectionMatrix", camera.projectionMatrix());
  _glyphShader->setUniformMat4Float("u_normalMatrix", camera.normalMatrix());
  _glyphShader->setUniformVec3Float("u_eyePosition", camera.getEyes());
  _glyphShader->setUniformVec4Float("u_lightPosition", light.getPosition());
  _glyphShader->setUniformFloat("u_lightConstantAttenuation", light.getConstantAttenuation());
  _glyphShader->setUniformFloat("u_lightLinearAttenuation", light.getLinearAttenuation());
  _glyphShader->setUniformFloat("u_lightQuadraticAttenuation", light.getQuadraticAttenuation());
  _glyphShader->setUniformColor("u_lightModelAmbientColor", light.getModelAmbientColor());
  _glyphShader->setUniformColor("u_lightAmbientColor", light.getAmbientColor());
  _glyphShader->setUniformColor("u_lightDiffuseColor", light.getDiffuseColor());
  _glyphShader->setUniformColor("u_lightSpecularColor", light.getSpecularColor());
  _glyphShader->setUniformColor("u_materialAmbientColor", Color(0, 0, 0));
  _glyphShader->setUniformColor("u_materialSpecularColor", Color(0, 0, 0));
  _glyphShader->setUniformFloat("u_materialShininess", 16);

  _glyphShader->setUniformTextureSampler("u_textures[0]", 0);
  _glyphShader->setUniformTextureSampler("u_textures[1]", 1);
  _glyphShader->setUniformTextureSampler("u_textures[2]", 2);
  _glyphShader->setUniformTextureSampler("u_textures[3]", 3);

  _glyphShader->setUniformBool("u_billboarding", _billboardMode);
}

const unsigned int glyphsAttributesDataStride = 19;

void GlyphsRenderer::renderGlyphsHardwareInstancing(int glyphId, const std::vector<tlp::Coord> &centers, const std::vector<tlp::Size> &sizes,
                                                    const std::vector<tlp::Color> &colors, const std::vector<std::string> &textures,
                                                    const std::vector<float> &borderWidths, const std::vector<tlp::Color> &borderColors,
                                                    const std::vector<Vec4f> &rotationData, bool forceFlatShading, bool swapYZ) {

  prepareGlyphDataHardwareInstancing(glyphId);
  Glyph *glyph = GlyphsManager::instance()->getGlyph(glyphId);

  unsigned int nbIndices = glyph->getGlyphVerticesIndices().size();
  unsigned int nbOutlines = glyph->getGlyphOutlineIndices().size();

  _glyphsDataBuffer[glyph]->bind();
  _glyphShader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float), BUFFER_OFFSET(0));
  unsigned int offset = 3;
  if (!glyph->getGlyphTexCoords().empty()) {
    _glyphShader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float),
                                         BUFFER_OFFSET(offset * sizeof(float)));
    offset += 2;
  }
  if (!glyph->getGlyphNormals().empty()) {
    _glyphShader->setVertexAttribPointer("a_normal", 3, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float),
                                         BUFFER_OFFSET(offset * sizeof(float)));
  }

  std::map<float, std::vector<unsigned int>> glyphsBorderWidth;
  for (size_t i = 0; i < centers.size(); ++i) {
    if (borderWidths.empty()) {
      glyphsBorderWidth[0].push_back(i);
    } else {
      glyphsBorderWidth[borderWidths[i]].push_back(i);
    }
  }

  std::map<float, std::vector<unsigned int>>::iterator it = glyphsBorderWidth.begin();

  for (; it != glyphsBorderWidth.end(); ++it) {

    unsigned int idx = 0;
    vector<float> outlineColorsData;
    vector<float> glyphsData;
    glyphsData.reserve(it->second.size() * glyphsAttributesDataStride);
    outlineColorsData.reserve(it->second.size() * 4);

    for (size_t i = 0; i < it->second.size(); ++i) {
      idx = it->second[i];

      addTlpVecToVecFloat(centers[idx], glyphsData);
      addTlpVecToVecFloat(sizes[idx], glyphsData);
      if (rotationData.empty()) {
        addTlpVecToVecFloat(Vec4f(0.0f, 0.0f, 1.0f, 0.0f), glyphsData);
      } else {
        addTlpVecToVecFloat(rotationData[idx], glyphsData);
      }
      addColorToVecFloat(colors[idx], glyphsData);
      if (borderColors.empty()) {
        addColorToVecFloat(Color::Black, outlineColorsData);
      } else {
        addColorToVecFloat(borderColors[idx], outlineColorsData);
      }

      if (!textures.empty() && !textures[idx].empty()) {
        glyphsData.push_back(GlTextureManager::instance()->getSamplerIdForTexture(textures[idx], true));
        addTlpVecToVecFloat(GlTextureManager::instance()->getCoordinatesOffsetsForTexture(textures[idx], true), glyphsData);
      } else {
        glyphsData.push_back(-1);
        addTlpVecToVecFloat(Vec4f(), glyphsData);
      }
    }

    _glyphsInstanceAttributesDataBuffer->bind();
    _glyphsInstanceAttributesDataBuffer->allocate(glyphsData);

    _glyphShader->setUniformBool("u_flatShading", glyph->glyph2D() || forceFlatShading);
    _glyphShader->setUniformBool("u_swapYZ", swapYZ);
    _glyphShader->setUniformBool("u_textureActivated", true);
    _glyphShader->setVertexAttribPointer("a_center", 3, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float), BUFFER_OFFSET(0), 1);
    _glyphShader->setVertexAttribPointer("a_scale", 3, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float),
                                         BUFFER_OFFSET(3 * sizeof(float)), 1);
    _glyphShader->setVertexAttribPointer("a_rotation", 4, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float),
                                         BUFFER_OFFSET(6 * sizeof(float)), 1);
    _glyphShader->setVertexAttribPointer("a_color", 4, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float),
                                         BUFFER_OFFSET(10 * sizeof(float)), 1);
    _glyphShader->setVertexAttribPointer("a_textureUnit", 1, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float),
                                         BUFFER_OFFSET(14 * sizeof(float)), 1);
    _glyphShader->setVertexAttribPointer("a_texCoordOffsets", 4, GL_FLOAT, GL_FALSE, glyphsAttributesDataStride * sizeof(float),
                                         BUFFER_OFFSET(15 * sizeof(float)), 1);

    _glyphsIndicesBuffer[glyph]->bind();
    if (nbIndices != 0) {
      glDrawElementsInstancedARB(GL_TRIANGLES, nbIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), it->second.size());
    }

    if (nbOutlines != 0 && (it->first > 0 || glyphId == tlp::NodeShape::CubeOutlinedTransparent)) {

      _glyphShader->setUniformBool("u_flatShading", true);
      _glyphShader->setUniformBool("u_textureActivated", false);
      for (size_t i = 0; i < it->second.size(); ++i) {
        _glyphsInstanceAttributesDataBuffer->write((i * glyphsAttributesDataStride + 10) * sizeof(float), 4 * sizeof(float),
                                                   &outlineColorsData[4 * i]);
      }
      if (it->first > 0) {
        glLineWidth(it->first);
      } else {
        glLineWidth(2.0f);
      }
      for (unsigned int i = 0; i < nbOutlines; ++i) {
        glDrawElementsInstancedARB(GL_LINE_STRIP, _glyphsOutlinesNbIndices[glyph][i], GL_UNSIGNED_SHORT,
                                   BUFFER_OFFSET(_glyphsOutlinesIndicesOffset[glyph][i]), it->second.size());
      }
    }
  }

  _glyphsIndicesBuffer[glyph]->release();
  _glyphsDataBuffer[glyph]->release();
  _glyphShader->disableAttributesArrays();
}

void GlyphsRenderer::renderGlyphsPseudoInstancing(int glyphId, const std::vector<tlp::Coord> &centers, const std::vector<tlp::Size> &sizes,
                                                  const std::vector<tlp::Color> &colors, const std::vector<std::string> &textures,
                                                  const std::vector<float> &borderWidths, const std::vector<tlp::Color> &borderColors,
                                                  const std::vector<Vec4f> &rotationData, bool forceFlatShading, bool swapYZ) {

  GLenum indicesType = GL_UNSIGNED_SHORT;
  if (_canUseUIntIndices) {
    indicesType = GL_UNSIGNED_INT;
  }

  prepareGlyphDataPseudoInstancing(glyphId);
  Glyph *glyph = GlyphsManager::instance()->getGlyph(glyphId);

  unsigned int nbIndices = glyph->getGlyphVerticesIndices().size();
  unsigned int nbOutlines = glyph->getGlyphOutlineIndices().size();
  unsigned nbInstancesByRenderingBatch = _maxGlyphInstanceByRenderingBatch[glyph];

  unsigned int indicesStride = 0;
  for (unsigned int i = 0; i < nbOutlines; ++i) {
    indicesStride += _glyphsOutlinesNbIndices[glyph][i];
  }

  _glyphsDataBuffer[glyph]->bind();
  _glyphsIndicesBuffer[glyph]->bind();
  _glyphShader->setVertexAttribPointer("a_position", 4, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float), BUFFER_OFFSET(0));
  unsigned int offset = 4;
  if (!glyph->getGlyphTexCoords().empty()) {
    _glyphShader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float),
                                         BUFFER_OFFSET(offset * sizeof(float)));
    offset += 2;
  }
  if (!glyph->getGlyphNormals().empty()) {
    _glyphShader->setVertexAttribPointer("a_normal", 3, GL_FLOAT, GL_FALSE, _glyphsDataStride[glyph] * sizeof(float),
                                         BUFFER_OFFSET(offset * sizeof(float)));
  }

  std::map<float, std::vector<unsigned int>> glyphsBorderWidth;
  for (size_t i = 0; i < centers.size(); ++i) {
    if (borderWidths.empty()) {
      glyphsBorderWidth[0].push_back(i);
    } else {
      glyphsBorderWidth[borderWidths[i]].push_back(i);
    }
  }

  std::map<float, std::vector<unsigned int>>::iterator it = glyphsBorderWidth.begin();

  for (; it != glyphsBorderWidth.end(); ++it) {

    unsigned int idx = 0;
    unsigned int cpt = 0;
    vector<float> centersData;
    vector<float> scalesData;
    vector<float> rotationAnglesData;
    vector<float> colorsData;
    vector<float> outlineColorsData;
    vector<float> textureUnitsData;
    vector<float> texCoordsOffsetData;

    centersData.reserve(it->second.size() * 3);
    scalesData.reserve(it->second.size() * 3);
    rotationAnglesData.reserve(it->second.size() * 4);
    colorsData.reserve(it->second.size() * 4);
    outlineColorsData.reserve(it->second.size() * 4);
    textureUnitsData.reserve(it->second.size());
    texCoordsOffsetData.reserve(it->second.size() * 4);

    for (size_t i = 0; i < it->second.size(); ++i) {

      idx = it->second[i];

      addTlpVecToVecFloat(centers[idx], centersData);
      addTlpVecToVecFloat(sizes[idx], scalesData);
      if (rotationData.empty()) {
        addTlpVecToVecFloat(Vec4f(0.0f, 0.0f, 0.0f, 1.0f), rotationAnglesData);
      } else {
        addTlpVecToVecFloat(rotationData[idx], rotationAnglesData);
      }
      addColorToVecFloat(colors[idx], colorsData);
      if (borderColors.empty()) {
        addColorToVecFloat(Color::Black, outlineColorsData);
      } else {
        addColorToVecFloat(borderColors[idx], outlineColorsData);
      }

      if (!textures.empty() && !textures[idx].empty()) {
        addTlpVecToVecFloat(GlTextureManager::instance()->getCoordinatesOffsetsForTexture(textures[idx], true), texCoordsOffsetData);
        textureUnitsData.push_back(GlTextureManager::instance()->getSamplerIdForTexture(textures[idx], true));
      } else {
        addTlpVecToVecFloat(Vec4f(), texCoordsOffsetData);
        textureUnitsData.push_back(-1);
      }
      ++cpt;
      if (cpt == nbInstancesByRenderingBatch) {
        _glyphShader->setUniformBool("u_flatShading", glyph->glyph2D() || forceFlatShading);
        _glyphShader->setUniformBool("u_swapYZ", swapYZ);
        _glyphShader->setUniformBool("u_textureActivated", true);
        _glyphShader->setUniformVec3FloatArray("u_center", nbInstancesByRenderingBatch, &centersData[0]);
        _glyphShader->setUniformVec4FloatArray("u_rotation", nbInstancesByRenderingBatch, &rotationAnglesData[0]);
        _glyphShader->setUniformVec3FloatArray("u_scale", nbInstancesByRenderingBatch, &scalesData[0]);
        _glyphShader->setUniformVec4FloatArray("u_color", nbInstancesByRenderingBatch, &colorsData[0]);
        _glyphShader->setUniformFloatArray("u_textureUnit", nbInstancesByRenderingBatch, &textureUnitsData[0]);
        _glyphShader->setUniformVec4FloatArray("u_texCoordOffsets", nbInstancesByRenderingBatch, &texCoordsOffsetData[0]);

        if (nbIndices != 0) {
          glDrawElements(GL_TRIANGLES, cpt * nbIndices, indicesType, BUFFER_OFFSET(0));
        }

        if (nbOutlines != 0 && (it->first > 0 || glyphId == tlp::NodeShape::CubeOutlinedTransparent)) {
          _glyphShader->setUniformBool("u_flatShading", true);
          _glyphShader->setUniformBool("u_textureActivated", false);
          _glyphShader->setUniformVec4FloatArray("u_color", nbInstancesByRenderingBatch, &outlineColorsData[0]);
          if (it->first > 0) {
            glLineWidth(it->first);
          } else {
            glLineWidth(2.0f);
          }

          for (unsigned int i = 0; i < nbOutlines; ++i) {
            for (unsigned int j = 0; j < cpt; ++j) {
              if (_canUseUIntIndices) {
                glDrawElements(GL_LINE_STRIP, _glyphsOutlinesNbIndices[glyph][i], indicesType,
                               BUFFER_OFFSET(_glyphsOutlinesIndicesOffset[glyph][i] + j * indicesStride * sizeof(unsigned int)));
              } else {
                glDrawElements(GL_LINE_STRIP, _glyphsOutlinesNbIndices[glyph][i], indicesType,
                               BUFFER_OFFSET(_glyphsOutlinesIndicesOffset[glyph][i] + j * indicesStride * sizeof(unsigned short)));
              }
            }
          }
        }

        cpt = 0;
        centersData.clear();
        scalesData.clear();
        rotationAnglesData.clear();
        colorsData.clear();
        outlineColorsData.clear();
        textureUnitsData.clear();
        texCoordsOffsetData.clear();
      }
    }
    if (cpt > 0) {
      _glyphShader->setUniformBool("u_flatShading", glyph->glyph2D() || forceFlatShading);
      _glyphShader->setUniformBool("u_swapYZ", swapYZ);
      _glyphShader->setUniformBool("u_textureActivated", true);
      _glyphShader->setUniformVec3FloatArray("u_center", cpt, &centersData[0]);
      _glyphShader->setUniformVec3FloatArray("u_scale", cpt, &scalesData[0]);
      _glyphShader->setUniformVec4FloatArray("u_rotation", cpt, &rotationAnglesData[0]);
      _glyphShader->setUniformVec4FloatArray("u_color", cpt, &colorsData[0]);
      _glyphShader->setUniformFloatArray("u_textureUnit", cpt, &textureUnitsData[0]);
      _glyphShader->setUniformVec4FloatArray("u_texCoordOffsets", cpt, &texCoordsOffsetData[0]);

      if (nbIndices != 0) {
        glDrawElements(GL_TRIANGLES, cpt * nbIndices, indicesType, BUFFER_OFFSET(0));
      }

      if (nbOutlines != 0 && (it->first > 0 || glyphId == tlp::NodeShape::CubeOutlinedTransparent)) {
        _glyphShader->setUniformBool("u_flatShading", true);
        _glyphShader->setUniformBool("u_textureActivated", false);
        _glyphShader->setUniformVec4FloatArray("u_color", cpt, &outlineColorsData[0]);
        if (it->first > 0) {
          glLineWidth(it->first);
        } else {
          glLineWidth(2.0f);
        }

        for (unsigned int i = 0; i < nbOutlines; ++i) {
          for (unsigned int j = 0; j < cpt; ++j) {
            if (_canUseUIntIndices) {
              glDrawElements(GL_LINE_STRIP, _glyphsOutlinesNbIndices[glyph][i], indicesType,
                             BUFFER_OFFSET(_glyphsOutlinesIndicesOffset[glyph][i] + j * indicesStride * sizeof(unsigned int)));
            } else {
              glDrawElements(GL_LINE_STRIP, _glyphsOutlinesNbIndices[glyph][i], indicesType,
                             BUFFER_OFFSET(_glyphsOutlinesIndicesOffset[glyph][i] + j * indicesStride * sizeof(unsigned short)));
            }
          }
        }
      }
    }
  }

  _glyphsDataBuffer[glyph]->release();
  _glyphsIndicesBuffer[glyph]->release();

  _glyphShader->disableAttributesArrays();
}
