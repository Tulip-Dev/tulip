/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include "tulip/TulipItemDelegate.h"

#include <QDebug>
#include <QEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMainWindow>
#include <QLabel>

#include <tulip/TulipModel.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemEditorCreators.h>
#include <tulip/GraphModel.h>
#include <tulip/Perspective.h>

using namespace tlp;

TulipItemDelegate::TulipItemDelegate(QObject* parent): QStyledItemDelegate(parent), _currentMonitoredChild(NULL), _currentMonitoredCombo(NULL) {
  registerCreator<bool>(new BooleanEditorCreator);
  registerCreator<int>(new LineEditEditorCreator<tlp::IntegerType>);
  registerCreator<unsigned int>(new LineEditEditorCreator<tlp::UnsignedIntegerType>);
  registerCreator<long>(new LineEditEditorCreator<tlp::LongType>);
  registerCreator<double>(new LineEditEditorCreator<tlp::DoubleType>);
  registerCreator<float>(new LineEditEditorCreator<tlp::FloatType>);
  registerCreator<std::string>(new QStringEditorCreator);
  registerCreator<QString>(new QStringEditorCreator);
  registerCreator<QStringList>(new QStringListEditorCreator);
  registerCreator<tlp::Color>(new ColorEditorCreator);
  registerCreator<tlp::Coord>(new CoordEditorCreator);
  registerCreator<tlp::Size>(new CoordEditorCreator);
  registerCreator<tlp::BooleanProperty*>(new PropertyEditorCreator<tlp::BooleanProperty>);
  registerCreator<tlp::DoubleProperty*>(new PropertyEditorCreator<tlp::DoubleProperty>);
  registerCreator<tlp::LayoutProperty*>(new PropertyEditorCreator<tlp::LayoutProperty>);
  registerCreator<tlp::StringProperty*>(new PropertyEditorCreator<tlp::StringProperty>);
  registerCreator<tlp::IntegerProperty*>(new PropertyEditorCreator<tlp::IntegerProperty>);
  registerCreator<tlp::SizeProperty*>(new PropertyEditorCreator<tlp::SizeProperty>);
  registerCreator<tlp::ColorProperty*>(new PropertyEditorCreator<tlp::ColorProperty>);
  registerCreator<tlp::BooleanVectorProperty*>(new PropertyEditorCreator<tlp::BooleanVectorProperty>);
  registerCreator<tlp::DoubleVectorProperty*>(new PropertyEditorCreator<tlp::DoubleVectorProperty>);
  registerCreator<tlp::CoordVectorProperty*>(new PropertyEditorCreator<tlp::CoordVectorProperty>);
  registerCreator<tlp::StringVectorProperty*>(new PropertyEditorCreator<tlp::StringVectorProperty>);
  registerCreator<tlp::IntegerVectorProperty*>(new PropertyEditorCreator<tlp::IntegerVectorProperty>);
  registerCreator<tlp::SizeVectorProperty*>(new PropertyEditorCreator<tlp::SizeVectorProperty>);
  registerCreator<tlp::ColorVectorProperty*>(new PropertyEditorCreator<tlp::ColorVectorProperty>);
  registerCreator<tlp::PropertyInterface*>(new PropertyInterfaceEditorCreator);
  registerCreator<tlp::NumericProperty*>(new PropertyEditorCreator<tlp::NumericProperty>);
  registerCreator<tlp::ColorScale>(new ColorScaleEditorCreator);
  registerCreator<tlp::StringCollection>(new StringCollectionEditorCreator);
  registerCreator<TulipFileDescriptor>(new TulipFileDescriptorEditorCreator);
  registerCreator<NodeShape::NodeShapes>(new NodeShapeEditorCreator);
  registerCreator<EdgeShape::EdgeShapes>(new EdgeShapeEditorCreator);
  registerCreator<EdgeExtremityShape::EdgeExtremityShapes>(new EdgeExtremityShapeEditorCreator);
  //registerCreator<std::vector<bool> >(new VectorEditorCreator<bool>);
  registerCreator<QVector<bool> >(new QVectorBoolEditorCreator);
  registerCreator<std::vector<Color> >(new VectorEditorCreator<Color>);
  registerCreator<std::vector<Coord> >(new VectorEditorCreator<Coord>);
  registerCreator<std::vector<double> >(new VectorEditorCreator<double>);
  registerCreator<std::vector<int> >(new VectorEditorCreator<int>);
  registerCreator<std::vector<Size> >(new VectorEditorCreator<Size>);
  registerCreator<std::vector<std::string> >(new VectorEditorCreator<std::string>);
  registerCreator<TulipFont>(new TulipFontEditorCreator);
  registerCreator<LabelPosition::LabelPositions>(new TulipLabelPositionEditorCreator);
  registerCreator<Graph*>(new GraphEditorCreator);
  registerCreator<std::set<tlp::edge> >(new EdgeSetEditorCreator);
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

QWidget* TulipItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QVariant v = index.model()->data(index);
  TulipItemEditorCreator *c = creator(v.userType());

  if (c == NULL) {
    return QStyledItemDelegate::createEditor(parent, option, index);
  }

  PropertyInterface* pi = index.data(TulipModel::PropertyRole).value<PropertyInterface*>();
  c->setPropertyToEdit(pi);
  QWidget* w = c->createWidget(parent);
  return w;
}

