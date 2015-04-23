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
#include "navigabletableview.h"

#include <QKeyEvent>
#include <QHeaderView>

#include <iostream>

NavigableTableView::NavigableTableView(QWidget *parent) :
  QTableView(parent) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
  horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
}

void NavigableTableView::keyPressEvent(QKeyEvent *event) {
  if(event->key() == Qt::Key_Home) {
    scrollToTop();
  }
  else if(event->key() == Qt::Key_End) {
    scrollToBottom();
  }
  else {
    QTableView::keyPressEvent(event);
  }
}

int NavigableTableView::sizeHintForRow(int row) const {
  if (!model())
    return -1;

  ensurePolished();
  int left = qMax(0, horizontalHeader()->visualIndexAt(0));
  int right = horizontalHeader()->visualIndexAt(viewport()->width());

  if (right < 0) right = model()->columnCount();

  int hint = 0;

  for (int column = left; column <= right; ++column) {

    if (horizontalHeader()->isSectionHidden(column))
      continue;

    QModelIndex index = model()->index(row, column);
    hint = qMax(hint, itemDelegate(index)->sizeHint(viewOptions(), index).height());
  }

  return hint;
}

int NavigableTableView::sizeHintForColumn(int col) const {
  if (!model())
    return -1;

  ensurePolished();
  int top = qMax(0, verticalHeader()->visualIndexAt(0));
  int bottom = verticalHeader()->visualIndexAt(viewport()->height());

  int hint = 0;

  if (bottom == -1 || (bottom+10) >= model()->rowCount()) {
    bottom = model()->rowCount() - 1;
  }
  else {
    bottom += 10;
  }

  for (int row = top; row <= bottom; ++row) {
    QModelIndex index = model()->index(row, col);
    hint = qMax(hint, itemDelegate(index)->sizeHint(viewOptions(), index).width());
  }

  return hint;
}

void NavigableTableView::paintEvent(QPaintEvent * event) {
  resizeTableRows();
  QTableView::paintEvent(event);
}

void NavigableTableView::resizeTableRows() {

  if (!model())
    return;

  int top = qMax(0, verticalHeader()->visualIndexAt(0));
  int bottom = verticalHeader()->visualIndexAt(viewport()->height());

  if (bottom == -1 || (bottom+10) >= model()->rowCount()) {
    bottom = model()->rowCount() - 1;
  }
  else {
    bottom += 10;
  }

  int left = qMax(0, horizontalHeader()->visualIndexAt(0));
  int right = horizontalHeader()->visualIndexAt(viewport()->width());

  if (right < 0) right = model()->columnCount();

  for (int i = top ; i <= bottom ; ++i)
    resizeRowToContents(i);

  for (int i = left ; i <= right ; ++i)
    resizeColumnToContents(i);

}
