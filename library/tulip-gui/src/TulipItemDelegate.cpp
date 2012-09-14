/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/TulipItemDelegate.h"

#include <QtCore/QDebug>
#include <tulip/TulipModel.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemEditorCreators.h>
#include <QtCore/QEvent>

using namespace tlp;

TulipItemDelegate::TulipItemDelegate(QObject* parent): QStyledItemDelegate(parent), _currentMonitoredChild(NULL), _currentMonitoredCombo(NULL) {
  registerCreator<bool>(new BooleanEditorCreator);
  registerCreator<int>(new LineEditEditorCreator<tlp::IntegerType>);
  registerCreator<unsigned int>(new LineEditEditorCreator<tlp::UnsignedIntegerType>);
  registerCreator<long>(new LineEditEditorCreator<tlp::LongType>);
  registerCreator<double>(new LineEditEditorCreator<tlp::DoubleType>);
  registerCreator<float>(new LineEditEditorCreator<tlp::FloatType>);
  registerCreator<std::string>(new LineEditEditorCreator<tlp::StringType>);
  registerCreator<QString>(new QStringEditorCreator);
  registerCreator<tlp::Color>(new ColorEditorCreator);
  registerCreator<tlp::Coord>(new CoordEditorCreator);
  registerCreator<tlp::Size>(new SizeEditorCreator);
  registerCreator<tlp::BooleanProperty*>(new PropertyEditorCreator<tlp::BooleanProperty>);
  registerCreator<tlp::DoubleProperty*>(new PropertyEditorCreator<tlp::DoubleProperty>);
  registerCreator<tlp::LayoutProperty*>(new PropertyEditorCreator<tlp::LayoutProperty>);
  registerCreator<tlp::StringProperty*>(new PropertyEditorCreator<tlp::StringProperty>);
  registerCreator<tlp::IntegerProperty*>(new PropertyEditorCreator<tlp::IntegerProperty>);
  registerCreator<tlp::SizeProperty*>(new PropertyEditorCreator<tlp::SizeProperty>);
  registerCreator<tlp::ColorProperty*>(new PropertyEditorCreator<tlp::ColorProperty>);
  registerCreator<tlp::PropertyInterface*>(new PropertyInterfaceEditorCreator);
  registerCreator<tlp::ColorScale>(new ColorScaleEditorCreator);
  registerCreator<tlp::StringCollection>(new StringCollectionEditorCreator);
  registerCreator<TulipFileDescriptor>(new TulipFileDescriptorEditorCreator);
  registerCreator<NodeShape>(new NodeShapeEditorCreator);
  registerCreator<EdgeShape>(new EdgeShapeEditorCreator);
  registerCreator<EdgeExtremityShape>(new EdgeExtremityShapeEditorCreator);
  registerCreator<std::vector<bool> >(new VectorEditorCreator<bool>);
  registerCreator<std::vector<Color> >(new VectorEditorCreator<Color>);
  registerCreator<std::vector<Coord> >(new VectorEditorCreator<Coord>);
  registerCreator<std::vector<double> >(new VectorEditorCreator<double>);
  registerCreator<std::vector<int> >(new VectorEditorCreator<int>);
  registerCreator<std::vector<Size> >(new VectorEditorCreator<Size>);
  registerCreator<std::vector<std::string> >(new VectorEditorCreator<std::string>);
  registerCreator<TulipFont>(new TulipFontEditorCreator);
  registerCreator<LabelPosition>(new TulipLabelPositionEditorCreator);
  registerCreator<Graph*>(new GraphEditorCreator);
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
  return _creators[typeId];
}

QWidget* TulipItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const {
  QVariant v = index.model()->data(index);
  TulipItemEditorCreator *c = creator(v.userType());

  if (!c)
    return NULL;

  QWidget* w = c->createWidget(parent);
  return w;
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

  if (!v.isValid()) {
#ifndef NDEBUG
    qWarning() << "Value for row(" << index.model()->headerData(index.row(),Qt::Vertical).toString() << ") - column(" << index.model()->headerData(index.column(),Qt::Horizontal).toString() << ") is invalid";
#endif
    return;
  }

  TulipItemEditorCreator *c = creator(v.userType());

  if (c == NULL)
    return;

  if (c != NULL && c->paint(painter,option,v))
    return;

  QStyledItemDelegate::paint(painter,option,index);
}

void TulipItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  QVariant data = index.data();
  tlp::Graph* g = index.data(TulipModel::GraphRole).value<tlp::Graph*>();
  bool isMandatory = true;
  QVariant mandatoryVar = index.data(TulipModel::MandatoryRole);

  if (mandatoryVar.isValid())
    isMandatory = mandatoryVar.value<bool>();

  TulipItemEditorCreator *c = creator(data.userType());

  if (!c)
    return;

  c->setEditorData(editor,data,isMandatory,g);
}

void TulipItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QVariant data = index.data();
  tlp::Graph* g = index.data(TulipModel::GraphRole).value<tlp::Graph*>();
  TulipItemEditorCreator *c = creator(data.userType());

  if (!c)
    return;

  model->setData(index,c->editorData(editor,g));
}

bool TulipItemDelegate::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::FocusOut && dynamic_cast<QComboBox*>(object) != NULL) {
    return true;
  }
  else if (event->type() == QEvent::ChildAdded) {
    QChildEvent* childEv = static_cast<QChildEvent*>(event);

    if (dynamic_cast<QComboBox*>(object) != NULL) {
      _currentMonitoredChild = childEv->child();
      _currentMonitoredCombo = static_cast<QComboBox*>(object);
      _currentMonitoredChild->installEventFilter(this);
      _currentMonitoredCombo->removeEventFilter(this);
      connect(_currentMonitoredCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboDataChanged()));
    }
  }
  else if (object == _currentMonitoredChild && event->type() == QEvent::Hide) {
    _currentMonitoredChild->removeEventFilter(this);
    _currentMonitoredChild = NULL;
    emit commitData(_currentMonitoredCombo);
    _currentMonitoredCombo->deleteLater();
    _currentMonitoredCombo = NULL;
    return true;
  }

  return QStyledItemDelegate::eventFilter(object,event);
}

void TulipItemDelegate::comboDataChanged() {
  emit commitData(static_cast<QWidget*>(sender()));
}
