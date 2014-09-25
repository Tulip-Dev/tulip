/*
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
#include <sstream>
#include <QSet>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPainter>

#include <tulip/ForEach.h>
#include <tulip/DataSet.h>
#include <tulip/VectorEditor.h>
#include <tulip/GraphPropertiesModel.h>


namespace tlp {

template<typename T>
QWidget* LineEditEditorCreator<T>::createWidget(QWidget *parent) const {
  return new QLineEdit(parent);
}

template<typename T>
void LineEditEditorCreator<T>::setEditorData(QWidget* editor, const QVariant &data,bool,tlp::Graph*) {
  typename T::RealType val = data.value<typename T::RealType>();
  static_cast<QLineEdit*>(editor)->setText(QString::fromUtf8(T::toString(val).c_str()));
  static_cast<QLineEdit*>(editor)->selectAll();
}

template<typename T>
QVariant LineEditEditorCreator<T>::editorData(QWidget* editor,tlp::Graph*) {
  std::string strVal = std::string(static_cast<QLineEdit*>(editor)->text().toUtf8().data());
  QVariant result;
  typename T::RealType val;

  if (T::fromString(val,strVal))
    result.setValue<typename T::RealType>(val);

  return result;
}

template<typename T>
QWidget* MultiLinesEditEditorCreator<T>::createWidget(QWidget *parent) const {
  QTextEdit *edit = new QTextEdit(parent);
  edit->setFocusPolicy(Qt::StrongFocus);
  edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  return edit;
}

template<typename T>
void MultiLinesEditEditorCreator<T>::setEditorData(QWidget* editor, const QVariant &data,bool,tlp::Graph*) {
  typename T::RealType val = data.value<typename T::RealType>();
  static_cast<QTextEdit*>(editor)->setPlainText(QString::fromUtf8(T::toString(val).c_str()));
  static_cast<QTextEdit*>(editor)->selectAll();
}

template<typename T>
QVariant MultiLinesEditEditorCreator<T>::editorData(QWidget* editor,tlp::Graph*) {
  std::string strVal = std::string(static_cast<QTextEdit*>(editor)->toPlainText().toUtf8().data());
  QVariant result;
  typename T::RealType val;

  if (T::fromString(val,strVal))
    result.setValue<typename T::RealType>(val);

  return result;
}

template<typename T>
QSize MultiLinesEditEditorCreator<T>::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  typename T::RealType val = data.value<typename T::RealType>();
  QString valS = QString::fromUtf8(T::toString(val).c_str());
  QStringList lines = valS.split(QLatin1Char('\n'));
  QFontMetrics fontMetrics(option.font);
  int height = 0;
  int width = 0;

  for (int i = 0 ; i < lines.count() ; ++i) {
    QRect textBB = fontMetrics.boundingRect(lines.at(i));
    height += textBB.height();
    width = std::max(width, textBB.width());
  }

  return QSize(width+15, height+5);
}

template<typename T>
bool MultiLinesEditEditorCreator<T>::paint(QPainter* painter, const QStyleOptionViewItem &option, const QVariant &data) const {
  TulipItemEditorCreator::paint(painter,option,data);
  QRect rect = option.rect;
  typename T::RealType val = data.value<typename T::RealType>();
  QString valS = QString::fromUtf8(T::toString(val).c_str());
  QStringList lines = valS.split(QLatin1Char('\n'));

  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setPen(option.palette.highlightedText().color());
    painter->setBrush(option.palette.highlightedText());
  }
  else {
    painter->setPen(option.palette.text().color());
    painter->setBrush(option.palette.text());
  }

  for (int i = 0 ; i < lines.count() ; ++i) {
    painter->drawText(rect.x(), rect.y() + i * rect.height()/lines.count(), rect.width(), rect.height()/lines.count(),Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, lines.at(i));
  }

  return true;
}

template<typename PROPTYPE>
QWidget* PropertyEditorCreator<PROPTYPE>::createWidget(QWidget* parent) const {
  return new QComboBox(parent);
}

template<typename PROPTYPE>
void PropertyEditorCreator<PROPTYPE>::setEditorData(QWidget* w, const QVariant& val,bool isMandatory,tlp::Graph* g) {
  if (g == NULL) {
    w->setEnabled(false);
    return;
  }

  PROPTYPE* prop = val.value<PROPTYPE*>();
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<PROPTYPE>* model = NULL;

  if (isMandatory)
    model = new GraphPropertiesModel<PROPTYPE>(g,false,combo);
  else
    model = new GraphPropertiesModel<PROPTYPE>(QObject::trUtf8("Select a property"),g,false,combo);

  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

template<typename PROPTYPE>
QVariant PropertyEditorCreator<PROPTYPE>::editorData(QWidget* w,tlp::Graph* g) {
  if (g == NULL)
    return QVariant();

  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<PROPTYPE>* model = static_cast<GraphPropertiesModel<PROPTYPE> *>(combo->model());
  QVariant var = model->data(model->index(combo->currentIndex(),0),TulipModel::PropertyRole);
  tlp::PropertyInterface* pi = var.value<tlp::PropertyInterface*>();
  PROPTYPE* prop = (PROPTYPE*)(pi);
  return QVariant::fromValue<PROPTYPE*>(prop);
}

template<typename PROPTYPE>
QString PropertyEditorCreator<PROPTYPE>::displayText(const QVariant& v) const {
  PROPTYPE *prop = v.value<PROPTYPE*>();

  if (prop==NULL)
    return QObject::trUtf8("Select a property");

  return QString::fromUtf8(prop->getName().c_str());
}

template<typename ElementType>
QWidget* VectorEditorCreator<ElementType>::createWidget(QWidget*) const {
  VectorEditor* w = new VectorEditor(NULL);
  w->setWindowFlags(Qt::Dialog);
  w->setWindowModality(Qt::ApplicationModal);
  return w;

}

template<typename ElementType>
void VectorEditorCreator<ElementType>::setEditorData(QWidget* editor, const QVariant& v,bool,tlp::Graph*) {
  QVector<QVariant> editorData;
  std::vector<ElementType> vect = v.value<std::vector<ElementType> >();

  for (size_t i=0; i < vect.size(); ++i)  {
    editorData.push_back(QVariant::fromValue<ElementType>(vect[i]));
  }

  static_cast<VectorEditor*>(editor)->setVector(editorData,qMetaTypeId<ElementType>());

  static_cast<VectorEditor*>(editor)->move(QCursor::pos());
}

template<typename ElementType>
QVariant VectorEditorCreator<ElementType>::editorData(QWidget* editor,tlp::Graph*) {
  std::vector<ElementType> result;
  QVector<QVariant> editorData = static_cast<VectorEditor*>(editor)->vector();
  foreach(QVariant v, editorData)
  result.push_back(v.value<ElementType>());
  return QVariant::fromValue<std::vector<ElementType> >(result);
}

// the template below is only used for displayText method implementation
template<typename T>
struct DisplayVectorDataType :public DataType {
  DisplayVectorDataType(void *value) :DataType(value) {}
  ~DisplayVectorDataType() {
  }
  DataType* clone() const {
    return NULL;
  }

  std::string getTypeName() const {
    return std::string(typeid(std::vector<T>).name());
  }
};


template<typename ElementType>
QString VectorEditorCreator<ElementType>::displayText(const QVariant &data) const {
  std::vector<ElementType> v = data.value<std::vector<ElementType> >();

  if (v.empty())
    return QString();

  // use a DataTypeSerializer if any
  DataTypeSerializer* dts =
    DataSet::typenameToSerializer(std::string(typeid(v).name()));

  if (dts) {
    DisplayVectorDataType<ElementType> dt(&v);

    std::stringstream sstr;
    dts->writeData(sstr, &dt);

    std::string str = sstr.str();

    QString qstr = QString::fromUtf8(str.c_str());

    if (qstr.size() > 45) {
      qstr.truncate(41);
      qstr.append(" ...");
    }

    return qstr;
  }

  if (v.size() == 1)
    return QString("1 element");

  return QString::number(v.size()) + QObject::trUtf8(" elements");
}

}
