/**
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

#include "tulip/RangeSlider.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QStyleOptionSlider>

namespace tlp {

RangeSlider::RangeSlider(QWidget *parent)
    : QSlider(parent), lower(0), upper(0), lowerPos(0), upperPos(0), offset(0), position(0),
      lastPressed(NoHandle), mainControl(LowerHandle), lowerPressed(QStyle::SC_None),
      upperPressed(QStyle::SC_None), movement(FreeMovement), firstMovement(false),
      blockTracking(false) {
  connect(this, SIGNAL(rangeChanged(int, int)), this, SLOT(updateRange(int, int)));
  connect(this, SIGNAL(sliderReleased()), this, SLOT(movePressedHandle()));
}

RangeSlider::RangeSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent), lower(0), upper(0), lowerPos(0), upperPos(0), offset(0),
      position(0), lastPressed(NoHandle), mainControl(LowerHandle), lowerPressed(QStyle::SC_None),
      upperPressed(QStyle::SC_None), movement(FreeMovement), firstMovement(false),
      blockTracking(false) {
  connect(this, SIGNAL(rangeChanged(int, int)), this, SLOT(updateRange(int, int)));
  connect(this, SIGNAL(sliderReleased()), this, SLOT(movePressedHandle()));
}

void RangeSlider::initStyleOption(QStyleOptionSlider *option, RangeHandle handle) const {
  if (!option)
    return;

  option->initFrom(this);
  option->subControls = QStyle::SC_None;
  option->activeSubControls = QStyle::SC_None;
  option->orientation = orientation();
  option->maximum = maximum();
  option->minimum = minimum();
  option->tickPosition = tickPosition();
  option->tickInterval = tickInterval();
  option->upsideDown = (orientation() == Qt::Horizontal)
                           ? (invertedAppearance() != (option->direction == Qt::RightToLeft))
                           : (!invertedAppearance());
  option->direction = Qt::LeftToRight; // we use the upsideDown option instead
  option->sliderPosition = (handle == LowerHandle ? lowerPos : upperPos);
  option->sliderValue = (handle == LowerHandle ? lower : upper);
  option->singleStep = singleStep();
  option->pageStep = pageStep();

  if (orientation() == Qt::Horizontal)
    option->state |= QStyle::State_Horizontal;
}

int RangeSlider::pixelPosToRangeValue(int pos) const {
  QStyleOptionSlider opt;
  initStyleOption(&opt);

  int sliderMin = 0;
  int sliderMax = 0;
  int sliderLength = 0;
  const QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
  const QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

  if (orientation() == Qt::Horizontal) {
    sliderLength = sr.width();
    sliderMin = gr.x();
    sliderMax = gr.right() - sliderLength + 1;
  } else {
    sliderLength = sr.height();
    sliderMin = gr.y();
    sliderMax = gr.bottom() - sliderLength + 1;
  }

  return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                         sliderMax - sliderMin, opt.upsideDown);
}

void RangeSlider::handleMousePress(const QPoint &pos, QStyle::SubControl &control, int value,
                                   RangeHandle handle) {
  QStyleOptionSlider opt;
  initStyleOption(&opt, handle);
  const QStyle::SubControl oldControl = control;
  control = style()->hitTestComplexControl(QStyle::CC_Slider, &opt, pos, this);
  const QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

  if (control == QStyle::SC_SliderHandle) {
    position = value;
    offset = pick(pos - sr.topLeft());
    lastPressed = handle;
    setSliderDown(true);
  }

  if (control != oldControl)
    update(sr);
}

void RangeSlider::setupPainter(QPainter *painter, Qt::Orientation orientation, qreal x1, qreal y1,
                               qreal x2, qreal y2) const {
  QColor highlight = palette().color(QPalette::Highlight);
  QLinearGradient gradient(x1, y1, x2, y2);
  gradient.setColorAt(0, highlight.dark(120));
  gradient.setColorAt(1, highlight.light(108));
  painter->setBrush(gradient);

  if (orientation == Qt::Horizontal)
    painter->setPen(QPen(highlight.dark(130), 0));
  else
    painter->setPen(QPen(highlight.dark(150), 0));
}

void RangeSlider::drawRange(QStylePainter *painter, const QRect &rect) const {
  QStyleOptionSlider opt;
  initStyleOption(&opt);

  // area
  QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

  if (opt.orientation == Qt::Horizontal)
    groove.adjust(0, 0, -1, 0);
  else
    groove.adjust(0, 0, 0, -1);

  // pen & brush
  painter->setPen(QPen(palette().color(QPalette::Dark).light(110), 0));

  if (opt.orientation == Qt::Horizontal)
    setupPainter(painter, opt.orientation, groove.center().x(), groove.top(), groove.center().x(),
                 groove.bottom());
  else
    setupPainter(painter, opt.orientation, groove.left(), groove.center().y(), groove.right(),
                 groove.center().y());

  // draw groove
  painter->drawRect(rect.intersected(groove));
}

void RangeSlider::drawHandle(QStylePainter *painter, RangeHandle handle) const {
  QStyleOptionSlider opt;
  initStyleOption(&opt, handle);
  opt.subControls = QStyle::SC_SliderHandle;
  QStyle::SubControl pressed = (handle == LowerHandle ? lowerPressed : upperPressed);

  if (pressed == QStyle::SC_SliderHandle) {
    opt.activeSubControls = pressed;
    opt.state |= QStyle::State_Sunken;
  }

  painter->drawComplexControl(QStyle::CC_Slider, opt);
}

void RangeSlider::triggerAction(QAbstractSlider::SliderAction action, bool main) {
  int value = 0;
  bool no = false;
  bool up = false;
  const int min = minimum();
  const int max = maximum();
  const RangeHandle altControl = (mainControl == LowerHandle ? UpperHandle : LowerHandle);

  blockTracking = true;

  switch (action) {
  case QAbstractSlider::SliderSingleStepAdd:
    if ((main && mainControl == UpperHandle) || (!main && altControl == UpperHandle)) {
      value = qBound(min, upper + singleStep(), max);
      up = true;
      break;
    }

    value = qBound(min, lower + singleStep(), max);
    break;

  case QAbstractSlider::SliderSingleStepSub:
    if ((main && mainControl == UpperHandle) || (!main && altControl == UpperHandle)) {
      value = qBound(min, upper - singleStep(), max);
      up = true;
      break;
    }

    value = qBound(min, lower - singleStep(), max);
    break;

  case QAbstractSlider::SliderToMinimum:
    value = min;

    if ((main && mainControl == UpperHandle) || (!main && altControl == UpperHandle))
      up = true;

    break;

  case QAbstractSlider::SliderToMaximum:
    value = max;

    if ((main && mainControl == UpperHandle) || (!main && altControl == UpperHandle))
      up = true;

    break;

  case QAbstractSlider::SliderMove:
    if ((main && mainControl == UpperHandle) || (!main && altControl == UpperHandle))
      up = true;

  case QAbstractSlider::SliderNoAction:
    no = true;
    break;

  default:
    qWarning("RangeSlider::triggerAction: Unknown action");
    break;
  }

  if (!no && !up) {
    if (movement == NoCrossing)
      value = qMin(value, upper);
    else if (movement == NoOverlapping)
      value = qMin(value, upper - 1);

    if (movement == FreeMovement && value > upper) {
      swapControls();
      setUpperPosition(value);
    } else {
      setLowerPosition(value);
    }
  } else if (!no) {
    if (movement == NoCrossing)
      value = qMax(value, lower);
    else if (movement == NoOverlapping)
      value = qMax(value, lower + 1);

    if (movement == FreeMovement && value < lower) {
      swapControls();
      setLowerPosition(value);
    } else {
      setUpperPosition(value);
    }
  }

  blockTracking = false;
  setLowerValue(lowerPos);
  setUpperValue(upperPos);
}

void RangeSlider::swapControls() {
  qSwap(lower, upper);
  qSwap(lowerPressed, upperPressed);
  lastPressed = (lastPressed == LowerHandle ? UpperHandle : LowerHandle);
  mainControl = (mainControl == LowerHandle ? UpperHandle : LowerHandle);
}

void RangeSlider::updateRange(int, int) {
  setRange(lower, upper);
}

void RangeSlider::movePressedHandle() {
  switch (lastPressed) {
  case LowerHandle:
    if (lowerPos != lower) {
      bool main = (mainControl == LowerHandle);
      triggerAction(QAbstractSlider::SliderMove, main);
    }

    break;

  case UpperHandle:
    if (upperPos != upper) {
      bool main = (mainControl == UpperHandle);
      triggerAction(QAbstractSlider::SliderMove, main);
    }

    break;

  default:
    break;
  }
}

RangeSlider::HandleMovementMode RangeSlider::handleMovementMode() const {
  return movement;
}

void RangeSlider::setHandleMovementMode(RangeSlider::HandleMovementMode mode) {
  movement = mode;
}

int RangeSlider::lowerValue() const {
  return qMin(lower, upper);
}

void RangeSlider::setLowerValue(int low) {
  setRange(low, upper);
}

int RangeSlider::upperValue() const {
  return qMax(lower, upper);
}

void RangeSlider::setUpperValue(int upp) {
  setRange(lower, upp);
}

void RangeSlider::setRange(int low, int upp) {
  low = qBound(minimum(), qMin(low, upp), maximum());
  upp = qBound(minimum(), qMax(low, upp), maximum());

  if (low != lower || upp != upper) {
    if (low != lower) {
      lower = low;
      lowerPos = low;
      emit lowerValueChanged(low);
    }

    if (upp != upper) {
      upper = upp;
      upperPos = upp;
      emit upperValueChanged(upp);
    }

    emit rangeChanged(lower, upper);
    update();
  }
}

int RangeSlider::lowerPosition() const {
  return lowerPos;
}

void RangeSlider::setLowerPosition(int low) {
  if (lowerPos != low) {
    lowerPos = low;

    if (!hasTracking())
      update();

    if (isSliderDown())
      emit lowerPositionChanged(low);

    if (hasTracking() && !blockTracking) {
      bool main = (mainControl == LowerHandle);
      triggerAction(SliderMove, main);
    }
  }
}

int RangeSlider::upperPosition() const {
  return upperPos;
}

void RangeSlider::setUpperPosition(int upp) {
  if (upperPos != upp) {
    upperPos = upp;

    if (!hasTracking())
      update();

    if (isSliderDown())
      emit upperPositionChanged(upp);

    if (hasTracking() && !blockTracking) {
      bool main = (mainControl == UpperHandle);
      triggerAction(SliderMove, main);
    }
  }
}

/*!
    \reimp
 */
