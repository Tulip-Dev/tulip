/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN


#ifndef QUICKACCESSBAR_H
#define QUICKACCESSBAR_H

#include <QWidget>

#include <tulip/CaptionItem.h>
#include <tulip/tulipconf.h>

class QGraphicsItem;

namespace Ui {
class QuickAccessBar;
}

namespace tlp {
class GlMainView;
class GlGraphRenderingParameters;
class GlGraphInputData;
class GlScene;
class TulipItemDelegate;
class ColorProperty;
class Color;
class PropertyInterface;

class TLP_QT_SCOPE QuickAccessBar : public QWidget {
  Q_OBJECT

protected:
  GlMainView* _mainView;
  GlScene* scene() const;

public:
  QuickAccessBar(QWidget *parent=0);

public slots:
  void setGlMainView(tlp::GlMainView*);
  virtual void reset()=0;

signals:
  void settingsChanged();
};

class TLP_QT_SCOPE QuickAccessBarImpl : public QuickAccessBar {
  Q_OBJECT

  Ui::QuickAccessBar* _ui;
  QGraphicsItem *_quickAccessBarItem;

  TulipItemDelegate* delegate;
  bool _resetting;
  GlGraphInputData* inputData() const;
  GlGraphRenderingParameters* renderingParameters() const;
  double _oldFontScale;
  double _oldNodeScale;
  bool _captionsInitialized;
  CaptionItem *_captions[4];
  void updateFontButtonStyle();
  void showHideCaption(CaptionItem::CaptionType captionType);
  void setAllValues(unsigned int eltType, PropertyInterface* prop);
  void setAllColorValues(unsigned int eltType, ColorProperty* prop,
                         const Color &color);

public:
  explicit QuickAccessBarImpl(QGraphicsItem *quickAccessBarItem,QWidget *parent = 0);
  virtual ~QuickAccessBarImpl();

public slots:
  void reset();

  void setBackgroundColor(const QColor&);
  void setColorInterpolation(bool);
  void setLabelColor(const QColor&);
  void setNodeColor(const QColor&);
  void setNodeBorderColor(const QColor&);
  void setEdgeColor(const QColor&);
  void setEdgeBorderColor(const QColor&);
  void setNodeShape();
  void setEdgeShape();
  void setNodeSize();
  void setEdgeSize();
  void setSizeInterpolation(bool);
  void showHideNodesColorCaption();
  void showHideNodesSizeCaption();
  void showHideEdgesColorCaption();
  void showHideEdgesSizeCaption();
  void takeSnapshot();
  void setEdgesVisible(bool);
  void setLabelsVisible(bool);
  void setLabelsScaled(bool);
  void selectFont();
  void setNodeLabelPosition();
};
}

#endif // QUICKACCESSBAR_H
///@endcond
