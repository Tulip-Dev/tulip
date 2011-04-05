#include "tulip/ToolbarItem.h"

#include "tulip/MirrorGraphicsEffect.h"
#include "tulip/HighlightGraphicsEffect.h"
#include "tulip/QtAnimationsManager.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QSequentialAnimationGroup>
#include <QtCore/QTimer>
#include <assert.h>

namespace tlp {
ToolbarItem::ToolbarItem(QGraphicsItem *parent,QGraphicsScene *scene)
  : QGraphicsItemGroup(parent,scene),
  _activeAction(0), _activeButton(0), _focusedButton(0), _expanded(false), _currentExpandAnimation(0), _collapseTimeout(0),
  _iconSize(32,32), _hoveredIconSize(42,42), _margin(0), _orientation(Qt::Horizontal),
  _buttonMarginWidth(0), _buttonBackgroundShape(PushButtonItem::NoShape),
  _animationMsec(100), _animationEasing(QEasingCurve::Linear) {
  setHandlesChildEvents(false);
  setCacheMode(QGraphicsItem::ItemCoordinateCache);
  setAcceptHoverEvents(true);
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
}
//==========================
void ToolbarItem::setActiveAction(QAction *action) {
  _activeAction = action;
  if (_activeAction) {
    if (_activeButton) {
      _activeButton->setAction(action);
      QPropertyAnimation *anim1 = new QPropertyAnimation(_activeButton,"borderColor",this);
      anim1->setStartValue(_buttonForegroundColor);
      anim1->setEndValue(_highlightColor);
      anim1->setDuration(1000);
      anim1->setEasingCurve(QEasingCurve::Linear);
      QPropertyAnimation *anim2 = new QPropertyAnimation(_activeButton,"borderColor",this);
      anim2->setStartValue(anim1->endValue());
      anim2->setEndValue(anim1->startValue());
      anim2->setEasingCurve(anim1->easingCurve());
      anim2->setDuration(anim1->duration());
      QSequentialAnimationGroup *anim = new QSequentialAnimationGroup(this);
      anim->addAnimation(anim1);
      anim->addAnimation(anim2);
      anim->setLoopCount(3);
      QtAnimationsManager::instance().startAnimation(_activeButton,anim,QtAnimationsManager::StopPreviousAnimation);
    }
    else
      _activeButton = buildButton(_activeAction);
  }
  else {
    delete _activeButton;
    _activeButton=0;
  }
}
//==========================
PushButtonItem *ToolbarItem::buildButton(QAction *action) {
  PushButtonItem *result = new PushButtonItem(action,_iconSize,this);
  result->setAnimationBehavior(AnimatedGraphicsObject::ContinuePreviousAnimation);
  result->setPos(_margin,_margin);
  result->setBorderColor(_buttonForegroundColor);
  result->setBackgroundColor(_buttonBackgroundColor);
  result->setBorderWidth(_buttonMarginWidth);
  result->setBackgroundShape(_buttonBackgroundShape);
  connect(result,SIGNAL(hovered(bool)),this,SLOT(buttonHovered(bool)),Qt::DirectConnection);
  connect(result,SIGNAL(clicked()),this,SLOT(buttonClicked()),Qt::DirectConnection);
  return result;
}
//==========================
void ToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->setPen(Qt::black);
  painter->drawRect(boundingRect());

  QPointF pos(_margin,_margin);
  QSizeF hbSize = hoveredButtonSize();
  QPointF hoveredGap((_hoveredIconSize.width() - _iconSize.width())/2, (_hoveredIconSize.height() - _iconSize.height())/2);

  if (_activeButton) {
    _activeButton->setPos(pos);
    _activeButton->setIconSize(_hoveredIconSize);
    pos+=QPointF(translationVector().x() * (_margin + hbSize.width()),translationVector().y() * (_margin + hbSize.height()));
    //separator
    pos+=QPointF(translationVector().x()*_margin,translationVector().y()*_margin);
  }

  if (!_expanded)
    return;

