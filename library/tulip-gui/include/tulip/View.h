#ifndef VIEW_H
#define VIEW_H

#include <tulip/tulipconf.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Observable.h>
#include <tulip/PluginLister.h>

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QPointF>
#include <QtCore/QSizeF>

class QGraphicsItem;
class QGraphicsLayoutItem;
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

  A View's object also provides methods to move/resize its graphics item. Those method are exposed in two Q_PROPERTY pos and size.
  Note that overloading those methods is strongly recommended since base beahvior could lead to poor-quality rendering.

  User-input interactions on views panel are provided by tlp::Interactor subclasses. A View thus contains several interactors and one active interactor.
  When changing active interactor, a callback function (View::activeInteractorChanged()) is called to providfe custom handling and interactor installation over the view

  When subclassing View, the following methods will always be called in this order (those are only methods that can be subclassed):
  @li View::interactorsInstalled()
  @li View::activeInteractorChanged() : Only if one or more interactor has been previously installed.
  @li View::setGraph : Defines the graph to be displayed.
  @li View::setState : Defines startup state or previously saved data.
  @li View::setupUi : Called when view should be completely initialized. Building UI can be made into this method.

  Once View::setupUi() is called, the View should be able to be drawn. However, the View::draw() slot should never be called by the View itself. Instead, the View should emit the View::drawNeeded() signal and wait for the upperleying controller to call the View::draw() slot.
  */
class TLP_QT_SCOPE View: public QObject, public tlp::WithDependency, public tlp::WithParameter, public tlp::Observable {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)
  Q_PROPERTY(QSizeF size READ size WRITE resize)

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
    This method should always return the same item. GUI building should be made into the View::setupUi() method.
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
  virtual tlp::DataSet state() const=0;

  /**
    @return The graph set on the View.
    @see setGraph()
    */
  tlp::Graph* graph() const;

  /**
    @return the position of the graphics item
    By default, this method returns graphicsItem()->pos()
    */
  virtual QPointF pos() const;

  /**
    @return the size of the graphics item
    By default, this method returns the graphicsItem's bounding rect, scaled to graphicsItem()->scale()
    */
  virtual QSizeF size() const;

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
  virtual void setState(const tlp::DataSet&)=0;

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
    @brief Setups the view's gui.
    Every method setting the view's context (setData, setGraph and so on) has already been called when this method is run.
    This method is only called once since View::graphicsItem should always return the same pointer.
    */
  virtual void setupUi()=0;

  /**
    @brief Set the item's position
    By default, this method calls graphicsItem()->setPos()
    */
  virtual void setPos(const QPointF&);

  /**
    @brief Set the item's size
    @warning By default, this method will scale the graphics item (calling QGraphicsItem::scale()). This default behavior may lead to bad-quality rendering in most cases.
    */
  virtual void resize(const QSizeF&);

signals:
  /**
    @brief Tells the upperleying system that a call to the draw method is needed.
    The upperleying system may then directly call the draw() method or wait until some events are previously processed.
    */
  void drawNeeded();

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
