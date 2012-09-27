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
///@cond DOXYGEN_HIDDEN

#ifndef _Tulip_MOUSESHOWELEMENTINFOS_H
#define _Tulip_MOUSESHOWELEMENTINFOS_H

#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTableView>
#include <QtGui/QGraphicsProxyWidget>
#include <tulip/ViewWidget.h>
#include <tulip/GlScene.h>

#include <tulip/InteractorComposite.h>

namespace tlp {

class View;
/**
 * We define a specific interactor to show element graph infos in eltProperties
 */
class TLP_QT_SCOPE MouseShowElementInfos : public InteractorComponent {
public:
  MouseShowElementInfos();
  virtual bool eventFilter(QObject* widget, QEvent* e);

  void viewChanged(View *);

  void clear();

  virtual bool pick(int x, int y, SelectedEntity &selectedEntity);

protected:

  ViewWidget *_view;
  QWidget *_informationsWidget;
  QGraphicsProxyWidget *_informationsWidgetItem;

  QTableView* tableView() const;
};

}



#endif
///@endcond