void RangeSlider::keyPressEvent(QKeyEvent *event) {
  QSlider::keyPressEvent(event);

  bool main = true;
  SliderAction action = SliderNoAction;

  switch (event->key()) {
  case Qt::Key_Left:
    main = (orientation() == Qt::Horizontal);
    action = !invertedAppearance() ? SliderSingleStepSub : SliderSingleStepAdd;
    break;

  case Qt::Key_Right:
    main = (orientation() == Qt::Horizontal);
    action = !invertedAppearance() ? SliderSingleStepAdd : SliderSingleStepSub;
    break;

  case Qt::Key_Up:
    main = (orientation() == Qt::Vertical);
    action = invertedControls() ? SliderSingleStepSub : SliderSingleStepAdd;
    break;

  case Qt::Key_Down:
    main = (orientation() == Qt::Vertical);
    action = invertedControls() ? SliderSingleStepAdd : SliderSingleStepSub;
    break;

  case Qt::Key_Home:
    main = (mainControl == LowerHandle);
    action = SliderToMinimum;
    break;

  case Qt::Key_End:
    main = (mainControl == UpperHandle);
    action = SliderToMaximum;
    break;

  default:
    event->ignore();
    break;
  }

  if (action)
    triggerAction(action, main);
}

/*!
    \reimp
 */
