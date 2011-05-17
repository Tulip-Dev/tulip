#ifndef GLYPHPREVIEWGENERATOR_H
#define GLYPHPREVIEWGENERATOR_H
#include <map>
#include <QtGui/QPixmap>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp{
    class Graph;
}

/**
  * @brief Handle previews for Glyphs plugins.
  **/
class GlyphPreviewGenerator
{
public:
    static GlyphPreviewGenerator & getInst();
    ~GlyphPreviewGenerator();
    /**
      * @brief Get the preview for the glyph with the given Id.
      */
    QPixmap getPreview(unsigned int pluginId);

private:
    GlyphPreviewGenerator();
    static GlyphPreviewGenerator* _instance;
    std::map<unsigned int,QPixmap> _previews;
    tlp::Graph* _graph;
    tlp::node _node;
};

/**
  * @brief Handle previews for Edge extremities glyphs plugins.
  **/
class EdgeExtremityGlyphPreviewGenerator
{
public:

    ~EdgeExtremityGlyphPreviewGenerator();
    static EdgeExtremityGlyphPreviewGenerator & getInst();
    /**
      * @brief Get the preview for the edge extremity glyph with the given Id.
      */
    QPixmap getPreview(unsigned int pluginId);

private:
    EdgeExtremityGlyphPreviewGenerator();
    static EdgeExtremityGlyphPreviewGenerator* _instance;
    std::map<unsigned int,QPixmap> _previews;
    tlp::Graph* _graph;
    tlp::edge _edge;

};

#endif // GLYPHPREVIEWGENERATOR_H
