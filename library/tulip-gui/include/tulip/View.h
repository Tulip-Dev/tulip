/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QSet>
#include <QList>
#include <QSize>

#include <tulip/tulipconf.h>
#include <tulip/Observable.h>
#include <tulip/Plugin.h>

class QGraphicsView;
class QGraphicsItem;
class QWidget;
class QPixmap;
class QMenu;
class QPointF;
class QPoint;

namespace tlp {
static const std::string VIEW_CATEGORY = "Panel";

class Interactor;

/**
  @ingroup Plugins

  @brief View plugins provide a way to dynamically add to a Tulip plateform various ways to visualize a graph.

  A view takes the following elements as inputs:
  @li A graph which contains the data to be displayed.
  @li A state (@c tlp::DataSet) which contains initialization parameters.

  As an output, a View must provide a @c QGraphicsView instance where all drawing is done.
  User interaction on a View is handled by the tlp::Interactor class. Several interactors can be installed on the same view but there can be only one active interactor at the same time.
  In the end, the view can provide several configuration widgets used to set up additional parameters.

  When a View gets created, the following methods will always be called in the following order:
  @li The constructor. Basically, you don't want to do anything in this method as View instance may be created at Tulip startup when the plugin system gets initialized. Subsequent methods will be called in order for the view to build UI elements
  @li View::setupUi(). Notifies the view it can now build GUI components since every part of its initial state should be valid by now. Once this method is called, any call to View::graphicsView() is expected to return a valid pointer object.
  @li View::setGraph. Sets the graph that is meant to be visualized in the View's panel.
  @li View::setState(). Sets initial data. This method may be used to restore a previously backed-up state retrieved from the View::state() method.
  @li View::interactorsInstalled(). Notifies the view of the available interactors. Interactors objects taken from the list have already been initialized.

  Once the View is initialized, none of the previously mentioned methods, except View::setGraph(), can be called again.
  View::setGraph method may be called again to notify the view that another graph should be displayed (this may be a sub/parent graph of the previously displayed graph or a graph coming from a totally different hierarchy)

  Views are meant to be managed by an overleying system. As a consequence, a view may not decide directly when to redraw.
  Thus, you should never call the View::draw() method. To notify the overleying system that your view needs to be redrawn, emit the View::drawNeeded() signal instead.

  A tlp::View subclass automatically inherits from the tlp::Observable interface. The tlp::View interface also automatically listn to its active graph to trigger handling trigger when this graph gets deleted.
  When the graph associated to a View gets deleted, the View::graphDeleted() callback is triggered. @see graphDeleted() for more information.
  */
class TLP_QT_SCOPE View: public QObject, public tlp::Plugin, public tlp::Observable {
  Q_OBJECT

  QList<tlp::Interactor*> _interactors;
  tlp::Interactor* _currentInteractor;
  tlp::Graph* _graph;

  QSet<tlp::Observable*> _triggers;

public:
  /**
    @brief Default constructor
    @warning Code of this method should almost be a no-op. Subsequent calls on other methods should allow you to setup your view.
    */
  View();

  /**
    @brief Destructor
    View's GUI components (graphics view, configuration widgets) responisbility belongs to the overleying system. Thus, the View is not in charge of deleting its graphcis view.
    View's interactors are already deleted in the top class.
    */
  virtual ~View();

  virtual std::string category() const {
    return VIEW_CATEGORY;
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_view.png";
  }

  /**
    @return the View's panel as a @c QGraphicsView instance.
    @note This method MUST ALWAYS return the same instance of a QGraphicsView.
    */
  virtual QGraphicsView* graphicsView() const=0;

  /**
    @return The list of interactors installed on this view.
    The list is always the same as the one given when View::setInteractors() was called.
    @see setInteractors();
    */
  QList<tlp::Interactor*> interactors() const;

  /**
    @return The currently active interactor.
    The active interactor is the one that currently recieve user inputs.
    @see setCurrentInteractor();
    @warning This method may return a NULL pointer if no interactor is currently active.
    */
  tlp::Interactor* currentInteractor() const;

