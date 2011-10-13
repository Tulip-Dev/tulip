#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/View.h>

namespace tlp {

/**
  @class ViewWidget allows the user to build View objects that return a QWidget instead of a QGraphicsItem.
  This class is compatible with most of Qt's widget (every widget that can be embedded in a QGraphicsProxyWidget, @see QGraphicsProxyWidget) and with tlp::GlMainWidget

  @warning When subclassing a ViewWidget, the ViewWidget::widget() method must be implemented to provide with the View's widget.
  The View::setupUi() method should not be implemented by any subclass of ViewWidget, the setupWidget() method should be used instead.
  */
class ViewWidget : public tlp::View {
  Q_OBJECT

  QGraphicsItem* _widgetItem;
public:
  ViewWidget();
  virtual ~ViewWidget();

  /**
    @see tlp::View
    */
  virtual QGraphicsItem* graphicsItem() const;

  /**
    @return The View's widget.
    */
  virtual QWidget* widget() const=0;

  /**
    @see tlp::View
    */
  virtual tlp::DataSet data() const=0;

public slots:
  /**
    @see tlp::View
    @warning This method should be implemented by any subclass of ViewWidget, @see setupWidget() instead
    */
  virtual void setupUi();

  /**
    @see tlp::View
    */
  virtual void draw()=0;

  /**
    @see tlp::View
    A standard behavior that calls the resize() method on the View's widget.
    */
  virtual void resize(const QSizeF &);

protected:
  /**
    @see tlp::View
    */
  virtual void activeInteractorChanged(tlp::Interactor*);

  /**
    @brief Constructs the view's widget.
    This method should be used instead of View::setupUi. After this method is called, the ViewWidget::widget() method should return a valid widget.
    */
  virtual void setupWidget()=0;
};

}

#endif // VIEWWIDGET_H
