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


#ifndef GLYPHPREVIEWGENERATOR_H
#define GLYPHPREVIEWGENERATOR_H

#include <map>

#include <QPixmap>

#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp {
class Graph;

/**
  * @brief Generate Qt previews for Glyphs plug-ins.
  **/
class TLP_QT_SCOPE GlyphPreviewRenderer {
public:
  static GlyphPreviewRenderer& instance();

  ~GlyphPreviewRenderer();

  /**
    * @brief Get the preview for the glyph with the given Id.
    */
  QPixmap render(unsigned int pluginId);

private:
  GlyphPreviewRenderer();
  static GlyphPreviewRenderer _instance;
  std::map<unsigned int,QPixmap> _previews;
  tlp::Graph* _graph;
  tlp::node _node;
};

/**
  * @brief Generate Qt previews for edge extremities glyphs plug-ins.
  **/
class TLP_QT_SCOPE EdgeExtremityGlyphPreviewRenderer {
public:

  static EdgeExtremityGlyphPreviewRenderer& instance();

  ~EdgeExtremityGlyphPreviewRenderer();

  /**
    * @brief Get the preview for the edge extremity glyph with the given Id.
    */
  QPixmap render(unsigned int pluginId);

private:
  EdgeExtremityGlyphPreviewRenderer();
  static EdgeExtremityGlyphPreviewRenderer _instance;
  std::map<unsigned int,QPixmap> _previews;
  tlp::Graph* _graph;
  tlp::edge _edge;

};
}
#endif // GLYPHPREVIEWGENERATOR_H
///@endcond
