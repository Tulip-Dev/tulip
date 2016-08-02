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
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/ShaderManager.h>
#include <tulip/GlShaderProgram.h>

using namespace std;
using namespace tlp;

static string shaderSrcPrefix =
#ifdef __EMSCRIPTEN__
  "precision highp float;\n"
  "precision highp int;\n"
#else
  "#version 120\n"
#endif
  ;

static string defaultVertexShaderSrc = shaderSrcPrefix + R"(
  uniform mat4 u_modelviewMatrix;
  uniform mat4 u_projectionMatrix;
  uniform vec4 u_color;
  uniform bool u_globalColor;
  uniform bool u_globalTexture;
  uniform bool u_textureActivated;
  uniform bool u_pointsRendering;
  uniform bool u_globalPointSize;
  uniform float u_pointSize;
  uniform vec4 u_texCoordOffsets;

  attribute vec3 a_position;
  attribute vec4 a_color;
  attribute vec2 a_texCoord;
  attribute float a_pointSize;
  attribute vec4 a_texCoordOffsets;

  varying vec4 v_color;
  varying vec2 v_texCoord;
  varying vec4 v_texCoordOffsets;

  void main() {
    gl_Position = u_projectionMatrix * u_modelviewMatrix * vec4(a_position, 1.0);
    if (u_globalColor) {
      v_color = u_color;
    } else {
      v_color = a_color;
    }
    if (u_textureActivated) {
      v_texCoord = a_texCoord;
      if (u_globalTexture) {
        v_texCoordOffsets = u_texCoordOffsets;
      } else {
        v_texCoordOffsets = a_texCoordOffsets;
      }
    }

    if (u_pointsRendering) {
      if (u_globalPointSize) {
        gl_PointSize = u_pointSize;
      } else {
        gl_PointSize = a_pointSize;
      }
    }
  }
)";


// original shader code can be found in the webgl-lines project (https://github.com/mattdesl/webgl-lines)
// from Matt DesLauriers (MIT License)
static string lineRenderingVertexShaderSrc = shaderSrcPrefix + R"(
  uniform mat4 u_modelviewMatrix;
  uniform mat4 u_projectionMatrix;
  uniform bool u_textureActivated;
  uniform vec4 u_texCoordOffsets;
  uniform float u_aspect;
  uniform float u_thickness;
  uniform bool u_miter;
  uniform vec3 u_lookDir;

  attribute vec3 a_position;
  attribute float a_direction;
  attribute vec3 a_next;
  attribute vec3 a_previous;
  attribute vec4 a_color;
  attribute vec2 a_texCoord;

  varying vec4 v_color;
  varying vec2 v_texCoord;
  varying vec4 v_texCoordOffsets;

  void main() {
    vec2 aspectVec = vec2(u_aspect, 1.0);
    mat4 projViewModel = u_projectionMatrix * u_modelviewMatrix;
    vec4 previousProjected = projViewModel * vec4(a_previous, 1.0);
    vec4 currentProjected = projViewModel * vec4(a_position, 1.0);
    vec4 nextProjected = projViewModel * vec4(a_next, 1.0);

    // get 2D screen space with W divide and aspect correction
    vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
    vec2 previousScreen = previousProjected.xy / previousProjected.w * aspectVec;
    vec2 nextScreen = nextProjected.xy / nextProjected.w * aspectVec;

    float len = u_thickness;
    float orientation = a_direction;

    // starting point uses (next - current)
    vec2 dir = vec2(0.0);
    if (currentScreen == previousScreen) {
      dir = normalize(nextScreen - currentScreen);
    }
    // ending point uses (current - previous)
    else if (currentScreen == nextScreen) {
      dir = normalize(currentScreen - previousScreen);
    }
    // somewhere in middle, needs a join
    else {
      // get directions from (C - B) and (B - A)
      vec2 dirA = normalize(currentScreen - previousScreen);
      if (u_miter) {
        vec2 dirB = normalize(nextScreen - currentScreen);
        // now compute the miter join normal and length
        vec2 tangent = normalize(dirA + dirB);
        vec2 perp = vec2(-dirA.y, dirA.x);
        vec2 miter = vec2(-tangent.y, tangent.x);
        dir = tangent;
        len = len / dot(miter, perp);
        if (len > 50.0 * u_thickness) {
          len = u_thickness;
        }
      } else {
        dir = dirA;
      }
    }
    vec2 normal = vec2(-dir.y, dir.x);
    normal *= len/2.0;
    normal.x /= u_aspect;

    vec4 offset = vec4(normal * orientation, 0.0, 0.0);
    gl_Position = (currentProjected + offset);
    gl_Position.w = 1.0;
    v_color = a_color;
    if (u_textureActivated) {
      v_texCoord = a_texCoord;
      v_texCoordOffsets = u_texCoordOffsets;
    }
  }
)";

