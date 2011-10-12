#ifndef VIEW_H
#define VIEW_H

#include <tulip/tulipconf.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Observable.h>
#include <tulip/PluginLister.h>

#include <QtCore/QObject>
#include <QtCore/QSet>

class QGraphicsItem;
class QWidget;

namespace tlp {
class Interactor;

/**
  @class View
  @brief The View class provides a panel to display a specific kind of vizualisation over a graph.

  View subclass are registered into the Tulip plugin system as View plugins. A View plugin contains the following elements:
  @li A graph (tlp::Graph*) representing the model that should be displayed by the view's panel.
  @li Some initialization data (tlp::DataSet) that can be saved and restored once the view is opened back.

  A view's panel (where graphical representation is drawn) is a subclass of QGraphicsItem. @see ViewWidget to build views using QWidget.
  @note Views are meant to be managed by some upperleying system. @see tlp::TulipWorkspace for a standard implementation and instructions on how to build your own View-controller class.

  User-input interactions on views panel are provided by tlp::Interactor subclasses. A View thus contains several interactors and one active interactor.
  The responsibility of managing interactors can be left to an upperlaying controller system (eg. tlp::TulipWorkspace) or directly to the View.
  The View::hasInteractorsResponsibility checks if Interactors should be drawn and managed by the View or not.

  Views can be set up using configuration widgets.
  Like interactors, configuration widgets handling can be done by the view or by the upperleying system.

  When subclassing a View object, the following methods will always be called in this order (those are only methods that can be subclassed):
  @li The View's constructor
  @li View::interactorsInstalled() : At this point, the View::hasInteractorsResponsibility() and View::hasConfigurationWidgetsResponsibility() methods can be called to check view's responsibilities
  @li View::activeInteractorChanged() : Only if one or more interactor has been previously installed.
  @li View::setGraph : Defines the graph to be displayed.
  @li View::setData : Defines startup context or previously saved data.

  Once View::setData() is called, the View should be able to be drawn. However, the View::draw() slot should never be called by the View itself. Instead, the View should emit the View::drawNeeded() signal and wait for the upperleying controller to call the View::draw() slot.
  */
class TLP_QT_SCOPE View: public QObject, public tlp::WithDependency, public tlp::WithParameter, public tlp::Observable {
  Q_OBJECT

  bool _hasInteractorsResponsibility;
  bool _hasConfigurationWidgetsResponsibility;

  QSet<tlp::Interactor*> _interactors;
  tlp::Interactor* _activeInteractor;

  QSet<tlp::Observable*> _triggers;

  tlp::Graph* _graph;

public:
  View();
  virtual ~View();

  /**
    @brief Returns the panel where drawing is done.
    The View does not have ownership on its panel. When closing a View, the panel will be destroyed before the View object is.
    */
  virtual QGraphicsItem* graphicsItem() const=0;

  /**
    @brief Installs a set of interactors on the View
    The list of interactors can then be retrieved using the View::interactors() method.
    After this method is called, the callback slot View::interactorsInstalled() is also called to provide custom implementation.
    */
  void setInteractors(const QSet<tlp::Interactor*>&);

  /**
    @return The list of installed interactors.
    */
  QSet<tlp::Interactor*> interactors() const;

  /**
    @brief Sets the active interactor.
    The active interactor is the one that should handle user inputs until another active interactor is set.
    The callback slot View::activeInteractorChanged() is called at this point to provide custom implementation.
    */
  void setActiveInteractor(tlp::Interactor*);

  /**
    @return The currently active interactor.
    */
  tlp::Interactor* activeInteractor() const;

  /**
    @brief Register a trigger for automatic redraw.
    As said in the class description, a View should never call the draw() method directly but emit the drawNeeded signal instead.
    This method is a convenient function that allows the user to register triggers (tlp::Observable subclasses).
    Once a trigger gets registered, the View will listen to it and emit the drawNeeded() signal as soon as the treatEvents() method is called.
    @see tlp::Observable for details on the Tulip observable mechanism.
    */
  void registerTrigger(tlp::Observable*);

  /**
    @brief Unregister a trigger
    @see registerTrigger();
    */
  void removeTrigger(tlp::Observable*);

  /**
    @return The list of currently registered triggers.
    */
  QList<tlp::Observable*> triggers() const;

  /**
    @return The list of configuration widgets.
    */
  inline virtual QList<QWidget*> configurationWidgets() const {
    return QList<QWidget*>();
  }

  /**
    @return the View's context data.
    This method can be called by an upperleying system to restore the View's internal data.
    */
  virtual tlp::DataSet data() const=0;

  /**
    @return The graph set on the View.
    @see setGraph()
    */
  tlp::Graph* graph() const;

public slots:
  /**
    @brief Sets the graph displayed by the View.
    @note the graphChanged callback method will be called for custom implementation
    */
  void setGraph(tlp::Graph*);

  /**
    @brief Set the context data.
    If the View previously gave data to the upperleying system (eg. On a previous instance of the application), this method could be used to restore View's state.
    */
  virtual void setData(const tlp::DataSet&)=0;

  /**
    @brief Draws the view's panel.
    A call to the draw() method means that data reprensented by the View has changed in some way and that the graphical representation should be updated accordingly.
    */
  virtual void draw()=0;

  /**
    @brief Refresh the view's panel
    A call to this method means that no data has changed but that the application context asks the View to redraw (panel is resized, restored, ...)
    */
  inline virtual void refresh() {
    draw();
  }

  /**
    @brief Called by the upperleying system to take the interactors management reponsibility
    */
  void takeInteractorsResponsibility() {
    _hasInteractorsResponsibility = false;
  }

  /**
    @brief Called by the upperleying system to take the configuration widgets management reponsibility
    */
  void takeConfigurationWidgetsReponsibility() {
    _hasConfigurationWidgetsResponsibility = false;
  }

signals:
  /**
    @brief Tells the upperleying system that a call to the draw method is needed.
    The upperleying system may then directly call the draw() method or wait until some events are previously processed.
    */
  void drawNeeded();

protected:
  /**
    @return true if the View should manage interactors (drawing, switching) or false if this reponsibility is left to the upperleying system.
    */
  bool hasInteractorsResponsibility() const {
    return _hasInteractorsResponsibility;
  }

  /**
    @return true if the View should manage configurationj widgets (drawing, switching) or false if this reponsibility is left to the upperleying system.
    */
  bool hasConfigurationWidgetsResponsibility() const {
    return _hasConfigurationWidgetsResponsibility;
  }

protected slots:
  /**
    @brief Custom user callback when a set of interactors has been installed.
    At this point, user can check the View::hasInteractorsResponsibility() and View::hasConfigurationWidgetsResponsibility() to start building GUI.
    */
  virtual void interactorsInstalled(const QSet<tlp::Interactor*>&);

  /**
    @brief Custom user callback when the active interactor has changed.
    @see Interactor::install()
    */
  virtual void activeInteractorChanged(tlp::Interactor*);

  /**
    @brief Custom user callback when a new graph is set on the view.
    From this point, the View::graph() method will return the newly set graph.
    */
  virtual void graphChanged(tlp::Graph*);
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
