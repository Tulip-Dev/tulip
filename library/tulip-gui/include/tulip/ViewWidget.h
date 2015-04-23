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

#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/View.h>

class QGraphicsItem;

namespace tlp {

/**
  @ingroup Plugins

  @brief ViewWidget provides convenience functions to allow the user to build a view plugin that displays a QWidget as its main element.

  The ViewWidget class will build a QGraphicsView that sets a widget as the background of the whole panel.
  Sublassing ViewWidget means that you'll have to provide a centralWidget (see ViewWidget::setCentralWidget) that will take up the whole panel and be drawn in the background.
  You can use the addToScene() and removeFromScene() methods to edit the QGraphicsItems that will drawn over the widget.

  By default, when an interactor gets active on a ViewWidget, it gets installed on the centralWidget (see Interactor::install)

  @note When creating a ViewWidget, you should overload setupWidget instead of setupUi. If you still want to implement setupUi, you must call the ViewWidget::setupUi() method first.
  */
class TLP_QT_SCOPE ViewWidget : public tlp::View {
  Q_OBJECT

  QSet<QGraphicsItem *> _items;
  QGraphicsView* _graphicsView;
  QWidget* _centralWidget;
  QGraphicsItem* _centralWidgetItem;

  void refreshItemsParenthood();

public:
  ViewWidget();
  virtual ~ViewWidget();

  /**
    @see View::graphicsView()
    @note This method should not be reimplemented as a subclass of ViewWidget
    */
  virtual QGraphicsView* graphicsView() const;

public slots:
  /**
    @see View::setupUi
    @note This method should not be reimplemented as a subclass of ViewWidget
    */
  virtual void setupUi();

  /**
    @brief Reimplemented from View::draw()
    By default, this method does nothing. We assume the widget is automatically repainted by Qt's windowing manager
    */
  virtual void draw() { }

protected slots:
  /**
    By default, the current interactor gets installed over the central widget.

    @see View::currentInteractorChanged()
    */
  virtual void currentInteractorChanged(tlp::Interactor*);

  virtual void graphDeleted(Graph *parentGraph);

protected:
  /**
    @brief Sets up the central widget.
    This is similar to View::setupUi in the sense that the purpose of setupWidget is to construct the GUI element.
    @warning This method MUST call the setCentralWidget to provide the ViewWidget with a valid widget.
    */
  virtual void setupWidget()=0;

  /**
    @brief Adds an item to the graphicsView that will be drawn on top of the widget
    This is a convenience function for the user to avoid taking care of item parenthood.
    */
  void addToScene(QGraphicsItem* item);

  /**
    @brief Removes a graphics item from the view.
    This is a convenience function for the user to avoid taking care of item parenthood.
    */
  void removeFromScene(QGraphicsItem* item);

  /**
    @brief Sets the widget to be drawn as the view's background.
    This method may be called several times. Parenthood between the widget and items added using addToScene will be automatically updated.
    @note The ViewWidget takes ownership of the central widget. The previous central widget gets deleted in the process.
    */
  void setCentralWidget(QWidget*, bool deleteOldCentralWidget=true);

  /**
    @return The graphics item associated to the central widget
    @see setCentralWidget
    */
  virtual QGraphicsItem* centralItem() const;

  QPixmap snapshot(const QSize &outputSize=QSize()) const;
};

}

#endif // VIEWWIDGET_H
