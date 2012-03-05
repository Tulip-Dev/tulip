/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef EDGEEXTREMITYGLYPH_H_
#define EDGEEXTREMITYGLYPH_H_

#include <tulip/TulipRelease.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Matrix.h>
#include <tulip/Plugin.h>
#include <tulip/PluginLister.h>
#include <tulip/Glyph.h>

namespace tlp {
typedef Matrix<float, 4> MatrixGL;
class GlGraphInputData;

class EdgeExtremityGlyphContext : public tlp::PluginContext {
public:
  GlGraphInputData *glGraphInputData;
  ///
  EdgeExtremityGlyphContext(Graph ** = NULL, GlGraphInputData *glGraphInputData = NULL, int =
                              5, int = 5) :
    glGraphInputData(glGraphInputData) {
  }
  ///
  ~EdgeExtremityGlyphContext() {
  }
};

class TLP_GL_SCOPE EdgeExtremityGlyph: public tlp::Plugin {
public:
  EdgeExtremityGlyph(tlp::PluginContext *gc);
  virtual ~EdgeExtremityGlyph();

  virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) = 0;

  virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
                                       const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) = 0;

protected:
  GlGraphInputData *edgeExtGlGraphInputData;
};

class TLP_GL_SCOPE EdgeExtremityGlyphFrom3DGlyph: public EdgeExtremityGlyph {
public:
  EdgeExtremityGlyphFrom3DGlyph(tlp::PluginContext *gc);
  virtual ~EdgeExtremityGlyphFrom3DGlyph();

  virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
                                       const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);
};

class TLP_GL_SCOPE EdgeExtremityGlyphFrom2DGlyph: public EdgeExtremityGlyph {
public:
  EdgeExtremityGlyphFrom2DGlyph(tlp::PluginContext *gc);
  virtual ~EdgeExtremityGlyphFrom2DGlyph();

  virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
                                       const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);
};

}

#endif /* EDGEEXTREMITYGLYPH_H_ */