  /**
    @return a list of widgets that can be used to set up the view.
    Since several widgets can be retrived, user will be able to select them from a combo box where each widget will be identified by its windowsTitle.
    @see View::applySettings()
    @warning This method must not instantiate configuration widgets on the fly.
    */
  virtual QList<QWidget*> configurationWidgets() const;

  /**
   * @brief This method can be used to change the configuration widgets' style
   * sheet. From Qt documentation: The style sheet contains a textual description of customizations to the
   *  widget's style, as described in the Qt Style Sheets document. see http://qt-project.org/doc/qt-4.7/stylesheet.html.
   * @return The stylesheet to use
   */
  virtual QString configurationWidgetsStyleSheet() const;

  /**
    @brief Backup the state of the view.
    This method is used to restore the View's parameters when it's re-opened.
    */
  virtual tlp::DataSet state() const=0;

  /**
    @return the graph displayed by the view.
    @note This method MUST return the same graph pointer that was previously passed down to setGraph.
    */
  tlp::Graph* graph() const;

  /**
    @return The list of currently registered triggers.
    @see View::addRedrawTrigger()
    */
  QSet<tlp::Observable*> triggers() const;

  /**
    @brief reimplemented from tlp::Observable to provide the triggers mechanism.
    @see View::addRedrawTrigger()
    */
  void treatEvents(const std::vector<Event> &events);

  /**
    @brief defines which item is considered as the central item in the view.
    The central item is considered to be a background item that will be set as parent of every graphics item added by the workspace into the view.
    By default, this method returns NULL, which means that no central item is defined.
    */
  virtual QGraphicsItem* centralItem() const;

  /**
    @brief Takes a snapshot of the view's screen and saves it into the given pixmap.
    The snapshot is scaled to outputSize. If a null size is given, the snapshot is to be on a 1:1 ratio
    @return A non-null pixmap of the snapshot was correctly taken.
    */
  virtual QPixmap snapshot(const QSize& outputSize=QSize()) const=0;

public slots:
  /**
  * @brief This method is called whenever the context menu is required on the view.
  * @param point The screen coordinates where the context menu should be displayed.
  */
  void showContextMenu(const QPoint& point, const QPointF &scenePoint);

  /**
   * @brief This method is a callback to notify the panel that the pop() method (undo) has just been called on the graph.
   * By default, this method will make a call to centerView()
   **/
  virtual void undoCallback();

  /**
    @brief This method applies settings changed in the configuration widgets
    This method may be called from the overleying system in various situations. The View is expected to apply settings in an optimized way to prevent extra redraws.
    By default, this method does nothing.
    */
  virtual void applySettings();

  /**
    @brief Reset the visualization to the center.
    This method is called after major changes into the data structure. At this point, the user point of view should be reset and brought back to a point where all the data can be seen.
    @note It is expected for the view to be redrawn when calling centerView
    For a 3D visualization, this method could be implemented by centering the camera. For a table, this could be done by setting the scroll bar to the top position etc...
    By default, this method calls draw().
    */
  virtual void centerView(bool graphChanged = false);


  /**
    @brief defines the list of interactors available on this View
    @note Calling this will trigger the View::interactorsInstalled() callback for custom handling.
  */
  virtual void setInteractors(const QList<tlp::Interactor*>&);

  /**
    @brief defines the active interactor that will receive user inputs.
    @note This method will first remove the previously active interactor (if any) using Interactor::uninstall()
    @note Calling this will trigger the View::currentInteractorChanged() callback for custom handling.
    @note Calling View::setCurrentInteractor(NULL) will only remove the previous current interactor.
  */
  void setCurrentInteractor(tlp::Interactor* currentInteractor);

  /**
    @brief Restores the state of the view.
    DataSet passed down to this method can come from a previous backup or be generated by the overleying system. It's up to the view to use this data or not.
    */
  virtual void setState(const tlp::DataSet&)=0;

  /**
    @brief Defines the graph that should be displayed by the View
    @note Calling setGraph triggers the View::graphChanged() callback for custom handling.
    @warning This method and its subsequent callback might be called several times.
    */
  void setGraph(tlp::Graph* graph);

