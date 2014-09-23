/**
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
#include "tulip/CaptionGraphicsSubItems.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

#include <tulip/DoubleProperty.h>
#include <tulip/ColorProperty.h>

using namespace std;

namespace tlp {

CaptionGraphicsBackgroundItem::CaptionGraphicsBackgroundItem(const QRect &rect):QGraphicsRectItem(rect),_interactionsActivated(false),_beginBackup(0.),_endBackup(1.),_captionContentPos(50,20) {

  // Range selector 1
  _rangeSelector1Item=new SelectionArrowItem(0,_captionContentPos);
  _rangeSelector1Item->setPos(_captionContentPos+QPoint(5,-30));
  _rangeSelector1Item->setVisible(false);
  _rangeSelector1Item->setParentItem(this);
  connect(_rangeSelector1Item,SIGNAL(circleMoved()),this,SLOT(updateCaption()));

  // Range selector 2
  _rangeSelector2Item=new SelectionArrowItem(1,_captionContentPos);
  _rangeSelector2Item->setPos(_captionContentPos+QPoint(5,130));
  _rangeSelector2Item->setVisible(false);
  _rangeSelector2Item->setParentItem(this);
  connect(_rangeSelector2Item,SIGNAL(circleMoved()),this,SLOT(updateCaption()));

  // Range selector text 1
  _rangeSelector1TextItem=new SelectionTextItem();
  _rangeSelector1TextItem->setParentItem(_rangeSelector1Item);

  // Range selector text 2
  _rangeSelector2TextItem=new SelectionTextItem();
  _rangeSelector2TextItem->setParentItem(_rangeSelector2Item);

  // Caption Rect Border
  _captionRectBorder=new QGraphicsRectItem(QRect(_captionContentPos,QSize(30,160)));
  _captionRectBorder->setParentItem(this);

  QFont font;
  font.setBold(true);

  // MinMax Labels Item
  _minTextItem=new QGraphicsTextItem();
  _minTextItem->setFont(font);
  _minTextItem->setParentItem(this);
  _maxTextItem=new QGraphicsTextItem();
  _maxTextItem->setFont(font);
  _maxTextItem->setParentItem(this);
  _min2TextItem=new QGraphicsTextItem();
  _min2TextItem->setFont(font);
  _min2TextItem->setParentItem(this);
  _max2TextItem=new QGraphicsTextItem();
  _max2TextItem->setFont(font);
  _max2TextItem->setParentItem(this);
  _min2LineItem=new QGraphicsLineItem(this);
  _max2LineItem=new QGraphicsLineItem(this);
  _min2TextItem->setPos(_captionContentPos+QPoint(35,95));
  _max2TextItem->setPos(_captionContentPos+QPoint(35,40));
  _min2LineItem->setLine(QLineF(_captionContentPos+QPoint(-5,107),_captionContentPos+QPoint(35,107)));
  _min2LineItem->setZValue(2);
  _max2LineItem->setLine(QLineF(_captionContentPos+QPoint(-5,53),_captionContentPos+QPoint(35,53)));
  _max2LineItem->setZValue(2);


  // Color caption Items
  _topCaptionRectItem=new QGraphicsRectItem(QRect(_captionContentPos,QSize(30,0)));
  _topCaptionRectItem->setParentItem(this);
  _bottomCaptionRectItem=new QGraphicsRectItem(QRect(_captionContentPos+QPoint(0,160),QSize(30,0)));
  _bottomCaptionRectItem->setParentItem(this);
  _middleCaptionRectItem=new MovableRectItem(QRect(_captionContentPos,QSize(30,160)),QRect(0,0,1,1),_rangeSelector1Item,_rangeSelector2Item);
  connect(_middleCaptionRectItem,SIGNAL(moved(float, float)),this,SLOT(updateCaption(float, float)));
  _middleCaptionRectItem->setParentItem(this);

  // Size caption Items
  _topSizeCaptionPathItem=new QGraphicsPathItem();
  _topSizeCaptionPathItem->setBrush(QBrush(QColor(200,200,200,100)));
  _topSizeCaptionPathItem->setParentItem(this);
  _topSizeCaptionPathItem->setPos(_captionContentPos);
  _bottomSizeCaptionPathItem=new QGraphicsPathItem();
  _bottomSizeCaptionPathItem->setBrush(QBrush(QColor(200,200,200,100)));
  _bottomSizeCaptionPathItem->setParentItem(this);
  _bottomSizeCaptionPathItem->setPos(_captionContentPos);
  _sizeCaptionPathItem=new MovablePathItem(QRect(0,0,1,1),_topSizeCaptionPathItem,_bottomSizeCaptionPathItem,_rangeSelector1Item,_rangeSelector2Item);
  connect(_sizeCaptionPathItem,SIGNAL(moved(float, float)),this,SLOT(updateCaption(float, float)));
  _sizeCaptionPathItem->setBrush(QBrush(QColor(200,200,200,255)));
  _sizeCaptionPathItem->setParentItem(this);
  _sizeCaptionPathItem->setPos(_captionContentPos);


  setAcceptHoverEvents(true);
}

void CaptionGraphicsBackgroundItem::generateColorCaption(const QGradient &activeGradient, const QGradient &hideGradient, const string &/*propertyName*/, double minValue, double maxValue) {
  _minValue=minValue;
  _maxValue=maxValue;

  _middleCaptionRectItem->setBrush(QBrush(activeGradient));
  _topCaptionRectItem->setBrush(QBrush(hideGradient));
  _bottomCaptionRectItem->setBrush(QBrush(hideGradient));

  _sizeCaptionPathItem->setVisible(false);
  _topSizeCaptionPathItem->setVisible(false);
  _bottomSizeCaptionPathItem->setVisible(false);
  _middleCaptionRectItem->setVisible(true);
  _topCaptionRectItem->setVisible(true);
  _bottomCaptionRectItem->setVisible(true);
  _captionRectBorder->setVisible(true);

  _minTextItem->setPlainText(QString::number(_minValue));
  _maxTextItem->setPlainText(QString::number(_maxValue));
  _min2TextItem->setPlainText(QString::number(_minValue+(_maxValue-_minValue)/3.).left(5));
  _max2TextItem->setPlainText(QString::number(_maxValue-(_maxValue-_minValue)/3.).left(5));


  float begin = (_rangeSelector1Item->pos().y()-_captionContentPos.y()+30)/160.;
  float end = (_rangeSelector2Item->pos().y()-_captionContentPos.y()+30)/160.;

  if(begin>end) {
    float tmp=begin;
    begin=end;
    end=tmp;
  }

  updateSelectionText(begin,end);

  _minTextItem->setPos(_captionContentPos+QPoint(17-_minTextItem->boundingRect().width()/2.,157));
  _maxTextItem->setPos(_captionContentPos+QPoint(17-_maxTextItem->boundingRect().width()/2.,-22));
}

