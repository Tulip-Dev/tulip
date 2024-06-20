/**
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

#ifndef GEOGRAPHIC_VIEW_SHOW_ELEMENT_INFO_H
#define GEOGRAPHIC_VIEW_SHOW_ELEMENT_INFO_H

#include <tulip/InteractorComposite.h>
#include <tulip/MouseShowElementInfo.h>

#include "GeographicView.h"

class QTableView;

namespace tlp {

class View;
class GlComplexPolygonItemEditor;

/**
 * We define a specific interactor to show element graph info in eltProperties
 */
class GeographicViewShowElementInfo : public MouseShowElementInfo {
  Q_OBJECT
public:
  GeographicViewShowElementInfo();

  void init() override;

  bool eventFilter(QObject *widget, QEvent *e) override;

  void viewChanged(View *) override;

  void clear() override;

  bool pick(int x, int y, SelectedEntity &selectedEntity) override;

public slots:
  void mouseMove();

protected:
  GeographicView *_view;
  GlComplexPolygonItemEditor *_editor;
};
} // namespace tlp

#endif // GEOGRAPHIC_VIEW_SHOW_ELEMENT_INFO_H
