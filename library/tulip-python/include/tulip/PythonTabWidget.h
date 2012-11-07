#ifndef PYTHONTABWIDGET_H
#define PYTHONTABWIDGET_H

#include <QtGui/QTabWidget>

#include <tulip/tulipconf.h>

class TLP_PYTHON_SCOPE PythonTabWidget : public QTabWidget {

public :

    explicit PythonTabWidget(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent * event);

};

#endif // PYTHONTABWIDGET_H
