#include "tulip/View.h"

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>

using namespace std;

namespace tlp {

  TemplateFactory<ViewCreatorFactory,ViewCreator, ViewCreatorContext *> *ViewCreatorFactory::factory;  

  View::View(const std::string &pluginName,QWidget *parent) :pluginName(pluginName),QWidget(parent) {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    mainLayout=new QVBoxLayout;

    gridLayout->addLayout(mainLayout, 0, 0, 1, 1);

    installEventFilter(this);
  }

  void View::setCentralWidget(QWidget *widget) {
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addWidget(widget);
  }

  bool View::eventFilter(QObject *object, QEvent *event) {
    specificEventFilter(object,event);

    if(event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *me = (QMouseEvent *) event;
      if(me->button() ==Qt::RightButton) {
	QMenu contextMenu(this);
	buildContextMenu(object,me,&contextMenu);
	if(!contextMenu.actions().isEmpty()) {
	  QAction* menuAction=contextMenu.exec(me->globalPos());
	  if(menuAction)
	    computeContextMenuAction(menuAction);
	}
      }
    }
  }

}
