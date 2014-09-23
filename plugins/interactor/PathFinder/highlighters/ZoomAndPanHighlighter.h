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

#ifndef ZOOMANDPANHIGHLIGHTER_H_
#define ZOOMANDPANHIGHLIGHTER_H_

#include "PathHighlighter.h"

#include <QObject>

namespace tlp {

class ZoomAndPanHighlighter: public PathHighlighter, public QObject {
public:
  inline ZoomAndPanHighlighter(): PathHighlighter("Zoom and pan") {}
  void highlight(const PathFinder *parent, tlp::GlMainWidget *glMainWidget, tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt);
  void draw(tlp::GlMainWidget *glMainWidget);
  bool isConfigurable() const;
  QWidget *getConfigurationWidget();
};
}
#endif /* ZOOMANDPANHIGHLIGHTER_H_ */