static string defaultFragmentShaderSrc = shaderSrcPrefix + R"(
  uniform bool u_textureActivated;
  uniform sampler2D u_texture;

  varying vec4 v_color;
  varying vec2 v_texCoord;
  varying vec4 v_texCoordOffsets;

  void main() {
    if (!u_textureActivated) {
      gl_FragColor = v_color;
    } else {
      vec2 texCoord = vec2(v_texCoordOffsets.x + v_texCoord.x * (v_texCoordOffsets.z - v_texCoordOffsets.x),
                           v_texCoordOffsets.y + v_texCoord.y * (v_texCoordOffsets.w - v_texCoordOffsets.y));
      gl_FragColor = v_color * texture2D(u_texture, texCoord);
    }
  }
)";

static string blinnPhongVertexShaderSrc = shaderSrcPrefix + R"(
  uniform mat4 u_modelviewMatrix;
  uniform mat4 u_projectionMatrix;
  uniform mat4 u_normalMatrix;

  uniform bool u_globalColor;
  uniform bool u_textureActivated;

  uniform vec3 u_eyePosition;
  uniform vec4 u_lightPosition;
  uniform float u_lightConstantAttenuation;
  uniform float u_lightLinearAttenuation;
  uniform float u_lightQuadraticAttenuation;
  uniform vec4 u_lightModelAmbientColor;
  uniform vec4 u_lightAmbientColor;
  uniform vec4 u_lightDiffuseColor;

  uniform vec4 u_materialAmbientColor;
  uniform vec4 u_materialDiffuseColor;

  attribute vec3 a_position;
  attribute vec4 a_color;
  attribute vec2 a_texCoord;
  attribute vec3 a_normal;

  varying vec4 v_diffuseColor;
  varying vec4 v_ambientGlobalColor;
  varying vec4 v_ambientColor;

  varying vec3 v_normal;
  varying vec3 v_lightDir;
  varying vec3 v_halfVector;
  varying float v_attenuation;

  varying vec2 v_texCoord;

  void main(){
    v_normal = normalize(mat3(u_normalMatrix) * a_normal);
    vec4 pos = u_modelviewMatrix * vec4(a_position, 1.0);
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

    if (u_globalColor) {
      v_diffuseColor = u_lightDiffuseColor * u_materialDiffuseColor;
    } else {
      v_diffuseColor = u_lightDiffuseColor * a_color;
    }

    v_ambientColor = u_lightAmbientColor * u_materialAmbientColor;
    v_ambientGlobalColor = u_lightModelAmbientColor * u_materialAmbientColor;

    gl_Position = u_projectionMatrix * pos;

    if (u_textureActivated) {
      v_texCoord = a_texCoord;
    }
  }
)";

static string blinnPhongFragmentShaderSrc = shaderSrcPrefix + R"(
  uniform bool u_flatShading;
  uniform bool u_textureActivated;
  uniform sampler2D u_texture;

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

  varying vec2 v_texCoord;

  void main() {
    if (u_flatShading) {
       gl_FragColor = v_diffuseColor;
       if (u_textureActivated) {
         gl_FragColor *= texture2D(u_texture, v_texCoord);
       }
       return;
    }
    vec3 normal = normalize(v_normal);
    vec4 color = v_ambientGlobalColor + v_ambientColor;
    float NdotL = max(dot(normal,normalize(v_lightDir)),0.0);
    if (NdotL > 0.0) {
      color += v_attenuation * v_diffuseColor * NdotL;
      vec3 halfV = normalize(v_halfVector);
      float NdotHV = max(dot(normal, halfV), 0.0);
      color += v_attenuation * u_materialSpecularColor * u_lightSpecularColor * pow(NdotHV, u_materialShininess);
    }

    if (u_textureActivated) {
      color *= texture2D(u_texture, v_texCoord);
    }

    gl_FragColor = color;
  }
)";

