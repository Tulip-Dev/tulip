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
#include <QtCore/QSet>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include <tulip/ForEach.h>
#include <tulip/VectorEditionWidget.h>
#include <tulip/GraphPropertiesModel.h>


namespace tlp {

template<typename T>
QWidget* LineEditEditorCreator<T>::createWidget(QWidget *parent) const {
  return new QLineEdit(parent);
}

template<typename T>
void LineEditEditorCreator<T>::setEditorData(QWidget* editor, const QVariant &data,bool,tlp::Graph*) {
  typename T::RealType val = data.value<typename T::RealType>();
  static_cast<QLineEdit*>(editor)->setText(T::toString(val).c_str());
}

template<typename T>
QVariant LineEditEditorCreator<T>::editorData(QWidget* editor,tlp::Graph*) {
  std::string strVal = static_cast<QLineEdit*>(editor)->text().toStdString();
  QVariant result;
  typename T::RealType val;

  if (T::fromString(val,strVal))
    result.setValue<typename T::RealType>(val);

  return result;
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
  return model->data(model->index(combo->currentIndex(),0),TulipModel::PropertyRole);
}

template<typename PROPTYPE>
QString PropertyEditorCreator<PROPTYPE>::displayText(const QVariant& v) const {
  PROPTYPE *prop = v.value<PROPTYPE*>();

  if (prop==NULL)
    return "";

  return prop->getName().c_str();
}

template<typename ElementType>
QWidget* VectorEditorCreator<ElementType>::createWidget(QWidget*) const {
  VectorEditionWidget* w = new VectorEditionWidget(NULL);
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
  static_cast<VectorEditionWidget*>(editor)->setVector(editorData,qMetaTypeId<ElementType>());
  static_cast<VectorEditionWidget*>(editor)->move(QCursor::pos());
}

template<typename ElementType>
QVariant VectorEditorCreator<ElementType>::editorData(QWidget* editor,tlp::Graph*) {
  std::vector<ElementType> result;
  QVector<QVariant> editorData = static_cast<VectorEditionWidget*>(editor)->vector();
  foreach(QVariant v, editorData)
    result.push_back(v.value<ElementType>());
  return QVariant::fromValue<std::vector<ElementType> >(result);
}

template<typename ElementType>
QString VectorEditorCreator<ElementType>::displayText(const QVariant &data) const {
  std::vector<ElementType> v = data.value<std::vector<ElementType> >();
  return QString::number(v.size()) + QObject::trUtf8(" elements");
}

}
