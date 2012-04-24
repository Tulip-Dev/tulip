#ifndef QUICKACCESSBAR_H
#define QUICKACCESSBAR_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/ScrollPopupButton.h>
#include <tulip/CaptionItem.h>

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
  QGraphicsItem *_quickAccessBarItem;
  tlp::GlMainView* _mainView;
  bool _resetting;
  tlp::GlGraphInputData* inputData() const;
  tlp::GlGraphRenderingParameters* renderingParameters() const;
  tlp::GlScene* scene() const;
  double _oldFontScale;
  double _oldNodeScale;
  bool _captionsInitialized;
  CaptionItem *_captions[4];
  void updateFontButtonStyle();
  void showHideCaption(CaptionItem::CaptionType captionType);

public:
  explicit QuickAccessBar(QGraphicsItem *quickAccessBarItem,QWidget *parent = 0);

public slots:
  void setGlMainView(tlp::GlMainView*);
  void reset();

  void setBackgroundColor(const QColor&);
  void setColorInterpolation(bool);
  void setLabelColor(const QColor&);
  void setNodeColor(const QColor&);
  void showHideNodesColorCaption();
  void showHideNodesSizeCaption();
  void showHideEdgesColorCaption();
  void showHideEdgesSizeCaption();
  void takeSnapshot();
  void setEdgesVisible(bool);
  void setLabelsVisible(bool);
  void selectFont();
};
}

#endif // QUICKACCESSBAR_H
