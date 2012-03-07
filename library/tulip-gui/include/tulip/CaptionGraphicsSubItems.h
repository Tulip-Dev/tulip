/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef CAPTIONGRAPHICSSUBITEMS_H
#define CAPTIONGRAPHICSSUBITEMS_H


#include <QtGui/QGraphicsItemGroup>
#include <QtGui/QComboBox>
#include <QtOpenGL/QGLFramebufferObject>
#include <tulip/View.h>
#include <tulip/Color.h>


#include <QtGui/QGraphicsSceneMouseEvent>

namespace tlp {

class ConfigurationIconItem : public QObject, public QGraphicsPixmapItem {

  Q_OBJECT

public :

  ConfigurationIconItem() {

  }

signals :

  void configurationIconPressed();

protected :

  void mousePressEvent( QGraphicsSceneMouseEvent *) {
    emit configurationIconPressed();
  }
};

class SelectionArrowItem : public QObject, public QGraphicsPathItem {

  Q_OBJECT

public :

  SelectionArrowItem(float initRangePos,const QPoint &initPos);

  bool sceneEvent ( QEvent * event );

signals :

  void circleMoved();

protected :

  int yPos;
  QPoint initPos;
};

class SelectionTextItem : public QGraphicsTextItem {

public :

  SelectionTextItem() {
    QFont f=font();
    f.setBold(true);
    setFont(f);
  }

protected :

  bool sceneEvent ( QEvent * event ) {
    return ((SelectionArrowItem*)parentItem())->sceneEvent(event);
  }

};

class ColorGradientItem : public QObject, public QGraphicsRectItem {

  Q_OBJECT

public :

  ColorGradientItem(const QRectF &rect, QGraphicsRectItem *topRect, QGraphicsRectItem *bottomRect,SelectionArrowItem *topCircle, SelectionArrowItem *bottomCircle);

signals :

  void moved();

protected :

  bool sceneEvent ( QEvent * event );

  QPoint _initPos;
  QGraphicsRectItem *_topRect;
  QGraphicsRectItem *_bottomRect;
  SelectionArrowItem *_topCircle;
  SelectionArrowItem *_bottomCircle;
};

class CaptionGraphicsBackgroundItem : public QObject, public QGraphicsRectItem {

  Q_OBJECT

public :

  CaptionGraphicsBackgroundItem(const QRect &rect);

  void generateCaption(const QGradient &activeGradient, const QGradient &hideGradient, const std::string &propertyName, double minValue, double maxValue);

public slots :

  void updateCaption();
  void configurationIconPressedSlot();

signals :

  void filterChanged(float begin, float end);
  void configurationIconPressed();

protected :

  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

  QPoint _captionContentPos;

  double _minValue;
  double _maxValue;

  QGraphicsTextItem *_propertyNameItem;
  QGraphicsTextItem *_minTextItem;
  QGraphicsTextItem *_maxTextItem;
  QGraphicsRectItem *_captionRectBorder;
  ConfigurationIconItem *_prefIcon;
  QGraphicsRectItem *_prefRect;
  QGraphicsRectItem *_topCaptionRectItem;
  ColorGradientItem *_middleCaptionRectItem;
  QGraphicsRectItem *_bottomCaptionRectItem;
  SelectionArrowItem *_circle1Item;
  SelectionArrowItem *_circle2Item;
  SelectionTextItem *_circle1ItemText;
  SelectionTextItem *_circle2ItemText;

};

}

#endif // CAPTIONGRAPHICSSUB_H
