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

#ifndef GOOGLEMAPSSHOWELEMENTINFO_H
#define GOOGLEMAPSSHOWELEMENTINFO_H

#include <tulip/InteractorComposite.h>

#include "GoogleMapsView.h"

class QTableView;

namespace tlp {

class View;
class GlComplexPolygonItemEditor;

/**
 * We define a specific interactor to show element graph infos in eltProperties
 */
class GoogleMapsShowElementInfo : public InteractorComponent {
public:
  GoogleMapsShowElementInfo();
  virtual bool eventFilter(QObject* widget, QEvent* e);

  void viewChanged(View *);

  void clear();

  virtual bool pick(int x, int y, SelectedEntity &selectedEntity);

protected:

  GoogleMapsView *_view;
  QWidget *_informationsWidget;
  QGraphicsProxyWidget *_informationsWidgetItem;
  GlComplexPolygonItemEditor *_editor;

  QTableView* tableView() const;
};

}

#endif // GOOGLEMAPSSHOWELEMENTINFO_H
