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
#include "GraphTableItemDelegate.h"

#include <tulip/Color.h>

#include "tulip/ColorButton.h"
#include "tulip/TlpQtTools.h"
#include "tulip/SizeEditor.h"
#include "tulip/ColorButton.h"
#include <tulip/CoordEditor.h>
#include <tulip/TulipFileDescriptorWidget.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/EdgeExtremityGlyphManager.h>

#include "VectorEditionWidget.h"
#include "ElementCollection.h"
#include "GraphTableModel.h"
#include "GlyphPreviewGenerator.h"

#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QTextLayout>

using namespace tlp;
using namespace std;


///NodeShapeEditorCreator

QWidget* NodeShapeEditorCreator::createWidget(QWidget*parent) const{
    QComboBox* combobox = new QComboBox(parent);
    string glyphName;
    forEach(glyphName,GlyphLister::availablePlugins()) {
        int glyphIndex = GlyphManager::getInst().glyphId(glyphName);
        //Create the glyph entry
        combobox->addItem(GlyphPreviewGenerator::getInst().getPreview(glyphIndex),tlpStringToQString(glyphName),glyphIndex);
    }
    return combobox;
}
void NodeShapeEditorCreator::setEditorData(QWidget* editor, const QVariant&data ,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    combobox->setCurrentIndex(combobox->findData(data.value<NodeShape>().nodeShapeId));
}

