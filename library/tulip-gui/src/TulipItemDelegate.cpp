#include "tulip/TulipItemDelegate.h"

#include <QtCore/QDebug>
#include <tulip/TulipModel.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemEditorCreators.h>

using namespace tlp;

TulipItemDelegate::TulipItemDelegate(QObject* parent): QStyledItemDelegate(parent) {
  registerCreator<bool>(new BooleanEditorCreator);
  registerCreator<int>(new LineEditEditorCreator<tlp::IntegerType>);
  registerCreator<unsigned int>(new LineEditEditorCreator<tlp::UnsignedIntegerType>);
  registerCreator<long>(new LineEditEditorCreator<tlp::LongType>);
  registerCreator<double>(new LineEditEditorCreator<tlp::DoubleType>);
  registerCreator<float>(new LineEditEditorCreator<tlp::FloatType>);
  registerCreator<std::string>(new LineEditEditorCreator<tlp::StringType>);
  registerCreator<tlp::Color>(new ColorEditorCreator);
  //    CHECK_TYPE(tlp::SizeType);

  //    CHECK_PROPERTY(tlp::BooleanProperty);
  //    CHECK_PROPERTY(tlp::DoubleProperty);
  //    CHECK_PROPERTY(tlp::LayoutProperty);
  //    CHECK_PROPERTY(tlp::StringProperty);
  //    CHECK_PROPERTY(tlp::IntegerProperty);
  //    CHECK_PROPERTY(tlp::SizeProperty);
  //    CHECK_PROPERTY(tlp::ColorProperty);


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

void TulipItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  QVariant data = index.data();
  tlp::Graph* g = index.data(TulipModel::GraphRole).value<tlp::Graph*>();
  TulipItemEditorCreator *c = creator(data.userType());
  if (!c)
    return;
  c->setEditorData(editor,data,g);
}

void TulipItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QVariant data = index.data();
  tlp::Graph* g = index.data(TulipModel::GraphRole).value<tlp::Graph*>();
  TulipItemEditorCreator *c = creator(data.userType());

  if (!c)
    return;

  model->setData(index,c->editorData(editor,g));
}
