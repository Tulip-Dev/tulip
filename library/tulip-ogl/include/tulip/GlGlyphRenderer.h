#ifndef GLGLYPHRENDERER_H
#define GLGLYPHRENDERER_H

#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

#include <vector>

namespace tlp {

class GlGraphInputData;
class GlShaderProgram;
class Glyph;
class EdgeExtremityGlyph;
class GlBox;

struct TLP_GL_SCOPE NodeGlyphData {

  NodeGlyphData() {}

  NodeGlyphData(Glyph *glyph, node n, float lod, const Coord &nodePos, const Size &nodeSize, float nodeRot, bool selected) :
    glyph(glyph), n(n), lod(lod), nodePos(nodePos), nodeSize(nodeSize), nodeRot(nodeRot), selected(selected) {}

  Glyph *glyph;
  node n;
  float lod;
  Coord nodePos;
  Size nodeSize;
  float nodeRot;
  bool selected;

};

struct TLP_GL_SCOPE EdgeExtremityGlyphData {

  EdgeExtremityGlyphData() {}

  EdgeExtremityGlyphData(EdgeExtremityGlyph *glyph, edge e, node source,
                         Color glyphColor, Color glyphBorderColor, float lod,
                         Coord beginAnchor, Coord srcAnchor, Size size, bool selected) :
    glyph(glyph), e(e), source(source), glyphColor(glyphColor), glyphBorderColor(glyphBorderColor),
    lod(lod), beginAnchor(beginAnchor), srcAnchor(srcAnchor), size(size), selected(selected) {}

  EdgeExtremityGlyph *glyph;
  edge e;
  node source;
  Color glyphColor;
  Color glyphBorderColor;
  float lod;
  Coord beginAnchor;
  Coord srcAnchor;
  Size size;
  bool selected;

};

class TLP_GL_SCOPE GlGlyphRenderer {

public:

  GlGlyphRenderer(GlGraphInputData *inputData) :
    _inputData(inputData), _renderingStarted(false) {}

  void startRendering();

  bool renderingHasStarted();

  void addNodeGlyphRendering(Glyph *glyph, node n, float lod, const Coord &nodePos,
                             const Size &nodeSize, float nodeRot, bool selected);

  void addEdgeExtremityGlyphRendering(EdgeExtremityGlyph *glyph, edge e, node source,
                                      Color glyphColor, Color glyphBorderColor, float lod,
                                      Coord beginAnchor, Coord srcAnchor, Size size, bool selected);

  void endRendering();

private:

  GlGraphInputData *_inputData;
  bool _renderingStarted;
  std::vector<NodeGlyphData> _nodeGlyphsToRender;
  std::vector<EdgeExtremityGlyphData> _edgeExtremityGlyphsToRender;
  static GlShaderProgram *_glyphShader;
  static GlBox* _selectionBox;

};

}

#endif // GLYPHRENDERER_H
