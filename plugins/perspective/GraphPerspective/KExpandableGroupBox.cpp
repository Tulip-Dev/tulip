/*
 This file is part of the KDE libraries
 Copyright (C) 2005-2007 Daniel Molkentin <molkentin@kde.org>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.    If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QTimeLine>

#include "KExpandableGroupBox.h"

/******************************************************************
 * Helper classes
 *****************************************************************/



ClickableLabel::ClickableLabel(QWidget* parent) :
  QLabel(parent) {
}

ClickableLabel::~ClickableLabel() {
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *e) {
  Q_UNUSED( e );
  emit clicked();
}



ArrowButton::ArrowButton(QWidget *parent) :
  QAbstractButton(parent) {
}

ArrowButton::~ArrowButton() {
}

void ArrowButton::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter p(this);
  QStyleOption opt;
  int h = sizeHint().height();
  opt.rect = QRect(0, (height() - h) / 2, h, h);
  opt.palette = palette();
  opt.state = QStyle::State_Children;
  if (isChecked())
    opt.state |= QStyle::State_Open;

  style()->drawPrimitive(QStyle::PE_IndicatorBranch, &opt, &p);
  p.end();
}

/******************************************************************
 * Private class
 *****************************************************************/

class KExpandableGroupBox::Private {
public:
  QGridLayout *gridLayout;
  QWidget *widget;
  ClickableLabel *label;
  ArrowButton *colButton;
  QTimeLine *timeline;
  QWidget *expander;
  QVBoxLayout *expanderLayout;
  bool animateExpansion;
  bool expanded;
  QString title;
};

/******************************************************************
 * Implementation
 *****************************************************************/

KExpandableGroupBox::KExpandableGroupBox(QWidget *parent) :
  QWidget(parent), d(new KExpandableGroupBox::Private) {
  init();
}
KExpandableGroupBox::KExpandableGroupBox(const QString& title, QWidget *parent) :
  QWidget(parent), d(new KExpandableGroupBox::Private) {
  init();
  setTitle(title);
}

void KExpandableGroupBox::init() {
  // don't be expensive on embedded devices
#if defined(Q_OS_WINCE) || defined(Q_WS_QWS)
  d->animateExpansion = false;
#else
  d->animateExpansion = true;
#endif
  d->expander = 0;
  d->expanderLayout = 0;
  d->timeline = new QTimeLine(150, this);
  d->timeline->setCurveShape(QTimeLine::EaseInOutCurve);
  connect(d->timeline, SIGNAL(valueChanged(qreal)), this, SLOT(animateExpansion(qreal)));

  d->widget = 0;
  d->gridLayout = new QGridLayout(this);
  d->gridLayout->setMargin(0);

  d->colButton = new ArrowButton;
  d->colButton->setCheckable(true);
  d->colButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

  d->label = new ClickableLabel;
  d->label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  d->label->setBuddy(d->widget);
  setFocusProxy(d->label);

  //test
  //d->label->hide();
  //d->colButton->hide();

  d->gridLayout->addWidget(d->colButton, 1, 1);
  d->gridLayout->addWidget(d->label, 1, 2);

  connect(d->label, SIGNAL(clicked()), d->colButton, SLOT(click()));

  connect(d->colButton, SIGNAL(toggled(bool)), SLOT(setExpanded(bool)));

  setExpanded(false);
  setEnabled(false);
}

KExpandableGroupBox::~KExpandableGroupBox() {
  delete d;
}

QWidget* KExpandableGroupBox::widget() const {
  return d->widget;
}
#include <iostream>
void KExpandableGroupBox::setWidget(QWidget *w) {
  if (!w) {
    d->expanderLayout->removeWidget(d->widget);
    return;
  }

  d->widget = w;
  d->label->setBuddy(d->widget);

  if (!d->animateExpansion) {
    if (!isExpanded()) {
      d->widget->hide();
    }
    d->gridLayout->addWidget(d->widget, 2, 2);
    d->gridLayout->setRowStretch(2, 1);
  } else {
    if (!d->expander) {
      d->expander = new QWidget(this);
      d->gridLayout->addWidget(d->expander, 2, 2);
      d->gridLayout->setRowStretch(2, 1);
      d->expanderLayout = new QVBoxLayout(d->expander);
      d->expanderLayout->setMargin(0);
      d->expanderLayout->setSpacing(0);
      d->expander->setFixedHeight(0);
    }

    d->widget->setParent(d->expander);
    d->widget->show();
    d->expanderLayout->addWidget(d->widget);
  }
  setEnabled(true);

  if (isExpanded()) {
    setExpanded(true);
  }
}

void KExpandableGroupBox::setTitle(const QString& title) {
  //d->label->setText(QString("<b>%1</b>").arg(title));
  d->label->setText(title);
  d->title = title;
}

QString KExpandableGroupBox::title() const {
  return d->title;
  //return d->label->text();
}

Qt::Alignment KExpandableGroupBox::alignment() const {
  return d->label->alignment();
}

void KExpandableGroupBox::setAlignment(Qt::Alignment a) {
  d->label->setAlignment(a);
}

void KExpandableGroupBox::setExpanded(bool expanded) {
  if (!d->widget) {
    return;
  }

  if (!d->animateExpansion) {
    if (!expanded)
      d->widget->setVisible(false);
  } else {
    if (expanded)
      d->expander->setVisible(true);
    d->widget->setVisible(expanded);
  }

  d->colButton->setChecked(expanded);
  d->timeline->setDirection(expanded ? QTimeLine::Forward : QTimeLine::Backward);
  d->timeline->start();
}

void KExpandableGroupBox::animateExpansion(qreal showAmount) {
  int pixels = (int) (d->widget->sizeHint().height() * showAmount);
  d->gridLayout->setRowMinimumHeight(2, pixels);

  if (!d->animateExpansion) {
    d->gridLayout->setRowMinimumHeight(2, pixels);

    if (showAmount == 1) {
      d->widget->setVisible(true);
    }
  } else {
    d->expander->setFixedHeight(pixels);
  }
}

bool KExpandableGroupBox::isExpanded() const {
  return d->colButton->isChecked();
}

void KExpandableGroupBox::setAnimateExpansion(bool animate) {
  d->animateExpansion = animate;
}

bool KExpandableGroupBox::animateExpansion() const {
  return d->animateExpansion;
}

void KExpandableGroupBox::paintEvent(QPaintEvent *ev) {
#if 0
  QPainter p(this);
  QStyleOption opt;
  int h = 16; //px
  opt.rect = QRect(0, 0, h, h);
  opt.palette = palette();
  opt.state = QStyle::State_Children;
  if (d->colButton->isChecked())
  opt.state |= QStyle::State_Open;
  style()->drawPrimitive(QStyle::PE_IndicatorBranch, &opt, &p);
  p.drawText(h, 0, width(), height(), Qt::TextShowMnemonic, d->title);
  p.end();
#endif
  QWidget::paintEvent(ev);
}

void KExpandableGroupBox::mouseReleaseEvent(QMouseEvent *ev) {
#if 0
  if ( (ev->button() & Qt::LeftButton) && QRect(0, 0, width(), 16).contains(ev->pos())) {
    setExpanded(!isExpanded());
  }
#endif
  QWidget::mouseReleaseEvent(ev);
}

QSize KExpandableGroupBox::minimumSizeHint() const {
  //    return QSize(0,16);
  return QWidget::minimumSizeHint();
}

//#include "kexpandablegroupbox.moc"
