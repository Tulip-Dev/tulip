#include "tulip/TulipItemEditorCreators.h"
#include <QtCore/QDebug>
#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>
#include <QtGui/QCheckBox>
#include <QtGui/QStylePainter>
#include <QtGui/QApplication>

using namespace tlp;

/*
  ColorEditorCreator
*/
QWidget* ColorEditorCreator::createWidget(QWidget *parent) const {
  return new ColorButton(parent);
}

bool ColorEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& v) const {
  painter->setBrush(colorToQColor(v.value<tlp::Color>()));
  painter->setPen(Qt::black);
  painter->drawRect(option.rect.x()+6,option.rect.y()+6,option.rect.width()-12,option.rect.height()-12);
  return true;
}

void ColorEditorCreator::setEditorData(QWidget *editor, const QVariant &data,tlp::Graph*) {
  static_cast<ColorButton*>(editor)->setTulipColor(data.value<tlp::Color>());
}

QVariant ColorEditorCreator::editorData(QWidget *editor,tlp::Graph*) {
  return QVariant::fromValue<tlp::Color>(static_cast<ColorButton*>(editor)->tulipColor());
}

/*
  BooleanEditorCreator
*/
QWidget* BooleanEditorCreator::createWidget(QWidget* parent) const {
  return new QCheckBox(parent);
}

QRect checkRect(const QStyleOptionViewItem &option,const QRect& bounding, QWidget* widget) {
  QStyleOptionButton opt;
  opt.QStyleOption::operator=(option);
  opt.rect = bounding;
  QStyle *style = widget ? widget->style() : QApplication::style();
  return style->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt, widget);
}

bool BooleanEditorCreator::paint(QPainter *p, const QStyleOptionViewItem &option, const QVariant &variant) const {
  QCheckBox cb;
  QStyleOptionViewItem opt(option);
  opt.rect = checkRect(option,option.rect,&cb);
  opt.state |= variant.toBool() ? QStyle::State_On : QStyle::State_Off;
  opt.state = opt.state & ~QStyle::State_HasFocus;
  QStyle* style = cb.style();
  style->drawPrimitive(QStyle::PE_IndicatorViewItemCheck,&opt,p,&cb);
  return true;
}

void BooleanEditorCreator::setEditorData(QWidget* editor, const QVariant &data,tlp::Graph*) {
  static_cast<QCheckBox*>(editor)->setChecked(data.toBool());
}

QVariant BooleanEditorCreator::editorData(QWidget* editor,tlp::Graph*) {
  return static_cast<QCheckBox*>(editor)->isChecked();
}