/**

https://github.com/mattdesl/glsl-fxaa
The MIT License (MIT) Copyright (c) 2014 Matt DesLauriers

Basic FXAA implementation based on the code on geeks3d.com with the
modification that the texture2DLod stuff was removed since it's
unsupported by WebGL.
--
From:
https://github.com/mitsuhiko/webgl-meincraft
Copyright (c) 2011 by Armin Ronacher.
Some rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * The names of the contributors may not be used to endorse or
      promote products derived from this software without specific
      prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
static string fxaaFunctionsSrc = R"(
  #ifndef FXAA_REDUCE_MIN
      #define FXAA_REDUCE_MIN   (1.0/ 128.0)
  #endif
  #ifndef FXAA_REDUCE_MUL
      #define FXAA_REDUCE_MUL   (1.0 / 8.0)
  #endif
  #ifndef FXAA_SPAN_MAX
      #define FXAA_SPAN_MAX     8.0
  #endif

  //optimized version for mobile, where dependent
  //texture reads can be a bottleneck
  vec4 fxaa(sampler2D tex, vec2 fragCoord, vec2 resolution,
              vec2 v_rgbNW, vec2 v_rgbNE,
              vec2 v_rgbSW, vec2 v_rgbSE,
              vec2 v_rgbM) {
      vec4 color;
      vec2 inverseVP = vec2(1.0 / resolution.x, 1.0 / resolution.y);
      vec3 rgbNW = texture2D(tex, v_rgbNW).xyz;
      vec3 rgbNE = texture2D(tex, v_rgbNE).xyz;
      vec3 rgbSW = texture2D(tex, v_rgbSW).xyz;
      vec3 rgbSE = texture2D(tex, v_rgbSE).xyz;
      vec4 texColor = texture2D(tex, v_rgbM);
      vec3 rgbM  = texColor.xyz;
      vec3 luma = vec3(0.299, 0.587, 0.114);
      float lumaNW = dot(rgbNW, luma);
      float lumaNE = dot(rgbNE, luma);
      float lumaSW = dot(rgbSW, luma);
      float lumaSE = dot(rgbSE, luma);
      float lumaM  = dot(rgbM,  luma);
      float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
      float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

      vec2 dir;
      dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
      dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

      float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                            (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

      float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
      dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
                max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                dir * rcpDirMin)) * inverseVP;

      vec3 rgbA = 0.5 * (
          texture2D(tex, fragCoord * inverseVP + dir * (1.0 / 3.0 - 0.5)).xyz +
          texture2D(tex, fragCoord * inverseVP + dir * (2.0 / 3.0 - 0.5)).xyz);
      vec3 rgbB = rgbA * 0.5 + 0.25 * (
          texture2D(tex, fragCoord * inverseVP + dir * -0.5).xyz +
          texture2D(tex, fragCoord * inverseVP + dir * 0.5).xyz);

      float lumaB = dot(rgbB, luma);
      if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
          color = vec4(rgbA, texColor.a);
      } else {
          color = vec4(rgbB, texColor.a);
      }
      return color;
  }

  void texcoords(vec2 fragCoord, vec2 resolution,
        out vec2 v_rgbNW, out vec2 v_rgbNE,
        out vec2 v_rgbSW, out vec2 v_rgbSE,
        out vec2 v_rgbM) {
    vec2 inverseVP = 1.0 / resolution.xy;
    v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
    v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
    v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
    v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
    v_rgbM = vec2(fragCoord * inverseVP);
  }

  vec4 applyFXAA(sampler2D tex, vec2 fragCoord, vec2 resolution) {
    vec2 v_rgbNW;
    vec2 v_rgbNE;
    vec2 v_rgbSW;
    vec2 v_rgbSE;
    vec2 v_rgbM;

  //compute the texture coords
    texcoords(fragCoord, resolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);

  //compute FXAA
    return fxaa(tex, fragCoord, resolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
  }
)";


map<string, ShaderManager *> ShaderManager::_instances;
string ShaderManager::_currentCanvasId("");


ShaderManager *ShaderManager::getInstance(const string &canvasId) {
  if (_instances.find(canvasId) == _instances.end()) {
    _instances[canvasId] = new ShaderManager();
  }
  return _instances[canvasId];
}

ShaderManager *ShaderManager::getInstance() {
  return getInstance(_currentCanvasId);
}

ShaderManager::ShaderManager() {
  _flatRenderingShader = new GlShaderProgram();
  _flatRenderingShader->addShaderFromSourceCode(GlShader::Vertex, defaultVertexShaderSrc);
  _flatRenderingShader->addShaderFromSourceCode(GlShader::Fragment, defaultFragmentShaderSrc);
  _flatRenderingShader->link();
  if (!_flatRenderingShader->isLinked()) {
    _flatRenderingShader->printInfoLog();
  }

  _blinnPhongRenderingShader = new GlShaderProgram();
  _blinnPhongRenderingShader->addShaderFromSourceCode(GlShader::Vertex, blinnPhongVertexShaderSrc);
  _blinnPhongRenderingShader->addShaderFromSourceCode(GlShader::Fragment, blinnPhongFragmentShaderSrc);
  _blinnPhongRenderingShader->link();
  if (!_blinnPhongRenderingShader->isLinked()) {
    _blinnPhongRenderingShader->printInfoLog();
  }

  _lineRenderingShader = new GlShaderProgram();
  _lineRenderingShader->addShaderFromSourceCode(GlShader::Vertex, lineRenderingVertexShaderSrc);
  _lineRenderingShader->addShaderFromSourceCode(GlShader::Fragment, defaultFragmentShaderSrc);
  _lineRenderingShader->link();
  if (!_lineRenderingShader->isLinked()) {
    _lineRenderingShader->printInfoLog();
  }

}

string ShaderManager::getFXAAFunctionsSource() {
  return fxaaFunctionsSrc;
}

string ShaderManager::getShaderSrcPrefix() {
  return shaderSrcPrefix;
}
