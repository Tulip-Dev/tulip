#ifndef _INTERACTOR_H
#define _INTERACTOR_H

#include <QtCore/QObject>
#include <QtGui/QAction>

#include <tulip/TulipRelease.h>
#include <tulip/MethodFactory.h>
#include <tulip/AbstractPluginInfo.h>
#include <tulip/PluginLister.h>
#include <tulip/InteractorManager.h>
#include <tulip/AbstractPluginInfo.h>

namespace tlp {

class View;

class TLP_QT_SCOPE Interactor: public QObject, public tlp::WithParameter, public tlp::WithDependency {
  Q_OBJECT
  Q_PROPERTY(unsigned int priority READ priority WRITE setPriority)
  Q_PROPERTY(QAction* action READ action)
  Q_PROPERTY(tlp::View* view READ view WRITE setView)
  Q_PROPERTY(QCursor cursor READ cursor)

public:
  virtual bool isCompatible(const std::string& viewName)=0;
  virtual QWidget* configurationWidget() const=0;

  virtual unsigned int priority() const=0;
  virtual QAction* action() const=0;
  virtual tlp::View* view() const=0;
  virtual QCursor cursor() const=0;
  virtual void construct()=0;

public slots:
  virtual void setPriority(unsigned int)=0;
  virtual void setView(tlp::View*)=0;
  virtual void install(QObject*)=0;
  virtual void uninstall()=0;
  virtual void undoIsDone()=0;

protected:
  inline virtual bool eventFilter(QObject* obj, QEvent* ev) { return QObject::eventFilter(obj,ev); }
};

class TLP_QT_SCOPE InteractorContext {
};

typedef StaticPluginLister<Interactor, InteractorContext*> InteractorLister;

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<Interactor,InteractorContext*>;
#endif

}

#define INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,G) POINTERCONTEXTPLUGINFACTORY(Interactor,C,N,A,D,I,R,G)
#define INTERACTORPLUGIN(C,N,A,D,I,R) INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,"")

//#define INTERACTORPLUGINVIEWEXTENSION(CN,CNT,BCNT,VCN,A,D,I,R)
//#define INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(CN,CNT,BCNT,VCN,A,D,I,R,P)

#endif
