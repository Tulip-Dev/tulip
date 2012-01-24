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
  PROPTYPE* prop = val.value<PROPTYPE*>();

  QSet<QString> locals,inherited;
  std::string name;
  forEach(name,g->getProperties()) {
    PropertyInterface* pi = g->getProperty(name);

    if (dynamic_cast<PROPTYPE*>(pi) == NULL)
      continue;

    if (g->existLocalProperty(name))
      locals.insert(name.c_str());
    else
      inherited.insert(name.c_str());
  }

  QFont f;
  f.setBold(true);
  QComboBox* combo = static_cast<QComboBox*>(w);
  combo->clear();

  int index=0;
  foreach(QString s,inherited) {
    combo->addItem(s);
    combo->setItemData(index,f,Qt::FontRole);
    combo->setItemData(index,QObject::trUtf8("Inherited"),Qt::ToolTipRole);

    if (prop && s == prop->getName().c_str())
      combo->setCurrentIndex(index);

    index++;
  }

  foreach(QString s,locals) {
    combo->addItem(s);
    combo->setItemData(index,QObject::trUtf8("Local"),Qt::ToolTipRole);

    if (prop && s == prop->getName().c_str())
      combo->setCurrentIndex(index);

    index++;
  }

  if (!prop) {
    combo->setCurrentIndex(0);
  }
}

template<typename PROPTYPE>
QVariant PropertyEditorCreator<PROPTYPE>::editorData(QWidget* w,tlp::Graph* g) {
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
