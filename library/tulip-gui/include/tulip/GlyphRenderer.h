#ifndef GLYPHPREVIEWGENERATOR_H
#define GLYPHPREVIEWGENERATOR_H
#include <map>
#include <QtGui/QPixmap>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp {
class Graph;


/**
  * @brief Generate Qt previews for Glyphs plug-ins.
  **/
class TLP_QT_SCOPE GlyphRenderer {
public:
  static GlyphRenderer & getInst();
  ~GlyphRenderer();
  /**
    * @brief Get the preview for the glyph with the given Id.
    */
  QPixmap render(unsigned int pluginId);

private:
  GlyphRenderer();
  static GlyphRenderer* _instance;
  std::map<unsigned int,QPixmap> _previews;
  tlp::Graph* _graph;
  tlp::node _node;
};

/**
  * @brief Generate Qt previews for edge extremities glyphs plug-ins.
  **/
class TLP_QT_SCOPE EdgeExtremityGlyphRenderer {
public:

  ~EdgeExtremityGlyphRenderer();
  static EdgeExtremityGlyphRenderer & getInst();
  /**
    * @brief Get the preview for the edge extremity glyph with the given Id.
    */
  QPixmap render(unsigned int pluginId);

private:
  EdgeExtremityGlyphRenderer();
  static EdgeExtremityGlyphRenderer* _instance;
  std::map<unsigned int,QPixmap> _previews;
  tlp::Graph* _graph;
  tlp::edge _edge;

};
}
#endif // GLYPHPREVIEWGENERATOR_H
