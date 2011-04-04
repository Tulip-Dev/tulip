#include "tulip/ToolbarItem.h"

#include "tulip/MirrorGraphicsEffect.h"
#include "tulip/HighlightGraphicsEffect.h"
#include "tulip/QtAnimationsManager.h"
#include <QtGui/QPainter>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QSequentialAnimationGroup>
#include <QtCore/QTimer>

namespace tlp {
ToolbarItem::ToolbarItem(QGraphicsItem *parent,QGraphicsScene *scene)
  : QGraphicsItemGroup(parent,scene),
  _activeAction(0), _activeButton(0), _focusedButton(0), _expanded(false), _currentExpandAnimation(0), _collapseTimeout(0),

  _iconSize(28,28), _hoveredIconSize(42,42), _margin(4), _orientation(Qt::Horizontal),

  _buttonMarginWidth(7), _buttonForegroundColor(QColor(200,200,200,150)), _buttonBackgroundColor(QColor(230,230,230,150)), _highlightColor(QColor(255,255,255,150)), _buttonBackgroundShape(PushButtonItem::CircleShape),

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
  QPointF pos(_margin,_margin);

  if (_activeButton) {
    _activeButton->setPos(pos);
    _activeButton->setIconSize(_hoveredIconSize);
    pos+=QPointF(translationVector().x()*_margin,translationVector().y()*_margin);
  }
  if (!_expanded)
    return;

  QSizeF hbSize = hoveredButtonSize();
  QPointF hoveredGap((_hoveredIconSize.width() - _iconSize.width())/2, (_hoveredIconSize.height() - _iconSize.height())/2);
  pos+= hoveredGap;

  for (int i=0; i < _actions.size(); ++i) {
    pos+=QPointF(translationVector().x() * (_margin + hbSize.width()),translationVector().y() * (_margin + hbSize.height()));

    PushButtonItem *btn = _actionButton[_actions[i]];
    if (btn->hovered()) {
      pos-=hoveredGap;
      btn->setAnimationBehavior(AnimatedGraphicsObject::StopPreviousAnimation);
      modifyButton(btn,_hoveredIconSize,pos);
      pos+=hoveredGap;
      btn->setAnimationBehavior(AnimatedGraphicsObject::ContinuePreviousAnimation);
    }

    modifyButton(btn,_iconSize,pos);
  }
}
//==========================
void ToolbarItem::modifyButton(PushButtonItem *btn, const QSize &newSize, const QPointF &newPos) const {
  btn->resizeItem(newSize,_animationMsec,_animationEasing);
  btn->moveItem(newPos,_animationMsec,_animationEasing);
}
//==========================
QRectF ToolbarItem::boundingRect() const {
  QSizeF hoveredIcon = hoveredButtonSize();
  QSizeF icon = buttonSize();
  if (!_expanded || _actions.size() == 0)
    return QRectF(QPointF(0,0), QSizeF(hoveredButtonSize().width()+_margin*2,hoveredButtonSize().height()+_margin*2));

  if (_orientation == Qt::Horizontal)
    return QRectF(0,0, (hoveredIcon.width()+_margin)*2 + (_margin + icon.width())*(_actions.size()-1)+_margin*2, hoveredIcon.height()+_margin*2);

  return QRectF(0,0,hoveredIcon.width()+_margin*2,(hoveredIcon.height()+_margin)*2 + (_margin + icon.height())*(_actions.size()-1)+_margin*2);

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
  return QSizeF(_hoveredIconSize.width() + (_buttonMarginWidth+3)*2, _hoveredIconSize.height() + (_buttonMarginWidth+3)*2);
}
//==========================
QSizeF ToolbarItem::buttonSize() const {
  return QSizeF(_iconSize.width() + (_buttonMarginWidth+3)*2, _iconSize.height() + (_buttonMarginWidth+3)*2);
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
