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
#include <QGraphicsView>

#include <tulip/ViewActionsManager.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/SnapshotDialog.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

ViewActionsManager::ViewActionsManager(View *view, GlMainWidget *widget, bool keepRatio)
    : _view(view), _glMainWidget(widget), _keepSizeRatio(keepRatio),
      _advAntiAliasingAction(nullptr) {
  // create actions and add them to _view->graphicsView()
  // to enable their keyboard shortcut
  _forceRedrawAction = new QAction("Force redraw", widget);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_forceRedrawAction, "Redraw the current view", "Shift+R");
  connect(_forceRedrawAction, SIGNAL(triggered()), this, SLOT(redraw()));
  _forceRedrawAction->setShortcut(tr("Ctrl+Shift+R"));
  _forceRedrawAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _view->graphicsView()->addAction(_forceRedrawAction);

  _centerViewAction = new QAction("Center view", widget);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _centerViewAction, "Make the view to fully display and center its contents", "Shif+C");
  connect(_centerViewAction, SIGNAL(triggered()), this, SLOT(centerView()));
  _centerViewAction->setShortcut(tr("Ctrl+Shift+C"));
  _centerViewAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _view->graphicsView()->addAction(_centerViewAction);

  _snapshotAction = new QAction("Take a snapshot", widget);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _snapshotAction, "Show a dialog to save a snapshot of the current view display", "Shift+P");
  connect(_snapshotAction, SIGNAL(triggered()), this, SLOT(openSnapshotDialog()));
  _snapshotAction->setShortcut(tr("Ctrl+Shift+P"));
  _snapshotAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _view->graphicsView()->addAction(_snapshotAction);
}

void ViewActionsManager::centerView() {
  _view->centerView();
}

void ViewActionsManager::redraw() {
  _view->refresh();
}

void ViewActionsManager::openSnapshotDialog() {
  SnapshotDialog dlg(_view, _view->graphicsView()->window());
  dlg.setSnapshotHasViewSizeRatio(_keepSizeRatio);
  dlg.exec();
}

void ViewActionsManager::setAntiAliasing(bool aa) {
  OpenGlConfigManager::setAntiAliasing(aa);
  if (_advAntiAliasingAction) {
    _advAntiAliasingAction->setVisible(aa);
    if (_advAntiAliasingAction->isChecked())
      _advAntiAliasingAction->setChecked(false);
    else
      _view->draw();
  } else
    _view->draw();
}

void ViewActionsManager::fillContextMenu(QMenu *menu) {
  menu->addAction("View")->setEnabled(false);
  menu->addSeparator();
  menu->addAction(_forceRedrawAction);
  menu->addAction(_centerViewAction);

  QAction *action = menu->addAction("Anti-aliasing");
  action->setToolTip(QString("Improve rendering quality"));
  action->setCheckable(true);
  action->setChecked(OpenGlConfigManager::antiAliasing());
  connect(action, SIGNAL(triggered(bool)), this, SLOT(setAntiAliasing(bool)));

  if (_advAntiAliasingAction)
    menu->addAction(_advAntiAliasingAction);

  menu->addAction(_snapshotAction);
}
