#include "navigabletableview.h"

#include <QtGui/QKeyEvent>

NavigableTableView::NavigableTableView(QWidget *parent) :
    QTableView(parent)
{
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