void CaptionGraphicsBackgroundItem::generateSizeCaption(const vector<pair<double,float> > &metricToSizeFilteredList, const string &/*propertyName*/, double minValue, double maxValue) {
  _minValue=minValue;
  _maxValue=maxValue;

  _sizeCaptionPathItem->setVisible(true);
  _topSizeCaptionPathItem->setVisible(true);
  _bottomSizeCaptionPathItem->setVisible(true);
  _middleCaptionRectItem->setVisible(false);
  _topCaptionRectItem->setVisible(false);
  _bottomCaptionRectItem->setVisible(false);
  _captionRectBorder->setVisible(false);

  _minTextItem->setPlainText(QString::number(_minValue));
  _maxTextItem->setPlainText(QString::number(_maxValue));
  _min2TextItem->setPlainText(QString::number(_minValue+(_maxValue-_minValue)/3.).left(5));
  _max2TextItem->setPlainText(QString::number(_maxValue-(_maxValue-_minValue)/3.).left(5));

  float begin = (_rangeSelector1Item->pos().y()-_captionContentPos.y()+30)/160.;
  float end = (_rangeSelector2Item->pos().y()-_captionContentPos.y()+30)/160.;

  if(begin>end) {
    float tmp=begin;
    begin=end;
    end=tmp;
  }

  updateSelectionText(begin,end);

  _minTextItem->setPos(_captionContentPos+QPoint(17-_minTextItem->boundingRect().width()/2.,157));
  _maxTextItem->setPos(_captionContentPos+QPoint(17-_maxTextItem->boundingRect().width()/2.,-22));

  _sizeCaptionPathItem->setDataToPath(metricToSizeFilteredList,minValue,maxValue);
}

