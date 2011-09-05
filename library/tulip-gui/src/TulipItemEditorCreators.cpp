#include "tulip/TulipItemEditorCreators.h"

#include <QtGui/QSpinBox>
#include <tulip/TlpQtTools.h>
#include <QtCore/QDebug>

namespace tlp {

QWidget* ColorEditorCreator::createWidget(QWidget *parent) const {
  return new QSpinBox(parent);
}

QByteArray ColorEditorCreator::valuePropertyName() const {
  return "";
}

bool ColorEditorCreator::paint(QPainter *painter, const QStyleOptionViewItem &option, const QVariant &v) const {
  painter->setBrush(colorToQColor(v.value<tlp::Color>()));
  painter->setPen(Qt::black);
  painter->drawRect(option.rect.x()+6,option.rect.y()+6,option.rect.width()-12,option.rect.height()-12);
  return true;
}
}
