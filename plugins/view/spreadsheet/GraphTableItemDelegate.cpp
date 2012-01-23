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
    registerCreator<std::vector<bool> >(new GenericVectorEditorCreator<bool>);
    registerCreator<std::vector<Color> >(new GenericVectorEditorCreator<Color>);
    registerCreator<std::vector<Coord> >(new GenericVectorEditorCreator<Coord>);
    registerCreator<std::vector<double> >(new GenericVectorEditorCreator<double>);
    registerCreator<std::vector<int> >(new GenericVectorEditorCreator<int>);
    registerCreator<std::vector<Size> >(new GenericVectorEditorCreator<Size>);
    registerCreator<std::vector<std::string> >(new GenericVectorEditorCreator<std::string>);
}

QWidget* GraphTableItemDelegate::createEditor(QWidget* p, const QStyleOptionViewItem& option,
                                              const QModelIndex& index) const {
    return TulipItemDelegate::createEditor(p, option, index);
}

void GraphTableItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                          const QModelIndex& index) const {
    TulipItemDelegate::setModelData(editor, model, index);
}

