/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef GLYPHRENDERER_H
#define GLYPHRENDERER_H

#include <QPixmap>
#include <QString>

#include <tulip/tulipconf.h>

#define GLYPH_PREVIEW_DIM 16

namespace tlp {

/**
 * @brief Generate Qt previews for Glyphs plug-ins.
 **/
class TLP_QT_SCOPE GlyphRenderer {
public:
  /**
   * @brief Get the preview for the glyph with the given Id.
   */
  static QPixmap render(int glyphId, int dim = GLYPH_PREVIEW_DIM);
  static QPixmap render(QString glyphName, int dim = GLYPH_PREVIEW_DIM);
};

/**
 * @brief Generate Qt previews for edge extremities glyphs plug-ins.
 **/
class TLP_QT_SCOPE EdgeExtremityGlyphRenderer {
public:
  /**
   * @brief Get the preview for the edge extremity glyph with the given Id.
   */
  static QPixmap render(int glyphId, int dim = GLYPH_PREVIEW_DIM);
  static QPixmap render(QString glyphName, int dim = GLYPH_PREVIEW_DIM);
};
} // namespace tlp
#endif // GLYPHRENDERER_H
///@endcond
