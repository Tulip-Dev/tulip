#include "tulip/ToolbarItem.h"

#include "tulip/PushButtonItem.h"
#include "tulip/MirrorGraphicsEffect.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <assert.h>

namespace tlp {
ToolbarItem::ToolbarItem(QGraphicsItem *parent,QGraphicsScene *scene)
  : QGraphicsItemGroup(parent,scene),
  _activeAction(0), _activeButton(0), _expanded(false), _currentExpandAnimation(0),
  _iconSize(32,32), _margin(6), _orientation(Qt::Horizontal), _animationMsec(100), _animationEasing(QEasingCurve::Linear),
  _outerOutline(0x9c,0x9a,0x94,0x00), _innerOutline(0xb3,0xaf,0xaf), _backgroundGradientStep1(0xdc,0xda,0xd5,0xd2), _backgroundGradientStep2(0xdc,0xda,0xd5,0x64) {
  setHandlesChildEvents(false);
  setCacheMode(QGraphicsItem::ItemCoordinateCache);
}
//==========================
ToolbarItem::~ToolbarItem() {
  if (_currentExpandAnimation)
    _currentExpandAnimation->stop();
}
//==========================
void ToolbarItem::addAction(QAction *action) {
  if (_actions.contains(action))
    return;
  _actions.push_back(action);
  PushButtonItem *btn = buildButton(action);
  _actionButton[action] = btn;
  if (!_expanded)
    btn->setVisible(false);

  if (!_activeAction)
    setActiveAction(action);

  layout();
}
//==========================
void ToolbarItem::removeAction(QAction *action) {
  if (!_actions.contains(action))
    return;
  _actions.remove(_actions.indexOf(action));

  PushButtonItem *btn = _actionButton[action];
  delete btn;
  _actionButton.remove(action);

  if (action == _activeAction) {
    if (_actions.size() > 0)
      setActiveAction(_actions[0]);
    else
      setActiveAction(0);
  }
  layout();
}
//==========================
void ToolbarItem::setActiveAction(QAction *action) {
  _activeAction = action;
  if (_activeAction) {
    if (_activeButton)
      _activeButton->setAction(action);
    else
      _activeButton = buildButton(_activeAction);
  }
  else {
    delete _activeButton;
    _activeButton=0;
  }
}
//==========================
void ToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//  painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

  QRectF outerRect = computeBoundingRect();

