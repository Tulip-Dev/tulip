
#include "OrientedPushButton.h"
#include <QtGui/QStylePainter>
#include <QtGui/QMenu>

OrientedPushButton::OrientedPushButton(QWidget* parent)
  : QPushButton(parent) {
  init();
}

OrientedPushButton::OrientedPushButton(const QString& text, QWidget* parent)
  : QPushButton(text, parent) {
  init();
}

OrientedPushButton::OrientedPushButton(const QIcon& icon, const QString& text, QWidget* parent)
  : QPushButton(icon, text, parent) {
  init();
}

OrientedPushButton::~OrientedPushButton() {
}

void OrientedPushButton::init() {
  _orientation = Qt::Horizontal;
  _mirrored = false;
}

Qt::Orientation OrientedPushButton::orientation() const {
  return _orientation;
}

void OrientedPushButton::setOrientation(Qt::Orientation orientation) {
  _orientation = orientation;

  switch (orientation) {
  case Qt::Horizontal:
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    break;

  case Qt::Vertical:
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    break;
  }
}

bool OrientedPushButton::mirrored() const {
  return _mirrored;
}

void OrientedPushButton::setMirrored(bool mirrored) {
  _mirrored = mirrored;
}

QSize OrientedPushButton::sizeHint() const {
  QSize size = QPushButton::sizeHint();

  if (_orientation == Qt::Vertical)
    size.transpose();

  return size;
}

QSize OrientedPushButton::minimumSizeHint() const {
  QSize size = QPushButton::minimumSizeHint();

  if (_orientation == Qt::Vertical)
    size.transpose();

  return size;
}

void OrientedPushButton::paintEvent(QPaintEvent*) {
  QStylePainter p(this);

  switch (_orientation) {
  case Qt::Horizontal:

    if (_mirrored) {
      p.rotate(180);
      p.translate(-width(), -height());
    }

    break;

  case Qt::Vertical:

    if (_mirrored) {
      p.rotate(-90);
      p.translate(-height(), 0);
    }
    else {
      p.rotate(90);
      p.translate(0, -width());
    }

    break;
  }

  p.drawControl(QStyle::CE_PushButton, getStyleOption());
}

QStyleOptionButton OrientedPushButton::getStyleOption() const {
  QStyleOptionButton opt;
  opt.initFrom(this);

  if (_orientation == Qt::Vertical) {
    QSize size = opt.rect.size();
    size.transpose();
    opt.rect.setSize(size);
  }

  opt.features = QStyleOptionButton::None;

  if (isFlat())
    opt.features |= QStyleOptionButton::Flat;

  if (menu())
    opt.features |= QStyleOptionButton::HasMenu;

  if (autoDefault() || isDefault())
    opt.features |= QStyleOptionButton::AutoDefaultButton;

  if (isDefault())
    opt.features |= QStyleOptionButton::DefaultButton;

  if (isDown() || (menu() && menu()->isVisible()))
    opt.state |= QStyle::State_Sunken;

  if (isChecked())
    opt.state |= QStyle::State_On;

  if (!isFlat() && !isDown())
    opt.state |= QStyle::State_Raised;


  opt.text = text();
  opt.icon = icon();
  opt.iconSize = iconSize();
  return opt;
}
