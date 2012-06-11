#ifndef SCENELAYERSCONFIGWIDGET_H
#define SCENELAYERSCONFIGWIDGET_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
class SceneLayersConfigWidget;
}

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE SceneLayersConfigWidget : public QWidget {
  Q_OBJECT

  Ui::SceneLayersConfigWidget* _ui;
  tlp::GlMainWidget* _glMainWidget;
public:
  explicit SceneLayersConfigWidget(QWidget *parent = NULL);
  virtual ~SceneLayersConfigWidget();

  void setGlMainWidget(tlp::GlMainWidget* glMainWidget);

signals:
  void drawNeeded();
};
}

#endif // SCENELAYERSCONFIGWIDGET_H
