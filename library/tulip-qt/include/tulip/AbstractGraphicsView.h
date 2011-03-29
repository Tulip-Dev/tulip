#ifndef ABSTRACTGRAPHICSVIEW_H
#define ABSTRACTGRAPHICSVIEW_H

#include <tulip/View.h>

#include <QtCore/QSet>

class QGraphicsProxyWidget;
class QGraphicsView;
class QGraphicsItem;
class QGLWidget;

namespace tlp {

/**
  * @brief The AbstractGraphicsView class is an abstract implementation of the View interface to easily provide user with Qt graphics view technology.
  *
  * Any Tulip view must provide a top level widget in which any drawing coming from the view will be done. Additional subwidgets may be added in the top level
  * window that will appear in the Tulip workspace.
  * The AbstractGraphicsView creates a QWidget as its top-level widget, containing a vertical layout with a QGraphicsView (the central view) in it. The user can then define the "central widget" which will be included in a QGraphicsProxyWidget
  * and drawn as the background of the QGraphicsScene.
  *
  * Any interactor installed on the view will be installed on the central widget. If there is no central widget, then Interactor installation will have no effect.
  */
class AbstractGraphicsView: public tlp::View {
  Q_OBJECT
public:
  AbstractGraphicsView();
  virtual ~AbstractGraphicsView();

  /**
    * @brief Constructs the view's UI
    * This method will build up the graphics view, if you choose to reimplement it, do as follow:
    * @code
    * QWidget *MyView::construct(QWidget *parent) {
    *   QWidget *result = AbstractGraphicsView::construct(parent);
    *   // result is the central view, you can cast it to a QGraphicsView and do whatever you like on it
    *   return result;
    * }
    * @endcode
    */
  virtual QWidget *construct(QWidget *parent);

  /**
    * @return The central view.
    */
  virtual QWidget *getWidget();

  /**
    * @brief a Default implementation to generate a picture from all the view.
    * This method takes a snapshot of the view and generates a picture from it. You can subclass this method if you want to implement a more specific behavior.
    */
  virtual bool createPicture(const std::string &pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset);

  /**
    * @brief Install interactors on the view
    * This method is called from the controller which gives to the view the list of compatible interactors.
    * @warning You SHOULD NOT reimplement this method since it fills some crucial variables in the object. If you still choose to reimplement it, you should do the same for the getInteractors(), setActiveInteractor() and getActiveInteractor() methods.
    * However, since this method will also draw the interactor tool bar graphics item, you may want to choose a different look and feel for it, in that case, see the buildInteractorsToolBar() method.
    * @see buildInteractorsToolBar()
    */
  virtual void setInteractors(const std::list<tlp::Interactor *> &interactors);

  /**
    * @brief This method SHOULD NOT be reimplemented, @see setInteractors() for details.
    * @return the list of installed interactors.
    */
  virtual std::list<tlp::Interactor *> getInteractors();

  /**
    * @brief Change the active interactor on the central widget.
    * @warning This method SHOULD NOT be reimplemented, @see setInteractors() for details.
    */
  virtual void setActiveInteractor(tlp::Interactor *interactor);

  /**
    * @brief This method SHOULD NOT be reimplemented, @see setInteractors() for details.
    * @return the active interactor
    */
  virtual tlp::Interactor *getActiveInteractor();

  /**
    * @return the central widget.
    */
  QWidget *getCentralWidget();

  /**
    * @brief Sets the central widget of the view
    * The central widget will be drawn as the background of the central view. Any interactor installed on this view will be installed on the central widget.
    * @warning The AbstractGraphicsView does not take ownership on the widget, you'll have to delete it manually if you ever change the central widget
    */
  void setCentralWidget(QWidget *);

protected:
  /**
    * The list of interactors installed on this view.
    * @warning The list will be empty until the setInteractors method has been called.
    */
  std::list<tlp::Interactor *> _interactors;

  /**
    * The active interactor.
    * @warning This pointer will be set to NULL until the setActiveInteractor has been first called.
    */
  tlp::Interactor *_activeInteractor;

  /**
    * The central view.
    * @warning This pointer will be NULL until the construct method has been called.
    */
  QGraphicsView *_centralView;

  /**
    * @brief The central layout
    * The main view will be stored in a vertical layout, you can add other widgets in it.
    */
  QVBoxLayout *_mainLayout;


  /**
    * @brief Builds and returns the interactors toolbar.
    * This methods builds a graphical tool for interactor management, it can be overloaded if you wish to implement your own toolbar.
    * When reimplementing this method, use the addToScene methdo to add your custom graphics item to the view
    * @see addToScene()
    * @see removeFromScene()
    * @warning This method is only called once after setInteractors is called.
    */
  virtual void buildInteractorsToolbar();

  /**
    * @brief Adds an item to the central scene.
    * The added item's parenthood will be managed by the view. Meaning that when the setCentralWidget(); method is called. The item's parent will automatically be reset
    * to the central widget's graphics item.
    * @warning The view does not take specific ownership of the item. Meaning that it will only be managed by Qt as long as it's in the central view.
    */
  void addToScene(QGraphicsItem *item);

  /**
    * @brief Removes an item from the scene.
    * Remove the corresponding item from the central scene.
    * @warning Since the view does not take ownership on those items, the item will not be deleted.
    */
  void removeFromScene(QGraphicsItem *item);

  /**
    * @brief Event handler for the resize events
    * The AbstractGraphicsView installs itself as an event handler on the main view in order to catch resize events.
    * When such an event occurs, the scene and the central widget get resized to fit the size of the main view.
    */
  bool eventFilter(QObject *obj, QEvent *e);

protected slots:
  void updateCentralView();

private:
  QWidget *_centralWidget;
  QGraphicsItem *_centralWidgetItem;
  QWidget *_mainWidget;

  QSet<QGraphicsItem *> _items;
  void refreshItemsParenthood();
};
}

#endif // ABSTRACTGRAPHICSVIEW_H
