#include "tulip/AbstractView.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QFrame>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMenu>

using namespace std;

namespace tlp {

  AbstractView::AbstractView(const std::string &pluginName,QWidget *parent) :View(pluginName,parent),_id(Interactor::invalidID)  {

    installEventFilter(this);
  }

  AbstractView::~AbstractView() {
    for (unsigned int i = 0; i > _interactors.size(); ++i)
      delete _interactors[i];
  }

  bool AbstractView::eventFilter(QObject *object, QEvent *event) {
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
  //==================================================
  Iterator<Interactor *> *AbstractView::getInteractors() const {
    return new StlIterator<Interactor *, vector<Interactor *>::const_iterator>(_interactors.begin(), _interactors.end());
  }
  //==================================================
  Interactor::ID AbstractView::pushInteractor(Interactor* interactor) {
    if (interactor) {
      interactor = interactor->clone();
      interactor->setID(++_id);
      _interactors.push_back(interactor);
      centralWidget->installEventFilter(interactor);
      //interactor->compute(centralWidget);
      //updateGL();
    }
    return _id;
  }
  //==================================================
  void AbstractView::popInteractor() {
    if (_interactors.size()) {
      Interactor *interactor = _interactors[_interactors.size() - 1];
      _interactors.pop_back();
      centralWidget->removeEventFilter(interactor);
      delete interactor;
    }
  }
  //==================================================
  void AbstractView::removeInteractor(Interactor::ID i) {
    for(vector<Interactor *>::iterator it =
	  _interactors.begin(); it != _interactors.end(); ++it) {
      if ((*it)->getID() == i) {
	removeEventFilter(*it);
	delete *it;
	_interactors.erase(it);
	break;
      }
    }
  }
  //==================================================
  Interactor::ID AbstractView::resetInteractors(Interactor *interactor) {
    for(vector<Interactor *>::iterator it =
	  _interactors.begin(); it != _interactors.end(); ++it) {
      removeEventFilter(*it);
      delete *it;
    }
    _interactors.clear();
    return pushInteractor(interactor);
  }
  //==================================================
  std::vector<tlp::Interactor::ID> AbstractView::resetInteractors(const std::vector<Interactor *> &new_interactors) {
    for(vector<Interactor *>::iterator it =
	  _interactors.begin(); it != _interactors.end(); ++it) {
      removeEventFilter(*it);
      delete *it;
    }
    _interactors.clear();
    vector<Interactor::ID> ids;
    for (vector<Interactor *>::const_iterator it =
	   new_interactors.begin(); it != new_interactors.end(); ++it)
      ids.push_back(pushInteractor(*it));
    return ids;
  }

}