QString TulipItemDelegate::displayText(const QVariant &value, const QLocale &locale) const {
  if (value.type() == QVariant::String)
    return value.toString();

  TulipItemEditorCreator *c = creator(value.userType());

  if (c != NULL) {
    return c->displayText(value);
  }

  return QStyledItemDelegate::displayText(value,locale);
}

QSize TulipItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  const QAbstractItemModel *model = index.model();

  if (model != NULL) {
    QVariant value = model->data(index);
    TulipItemEditorCreator *c = creator(value.userType());

    if (c != NULL) {
      QSize s = c->sizeHint(option, index);

      if (s.isValid()) {
        return s;
      }
    }
  }

  return QStyledItemDelegate::sizeHint(option, index);
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


QVariant TulipItemDelegate::showEditorDialog(tlp::ElementType elType,
    tlp::PropertyInterface* pi,
    tlp::Graph* g,
    TulipItemDelegate* delegate,
    QWidget* dialogParent,
    unsigned int id) {
  QVariant value;

  if (elType == tlp::NODE) {
    node n(id);

    if (n.isValid())
      value = GraphModel::nodeValue(id, pi);
    else
      value = GraphModel::nodeDefaultValue(pi);
  }
  else {
    edge e(id);

    if (e.isValid())
      value = GraphModel::edgeValue(id, pi);
    else
      value = GraphModel::edgeDefaultValue(pi);
  }

  TulipItemEditorCreator* creator = delegate->creator(value.userType());

  if (dialogParent == NULL && Perspective::instance())
    dialogParent = Perspective::instance()->mainWindow();

  creator->setPropertyToEdit(pi);
  QWidget* w = creator->createWidget(dialogParent);
  creator->setEditorData(w,value,g);

  QDialog* dlg = dynamic_cast<QDialog*>(w);

  if (dlg == NULL) {
    // create a dialog on the fly
    dlg = new QDialog(dialogParent);
    dlg->setWindowTitle(elType == NODE ? "Set nodes values"
                        : "Set edges values");
    QVBoxLayout* layout = new QVBoxLayout;
    dlg->setLayout(layout);
    layout->addWidget(new QLabel(pi->getName().c_str()));
    layout->addWidget(w);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok,Qt::Horizontal);
    layout->addWidget(buttonBox);
    QWidget::setTabOrder(w, buttonBox);
    QObject::connect(buttonBox,SIGNAL(accepted()),dlg,SLOT(accept()));
    QObject::connect(buttonBox,SIGNAL(rejected()),dlg,SLOT(reject()));
  }

  QVariant result;

  if (dlg->exec() == QDialog::Accepted)
    result = creator->editorData(w,g);

  delete dlg;
  return result;
}