  pos+= hoveredGap;
  for (int i=0; i < _actions.size(); ++i) {
    PushButtonItem *btn = _actionButton[_actions[i]];
    if (btn->hovered()) {
      pos-=hoveredGap;
      btn->setAnimationBehavior(AnimatedGraphicsObject::StopPreviousAnimation);
      modifyButton(btn,_hoveredIconSize,pos);
      pos+=hoveredGap;
      btn->setAnimationBehavior(AnimatedGraphicsObject::ContinuePreviousAnimation);
    }
    else
      modifyButton(btn,_iconSize,pos);
    pos+=QPointF(translationVector().x() * (_margin + hbSize.width()),translationVector().y() * (_margin + hbSize.height()));
  }
}
//==========================
void ToolbarItem::modifyButton(PushButtonItem *btn, const QSize &newSize, const QPointF &newPos) const {
  btn->resizeItem(newSize,_animationMsec,_animationEasing);
  btn->moveItem(newPos,_animationMsec,_animationEasing);
}
//==========================
QRectF ToolbarItem::boundingRect() const {
  QSizeF hbSize = hoveredButtonSize();
  QPointF tVect = translationVector();
  QSizeF size(_margin*2+hbSize.width(),_margin*2+hbSize.height());
  if (_expanded) {
    size = QSizeF(size.width() + _actions.size() * tVect.x() * (_margin + hbSize.width()),
                  size.height() + _actions.size() *  tVect.y() * (_margin + hbSize.height()));
    size = QSizeF(size.width() + tVect.x() * _margin, size.height() + tVect.y() * _margin);
  }

  return QRectF(QPointF(0,0),size);
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
QSizeF ToolbarItem::hoveredButtonSize() const {
  if (!_activeButton)
    return QSizeF();
  return _activeButton->boundingRect().size();
}
//==========================
QSizeF ToolbarItem::buttonSize() const {
  if (!_activeButton)
    return QSizeF();
  PushButtonItem *btn = 0;
  for (int i=0;i<_actions.size();++i) {
    PushButtonItem *btn = _actionButton[_actions[i]];
    if (!btn->hovered())
      return btn->boundingRect().size();
  }
  assert(btn);
  return btn->boundingRect().size() - (_hoveredIconSize - _iconSize);
}
//==========================
void ToolbarItem::buttonHovered(bool f) {
  _focusedButton = static_cast<PushButtonItem *>(sender());
  update();
}
//==========================
void ToolbarItem::buttonClicked() {
  PushButtonItem *btn = static_cast<PushButtonItem *>(sender());
  if (btn != _activeButton)
    setActiveAction(btn->action());
  if (btn == _activeButton)
    emit activeButtonClicked();
  else
    emit buttonClicked(btn);
}
//==========================
void ToolbarItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  if (_collapseTimeout) {
    delete _collapseTimeout;
    _collapseTimeout = 0;
  }
  for (int i=0; i < _actions.size(); ++i)
    _actionButton[_actions[i]]->setAnimationBehavior(AnimatedGraphicsObject::ContinuePreviousAnimation);

  setExpanded(true);
}
//==========================
void ToolbarItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  if (_collapseTimeout) {
    delete _collapseTimeout;
    _collapseTimeout = 0;
  }
  _collapseTimeout = new QTimer(this);
  _collapseTimeout->setSingleShot(true);
  _collapseTimeout->setInterval(2000);
  connect(_collapseTimeout,SIGNAL(timeout()),this,SLOT(collapse()));
  _collapseTimeout->start();
  for (int i=0; i < _actions.size(); ++i)
    _actionButton[_actions[i]]->setAnimationBehavior(AnimatedGraphicsObject::StopPreviousAnimation);
  _focusedButton = 0;
  update();
}
//==========================
void ToolbarItem::setExpanded(bool f) {
  if (_currentExpandAnimation)
    _currentExpandAnimation->stop();

  if (_expanded == f)
    return;

  _expanded = f;
  emit expanded(f);

  _currentExpandAnimation = new QParallelAnimationGroup();
  for (QMap<QAction *,PushButtonItem *>::iterator it = _actionButton.begin(); it != _actionButton.end(); ++it) {
    PushButtonItem *btn = it.value();
    if (_expanded)
      btn->setVisible(true);

    btn->setAcceptHoverEvents(false);

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
}
//==========================
void ToolbarItem::expandAnimationFinished() {
  _currentExpandAnimation = 0;
  for (QMap<QAction *,PushButtonItem *>::iterator it = _actionButton.begin(); it != _actionButton.end(); ++it) {
    it.value()->setVisible(_expanded);
    it.value()->setAcceptHoverEvents(true);
  }
}
//==========================
void ToolbarItem::collapse() {
  if (_collapseTimeout) {
    delete _collapseTimeout;
    _collapseTimeout = 0;
  }
  setExpanded(false);
}
}
