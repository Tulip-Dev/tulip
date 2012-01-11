#ifndef QUICKACCESSBAR_H
#define QUICKACCESSBAR_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
class QuickAccessBar;
}

namespace tlp {
class GlMainView;
class GlGraphRenderingParameters;
class GlGraphInputData;
class GlScene;

class QuickAccessBar : public QWidget {
  Q_OBJECT

  Ui::QuickAccessBar* _ui;
  tlp::GlMainView* _mainView;
  bool _resetting;
  tlp::GlGraphInputData* inputData() const;
  tlp::GlGraphRenderingParameters* renderingParameters() const;
  tlp::GlScene* scene() const;
  double _oldFontScale;
  double _oldNodeScale;

  void updateFontButtonStyle();
public:
  explicit QuickAccessBar(QWidget *parent = 0);
  
signals:
  
public slots:
  void setGlMainView(tlp::GlMainView*);
  void reset();

  void setBackgroundColor(const QColor&);
  void setColorInterpolation(bool);
  void scaleFont(int);
  void setLabelColor(const QColor&);
  void setNodeColor(const QColor&);
  void scaleNodes(int);
  void takeSnapshot();
  void setLabelProperty(const QString&);
  void setEdgesVisible(bool);
  void setEdgesLabelsVisible(bool);
  void setNodesLabelsVisible(bool);
  void selectFont();
};
}

#endif // QUICKACCESSBAR_H
