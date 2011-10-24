/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/tulipconf.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Observable.h>
#include <tulip/PluginLister.h>
#include <tulip/MethodFactory.h>

#include <QtCore/QObject>
#include <QtCore/QSet>

class QGraphicsView;
class QWidget;

namespace tlp {
class Interactor;

/**
  @class View plugins provide a way to dynamically add to a Tulip plateform various ways to visualize a graph.

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
  */
class TLP_QT_SCOPE View: public QObject, public tlp::WithDependency, public tlp::WithParameter, public tlp::Observable {
  Q_OBJECT

  QList<tlp::Interactor*> _interactors;
  tlp::Interactor* _currentInteractor;
  tlp::Graph* _graph;

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
    */
  virtual QList<QWidget*> configurationWidgets() const;

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

public slots:
  /**
    @brief defines the list of interactors available on this View
    @note Calling this will trigger the View::interactorsInstalled() callback for custom handling.
  */
  void setInteractors(const QList<tlp::Interactor*>&);

  /**
    @brief defines the active interactor that will recieve user inputs.
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
  virtual void draw(tlp::PluginProgress* pluginProgress)=0;

  /**
    @brief Refresh the View's panel.
    Calling refresh() means that no internal data has been modified. This can happen when the view's panel gets resized, restored etc
    */
  inline virtual void refresh(tlp::PluginProgress* pluginProgress) {
    draw(pluginProgress);
  }

  /**
    @brief Sets up GUI elements belonging to the View.
    This method is called once the initial state as been set (using setGraph and setState) and is called only once.
    */
  virtual void setupUi()=0;

signals:
  /**
    @brief Inform the overleying subsystem that this view needs to be drawn.
    @note Dependeing on the overlying implementation, a subsequent call to draw might not be immediate.
    */
  void drawNeeded();

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
