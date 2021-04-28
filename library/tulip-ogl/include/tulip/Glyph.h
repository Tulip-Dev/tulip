/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef GLYPH_H
#define GLYPH_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/Coord.h>
#include <tulip/Plugin.h>
#include <tulip/PluginContext.h>
#include <tulip/Size.h>

namespace tlp {

static const std::string GLYPH_CATEGORY = "Node shape";

struct BoundingBox;
class Graph;
struct node;
class GlGraphInputData;
class GlRect;

class GlyphContext : public PluginContext {
public:
  GlGraphInputData *glGraphInputData;
  ///
  GlyphContext(Graph ** = nullptr, GlGraphInputData *glGraphInputData = nullptr,
               int = 5, int = 5)
      : glGraphInputData(glGraphInputData) {}
  ///
  ~GlyphContext() override {}
};
//==========================================================
class TLP_GL_SCOPE Glyph : public tlp::Plugin {
public:
  std::string category() const override { return GLYPH_CATEGORY; }
  std::string icon() const override {
    return ":/tulip/gui/icons/32/plugin_glyph.png";
  }

  Glyph(const tlp::PluginContext *context);
  ~Glyph() override;

  virtual std::string getGroup() const { return ""; }

  virtual void getIncludeBoundingBox(BoundingBox &boundingBox, node);

  virtual void getTextBoundingBox(BoundingBox &boundingBox, node n);

  virtual void draw(node, float) = 0;
  /*
   * return a point where an edge coming from "from" can be attached
   * by default, the point will be on the surface of the largest sphere
   * contained inside the unit cube (before scaling).
   */
  virtual Coord getAnchor(const Coord &nodeCenter, const Coord &from,
                          const Size &scale, const double zRotation) const;

  /**
   * Return if the Glyph render its label (return true) or if GlNode have to
   * render label (return false)
   */
  virtual bool renderLabel() { return false; }

  /**
   * Return if the Glyph supports shader rendering optimization (see GlNode.cpp)
   */
  virtual bool shaderSupported() const { return true; }

  /**
   * draw a preconfigured GlRect in the screen plane
   */
  static void drawRectInScreenPlane(GlRect &rect, const Size &size,
                                    bool disableMasks);

  GlGraphInputData *glGraphInputData;

protected:
  /*
   * called by public method getAnchor to actually compute the anchor point
   * vector is coordinate of the point to anchor to, relative to nodecenter
   * glyph size is (1,1,1)
   * this is the method to redefine for each glyph where the default 'getAnchor'
   * method is inapropriated Returned value is a vector to be applied to
   * 'nodeCenter' in the public method
   */
  virtual Coord getAnchor(const Coord &vector) const;
};
//==========================================================
class TLP_GL_SCOPE NoShaderGlyph : public Glyph {
public:
  NoShaderGlyph(const tlp::PluginContext *context = nullptr) : Glyph(context) {}
  bool shaderSupported() const override { return false; }
};
} // namespace tlp

#define GLYPHINFORMATION(NAME, AUTHOR, DATE, INFO, RELEASE, ID)                \
  PLUGININFORMATION(NAME, AUTHOR, DATE, INFO, RELEASE, "")                     \
  int id() const override { return ID; }

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // GLYPH_H
///@endcond
