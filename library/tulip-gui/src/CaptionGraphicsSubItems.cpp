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
#include "tulip/CaptionGraphicsSubItems.h"

#include <QtGui/QGraphicsProxyWidget>

#include <tulip/DoubleProperty.h>
#include <tulip/ColorProperty.h>

using namespace std;

namespace tlp {

CaptionGraphicsBackgroundItem::CaptionGraphicsBackgroundItem(const QRect &rect):QGraphicsRectItem(rect),_captionContentPos(10,20) {
  _circle1Item=new SelectionArrowItem(0,_captionContentPos);
  _circle2Item=new SelectionArrowItem(1,_captionContentPos);

  _topCaptionRectItem=new QGraphicsRectItem(QRect(_captionContentPos,QSize(30,0)));
  _bottomCaptionRectItem=new QGraphicsRectItem(QRect(_captionContentPos+QPoint(0,160),QSize(30,0)));
  _middleCaptionRectItem=new ColorGradientItem(QRect(_captionContentPos,QSize(30,160)),_topCaptionRectItem,_bottomCaptionRectItem,_circle1Item,_circle2Item);

  _captionRectBorder=new QGraphicsRectItem(QRect(_captionContentPos,QSize(30,160)));

  _prefIcon=new ConfigurationIconItem();
  _prefIcon->setPos(QPointF(0,-20));
  _prefIcon->setVisible(false);
  _prefIcon->setZValue(2);
  _prefRect=new QGraphicsRectItem(0,-20,20,20);
  _prefRect->setVisible(false);
  _prefRect->setPen(QPen(QColor(0,0,0,0)));

  _circle1Item->setPos(_captionContentPos+QPoint(5,-30));
  _circle2Item->setPos(_captionContentPos+QPoint(5,130));

  QFont font;
  font.setBold(true);
  _propertyNameItem = new QGraphicsTextItem();
  _propertyNameItem->setPos(0,195);
  _propertyNameItem->setFont(font);
  _minTextItem=new QGraphicsTextItem();
  _minTextItem->setFont(font);
  _maxTextItem=new QGraphicsTextItem();
  _maxTextItem->setFont(font);

  _circle1ItemText=new SelectionTextItem();
  _circle2ItemText=new SelectionTextItem();

  connect(_middleCaptionRectItem,SIGNAL(moved()),this,SLOT(updateCaption()));
  connect(_circle1Item,SIGNAL(circleMoved()),this,SLOT(updateCaption()));
  connect(_circle2Item,SIGNAL(circleMoved()),this,SLOT(updateCaption()));
  connect(_prefIcon,SIGNAL(configurationIconPressed()),this,SLOT(configurationIconPressedSlot()));

  setAcceptHoverEvents(true);
  _captionRectBorder->setParentItem(this);
  _captionRectBorder->setAcceptHoverEvents(true);

  _topCaptionRectItem->setParentItem(this);
  _topCaptionRectItem->setAcceptHoverEvents(true);
  _middleCaptionRectItem->setParentItem(this);
  _middleCaptionRectItem->setAcceptHoverEvents(true);
  _bottomCaptionRectItem->setParentItem(this);
  _bottomCaptionRectItem->setAcceptHoverEvents(true);

  _circle1Item->setParentItem(this);
  _circle1Item->setAcceptHoverEvents(true);
  _circle2Item->setParentItem(this);
  _circle2Item->setAcceptHoverEvents(true);

  _circle1ItemText->setParentItem(_circle1Item);
  _circle1Item->setAcceptHoverEvents(true);
  _circle2ItemText->setParentItem(_circle2Item);
  _circle2Item->setAcceptHoverEvents(true);

  _prefIcon->setParentItem(this);
  _prefIcon->setAcceptHoverEvents(true);
  _prefRect->setParentItem(this);
  _prefRect->setAcceptHoverEvents(true);

  _propertyNameItem->setParentItem(this);
  _propertyNameItem->setAcceptHoverEvents(true);
  _minTextItem->setParentItem(this);
  _minTextItem->setAcceptHoverEvents(true);
  _maxTextItem->setParentItem(this);
  _maxTextItem->setAcceptHoverEvents(true);

  installSceneEventFilter(_middleCaptionRectItem);
  installSceneEventFilter(_circle1Item);
  installSceneEventFilter(_circle2Item);
  installSceneEventFilter(_circle1ItemText);
  installSceneEventFilter(_circle2ItemText);

  setAcceptHoverEvents(true);
}

void CaptionGraphicsBackgroundItem::generateCaption(const QGradient &activeGradient, const QGradient &hideGradient, const string &propertyName, double minValue, double maxValue) {
  _minValue=minValue;
  _maxValue=maxValue;

  _prefIcon->setPixmap(QPixmap(":/tulip/gui/icons/16/preferences-other.png"));
  _prefIcon->setAcceptHoverEvents(true);

  _middleCaptionRectItem->setBrush(QBrush(activeGradient));
  _topCaptionRectItem->setBrush(QBrush(hideGradient));
  _bottomCaptionRectItem->setBrush(QBrush(hideGradient));

  _propertyNameItem->setPlainText(propertyName.c_str());

  _minTextItem->setPlainText(QString::number(_minValue));
  _maxTextItem->setPlainText(QString::number(_maxValue));

  _circle1ItemText->setPlainText(QString::number(_maxValue));
  _circle2ItemText->setPlainText(QString::number(_minValue));

  _circle1ItemText->setPos(QPoint(60-_circle1ItemText->boundingRect().width()/2.,17));
  _circle2ItemText->setPos(QPoint(60-_circle2ItemText->boundingRect().width()/2.,17));

  _minTextItem->setPos(QPointF(_captionContentPos+QPoint(-5,157)));
  _maxTextItem->setPos(QPointF(_captionContentPos+QPoint(-5,-22)));
}

void CaptionGraphicsBackgroundItem::updateCaption() {
  float begin = (_circle1Item->pos().y()-_captionContentPos.y()+30)/160.;
  float end = (_circle2Item->pos().y()-_captionContentPos.y()+30)/160.;

  if(begin>end){
    float tmp=begin;
    begin=end;
    end=tmp;
  }

  QString text1=QString::number(_minValue+(1-end)*(_maxValue-_minValue));
  if(text1.length()>5)
    text1=text1.left(5);
  QString text2=QString::number(_minValue+(1-begin)*(_maxValue-_minValue));
  if(text2.length()>5)
    text2=text2.left(5);

  _circle1ItemText->setPlainText(text1);
  _circle2ItemText->setPlainText(text2);
  _circle1ItemText->setPos(QPoint(60-_circle1ItemText->boundingRect().width()/2.,17));
  _circle2ItemText->setPos(QPoint(60-_circle2ItemText->boundingRect().width()/2.,17));

  _topCaptionRectItem->setRect(QRect(_captionContentPos,QSize(30,begin*160)));
  _middleCaptionRectItem->setRect(QRect(_captionContentPos+QPoint(0,begin*160),QSize(30,(end-begin)*160)));
  _bottomCaptionRectItem->setRect(QRect(_captionContentPos+QPoint(0,end*160),QSize(30,160-end*160)));

  emit filterChanged(begin,end);
}

void CaptionGraphicsBackgroundItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  _prefIcon->setVisible(true);
  _prefRect->setVisible(true);
}

void CaptionGraphicsBackgroundItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  _prefIcon->setVisible(false);
  _prefRect->setVisible(false);
}

void CaptionGraphicsBackgroundItem::configurationIconPressedSlot(){
  emit configurationIconPressed();
}

SelectionArrowItem::SelectionArrowItem(float initRangePos,const QPoint &initPos):
  QGraphicsPathItem(),yPos(initRangePos*160),initPos(initPos){

  setFiltersChildEvents(true);

  QPainterPath circle;
  circle.moveTo(25,30);
  circle.lineTo(33,23);
  circle.lineTo(80,23);
  circle.lineTo(80,37);
  circle.lineTo(33,37);
  circle.lineTo(25,30);

  setPath(circle);

  setBrush(QBrush(QColor(255,255,255,255)));
  //setZValue(2);
  setFlags(QGraphicsItem::ItemIsMovable);
}

bool SelectionArrowItem::sceneEvent ( QEvent * event ){
  if(event->type()==QEvent::GraphicsSceneMouseMove){
    QGraphicsSceneMouseEvent *e=(QGraphicsSceneMouseEvent *)event;
    qreal diffPosY=e->pos().y()-e->lastPos().y();
    if(pos().y()+diffPosY>initPos.y()+130)
      diffPosY=initPos.y()+130-pos().y();
    if(pos().y()+diffPosY<initPos.y()-30)
      diffPosY=initPos.y()-30-pos().y();

    moveBy(0,diffPosY);


    emit circleMoved();

    return true;
  }

  return false;
}

ColorGradientItem::ColorGradientItem(const QRectF &rect, QGraphicsRectItem *topRect,QGraphicsRectItem *bottomRect,SelectionArrowItem *topCircle, SelectionArrowItem *bottomCircle)
  :QGraphicsRectItem(rect),_initPos(rect.x(),rect.y()),_topRect(topRect),_bottomRect(bottomRect),_topCircle(topCircle),_bottomCircle(bottomCircle){
  setFlags(QGraphicsItem::ItemIsMovable);
}

bool ColorGradientItem::sceneEvent ( QEvent * event ){
  if(event->type()==QEvent::GraphicsSceneMouseMove){
    QGraphicsSceneMouseEvent *e=(QGraphicsSceneMouseEvent *)event;
    qreal diffPosY=e->pos().y()-e->lastPos().y();

    if(rect().bottom()+diffPosY>_initPos.y()+160)
      diffPosY=_initPos.y()+160-rect().bottom();
    if(rect().top()+diffPosY<_initPos.y())
      diffPosY=_initPos.y()-rect().top();

    QRectF coord=rect();
    coord.translate(0,diffPosY);
    setRect(coord);

    _topCircle->setPos(_initPos.x()+5,rect().bottom()-30);
    _bottomCircle->setPos(_initPos.x()+5,rect().top()-30);

    emit moved();

    return true;
  }

  return false;
}

}
