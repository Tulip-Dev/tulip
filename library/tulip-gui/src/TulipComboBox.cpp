/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QMenu>

#include "tulip/TulipComboBox.h"

using namespace tlp;

void TulipComboBox::showPopup() {
  QMenu menu;
  QAction *action;

  for (int i = 0; i < count(); ++i) {
    action = menu.addAction(itemText(i));
    if (i == currentIndex())
      menu.setActiveAction(action);
  }

  QComboBox cb;
  QPalette palette = cb.palette();
  QFont f = cb.font();

  // set a combo like stylesheet
  menu.setStyleSheet(QString("QMenu { font: %1pt; }"
                             "QMenu::item {border-image: none; border-width: 4; padding: 0px 6px;"
                             "color: %2; background-color: %3;} "
                             "QMenu::item:selected {color: %4; background-color: %5}")
                         .arg(f.pointSize() - 1)
                         .arg(palette.color(QPalette::Active, QPalette::Text).name())
                         .arg(palette.color(QPalette::Active, QPalette::Base).name())
                         .arg(palette.color(QPalette::Active, QPalette::HighlightedText).name())
                         .arg(palette.color(QPalette::Active, QPalette::Highlight).name()));
  // compute a combo like position
  // to popup the menu
  QWidget *pViewport = QApplication::widgetAt(QCursor::pos());
  QWidget *pView = pViewport->parentWidget();
  QGraphicsView *pGraphicsView = static_cast<QGraphicsView *>(pView);
  QGraphicsItem *pGraphicsItem =
      pGraphicsView->items(pViewport->mapFromGlobal(QCursor::pos())).first();
  QPoint popupPos =
      pGraphicsView->mapToGlobal(pGraphicsView->mapFromScene(pGraphicsItem->mapToScene(
          static_cast<QGraphicsProxyWidget *>(pGraphicsItem)->subWidgetRect(this).bottomLeft())));

  action = menu.exec(popupPos);

  if (action)
    setCurrentText(action->text());
}