  { // Enclosing rect
    QPen pen;
    pen.setColor(_outerOutline);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRoundedRect(outerRect,7,7);

    QRectF innerRect(1,1,outerRect.width()-2,outerRect.height()-2);
    pen.setColor(_innerOutline);
    pen.setWidth(1);
    painter->setPen(pen);
    QLinearGradient gradient(0,0,0,innerRect.height());
    gradient.setColorAt(0,_backgroundGradientStep1);
    gradient.setColorAt(1,_backgroundGradientStep2);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(innerRect,7,7);
  }
  if (_expanded) { // separator
    QPointF marginVector = _margin * translationVector();
    QPointF iconVector(_iconSize.width() * translationVector().x(), _iconSize.height() * translationVector().y());

    QPointF start = QPointF(_margin,_margin) + iconVector + marginVector;
    QPointF end(start.x() + _iconSize.width() * translationVector().y(), start.y() + _iconSize.height() * translationVector().x());
    QPen pen;
    QLinearGradient gradient(start,end);
    gradient.setColorAt(0,_outerOutline);
    gradient.setColorAt(0.2, _innerOutline);
    gradient.setColorAt(0.8, _innerOutline);
    gradient.setColorAt(1,_outerOutline);
    pen.setBrush(QBrush(gradient));
    painter->setPen(pen);
    painter->drawLine(start,end);
  }
  QGraphicsItemGroup::paint(painter,option,widget);
}
//==========================
QRectF ToolbarItem::boundingRect() const {
  return computeBoundingRect();
}
//==========================
void ToolbarItem::layout() {
  if (_actions.empty() || !_expanded)
    return;

  // Position items
  QPointF marginVector = _margin * translationVector();
  QPointF pos(_margin,_margin);
  QPointF iconVector(_iconSize.width() * translationVector().x(), _iconSize.height() * translationVector().y());

  _activeButton->moveItem(pos, _animationMsec, _animationEasing);
  pos += iconVector + marginVector * 2;
  for (int i=0;i < _actions.size(); ++i) {
    PushButtonItem *btn = _actionButton[_actions[i]];

    if (btn->hovered()) {
        pos-=QPointF(_margin,_margin);
        modifyButton(btn,hoveredIconSize(),pos);
        pos+=QPointF(_margin,_margin);
    }

    else
      modifyButton(btn,_iconSize,pos);

    pos += iconVector + marginVector;
  }
}
//==========================
QPointF ToolbarItem::translationVector() const {
  switch (_orientation) {
  case Qt::Horizontal:
    return QPointF(1,0);
    break;
  case Qt::Vertical:
    return QPointF(0,1);
    break;
  }
}
//==========================
QSize ToolbarItem::hoveredIconSize() const {
  return QSize(_iconSize.width() + 2 * (_margin - 1), _iconSize.height() + 2 * (_margin - 1));
}
//==========================
QRectF ToolbarItem::computeBoundingRect() const {
  if (!_expanded)
    return QRectF(0,0,hoveredIconSize().width(), hoveredIconSize().height());

  if (_orientation == Qt::Horizontal)
    return QRectF(0,0,(_actions.size() + 1) * (_margin + _iconSize.width()) + _margin*2, hoveredIconSize().height());
  return QRectF(0,0,hoveredIconSize().width(),(_actions.size() + 1) * (_margin + _iconSize.height()) + _margin*2);
}
//==========================
PushButtonItem *ToolbarItem::buildButton(QAction *action) {
  PushButtonItem *result = new PushButtonItem(action,_iconSize,this);
  result->setAnimationBehavior(AnimatedGraphicsObject::ContinuePreviousAnimation); // smooth up animations
  result->setGraphicsEffect(new MirrorGraphicsEffect(-1 * _margin, _margin+3));
  result->setPos(_margin,_margin);
  connect(result,SIGNAL(hovered(bool)),this,SLOT(buttonHovered(bool)),Qt::DirectConnection);
  connect(result,SIGNAL(clicked()),this,SLOT(buttonClicked()),Qt::DirectConnection);
  return result;
}
//==========================
void ToolbarItem::buttonHovered(bool f) {
  layout();
}
//==========================
void ToolbarItem::buttonClicked() {
  PushButtonItem *btn = static_cast<PushButtonItem *>(sender());
  setActiveAction(btn->action());

  if (btn == _activeButton)
    setExpanded(!_expanded);
}
//==========================
void ToolbarItem::modifyButton(PushButtonItem *btn, const QSize &newSize, const QPointF &newPos) const {
  btn->resizeItem(newSize,_animationMsec,_animationEasing);
  btn->moveItem(newPos,_animationMsec,_animationEasing);
}
//==========================
void ToolbarItem::setExpanded(bool f) {
  if (_currentExpandAnimation)
    _currentExpandAnimation->stop();

  _expanded = f;

  _currentExpandAnimation = new QParallelAnimationGroup();
  for (QMap<QAction *,PushButtonItem *>::iterator it = _actionButton.begin(); it != _actionButton.end(); ++it) {
    PushButtonItem *btn = it.value();
    if (_expanded)
      btn->setVisible(true);

    if (!f) {
      QPropertyAnimation *posAnim = new QPropertyAnimation(btn,"pos");
      posAnim->setStartValue(btn->pos());
      posAnim->setEndValue(QPointF(_margin,_margin));
      posAnim->setEasingCurve(_animationEasing);
      posAnim->setDuration(_animationMsec);
      _currentExpandAnimation->addAnimation(posAnim);
    }

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(btn,"opacity");
    fadeAnim->setStartValue(btn->opacity());
    fadeAnim->setEndValue((f ? 1 : 0));
    fadeAnim->setEasingCurve(_animationEasing);
    fadeAnim->setDuration(_animationMsec);
    _currentExpandAnimation->addAnimation(fadeAnim);
  }

  connect(_currentExpandAnimation, SIGNAL(finished()), this, SLOT(expandAnimationFinished()));
  _currentExpandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  layout();
}
//==========================
void ToolbarItem::expandAnimationFinished() {
  _currentExpandAnimation = 0;
  for (QMap<QAction *,PushButtonItem *>::iterator it = _actionButton.begin(); it != _actionButton.end(); ++it)
    it.value()->setVisible(_expanded);
}
}
