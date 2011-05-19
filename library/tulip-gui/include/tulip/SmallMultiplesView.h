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
#ifndef SMALLMULTIPLESVIEW_H
#define SMALLMULTIPLESVIEW_H

#include "tulip/AbstractView.h"
#include <QtCore/QVector>
class QGraphicsView;
class QGraphicsItem;

namespace tlp {

class GlMainWidgetGraphicsView;
class GlMainWidget;
class GlLayer;

/**
  * @brief This class provides an abstraction for the View class to help user creates small multiples views.
  * A Small multiples view is used to display various visualisation of a same data set in the same window.
  * Basically, the Small multiples view displays an overviews of items the user can select to set focus on them.
  *
  * @example We want to display frequency histograms for metrics in a graph. This graph can contains several metrics and we want to display one histogram per each.
  *          Each item will correspond to a metric. In the overview mode, we will display raw overviews for our histograms.
  *          When the user sets focus on one item, we will display a more accurate representation of this peculiar histogram and allow specific interactions on it.
  *
  * In depth, the SmallMultiplesView contains a GlMainWidget, displaying a graph (different from the one set on the view). Each node of this graph is associated to an item.
  * When sublassing SmallMultiplesView, the user can define several properties on each of those items. Values for those properties are retrieved from the virtual data method.
  * This method is meant to return a QVariant containing a value whose type differs according the role the SmallMultiplesView wants to display:
  *
  * Role     |   Type in the QVariant | Description
  *          |                        |
  * Label    |         QString        | The name that will be displayed beyond the item preview in overview mode.
  * Texture  |         QString        | The texutre used to preview the item in overview mode.
  * Position |       tlp::Coord       | The position of the item's preview in overview mode.
  *
  * By default, a SmallMultiplesView always comes along the Small multiples navigation interactor. This interactor provides a way to navigate in overview and to select items by double-clicking them.
  * When not in overview mode. This interactor only alow the user to double click to switch back to overview.
  * To add this interactor to your SmallMultiplesView implementation, use the following macro:
  * @code INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(<class name for the interactor>, "Interactor name", "InteractorSmallMultiplesNavigation", "<Name of the view>", "<Author>", "<Date>", "<description>", "<version>", INT_MAX)
  * @warning Note that the priority defined with this macro should always be set to the highest value possible (INT_MAX) to ensure that this interactor will always be installed before all others.
  *
  * We will now detail more about the behavior in overview and specific modes.
  *
  * . Overview mode: Displays a graph where each node is asociated to an item. The graph is displayed in a GlMainWidget in a layer called "overview"
  *                  @warning The overview layer mus never be deleted
  *                  In this mode, only the small multiples navigation interactor is enabled (@see toggleInteractors(bool)). The role methos is called to get information about items.
  *                  The user may emit the dataChanged signal to inform the view that data needs to be refreshed on one or several items.
  *                  When the user double clicks on an item, the itemSelected method is called.
  *
  * . Specific mode: When the isOverview method returns false, the SmallMultiplesView is considered to be in the specific mode.
  *                  The user can redefine this method to change the default behavior (we consider to be in the specific mode when the overview layer is not visible)
  *                  When in the specifc mode, you may want to call the toggleInteractors(bool) method to enable your specific interactors.
  *                  When the small multiples navigation interactor is enabled in this mode, the overviewSelected method is called when the user double clicks.
  *
  */
class TLP_QT_SCOPE SmallMultiplesView: public AbstractView {
  Q_OBJECT
public:

  /**
    * @brief Defines the roles that the view can ask for when refreshing data for an item.
    * Each role is associated to a data type stored in a QVariant returned by the data method.
    * @see data(int id, SmallMultiplesDataRole role);
    */
  enum SmallMultiplesDataRole {
    Label     = 1, // 0b001 QString
    Texture   = 2, // 0b010 QString
    Position  = 4, // 0b100 tlp::Coord
    AllRoles  = 7
  };
  Q_DECLARE_FLAGS(Roles, SmallMultiplesDataRole)

  SmallMultiplesView();
  virtual ~SmallMultiplesView();
  virtual QWidget *construct(QWidget *parent);
  virtual void setActiveInteractor(Interactor *interactor);

  /**
    * @brief The data method is called to retrieve informations to display items in the overview mode.
    * @param id The id of the item. Items are identified by confitgous identifiers starting from 0 to countItems()-1.
    * @param role The role we want to update, can be Label, Texture or Position. (Cannot be AllRoles)
    *
    * @return A QVariant containing data whose type depends on the role we want to refresh:
    *
    * Role     |   Type in the QVariant | Description
    *          |                        |
    * Label    |         QString        | The name that will be displayed beyond the item preview in overview mode.
    * Texture  |         QString        | The texutre used to preview the item in overview mode.
    * Position |       tlp::Coord       | The position of the item's preview in overview mode.
    */
  virtual QVariant data(int id, SmallMultiplesDataRole role);

