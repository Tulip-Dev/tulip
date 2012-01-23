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

#ifndef TULIPTABLEWIDGETITEMDELEGATE_H_
#define TULIPTABLEWIDGETITEMDELEGATE_H_
#include <QtGui/QStyledItemDelegate>
#include <tulip/tulipconf.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemDelegate.h>


#include "VectorEditionWidget.h"

class NodeShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
};

class EdgeExtremityShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
};

class EdgeShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

template<typename ElementType>
class GenericVectorEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const{
      return new VectorEditionWidget(parent);
  }

  virtual void setEditorData(QWidget* editor, const QVariant& data,tlp::Graph*){
      std::vector<ElementType> v = data.value<std::vector<ElementType> >();
      VectorEditionWidget* vEditor = static_cast<VectorEditionWidget*>(editor);
      vEditor->setInterface(new ListPropertyWidgetTypeManger< tlp::TypeInterface<ElementType> >(v));
  }

  virtual QVariant editorData(QWidget* editor,tlp::Graph*){
      VectorEditionWidget* vEditor = static_cast<VectorEditionWidget*>(editor);
      return QVariant::fromValue<std::vector<ElementType> >(static_cast< ListPropertyWidgetTypeManger< tlp::TypeInterface<ElementType> >* >(vEditor->getInterface())->getResultValue());
  }
  virtual QString displayText(const QVariant &data) const{
      std::vector<ElementType> v = data.value<std::vector<ElementType> >();
      return QString::fromUtf8(tlp::SerializableVectorType<ElementType,false>::toString(v).c_str());
  }
};




/**
* @brief QItemDelegate to display and edit data from a GraphTableModel in the Qt model/view architecture.
**/
class GraphTableItemDelegate: public tlp::TulipItemDelegate {
  Q_OBJECT
public:
  GraphTableItemDelegate(QObject* parent=NULL) ;
  virtual ~GraphTableItemDelegate() {
  }
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option,
                        const QModelIndex& index) const;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;  

};

#endif /* TULIPTABLEWIDGETITEMDELEGATE_H_ */
