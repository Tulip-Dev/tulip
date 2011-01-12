#ifndef SMALLMULTIPLESVIEW_H
#define SMALLMULTIPLESVIEW_H

#include "tulip/AbstractView.h"
#include <QtCore/QVector>

namespace tlp {

class GlMainWidget;
class AbstractSmallMultiplesModel;

/**
  * @brief The SmallMultiplesView class provides an abstraction containing convenience function to present multiple DataSet in a Small Multiples way.
  * The small multiples organization provides user with a way to display several data sets in a single window.
  * This view provides with two distinct modes:
  *  . Overview mode: Every data set is previewed in a small square user can double click to visualize this peculiar data.
  *  . Specific mode: A widget provided by the user to view a specific data set. Custom interactions can be defined for this mode and installed as interactors.
  * The SmallMultiplesView subclasses the AbstractView but does not implement any pure-virtual function from the View interface. Thus the implementation of the view's behavior is
  * still left to the user.
  *
  * The SmallMultiplesView class comes along with an AbstractSmallMultiplesModel object the user may have to subclass too. This object provides the view with data sets previews, names
  * and positions in the overview.
  *
  * @see AbstractSmallMultiplesModel
  */
class TLP_QT_SCOPE SmallMultiplesView: public AbstractView {
  Q_OBJECT
public:
  /**
    * @brief Construct a SmallMultiplesView using a specific model.
    * @param The model used to retreive data sets previews, names and positions in the overview.
    */
  SmallMultiplesView(AbstractSmallMultiplesModel *model);

  virtual ~SmallMultiplesView();
  AbstractSmallMultiplesModel *model();

  /**
    * @brief Constructs the view UI.
    * You must call this method if you implement it on your SmallMultiplesView subclass.
    */
  virtual QWidget *construct(QWidget *parent);

  /**
    * The overview window is a GlMainWidget. Each data set corresponds to a node. You may want to use this fnuction to add custom
    * background entities in the overview window or to provide custom items layouts (using tulip graph layout algorithms).
    * @return The overview widget (as a GlMainWidget)
    * @see GlMainWidget
    */
  GlMainWidget *overview() const;

  /**
    * @brief Centers the overview scene.
    */
  void centerOverview();

  /**
    * @return true if the SmallMultiplesView is currently displaying the overview. To get the currently displayed widget, use getCentralWidget instead
    * @see AbstractView::getCentralWidget
    */
  bool isOverview() const;

  /**
    * When asking for data, the view references each data set with this insertion order (thus, the first dataset will be 0, the second 1 and so on).
    * In the overview, each data set is associated to a node, this method retrieves the item id corresponding to a node in the overview
    * @return The item id of the corresponding node
    * @see AbstractSmallMultiplesModel
    */
  int nodeItemId(node);

  /**
    * @return true if zoom animations are activated.
    * @see setZoomAnimationActivated
    */
  bool isZoomAnimationActivated() const { return _zoomAnimationActivated; }

  /**
    * @brief When set to true, the SmallMultipleView will provide smooth zoom in and out when the user selects an item or switch back to the overview.
    * @param f zoom animations toggle.
    */
  void setZoomAnimationActivated(bool f) { _zoomAnimationActivated = f; }

public slots:
  /**
    * @brief Switchs to the data sets overview.
    */
  void switchToOverview(bool destroyOldWidget=false);

  /**
    * @brief Makes a widget the window currently displayed in the view.
    * Once you are on this custom widget, you may switch back to overview mode by double clicking (using the small multiples view interactor)
    */
  void switchToWidget(QWidget *);

  /**
    * @brief Toggle interactors that are not specific to this view on or off.
    * @warning The SmallMultiplesView navigation interactor should never be disabled. The SmallMultiplesView consider that it is the first (highest  priority) into the list of interactors.
    */
  void toggleInteractors(bool);

  /**
    * @brief Selects an item in the overview
    * @param the item id (not the node id) of the item to select
    */
  void selectItem(int);

protected:
  /**
    * Called when an item was selected.
    */
  virtual void itemSelected(int) {}

protected slots:
  void dataChanged();
  void dataChanged(int);
  void itemAdded();
  void itemDeleted(int);

private:
  AbstractSmallMultiplesModel *_model;
  GlMainWidget *_overview;
  QVector<node> _items;
  bool _zoomAnimationActivated;

  void refreshItemsCount();
};
}

#endif // SMALLMULTIPLESVIEW_H
