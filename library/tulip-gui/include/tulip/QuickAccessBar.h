/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
class QAbstractButton;
class QPushButton;

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
class ColorButton;

class TLP_QT_SCOPE QuickAccessBar : public QWidget {
  Q_OBJECT

protected:
  GlMainView *_mainView;
  GlScene *scene() const;
  GlGraphInputData *inputData() const;
  GlGraphRenderingParameters *renderingParameters() const;

public:
  QuickAccessBar(QWidget *parent = 0);
public slots:
  void setGlMainView(tlp::GlMainView *);
  virtual void reset() = 0;

signals:
  void settingsChanged();
};

class TLP_QT_SCOPE QuickAccessBarImpl : public QuickAccessBar {
  Q_OBJECT
  Ui::QuickAccessBar *_ui;
  QGraphicsItem *_quickAccessBarItem;

  TulipItemDelegate *delegate;
  bool _resetting;
  double _oldFontScale;
  double _oldNodeScale;
  bool _captionsInitialized;
  CaptionItem *_captions[4];

public:
  enum QuickAccessButton {
    NODESCOLORCAPTION = 0x1,
    NODESSIZECAPTION = 0x2,
    EDGESCOLORCAPTION = 0x4,
    EDGESIZECAPTION = 0x8,
    SCREENSHOT = 0x10,
    BACKGROUNDCOLOR = 0x20,
    NODECOLOR = 0x40,
    EDGECOLOR = 0x80,
    NODEBORDERCOLOR = 0x100,
    EDGEBORDERCOLOR = 0x200,
    LABELCOLOR = 0x400,
    COLORINTERPOLATION = 0x800,
    SIZEINTERPOLATION = 0x1000,
    SHOWEDGES = 0x2000,
    SHOWLABELS = 0x4000,
    LABELSSCALED = 0x8000,
    NODESHAPE = 0x10000,
    EDGESHAPE = 0x20000,
    NODESIZE = 0x40000,
    EDGESIZE = 0x80000,
    NODELABELPOSITION = 0x100000,
    SELECTFONT = 0x200000,
    SHOWNODES = 0x400000,
    ALLBUTTONS = 0xFFFFFF
  };
  Q_DECLARE_FLAGS(QuickAccessButtons, QuickAccessButton)

  explicit QuickAccessBarImpl(QGraphicsItem *quickAccessBarItem = NULL,
                              QuickAccessButtons button = ALLBUTTONS, QWidget *parent = 0);
  virtual ~QuickAccessBarImpl();

  QPushButton *showEdgesButton();
  QPushButton *showNodesButton();
  ColorButton *backgroundColorButton();
  QPushButton *showLabelsButton();
  QPushButton *showLabelScaled();
  QPushButton *showColorInterpolation();

protected:
  void addButtonAtEnd(QAbstractButton *button);
  void addButtonsAtEnd(const QVector<QAbstractButton *> &buttonvect);
  void addSeparator();
  void updateFontButtonStyle();
  void showHideCaption(CaptionItem::CaptionType captionType);
  void setAllValues(unsigned int eltType, PropertyInterface *prop);
  void setAllColorValues(unsigned int eltType, ColorProperty *prop, const Color &color);

public slots:
  void reset();

  virtual void setBackgroundColor(const QColor &);
  virtual void setColorInterpolation(bool);
  void setLabelColor(const QColor &);
  void setNodeColor(const QColor &);
  void setNodeBorderColor(const QColor &);
  void setEdgeColor(const QColor &);
  void setEdgeBorderColor(const QColor &);
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
  virtual void setEdgesVisible(bool);
  virtual void setNodesVisible(bool);
  virtual void setLabelsVisible(bool);
  virtual void setLabelsScaled(bool);
  void selectFont();
  void setNodeLabelPosition();
};
}
Q_DECLARE_OPERATORS_FOR_FLAGS(tlp::QuickAccessBarImpl::QuickAccessButtons)
#endif // QUICKACCESSBAR_H
///@endcond
