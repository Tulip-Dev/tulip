#ifndef VIEW_H
#define VIEW_H

#include <tulip/tulipconf.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Observable.h>
#include <tulip/PluginLister.h>

#include <QtCore/QObject>
#include <QtCore/QSet>

class QGraphicsView;
class QWidget;

namespace tlp {
class Interactor;

class TLP_QT_SCOPE View: public QObject, public tlp::WithDependency, public tlp::WithParameter, public tlp::Observable {
  Q_OBJECT

  QList<tlp::Interactor*> _interactors;
  tlp::Interactor* _currentInteractor;
  tlp::Graph* _graph;

public:
  View();
  virtual ~View();

  virtual QGraphicsView* graphicsView() const=0;

  QList<tlp::Interactor*> interactors() const;
  tlp::Interactor* currentInteractor() const;

  virtual QList<QWidget*> configurationWidgets() const;

  virtual tlp::DataSet state() const=0;
  tlp::Graph* graph() const;

public slots:
  void setInteractors(const QList<tlp::Interactor*>&);
  void setCurrentInteractor(tlp::Interactor*);

  virtual void setState(const tlp::DataSet&)=0;
  void setGraph(tlp::Graph*);

  virtual void draw()=0;
  inline virtual void refresh() {
    draw();
  }

  virtual void setupUi()=0;

signals:
  void drawNeeded();

protected slots:
  virtual void interactorsInstalled(const QList<tlp::Interactor*>&)=0;
  virtual void currentInteractorChanged(tlp::Interactor*);
  virtual void graphChanged(tlp::Graph*)=0;
};

struct TLP_QT_SCOPE ViewContext {
};

typedef StaticPluginLister<View, ViewContext*> ViewLister;

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<View,ViewContext*>;
#endif
}

#define VIEWPLUGINOFGROUP(C,N,A,D,I,R,G) POINTERCONTEXTPLUGINFACTORY(View,C,N,A,D,I,R,G)
#define VIEWPLUGIN(C,N,A,D,I,R) VIEWPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif /* VIEW_H_ */
