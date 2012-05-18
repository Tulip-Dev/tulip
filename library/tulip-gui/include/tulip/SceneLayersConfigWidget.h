#ifndef SCENELAYERSCONFIGWIDGET_H
#define SCENELAYERSCONFIGWIDGET_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
class SceneLayersConfigWidget;
}

namespace tlp {

class GlMainView;

class TLP_QT_SCOPE SceneLayersConfigWidget : public QWidget {
  Q_OBJECT

  Ui::SceneLayersConfigWidget* _ui;
  tlp::GlMainView* _view;
public:
  explicit SceneLayersConfigWidget(QWidget *parent = NULL);
  virtual ~SceneLayersConfigWidget();

  void setView(tlp::GlMainView* view);

signals:
  void drawNeeded();
};
}

#endif // SCENELAYERSCONFIGWIDGET_H
