#include "tulip/ToolbarItem.h"

#include "tulip/PushButtonItem.h"

namespace tlp {
ToolbarItem::ToolbarItem(QGraphicsItem *parent,QGraphicsScene *scene)
  : QGraphicsItemGroup(parent,scene),
  _activeAction(0), _activeButton(0),
  _iconSize(32,32), _margin(5), _orientation(Qt::Horizontal), _animationMsec(100), _animationEasing(QEasingCurve::Linear) {
  setHandlesChildEvents(false);
}
//==========================
ToolbarItem::~ToolbarItem() {
}
//==========================
void ToolbarItem::addAction(QAction *action) {
  if (_actions.contains(action))
    return;
  _actions.push_back(action);
  _actionButton[action] = new PushButtonItem(action,_iconSize, this);

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
  delete _activeButton;
  _activeButton = 0;
  if (_activeAction)
    _activeButton = new PushButtonItem(_activeAction,_iconSize,this);
}
//==========================
void ToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QGraphicsItemGroup::paint(painter,option,widget);
}
//==========================
QRectF ToolbarItem::boundingRect() const {
  QRectF result = QRectF(0,0,(_actions.size() + 1) * (_margin + _iconSize.width()) + _margin, hoveredIconSize().height());

  // Position items
  QPointF pos(0,_margin);
  QPointF marginVector = _margin * translationVector();
  QPointF iconVector(_iconSize.width() * translationVector().x(), _iconSize.height() * translationVector().y());

  pos += marginVector;
  _activeButton->moveItem(pos, _animationMsec, _animationEasing);
  pos += iconVector + marginVector * 2;
  for (int i=0;i < _actions.size(); ++i) {
    _actionButton[_actions[i]]->moveItem(pos,_animationMsec, _animationEasing);
    pos += iconVector + marginVector;
  }

  return result;
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
QSizeF ToolbarItem::hoveredIconSize() const {
  return QSizeF(_iconSize.width() + 2 * (_margin - 1), _iconSize.height() + 2 * (_margin - 1));
}
}