QVariant NodeShapeEditorCreator::editorData(QWidget*editor,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    return QVariant::fromValue<NodeShape>(NodeShape(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString NodeShapeEditorCreator::displayText(const QVariant & data) const{
    return tlpStringToQString(GlyphManager::getInst().glyphName(data.value<NodeShape>().nodeShapeId));
}

bool NodeShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const{

    QStyleOptionViewItemV4 opt = option;
    opt.features |= QStyleOptionViewItemV2::HasDecoration;
    QPixmap pixmap = GlyphPreviewGenerator::getInst().getPreview(data.value<NodeShape>().nodeShapeId);
    opt.icon = QIcon(pixmap);
    opt.decorationSize = pixmap.size();

    opt.features |= QStyleOptionViewItemV2::HasDisplay;
    opt.text = displayText(data);

    QStyle *style = QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, NULL);
    return true;
}

///EdgeExtremityShapeEditorCreator
QWidget* EdgeExtremityShapeEditorCreator::createWidget(QWidget* parent) const{
    QComboBox* combobox = new QComboBox(parent);
    combobox->addItem(QString("NONE"),EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
    string glyphName;
    forEach(glyphName,EdgeExtremityGlyphLister::availablePlugins()) {
        int glyphIndex = EdgeExtremityGlyphManager::getInst().glyphId(glyphName);
        //Create the glyph entry
        combobox->addItem(EdgeExtremityGlyphPreviewGenerator::getInst().getPreview(glyphIndex),tlpStringToQString(glyphName),glyphIndex);
    }
    return combobox;
}
void EdgeExtremityShapeEditorCreator::setEditorData(QWidget* editor, const QVariant& data,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    combobox->setCurrentIndex(combobox->findData(data.value<EdgeExtremityShape>().edgeExtremityShapeId));
}

QVariant EdgeExtremityShapeEditorCreator::editorData(QWidget* editor,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    return QVariant::fromValue<EdgeExtremityShape>(EdgeExtremityShape(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeExtremityShapeEditorCreator::displayText(const QVariant &data) const{
    return tlpStringToQString(EdgeExtremityGlyphManager::getInst().glyphName(data.value<NodeShape>().nodeShapeId));
}

bool EdgeExtremityShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const{
    QStyleOptionViewItemV4 opt = option;
    opt.features |= QStyleOptionViewItemV2::HasDecoration;
    QPixmap pixmap = GlyphPreviewGenerator::getInst().getPreview(data.value<EdgeExtremityShape>().edgeExtremityShapeId);
    opt.icon = QIcon(pixmap);
    opt.decorationSize = pixmap.size();

    opt.features |= QStyleOptionViewItemV2::HasDisplay;
    opt.text = displayText(data);

    QStyle *style = QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, NULL);
    return true;
}

///EdgeShapeEditorCreator
QWidget* EdgeShapeEditorCreator::createWidget(QWidget* parent) const{
    QComboBox* combobox = new QComboBox(parent);
    for (int i = 0; i < GlGraphStaticData::edgeShapesCount; i++) {
        combobox->addItem(tlpStringToQString(GlGraphStaticData::edgeShapeName(GlGraphStaticData::edgeShapeIds[i])),QVariant(GlGraphStaticData::edgeShapeIds[i]));
    }
    return combobox;
}
void EdgeShapeEditorCreator::setEditorData(QWidget* editor, const QVariant& data,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    combobox->setCurrentIndex(combobox->findData(static_cast<int>(data.value<EdgeShape>())));
}

QVariant EdgeShapeEditorCreator::editorData(QWidget* editor,Graph*){
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    return QVariant::fromValue<EdgeShape>(static_cast<EdgeShape>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeShapeEditorCreator::displayText(const QVariant &data) const{
    return tlpStringToQString(GlGraphStaticData::edgeShapeName(data.value<EdgeShape>()));
}




GraphTableItemDelegate::GraphTableItemDelegate(QObject* parent) :
    TulipItemDelegate(parent) {
    registerCreator<NodeShape>(new NodeShapeEditorCreator);
    registerCreator<EdgeShape>(new EdgeShapeEditorCreator);
    registerCreator<EdgeExtremityShape>(new EdgeExtremityShapeEditorCreator);
}

QWidget* GraphTableItemDelegate::createEditor(QWidget* p, const QStyleOptionViewItem& option,
                                              const QModelIndex& index) const {
    //  QVariant data = index.data(Qt::EditRole);

    //  //Create the right editor in function of the data type.
    //  //If the data are int check if there is Glyph, Edge Extremity or Edge shape
    //  if(data.userType() == QVariant::Double) {
    //    QLineEdit *lineEdit = new QLineEdit(p);
    //    lineEdit->setValidator(new QDoubleValidator(lineEdit));
    //    lineEdit->setText(QString::number(data.toDouble()));
    //    return lineEdit;
    //  }
    //  else

    //    //If data are QString check if there is file.
    //    if(data.userType() == QVariant::String) {
    //      return QStyledItemDelegate::createEditor(p, option, index);
    //    }
    //    else

    //      //Create editor for user defined type.
    //      if(data.userType() == qMetaTypeId< Color >()) {
    //        ColorButton* button =  new ColorButton(p);
    //        button->setColor(colorToQColor(data.value<Color>()));
    //        button->setFocusPolicy(Qt::StrongFocus);
    //        return button;
    //      }
    //      else if(data.userType() == qMetaTypeId< Coord >()) {
    //        CoordEditor *editor = new CoordEditor(p);
    //        editor->setCoord(data.value<Coord>());
    //        editor->setAutoFillBackground(true);
    //        editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //        editor->setFocusPolicy(Qt::StrongFocus);
    //        return editor;
    //      }
    //      else if(data.userType() == qMetaTypeId< Size >()) {
    //        SizeEditor * editor = new  SizeEditor(p);
    //        editor->setSize(data.value<Size>());
    //        editor->setAutoFillBackground(true);
    //        editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //        editor->setFocusPolicy(Qt::StrongFocus);
    //        return editor;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<bool > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(BOOLEAN_PROPERTY_RTTI,new ListPropertyWidgetTypeManger<BooleanType>(data.value<vector<bool> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<Color > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(COLORPROPERTY_RTTI,new ListPropertyWidgetTypeManger<ColorType>(data.value<vector<Color> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<Coord > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(LAYOUTPROPERTY_RTTI,new ListPropertyWidgetTypeManger<PointType>(data.value<vector<Coord> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<double > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(DOUBLEPROPERTY_RTTI,new ListPropertyWidgetTypeManger<DoubleType>(data.value<vector<double> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<int > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(INTEGER_PROPERTY_RTTI,new ListPropertyWidgetTypeManger<IntegerType>(data.value<vector<int> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<Size > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(SIZEPROPERTY_RTTI,new ListPropertyWidgetTypeManger<SizeType>(data.value<vector<Size> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< vector<string > >()) {
    //        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
    //        vectorWidget->setInterface(STRINGPROPERTY_RTTI,new ListPropertyWidgetTypeManger<StringType>(data.value<vector<string> >()));
    //        return vectorWidget;
    //      }
    //      else if(data.userType() == qMetaTypeId< ElementCollection >()) {
    //        ElementCollection *collection = new ElementCollection(data.value< ElementCollection >());
    //        QComboBox *comboBox = new QComboBox(p);
    //        comboBox->setModel(collection);
    //        collection->setParent(comboBox);
    //        QList<int> selection = collection->selectedElement();

    //        if(!selection.empty()) {
    //          comboBox->setCurrentIndex(selection.front());
    //        }

    //        return comboBox;
    //      }
    //      else if(data.userType() == qMetaTypeId< TulipFileDescriptor >()) {
    //        TulipFileDescriptor url = data.value<TulipFileDescriptor>();
    //        return createFileNameEditor(p,url,"");
    //      }
    //      else {
    //        return QStyledItemDelegate::createEditor(p, option, index);
    //      }

    return TulipItemDelegate::createEditor(p, option, index);
}

void GraphTableItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                          const QModelIndex& index) const {
    //  QVariant data = index.data(Qt::EditRole);

    //  if(data.userType() == QVariant::Double) {
    //    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    //    model->setData(index,QVariant(lineEdit->text().toDouble()));
    //  }
    //  else if(data.userType() == qMetaTypeId< Color >()) {
    //    QVariant v;
    //    v.setValue<Color>(QColorToColor(dynamic_cast<ColorButton*>(editor)->color()));
    //    model->setData(index,v);
    //  }
    //  else if(data.userType() == qMetaTypeId< Coord >()) {
    //    QVariant v;
    //    v.setValue<Coord>((static_cast<CoordEditor*>(editor))->coord());
    //    model->setData(index,v);
    //  }
    //  else if(data.userType() == qMetaTypeId< Size >()) {
    //    QVariant v;
    //    v.setValue<Size>((static_cast<SizeEditor*>(editor)->size()));
    //    model->setData(index,v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<bool > >()) {
    //    QVariant v;
    //    v.setValue<vector<bool > >(((ListPropertyWidgetTypeManger<BooleanType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<Color > >()) {
    //    QVariant v;
    //    v.setValue<vector<Color > >(((ListPropertyWidgetTypeManger<ColorType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<Coord > >()) {
    //    QVariant v;
    //    v.setValue<vector<Coord > >(((ListPropertyWidgetTypeManger<PointType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<double > >()) {
    //    QVariant v;
    //    v.setValue<vector<double > >(((ListPropertyWidgetTypeManger<DoubleType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<int > >()) {
    //    QVariant v;
    //    v.setValue<vector<int > >(((ListPropertyWidgetTypeManger<IntegerType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<Size > >()) {
    //    QVariant v;
    //    v.setValue<vector<Size > >(((ListPropertyWidgetTypeManger<SizeType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< vector<string > >()) {
    //    QVariant v;
    //    v.setValue<vector<string > >(((ListPropertyWidgetTypeManger<StringType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
    //    model->setData(index, v);
    //  }
    //  else if(data.userType() == qMetaTypeId< ElementCollection >()) {
    //    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    //    ElementCollection collection(*qobject_cast<ElementCollection*>(comboBox->model()));
    //    collection.setAllElementSelection(false);
    //    collection.setElementSelection(comboBox->currentIndex(),true);
    //    model->setData(index,QVariant::fromValue<ElementCollection>(collection));
    //  }
    //  else if(data.userType() == qMetaTypeId< TulipFileDescriptor >()) {
    //    TulipFileDescriptorWidget *fileEditor = qobject_cast<TulipFileDescriptorWidget*>(editor);
    //    TulipFileDescriptor url(fileEditor->data());
    //    model->setData(index,QVariant::fromValue<TulipFileDescriptor>(url));
    //  }
    //  else {
    TulipItemDelegate::setModelData(editor, model, index);
    //  }
}

//QWidget* GraphTableItemDelegate::createFileNameEditor(QWidget* parent, const TulipFileDescriptor& defaultFileName, const QString& filenameFilter) const {
//  TulipFileDescriptorWidget *editor = new TulipFileDescriptorWidget(parent);
//  editor->setData(defaultFileName);
//  editor->setNameFilter(filenameFilter);
//  editor->setAutoFillBackground(true);
//  return editor;
//}

void GraphTableItemDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QVariant data = index.data();
    //Get the normalized value of the element.
    QVariant normalizedValue = index.data(GraphTableModel::NormalizedValueRole);

    //If we have a normalized value represent it as an histogram.
    if(normalizedValue.isValid() && normalizedValue.type()==QVariant::Double) {
        QStyleOptionViewItemV4 opt = static_cast<QStyleOptionViewItemV4>(option);
        initStyleOption(&opt,index);
        painter->save();

        //Change the color of the background if the element is selected
        if (opt.state & QStyle::State_Selected)
            painter->fillRect(opt.rect, opt.palette.highlight());

        //Draw histogram
        double normalization = normalizedValue.toDouble();
        QRect histogramFrame = opt.rect;
        QRect histogramRect(histogramFrame.topLeft(),QSize(histogramFrame.width()*normalization,histogramFrame.height()));
        painter->fillRect(histogramRect,QBrush(Qt::lightGray));
        //Draw value
        QString text = data.toString();
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect boundingRect = opt.rect;
        boundingRect.adjust(textMargin, 0, -textMargin, 0);
        //Change the color of the text if the element is selected
        QPen pen = painter->pen();

        if (opt.state & QStyle::State_Selected)
            pen.setColor(opt.palette.color(QPalette::HighlightedText));
        else
            pen.setColor(opt.palette.color(QPalette::Text));

        painter->setPen(pen);
        QTextOption textOption;
        const bool wrapText = opt.features & QStyleOptionViewItemV2::WrapText;
        textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
        textOption.setTextDirection(opt.direction);
        textOption.setAlignment(QStyle::visualAlignment(opt.direction, opt.displayAlignment));
        painter->drawText(boundingRect,text,textOption);
        painter->restore();
    }
    else {
        TulipItemDelegate::paint(painter,option,index);
    }
}
