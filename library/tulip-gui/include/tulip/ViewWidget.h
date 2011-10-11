#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/View.h>

namespace tlp {

class ViewWidget : public tlp::View {
public:
  ViewWidget();
  virtual ~ViewWidget();

//  virtual QGraphicsItem* graphicsItem() const;
};

}

#endif // VIEWWIDGET_H