void RangeSlider::mousePressEvent(QMouseEvent *event) {
  if (minimum() == maximum() || (event->buttons() ^ event->button())) {
    event->ignore();
    return;
  }

  handleMousePress(event->pos(), upperPressed, upper, UpperHandle);

  if (upperPressed != QStyle::SC_SliderHandle)
    handleMousePress(event->pos(), lowerPressed, lower, LowerHandle);

  firstMovement = true;
  event->accept();
}

/*!
    \reimp
 */
void RangeSlider::mouseMoveEvent(QMouseEvent *event) {
  if (lowerPressed != QStyle::SC_SliderHandle && upperPressed != QStyle::SC_SliderHandle) {
    event->ignore();
    return;
  }

  QStyleOptionSlider opt;
  initStyleOption(&opt);
  const int m = style()->pixelMetric(QStyle::PM_MaximumDragDistance, &opt, this);
  int newPosition = pixelPosToRangeValue(pick(event->pos()) - offset);

  if (m >= 0) {
    const QRect r = rect().adjusted(-m, -m, m, m);

    if (!r.contains(event->pos())) {
      newPosition = position;
    }
  }

  // pick the preferred handle on the first movement
  if (firstMovement) {
    if (lower == upper) {
      if (newPosition < lowerValue()) {
        swapControls();
        firstMovement = false;
      }
    } else {
      firstMovement = false;
    }
  }

  if (lowerPressed == QStyle::SC_SliderHandle) {
    if (movement == NoCrossing)
      newPosition = qMin(newPosition, upperValue());
    else if (movement == NoOverlapping)
      newPosition = qMin(newPosition, upperValue() - 1);

    if (movement == FreeMovement && newPosition > upper) {
      swapControls();
      setUpperPosition(newPosition);
    } else {
      setLowerPosition(newPosition);
    }
  } else if (upperPressed == QStyle::SC_SliderHandle) {
    if (movement == NoCrossing)
      newPosition = qMax(newPosition, lowerValue());
    else if (movement == NoOverlapping)
      newPosition = qMax(newPosition, lowerValue() + 1);

    if (movement == FreeMovement && newPosition < lower) {
      swapControls();
      setLowerPosition(newPosition);
    } else {
      setUpperPosition(newPosition);
    }
  }

  event->accept();
}

