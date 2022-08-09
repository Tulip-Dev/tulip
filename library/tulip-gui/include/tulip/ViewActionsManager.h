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

#ifndef VIEW_ACTIONS_MANAGER_H
#define VIEW_ACTIONS_MANAGER_H

#include <QMenu>
#include <QAction>

#include <tulip/View.h>
#include <tulip/DataSet.h>
#include <tulip/GlMainWidget.h>

namespace tlp {
class TLP_QT_SCOPE ViewActionsManager : public QObject {
  Q_OBJECT

  View *_view;
  GlMainWidget *_glMainWidget;
  bool _keepSizeRatio;
  QAction *_centerViewAction;
  QAction *_forceRedrawAction;
  QAction *_advAntiAliasingAction;
  QAction *_snapshotAction;

public:
  ViewActionsManager(View *view, GlMainWidget *widget, bool keepRatio);

  // the function below must be called by the associated view
  // when overloading the View class corresponding method
  void fillContextMenu(QMenu *menu);

protected slots:
  void centerView();
  void redraw();
  void openSnapshotDialog();
  void setAntiAliasing(bool);
};
} // namespace tlp

#endif /* VIEW_ACTIONS_MANAGER_H */

///@endcond
