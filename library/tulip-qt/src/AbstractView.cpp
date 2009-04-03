#include "tulip/AbstractView.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QFrame>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMenu>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>

#include "tulip/Interactor.h"

using namespace std;

namespace tlp {

  AbstractView::AbstractView() :View(),centralWidget(NULL),activeInteractor(NULL)  {

  }

  AbstractView::~AbstractView() {
    for(list<Interactor *>::iterator it=interactors.begin();it!=interactors.end();++it){
      delete (*it);
    }
  }

  QWidget *AbstractView::construct(QWidget *parent) {
  	widget=new QWidget(parent);
  	QGridLayout *gridLayout = new QGridLayout(widget);
  	gridLayout->setSpacing(0);
  	gridLayout->setMargin(0);

  	mainLayout=new QVBoxLayout;

  	gridLayout->addLayout(mainLayout, 0, 0, 1, 1);

  	// Add this to by-pass a bug in Qt 4.4.1
  	// In the QWorkspace if the widget doesn't have a QGLWidget this widget pass below others widget
  	QFrame *frame = new QFrame(widget);
  	frame->setGeometry(QRect(0, 0, 0, 0));
  	QGridLayout *gridLayout_2 = new QGridLayout(frame);
  	QWidget *widget_2 = new QGLWidget(frame);

  	widget->installEventFilter(this);

  	return widget;
  }

  void AbstractView::setInteractors(const std::list<Interactor *> &interactorsList){
    interactors=interactorsList;
    for(list<Interactor *>::iterator it=interactors.begin();it!=interactors.end();++it){
      (*it)->setView(this);
    }
  }

  list<Interactor *> AbstractView::getInteractors() {
    return interactors;
  }

  void AbstractView::setActiveInteractor(Interactor *interactor){
    if(activeInteractor)
      activeInteractor->remove();
    interactor->install(centralWidget);
    activeInteractor=interactor;
  }

  void AbstractView::setCentralWidget(QWidget *widget) {
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addWidget(widget);
    centralWidget=widget;
  }

  bool AbstractView::eventFilter(QObject *object, QEvent *event) {
    specificEventFilter(object,event);

    if(event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *me = (QMouseEvent *) event;
      if(me->button() ==Qt::RightButton) {
        QMenu contextMenu(getWidget());
        buildContextMenu(object,me,&contextMenu);
        if(!contextMenu.actions().isEmpty()) {
          QAction* menuAction=contextMenu.exec(me->globalPos());
          if(menuAction)
            computeContextMenuAction(menuAction);
        }
      }
    }
    return false;
  }

}