  /**
    @brief Asks the view to draw.
    A call to draw() means that internal data has most probably been modified and that the View should take that into account when drawing.
    */
  virtual void draw()=0;

  /**
    @brief Refresh the View's panel.
    Calling refresh() means that no internal data has been modified. This can happen when the view's panel gets resized, restored etc
    */
  inline virtual void refresh() {
    draw();
  }

  /**
    @brief Sets up GUI elements belonging to the View.
    This method is called once the initial state as been set (using setGraph and setState) and is called only once.
    */
  virtual void setupUi()=0;

  /**
    @brief This method is inherited from tlp::Observable and allows the view to trigger custom callback when its associated graph gets deleted.
    @warning When overriding this method. You MUST always make a call to View::treatEvent before doing anything in order to keep this callback working.
    */
  virtual void treatEvent(const Event&);

  /**
    @brief Registers a new trigger for automatic view drawing.
    Triggers are tlp::Observable subclasses. Once registered, the view will listen to the trigger's events and emit the drawNeeded signal each time the Observable::treatEvents() callback is run.
    For more information about the Observable system, @see tlp::Observable

    @note This is a convenience function. However, using triggers prevent from performign extra checks on the data structure to know if a redraw must me made or not. For more control over event handling, you will have to implement your own treatEvent/treatEvents callback.
    @warning If your tlp::View subclass overloads the treatEvents method. You must make sure to call the View::treatEvents method in order to keep the triggers system working.
    */
  void addRedrawTrigger(tlp::Observable*);

  /**
    @brief Removes a trigger from the list of registered triggers. Event coming from this trigger will no longer trigger the drawNeeded signal.
    @see View::addRedrawTrigger()
    */
  void removeRedrawTrigger(tlp::Observable*);

  /**
    @brief Clears the list of attached triggers
    This method removes all triggers associated to the View.
    @note From the moment this method is called, no update on previous triggers will be considered. Even if this is called during an Observable::holdObservers()
    */
  void clearRedrawTriggers();

  /**
    @brief This function emit the signal drawNeeded
    */
  void emitDrawNeededSignal();

signals:
  /**
    @brief Inform the overlying subsystem that this view needs to be drawn.
    @note Depending on the overlying implementation, a subsequent call to draw might not be immediate.
    */
  void drawNeeded();

  /**
    @brief Emitted after the setGraph method has been called.
    @note This signal is emitted from the non-virtual View::setGraph() method thus cannot be prevented.
    */
  void graphSet(tlp::Graph*);

  void interactorsChanged();

protected slots:
  /**
    @brief Callback method after setInteractors() was called.
    At this point, a call to View::interactors() is considered valid.
    */
  virtual void interactorsInstalled(const QList<tlp::Interactor*>& interactors);

  /**
    @brief Callback method after setCurrentInteractor() was called.
    At this point, a call to View::currentInteractor() is considered valid and return the newly active interactor.
    @warning The interactor passed down to this method MAY BE a NULL pointer ! This means that no current interactor should be set.
    */
  virtual void currentInteractorChanged(tlp::Interactor*);

  /**
    @brief Callback method after setGraph() was called.
    At this point, a call to View::graph() is considered valid and return the lastly set graph.
    */
  virtual void graphChanged(tlp::Graph*)=0;

  /**
    @brief Called when the graph associated to the view gets deleted.
    This method should call setGraph to input a new graph pointer (NULL or valid)
    @param parentGraph The parent of the graph that was just deleted. If there is no parent available (eg. the graph was root), parentGraph is NULL
    */
  virtual void graphDeleted(tlp::Graph* parentGraph)=0;

  /**
   * @brief fills the context menu with entries related to the view.
   * This method is called whenever the context menu is displayed on the panel.
   * @param QMenu The popup menu that will be displayed. This menu should be populated with context action related to the panel.
   */
  virtual void fillContextMenu(QMenu*,const QPointF &) {}
};

}

#endif /* VIEW_H_ */
