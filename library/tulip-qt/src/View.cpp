#include "tulip/View.h"

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QFrame>
#include <QtOpenGL/QGLWidget>

using namespace std;

namespace tlp {

  TemplateFactory<ViewCreatorFactory,ViewCreator, ViewCreatorContext *> *ViewCreatorFactory::factory;  

  View::View(const std::string &pluginName,QWidget *parent) :pluginName(pluginName),QWidget(parent),centralWidget(NULL) {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    mainLayout=new QVBoxLayout;

    gridLayout->addLayout(mainLayout, 0, 0, 1, 1);

    // Add this to by-pass a bug in Qt 4.4.1
    // In the QWorkspace if the widget doesn't have a QGLWidget this widget pass below others widget
    QFrame *frame = new QFrame(this);
    frame->setGeometry(QRect(0, 0, 0, 0));
    QGridLayout *gridLayout_2 = new QGridLayout(frame);
    QWidget *widget_2 = new QGLWidget(frame);

    installEventFilter(this);
  }

  void View::setCentralWidget(QWidget *widget) {
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addWidget(widget);
    centralWidget=widget;
  }

}
