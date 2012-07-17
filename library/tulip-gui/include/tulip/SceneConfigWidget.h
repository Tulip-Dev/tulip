#ifndef SCENECONFIGWIDGET_H
#define SCENECONFIGWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>
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

  bool _resetting;

public:
  explicit SceneConfigWidget(QWidget *parent = NULL);

signals:
  void settingsApplied();

public slots:
  void setGlMainWidget(tlp::GlMainWidget*);
  void resetChanges();
  void applySettings();

protected slots:
  void settingsChanged();

protected:
  bool eventFilter(QObject *, QEvent *);

};
}

#endif // SCENECONFIGWIDGET_H
