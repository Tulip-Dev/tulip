#include "tulip/TulipItemDelegate.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemEditorCreators.h>

namespace tlp {

TulipItemDelegate::TulipItemDelegate(QObject* parent): QStyledItemDelegate(parent) {
  registerCreator<tlp::Color>(new ColorEditorCreator);
}

TulipItemDelegate::~TulipItemDelegate() {
  foreach(tlp::TulipItemEditorCreator* v,_creators.values())
  delete v;
}

void TulipItemDelegate::unregisterCreator(tlp::TulipItemEditorCreator* c) {
  int k = _creators.key(c,INT_MIN);

  if (k != INT_MIN)
    _creators.remove(k);
}

tlp::TulipItemEditorCreator* TulipItemDelegate::creator(int typeId) const {
  tlp::TulipItemEditorCreator *result = NULL;

  if (_creators.contains(typeId))
    result = _creators[typeId];

  return result;
}

QWidget* TulipItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const {
  QVariant v = index.model()->data(index);
  TulipItemEditorCreator *c = creator(v.userType());

  if (!c)
    return NULL;

  return c->createWidget(parent);
}

QString TulipItemDelegate::displayText(const QVariant &value, const QLocale &locale) const {
  if (value.type() == QVariant::String)
    return value.toString();

  TulipItemEditorCreator *c = creator(value.userType());

  if (c)
    return c->displayText(value);

  return QStyledItemDelegate::displayText(value,locale);
}

void TulipItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant bgColor = index.data(Qt::BackgroundRole),
           fgColor = index.data(Qt::ForegroundRole);

  if (bgColor.isValid() && bgColor.type() == QVariant::Color)
    painter->setBrush(bgColor.value<QColor>());
  else
    painter->setBrush(Qt::transparent);

  if (fgColor.isValid() && fgColor.type() == QVariant::Color)
    painter->setPen(fgColor.value<QColor>());
  else
    painter->setPen(Qt::transparent);

  painter->drawRect(option.rect);

  QVariant v = index.data();
  TulipItemEditorCreator *c = creator(v.userType());

  if (c && c->paint(painter,option,v))
    return;

  QStyledItemDelegate::paint(painter,option,index);
}

}
