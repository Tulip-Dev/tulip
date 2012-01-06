#ifndef SCENECONFIGWIDGET_H
#define SCENECONFIGWIDGET_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
class SceneConfigWidget;
}

namespace tlp {
class GlMainWidget;

class TLP_QT_SCOPE SceneConfigWidget : public QWidget {
  Q_OBJECT
  Ui::SceneConfigWidget* _ui;
  tlp::GlMainWidget* _glMainWidget;

public:
  explicit SceneConfigWidget(QWidget *parent = 0);

signals:

public slots:
  void setGlMainWidget(tlp::GlMainWidget*);
  void resetChanges();

};
}

#endif // SCENECONFIGWIDGET_H
