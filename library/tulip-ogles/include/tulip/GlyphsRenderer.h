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

#ifndef GLYPHSRENDERER_H
#define GLYPHSRENDERER_H

#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/TulipViewSettings.h>

#include <vector>

namespace tlp {

class GlShaderProgram;
class Glyph;
class GlBuffer;
class Camera;
class Light;

class TLP_GLES_SCOPE GlyphsRenderer {

public:
  static GlyphsRenderer *instance();

  static GlyphsRenderer *instance(const std::string &canvasId);

  static void setCurrentCanvasId(const std::string &canvasId) {
    _currentCanvasId = canvasId;
  }

  void setBillboardMode(bool billboardMode) {
    _billboardMode = billboardMode;
  }

  void
  renderGlyphs(const Camera &camera, const Light &light, int glyphId, const std::vector<tlp::Coord> &centers, const std::vector<tlp::Size> &sizes,
               const std::vector<tlp::Color> &colors, const std::vector<std::string> &textures = std::vector<std::string>(),
               const std::vector<float> &borderWidths = std::vector<float>(), const std::vector<tlp::Color> &borderColors = std::vector<tlp::Color>(),
               const std::vector<tlp::Vec4f> &rotationAxisAndAngles = std::vector<tlp::Vec4f>(), bool forceFlatShading = false, bool swapYZ = false);

  void renderGlyph(const Camera &camera, const Light &light, int glyphId, const tlp::Coord &center, const tlp::Size &size, const tlp::Color &color,
                   const std::string &texture = "", const float &borderWidth = 0, const tlp::Color &borderColor = tlp::Color(),
                   const tlp::Vec4f &rotationAxisAndAngle = tlp::Vec4f(0.f, 0.f, 1.f, 0.f), bool forceFlatShading = false, bool swapYZ = false);

private:
  GlyphsRenderer();

  void prepareGlyphDataPseudoInstancing(int glyphId);
  void prepareGlyphDataHardwareInstancing(int glyphId);

  void setupGlyphsShader(const Camera &camera, const Light &light);

  void renderGlyphsHardwareInstancing(int glyphId, const std::vector<tlp::Coord> &centers, const std::vector<tlp::Size> &sizes,
                                      const std::vector<tlp::Color> &colors, const std::vector<std::string> &textures,
                                      const std::vector<float> &borderWidths, const std::vector<tlp::Color> &borderColors,
                                      const std::vector<tlp::Vec4f> &rotationAngles, bool forceFlatShading, bool swapYZ);

  void renderGlyphsPseudoInstancing(int glyphId, const std::vector<tlp::Coord> &centers, const std::vector<tlp::Size> &sizes,
                                    const std::vector<tlp::Color> &colors, const std::vector<std::string> &textures,
                                    const std::vector<float> &borderWidths, const std::vector<tlp::Color> &borderColors,
                                    const std::vector<tlp::Vec4f> &rotationAngles, bool forceFlatShading, bool swapYZ);

  bool _canUseUIntIndices;
  bool _canUseHardwareInstancing;

  GlShaderProgram *_glyphShader;

  std::map<Glyph *, GlBuffer *> _glyphsDataBuffer;
  std::map<Glyph *, GlBuffer *> _glyphsIndicesBuffer;

  GlBuffer *_glyphsInstanceAttributesDataBuffer;

  std::map<Glyph *, unsigned int> _glyphsDataStride;
  std::map<Glyph *, std::vector<unsigned int>> _glyphsOutlinesNbIndices;
  std::map<Glyph *, std::vector<unsigned int>> _glyphsOutlinesIndicesOffset;
  std::map<Glyph *, unsigned int> _maxGlyphInstanceByRenderingBatch;

  bool _billboardMode;

  static std::map<std::string, GlyphsRenderer *> _instances;
  static std::string _currentCanvasId;
};
}

#endif // GLYPHSRENDERER_H