void CaptionGraphicsBackgroundItem::updateSelectionText(float begin, float end) {
  QString text1=QString::number(_minValue+(1-begin)*(_maxValue-_minValue));

  if(text1.length()>5)
    text1=text1.left(5);

  QString text2=QString::number(_minValue+(1-end)*(_maxValue-_minValue));

  if(text2.length()>5)
    text2=text2.left(5);

  if(_rangeSelector1Item->pos().y()>_rangeSelector2Item->pos().y()) {
    _rangeSelector1TextItem->setPlainText(text2);
    _rangeSelector2TextItem->setPlainText(text1);
  }
  else {
    _rangeSelector1TextItem->setPlainText(text1);
    _rangeSelector2TextItem->setPlainText(text2);
  }

  _rangeSelector1TextItem->setPos(QPoint(60-_rangeSelector1TextItem->boundingRect().width()/2.,17));
  _rangeSelector2TextItem->setPos(QPoint(60-_rangeSelector2TextItem->boundingRect().width()/2.,17));

}

void CaptionGraphicsBackgroundItem::updateCaption() {
  float end = (_rangeSelector1Item->pos().y()-_captionContentPos.y()+30)/160.;
  float begin = (_rangeSelector2Item->pos().y()-_captionContentPos.y()+30)/160.;
  updateCaption(begin,end);
}

void CaptionGraphicsBackgroundItem::updateCaption(float begin ,float end) {

  _rangeSelector2Item->setPos(_captionContentPos.x()+5,begin*160.+_captionContentPos.y()-30);
  _rangeSelector1Item->setPos(_captionContentPos.x()+5,end*160.+_captionContentPos.y()-30);
  _sizeCaptionPathItem->setPos(_captionContentPos);
  _topSizeCaptionPathItem->setPos(_captionContentPos);
  _bottomSizeCaptionPathItem->setPos(_captionContentPos);

  _captionRectBorder->setPos(_captionContentPos-QPoint(50,20));

  if(!_interactionsActivated) {
    _minTextItem->setPos(QPointF(_captionContentPos+QPoint(17-_minTextItem->boundingRect().width()/2.,157)));
    _maxTextItem->setPos(QPointF(_captionContentPos+QPoint(17-_maxTextItem->boundingRect().width()/2.,-22)));
    _min2TextItem->setVisible(true);
    _max2TextItem->setVisible(true);
    _min2LineItem->setVisible(true);
    _max2LineItem->setVisible(true);
  }
  else {
    _minTextItem->setPos(QPointF(_captionContentPos+QPoint(-5,157)));
    _maxTextItem->setPos(QPointF(_captionContentPos+QPoint(-5,-22)));
    _min2TextItem->setVisible(false);
    _max2TextItem->setVisible(false);
    _min2LineItem->setVisible(false);
    _max2LineItem->setVisible(false);
  }

  if(begin>end) {
    float tmp=begin;
    begin=end;
    end=tmp;
  }

  updateSelectionText(begin,end);

  _topCaptionRectItem->setRect(QRect(_captionContentPos,QSize(30,begin*160)));
  _middleCaptionRectItem->setRect(QRect(_captionContentPos+QPoint(0,begin*160),QSize(30,(end-begin)*160)));
  _middleCaptionRectItem->setInternalRect(QRectF(0,begin,0,end-begin));
  _bottomCaptionRectItem->setRect(QRect(_captionContentPos+QPoint(0,end*160),QSize(30,160-end*160)));

  _sizeCaptionPathItem->setRect(QRectF(0,begin,0,end-begin));

  emit filterChanged(1.-end,1.-begin);
}

bool CaptionGraphicsBackgroundItem::sceneEvent ( QEvent * event ) {
  if(event->type()==QEvent::GraphicsSceneMousePress) {
    if(((QGraphicsSceneMouseEvent*)event)->button()==Qt::LeftButton) {
      activateInteractions(!_interactionsActivated);
      return true;
    }
  }

  return QGraphicsRectItem::sceneEvent(event);
}

void CaptionGraphicsBackgroundItem::activateInteractions() {
  activateInteractions(true);
}

void CaptionGraphicsBackgroundItem::removeInteractions() {
  activateInteractions(false);
}

