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

class TLP_GL_SCOPE EdgeExtremityGlyph: public Plugin {
public:
  virtual std::string category() const {
    return QObject::trUtf8("Edge extremities").toStdString();
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_glyph.png";
  }

  EdgeExtremityGlyph(const tlp::PluginContext *context);
  virtual ~EdgeExtremityGlyph();
  virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) = 0;
  void get2DTransformationMatrix(const Coord &src, const Coord &dest,const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);
  void get3DTransformationMatrix(const Coord &src, const Coord &dest,const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);
protected:
  GlGraphInputData *edgeExtGlGraphInputData;
};

}

#endif /* EDGEEXTREMITYGLYPH_H_ */
