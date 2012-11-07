#ifndef PYTHONTABWIDGET_H
#define PYTHONTABWIDGET_H

#include <QtGui/QTabWidget>

class PythonTabWidget : public QTabWidget {

public :

    explicit PythonTabWidget(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent * event);

};

#endif // PYTHONTABWIDGET_H