  /**
    * @brief Count the items displayed in the overview mode.
    * @warning This method is not const, meaning that you can modify your object when counting your items.
    * @returns The number of items displayed in overview mode.
    */
  virtual int countItems() = 0;

  /**
    * @returns The GlMainWidget used to display the overview.
    */
  GlMainWidget *overview() const;

  /**
    * @brief Center the overview's scene. If the zoomAnimationActivated flag is set, this will trigger a smooth animation to center the camera.
    * @see isZoomAnimationActivated.
    */
  void centerOverview();

  /**
    * @brief Used to retrieve this id of the item associated to the given node in the overview graph.
    * @warning You must be cautious with this method by ensuring that the given node is an element of the overview graph.
    */
  int nodeItemId(node);

  /**
    * @brief When set, the zoomAnimationActivated provides smooth camera transitions when selecting an item in the overview mode or when centering the overview's camera.
    * Default value is true.
    */
  bool isZoomAnimationActivated() const { return _zoomAnimationActivated; }

  /**
    * @brief Defines a maximum size for items' labels in overview mode.
    * If a label's length goes beyond the maximum label size, the label is cropped with the '...' suffix.
    * If set to -1, then labels will never be wrapped.
    * Default value is -1.
    */
  int maxLabelSize() const { return _maxLabelSize; }

  /**
    * @brief By default, the data method organizes items position in a square shape in the overview mode. This value defines the spacing between to items in this square.
    * @warning When overriding the data method for the Position role, this value has no effect.
    */
  double spacing() const { return _spacing; }

  /**
    * @brief Shows or hides the "overview" layer in the GlMainWidget, making the overview visible or not.
    * @warning By default, when hiding the overview, the SmallMultiplesView will be considered in specific mode.
    * @see isOverview()
    */
  void setOverviewVisible(bool);

  /**
    * @brief Used to retrieve the overview layer in the GlMainWidget.
    */
  GlLayer *overviewLayer() const;

  /**
    * @brief Indicates if the view is currently in overview mode or not. This will affect the behavior of the Small multiples navigation interactor (see class description)
    * This method can be subclassed. By default, we consider that we are in overview mode when the overview layer is visible.
    */
  virtual bool isOverview() const { return isOverviewVisible(); }

  /**
    * @brief Check if the overview layer is visible.
    * @see isOverview()
    */
  bool isOverviewVisible() const;

  /**
    * @brief Called when an item has been selected in overview mode.
    * @warning if the zoomAnimationActivated flag is set, this method will be called after the zoom animation is done.
    */
  virtual void itemSelected(int) {}

  /**
    * @brief Called to indicate that the user requested to switch back to overview from the specific mode.
    */
  virtual void overviewSelected() {}

signals:
  /**
    * @brief Triggered to prevent that data must be refreshed for the specific range of items.
    * @warning The view will also call the countItems method to check if new items have been added in the meantime, so if an item has been added and yopu want to display it, just emit the following signal:
    * @code emit changeData(itemId,itemId,AllRoles);
    */
  void changeData(int fromId, int toId, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);

  /**
    * @brief Reverses the id of two items.
    */
  void reverseItems(int a,int b);

public slots:
  /**
    * @brief When set, the zoomAnimationActivated provides smooth camera transitions when selecting an item in the overview mode or when centering the overview's camera.
    * Default value is true.
    */
  void setZoomAnimationActivated(bool f) { _zoomAnimationActivated = f; }

  /**
    * @brief Defines a maximum size for items' labels in overview mode.
    * If a label's length goes beyond the maximum label size, the label is cropped with the '...' suffix.
    * If set to -1, then labels will never be wrapped.
    * Default value is -1.
    */
  void setMaxLabelSize(int s) { _maxLabelSize = s; }

  /**
    * @brief By default, the data method organizes items position in a square shape in the overview mode. This value defines the spacing between to items in this square.
    * @warning When overriding the data method for the Position role, this value has no effect.
    */
  void setSpacing(double s) { _spacing = s; }

  /**
    * @brief Toggles specific interactors for this view.
    * @warning Specific interactors should only be enabled in specific mode.
    */
  void toggleInteractors(bool);

  /**
    * @brief Selects an item in overview mode.
    * If the zoomAnimationActivated flag is set, a smooth animation will be run to zoom on the given item
    */
  void selectItem(int);
  /**
    * @brief Zoom on an item in overview mode.
    */
  void zoomOnItem(int);

protected slots:
  void dataChanged(int from, int to, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);
  void dataChanged(int id, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);
  void refreshItems();
  void addItem();
  void delItem(int);
  void itemsReversed(int,int);

private:
  GlMainWidget *_glMainWidget;
  QVector<node> _items;
  bool _zoomAnimationActivated;
  bool _autoDisableInteractors;
  int _maxLabelSize;
  double _spacing;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SmallMultiplesView::Roles)
}

#endif // SMALLMULTIPLESVIEW_H