void CaptionGraphicsBackgroundItem::activateInteractions(bool activate) {

  if(activate==_interactionsActivated)
    return;

  if(activate)
    emit interactionsActivated();
  else
    emit interactionsRemoved();

  if(activate==false && activate!=_interactionsActivated) {
    _endBackup = (_rangeSelector1Item->pos().y()-_captionContentPos.y()+30)/160.;
    _beginBackup = (_rangeSelector2Item->pos().y()-_captionContentPos.y()+30)/160.;
  }

  bool useStoredBeginEnd=false;

  if(activate==true && activate!=_interactionsActivated) {
    useStoredBeginEnd=true;
  }

  _interactionsActivated=activate;

  if(activate)
    _captionContentPos=QPoint(10,20);
  else
    _captionContentPos=QPoint(50,20);

  _rangeSelector1Item->setVisible(activate);
  _rangeSelector2Item->setVisible(activate);

  if(useStoredBeginEnd)
    updateCaption(_beginBackup,_endBackup);
  else
    updateCaption(0.,1.);
}

void CaptionGraphicsBackgroundItem::configurationIconPressedSlot() {
  emit configurationIconPressed();
}

SelectionArrowItem::SelectionArrowItem(float initRangePos,const QPoint &initPos):
  QGraphicsPathItem(),yPos(initRangePos*160),initPos(initPos) {

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

bool SelectionArrowItem::sceneEvent ( QEvent * event ) {
  if(event->type()==QEvent::GraphicsSceneMouseMove) {
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

MovableRectItem::MovableRectItem(const QRectF &rect,const QRectF &size, SelectionArrowItem *topCircle, SelectionArrowItem *bottomCircle)
  :QGraphicsRectItem(rect),_currentRect(size),_initPos(rect.x(),rect.y()),_topCircle(topCircle),_bottomCircle(bottomCircle) {
  setFlags(QGraphicsItem::ItemIsMovable);
}

bool MovableRectItem::sceneEvent ( QEvent * event ) {
  if(event->type()==QEvent::GraphicsSceneMouseMove) {
    QGraphicsSceneMouseEvent *e=(QGraphicsSceneMouseEvent *)event;
    qreal diffPosY=e->pos().y()-e->lastPos().y();

    if(_currentRect.bottom()*160+diffPosY>160)
      diffPosY=160-_currentRect.bottom()*160;

    if(_currentRect.top()*160+diffPosY<0)
      diffPosY=-_currentRect.top()*160;

    _currentRect.translate(0,diffPosY/160.);

    setRect(_initPos.x(),_initPos.y(),_currentRect.width()*160,_currentRect.height()*160);

    emit moved(_currentRect.top(),_currentRect.bottom());

    return true;
  }

  return false;
}

void MovableRectItem::setInternalRect(const QRectF &rect) {
  _currentRect=rect;
}


MovablePathItem::MovablePathItem(const QRectF &rect, QGraphicsPathItem *topPathItem, QGraphicsPathItem *bottomPathItem, SelectionArrowItem *topCircle, SelectionArrowItem *bottomCircle)
  :QGraphicsPathItem(),_currentRect(rect),_topPathItem(topPathItem),_bottomPathItem(bottomPathItem),_topCircle(topCircle),_bottomCircle(bottomCircle) {
  setFlags(QGraphicsItem::ItemIsMovable);
}

void MovablePathItem::setDataToPath(const vector<pair<double,float> > &metricToSizeFilteredList, double minMetric, double maxMetric) {
  _metricToSizeFilteredList=metricToSizeFilteredList;
  _minMetric=minMetric;
  _maxMetric=maxMetric;
  updatePath();
}

void MovablePathItem::setRect(const QRectF &rect) {
  _currentRect=rect;
  updatePath();
}

float computeToto(float y,float x1, float x2, float y1, float y2) {
  return x1+((x2-x1)/(y2-y1))*(y-y1);
}

void MovablePathItem::updatePath() {
  if(_metricToSizeFilteredList.empty())
    return ;

  vector<QPainterPath> paths;
  paths.resize(3);
  vector<vector<QPoint> >pathsPoints;
  pathsPoints.resize(3);

  double secondLimit=_minMetric+(1.-_currentRect.y())*(_maxMetric-_minMetric);
  double firstLimit=_minMetric+(1.-(_currentRect.y()+_currentRect.height()))*(_maxMetric-_minMetric);

  pair<double,float> lastValue=*_metricToSizeFilteredList.begin();
  int state=0;

  for(vector<pair<double,float> >::iterator it=_metricToSizeFilteredList.begin(); it!=_metricToSizeFilteredList.end(); ++it) {
    if((*it).first<firstLimit) {
      if(state==0) {
        //init of first path
        pathsPoints[0].push_back(QPoint(15+15.*(*it).second,160));
        state=1;
      }
      else {
        pathsPoints[0].push_back(QPoint(15+15*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
      }
    }
    else if((*it).first<=secondLimit) {
      if(state==1) {
        //init of second path
        float midValue=computeToto(firstLimit,lastValue.second,(*it).second,lastValue.first,(*it).first);
        pathsPoints[0].push_back(QPoint(15+15.*midValue,160-160*(firstLimit-_minMetric)/(_maxMetric-_minMetric)));
        pathsPoints[1].push_back(QPoint(15+15.*midValue,160-160*(firstLimit-_minMetric)/(_maxMetric-_minMetric)));
        pathsPoints[1].push_back(QPoint(15+15.*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
        state=2;
      }
      else if(state<=1) {
        pathsPoints[1].push_back(QPoint(15+15.*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
        state=2;
      }
      else {
        pathsPoints[1].push_back(QPoint(15+15*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
      }

    }
    else {
      if(state==2) {
        //init of third path
        float midValue=computeToto(secondLimit,lastValue.second,(*it).second,lastValue.first,(*it).first);
        pathsPoints[1].push_back(QPoint(15+15.*midValue,160-160*(secondLimit-_minMetric)/(_maxMetric-_minMetric)));
        pathsPoints[2].push_back(QPoint(15+15.*midValue,160-160*(secondLimit-_minMetric)/(_maxMetric-_minMetric)));
        pathsPoints[2].push_back(QPoint(15+15.*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
        state=3;
      }
      else {
        pathsPoints[2].push_back(QPoint(15+15*(*it).second,160-160*((*it).first-_minMetric)/(_maxMetric-_minMetric)));
      }
    }

    lastValue=(*it);
  }

  if(state==2) {
    pathsPoints[1].push_back(QPoint(15+15*lastValue.second,160-160*(lastValue.first-_minMetric)/(_maxMetric-_minMetric)));
  }

  for(unsigned int i1=0; i1<pathsPoints.size(); ++i1) {
    for(unsigned int i2=0; i2<pathsPoints[i1].size(); ++i2) {
      if(i2==0) {
        paths[i1].moveTo(pathsPoints[i1][i2]);
      }
      else {
        paths[i1].lineTo(pathsPoints[i1][i2]);
      }
    }

    for(int i2=(int)pathsPoints[i1].size()-1; i2>=0; --i2) {
      paths[i1].lineTo(QPoint(30-pathsPoints[i1][i2].x(),pathsPoints[i1][i2].y()));
    }

    if(!pathsPoints[i1].empty()) {
      paths[i1].lineTo(QPoint(30-pathsPoints[i1][0].x(),pathsPoints[i1][0].y()));
      paths[i1].lineTo(pathsPoints[i1][0]);
    }
  }


  _topPathItem->setPath(paths[0]);
  setPath(paths[1]);
  _bottomPathItem->setPath(paths[2]);
}

bool MovablePathItem::sceneEvent ( QEvent * event ) {
  if(event->type()==QEvent::GraphicsSceneMouseMove) {
    QGraphicsSceneMouseEvent *e=(QGraphicsSceneMouseEvent *)event;
    qreal diffPosY=e->pos().y()-e->lastPos().y();

    if(_currentRect.bottom()*160+diffPosY>160)
      diffPosY=160-_currentRect.bottom()*160;

    if(_currentRect.top()*160+diffPosY<0)
      diffPosY=-_currentRect.top()*160;

    updatePath();
    _currentRect.translate(0,diffPosY/160.);

    emit moved(_currentRect.top(),_currentRect.bottom());

    return true;
  }

  return false;
}

SelectionTextItem::SelectionTextItem() {
  QFont f=font();
  f.setBold(true);
  setFont(f);
}

}