/*!
    \reimp
 */
void RangeSlider::mouseReleaseEvent(QMouseEvent *event) {
  QSlider::mouseReleaseEvent(event);
  setSliderDown(false);
  lowerPressed = QStyle::SC_None;
  upperPressed = QStyle::SC_None;
  update();
}

/*!
    \reimp
 */
void RangeSlider::paintEvent(QPaintEvent *) {
  QStylePainter painter(this);

  // groove & ticks
  QStyleOptionSlider opt;
  initStyleOption(&opt);
  opt.sliderValue = 0;
  opt.sliderPosition = 0;
  opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderTickmarks;
  painter.drawComplexControl(QStyle::CC_Slider, opt);

  // handle rects
  opt.sliderPosition = lowerPos;
  const QRect lr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
  const int lrv = pick(lr.center());
  opt.sliderPosition = upperPos;
  const QRect ur = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
  const int urv = pick(ur.center());

  // span
  const int minv = qMin(lrv, urv);
  const int maxv = qMax(lrv, urv);
  const QPoint c = QRect(lr.center(), ur.center()).center();
  QRect spanRect;

  if (orientation() == Qt::Horizontal)
    spanRect = QRect(QPoint(minv, c.y() - 2), QPoint(maxv, c.y() + 1));
  else
    spanRect = QRect(QPoint(c.x() - 2, minv), QPoint(c.x() + 1, maxv));

  drawRange(&painter, spanRect);

  // handles
  switch (lastPressed) {
  case LowerHandle:
    drawHandle(&painter, UpperHandle);
    drawHandle(&painter, LowerHandle);
    break;

  case UpperHandle:
  default:
    drawHandle(&painter, LowerHandle);
    drawHandle(&painter, UpperHandle);
    break;
  }
}

} // namespace tlp
