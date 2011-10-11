#ifndef VIEW_H
#define VIEW_H

#include <tulip/tulipconf.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Observable.h>
#include <tulip/PluginLister.h>

#include <QtCore/QObject>
#include <QtCore/QList>

class QGraphicsItem;
class QWidget;

namespace tlp {

class Interactor;

class TLP_QT_SCOPE View: public QObject, public tlp::WithDependency, public tlp::WithParameter, public tlp::Observable {
public:
  View();
  virtual ~View();

  virtual QGraphicsItem* graphicsItem() const=0;

  virtual void setInteractors(const QList<tlp::Interactor*>&);
  virtual QList<tlp::Interactor*> interactors() const;
  virtual void setActiveInteractor(tlp::Interactor*);
  virtual tlp::Interactor* activeInteractor() const;

  virtual void setData(const tlp::DataSet&)=0;
  virtual tlp::DataSet data() const=0;
  virtual void setGraph(tlp::Graph*)=0;
  virtual tlp::Graph* graph() const=0;

  void registerObservableData(tlp::Observable*);
  void removeObservableData(tlp::Observable*);

  QList<QWidget*> configurationWidgets() const;

  virtual void draw()=0;
  virtual void refresh() {
    draw();
  }
};

struct TLP_QT_SCOPE ViewContext {
};

typedef StaticPluginLister<View, ViewContext*> ViewLister;

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<View,ViewContext *>;
#endif
}

#define VIEWPLUGINOFGROUP(C,N,A,D,I,R,G) POINTERCONTEXTPLUGINFACTORY(View,C,N,A,D,I,R,G)
#define VIEWPLUGIN(C,N,A,D,I,R) VIEWPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif /* VIEW_H_ */
