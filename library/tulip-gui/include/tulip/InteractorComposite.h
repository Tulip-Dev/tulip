#ifndef INTERACTORCOMPOSITE_H
#define INTERACTORCOMPOSITE_H

#include <tulip/Interactor.h>
#include <QtCore/QList>

namespace tlp {

class TLP_QT_SCOPE InteractorComponent: public QObject {
  Q_OBJECT

  tlp::View* _view;

public:
  virtual void init();
  virtual bool eventFilter(QObject*, QEvent*);

  void setView(tlp::View* view);
  tlp::View* view() const;
};

class InteractorComposite : public tlp::Interactor {
  Q_OBJECT
  unsigned int _priority;
  QAction* _action;
  tlp::View* _view;
  QObject* _lastTarget;

protected:
  QList<tlp::InteractorComponent*> _components;
  void setLastTarget(QObject*);
  QObject* lastTarget() const;

public:
  typedef QList<InteractorComponent*>::iterator iterator;
  typedef QList<InteractorComponent*>::const_iterator const_iterator;

  InteractorComposite(const QIcon& icon, const QString& text="");
  virtual ~InteractorComposite();

  virtual bool isCompatible(const std::string& viewName)=0;
  virtual QWidget* configurationWidget() const=0;

  virtual unsigned int priority() const;
  virtual tlp::View* view() const;

  virtual QAction* action() const;

  virtual QCursor cursor() const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  void push_back(InteractorComponent*);
  void push_front(InteractorComponent*);

public slots:
  virtual void construct()=0;
  virtual void undoIsDone();

  virtual void setPriority(unsigned int);
  virtual void setView(tlp::View*);

  virtual void install(QObject*);
  virtual void uninstall();
};

}
#endif // INTERACTORCOMPOSITE_H
