#include "tulip/TulipItemEditorCreators.h"

#include <tulip/TlpQtTools.h>
#include <QtCore/QDebug>
#include <tulip/ColorButton.h>

using namespace tlp;

QWidget* ColorEditorCreator::createWidget(QWidget *parent) const {
  return new ColorButton(parent);
}

bool ColorEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option, const QVariant &v) const {
  painter->setBrush(colorToQColor(v.value<tlp::Color>()));
  painter->setPen(Qt::black);
  painter->drawRect(option.rect.x()+6,option.rect.y()+6,option.rect.width()-12,option.rect.height()-12);
  return true;
}

void ColorEditorCreator::setEditorData(QWidget *editor, const QVariant &data) {
  static_cast<ColorButton*>(editor)->setTulipColor(data.value<tlp::Color>());
}

QVariant ColorEditorCreator::editorData(QWidget *editor) {
  return QVariant::fromValue<tlp::Color>(static_cast<ColorButton*>(editor)->tulipColor());
}
