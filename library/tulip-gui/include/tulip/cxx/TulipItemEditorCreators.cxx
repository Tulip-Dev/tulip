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
void LineEditEditorCreator<T>::setEditorData(QWidget* editor, const QVariant &data,tlp::Graph*) {
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
void PropertyEditorCreator<PROPTYPE>::setEditorData(QWidget* w, const QVariant& val,tlp::Graph* g) {
  if (g == NULL) {
    w->setEnabled(false);
    return;
  }

  PROPTYPE* prop = val.value<PROPTYPE*>();
  QPoint pos = w->pos();
  // because this PropertyEditor and the QWidget w
  // may be used to managed the value of several cells,
  // we use a QSet to register the cells for which we need
  // to manage a null property.
  // To uniquely identify theses cells (in which the editor w
  // takes place), we record its position as an unsigned int
  // 16 bits for x and 16 bits for y
  unsigned int ipos = (pos.x() << 16) + pos.y();
  if (prop == NULL)
    // indicates that a null property
    // has to be managed in that cell
    hasNullProperty.insert(ipos);
    
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<PROPTYPE>* model = new GraphPropertiesModel<PROPTYPE>(g,false,hasNullProperty.contains(ipos),combo);
  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

template<typename PROPTYPE>
QVariant PropertyEditorCreator<PROPTYPE>::editorData(QWidget* w,tlp::Graph* g) {
  if (g == NULL)
    return QVariant();

  QComboBox* combo = static_cast<QComboBox*>(w);
  std::string propName = combo->currentText().toStdString();
  PROPTYPE *prop = NULL;

  if (g->existProperty(propName))
    prop = g->getProperty<PROPTYPE>(propName);

  return QVariant::fromValue<PROPTYPE*>(prop);
}

template<typename PROPTYPE>
QString PropertyEditorCreator<PROPTYPE>::displayText(const QVariant& v) const {
  PROPTYPE *prop = v.value<PROPTYPE*>();

  if (prop==NULL)
    return "";

  return prop->getName().c_str();
}

template<typename ElementType>
QWidget* GenericVectorEditorCreator<ElementType>::createWidget(QWidget* parent) const {
  return new VectorEditionWidget(parent);
}

template<typename ElementType>
void GenericVectorEditorCreator<ElementType>::setEditorData(QWidget* editor, const QVariant& data,tlp::Graph*) {
  std::vector<ElementType> v = data.value<std::vector<ElementType> >();
  VectorEditionWidget* vEditor = static_cast<VectorEditionWidget*>(editor);
  vEditor->setInterface(new GenericContainer< tlp::TypeInterface<ElementType> >(v));
}

template<typename ElementType>
QVariant GenericVectorEditorCreator<ElementType>::editorData(QWidget* editor,tlp::Graph*) {
  VectorEditionWidget* vEditor = static_cast<VectorEditionWidget*>(editor);
  return QVariant::fromValue<std::vector<ElementType> >(static_cast< GenericContainer< tlp::TypeInterface<ElementType> >* >(vEditor->getInterface())->getResultValue());
}

template<typename ElementType>
QString GenericVectorEditorCreator<ElementType>::displayText(const QVariant &data) const {
  std::vector<ElementType> v = data.value<std::vector<ElementType> >();
  return QString::fromUtf8(tlp::SerializableVectorType<ElementType,false>::toString(v).c_str());
}


}
