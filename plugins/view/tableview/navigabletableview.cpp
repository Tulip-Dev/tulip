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
#include "navigabletableview.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QHeaderView>

#include <iostream>

NavigableTableView::NavigableTableView(QWidget *parent) :
    QTableView(parent) , _sendSignalOnResize(true) {
    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
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
    int left = 0;
    int right = model()->columnCount();
    int hint = 0;

    for (int column = left; column < right; ++column) {

        if (horizontalHeader()->isSectionHidden(column))
            continue;

        QModelIndex index = model()->index(row, column);
        hint = qMax(hint, itemDelegate(index)->sizeHint(viewOptions(), index).height());
    }
    return hint;
}

void NavigableTableView::scrollContentsBy (int dx, int dy) {
    QTableView::scrollContentsBy(dx, dy);
    emit resizeTableRows();
}

void NavigableTableView::resizeEvent(QResizeEvent * event) {
    QTableView::resizeEvent(event);
    if (_sendSignalOnResize) {
        emit resizeTableRows();
    }
}
