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
#include "tulip/TulipItemEditorCreators.h"

#include <QtCore/QDebug>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QStylePainter>
#include <QtGui/QApplication>
#include <QtGui/QLinearGradient>
#include <QtGui/QPaintEvent>
#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>
#include <tulip/ColorScaleButton.h>
#include <tulip/TulipFileDescriptorWidget.h>
#include <tulip/CoordEditor.h>
#include <tulip/GlyphRenderer.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/TulipFontDialog.h>
#include <tulip/GlyphManager.h>
#include <tulip/GraphPropertiesModel.h>
#include <QtGui/QColorDialog>
#include <tulip/GlLabel.h>
#include <tulip/Perspective.h>
#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>

using namespace tlp;

/*
 * Base class
 */
bool TulipItemEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant &) const {
  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setBrush(option.palette.highlight());
    painter->setPen(Qt::transparent);
    painter->drawRect(option.rect);
  }

  return false;
}

/*
  ColorEditorCreator
*/
QWidget* ColorEditorCreator::createWidget(QWidget *) const {
  QColorDialog *colorDialog = new QColorDialog(NULL);
  colorDialog->setOptions(colorDialog->options() | QColorDialog::ShowAlphaChannel);
  colorDialog->setModal(true);
  colorDialog->move(QCursor::pos());
  return colorDialog;
}

bool ColorEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& v) const {
  TulipItemEditorCreator::paint(painter,option,v);
  painter->setBrush(colorToQColor(v.value<tlp::Color>()));
  painter->setPen(Qt::black);
  painter->drawRect(option.rect.x()+6,option.rect.y()+6,option.rect.width()-12,option.rect.height()-12);
  return true;
}

void ColorEditorCreator::setEditorData(QWidget *editor, const QVariant &data, bool, tlp::Graph*) {
  static_cast<QColorDialog*>(editor)->setCurrentColor(colorToQColor(data.value<tlp::Color>()));
}

QVariant ColorEditorCreator::editorData(QWidget *editor,tlp::Graph*) {
  return QVariant::fromValue<tlp::Color>(QColorToColor(static_cast<QColorDialog*>(editor)->currentColor()));
}

/*
  BooleanEditorCreator
*/
QWidget* BooleanEditorCreator::createWidget(QWidget* parent) const {
  return new QComboBox(parent);
}

void BooleanEditorCreator::setEditorData(QWidget* editor, const QVariant &v, bool, tlp::Graph*) {
  QComboBox* cb = static_cast<QComboBox*>(editor);
  cb->addItem("false");
  cb->addItem("true");
  cb->setCurrentIndex(v.toBool() ? 1 : 0);
}

QVariant BooleanEditorCreator::editorData(QWidget* editor,tlp::Graph*) {
  return static_cast<QComboBox*>(editor)->currentIndex() == 1;
}

QString BooleanEditorCreator::displayText(const QVariant& v) const {
  return v.toBool() ? "true" : "false";
}

/*
  CoordEditorCreator
*/
QWidget* CoordEditorCreator::createWidget(QWidget*) const {
  return new CoordEditor(NULL);
}

void CoordEditorCreator::setEditorData(QWidget* w, const QVariant& v, bool, tlp::Graph*) {
  static_cast<CoordEditor*>(w)->setCoord(v.value<tlp::Coord>());
}

QVariant CoordEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return QVariant::fromValue<tlp::Coord>(static_cast<CoordEditor*>(w)->coord());
}

/*
  PropertyInterfaceEditorCreator
*/
QWidget* PropertyInterfaceEditorCreator::createWidget(QWidget* parent) const {
  return new QComboBox(parent);
}

void PropertyInterfaceEditorCreator::setEditorData(QWidget* w, const QVariant& val, bool isMandatory, tlp::Graph* g) {
  if (g == NULL) {
    w->setEnabled(false);
    return;
  }

  PropertyInterface* prop = val.value<PropertyInterface*>();
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<PropertyInterface>* model = NULL;

  if (isMandatory)
    model = new GraphPropertiesModel<PropertyInterface>(g,false,combo);
  else
    model = new GraphPropertiesModel<PropertyInterface>(QObject::trUtf8("Select a property"),g,false,combo);

  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

QVariant PropertyInterfaceEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<PropertyInterface>* model = static_cast<GraphPropertiesModel<PropertyInterface> *>(combo->model());
  return model->data(model->index(combo->currentIndex(),0),TulipModel::PropertyRole);
}

QString PropertyInterfaceEditorCreator::displayText(const QVariant& v) const {
  PropertyInterface *prop = v.value<PropertyInterface*>();

  if (prop==NULL)
    return "";

  return prop->getName().c_str();
}

/*
  ColorScaleEditorCreator
*/
QWidget* ColorScaleEditorCreator::createWidget(QWidget* parent) const {
  return new ColorScaleButton(ColorScale(),parent);
}

bool ColorScaleEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& var) const {
  TulipItemEditorCreator::paint(painter,option,var);
  ColorScaleButton::paintScale(painter,option.rect,var.value<ColorScale>());
  return true;
}

void ColorScaleEditorCreator::setEditorData(QWidget* w, const QVariant& var,bool,tlp::Graph*) {
  static_cast<ColorScaleButton*>(w)->setColorScale(var.value<ColorScale>());
}

QVariant ColorScaleEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return QVariant::fromValue<ColorScale>(static_cast<ColorScaleButton*>(w)->colorScale());
}

/*
  StringCollectionEditorCreator
*/
QWidget* StringCollectionEditorCreator::createWidget(QWidget* parent) const {
  return new QComboBox(parent);
}

void StringCollectionEditorCreator::setEditorData(QWidget* widget, const QVariant& var, bool,tlp::Graph*) {
  StringCollection col = var.value<StringCollection>();
  QComboBox* combo = static_cast<QComboBox*>(widget);

  for(unsigned int i=0; i<col.size(); ++i)
    combo->addItem(col[i].c_str());

  combo->setCurrentIndex(col.getCurrent());
}

QVariant StringCollectionEditorCreator::editorData(QWidget* widget, tlp::Graph*) {
  QComboBox* combo = static_cast<QComboBox*>(widget);
  StringCollection col;

  for(int i=0; i < combo->count(); ++i)
    col.push_back(combo->itemText(i).toStdString());

  col.setCurrent(combo->currentIndex());
  return QVariant::fromValue<StringCollection>(col);
}

QString StringCollectionEditorCreator::displayText(const QVariant &var) const {
  StringCollection col = var.value<StringCollection>();
  return col[col.getCurrent()].c_str();
}

// this class is defined to properly catch the return status
// of a QFileDialog. calling QDialog::result() instead does not work
class TulipFileDialog :public QFileDialog {
public:
  TulipFileDialog(QWidget* w): QFileDialog(w), ok(QDialog::Rejected) {
  }
  ~TulipFileDialog() {}
  int ok;
  void done(int res) {
    ok = res;
    QFileDialog::done(res);
  }
};

/*
  TulipFileDescriptorEditorCreator
  */
QWidget* TulipFileDescriptorEditorCreator::createWidget(QWidget*) const {
  QFileDialog* dlg = new TulipFileDialog(Perspective::instance()->mainWindow());
  dlg->setOption(QFileDialog::DontUseNativeDialog, true);
  dlg->setMinimumSize(300,400);
  return dlg;
}


void TulipFileDescriptorEditorCreator::setEditorData(QWidget* w, const QVariant& v, bool, tlp::Graph*) {
  TulipFileDescriptor desc = v.value<TulipFileDescriptor>();
  QFileDialog* dlg = static_cast<QFileDialog*>(w);
  QString path;

  if (desc.absolutePath.isEmpty())
    path = QDir::currentPath();
  else
    path = QFileInfo(desc.absolutePath).absolutePath();

  dlg->setDirectory(path);

  if (desc.type == TulipFileDescriptor::Directory) {
    dlg->setFileMode(QFileDialog::Directory);
    dlg->setOption(QFileDialog::ShowDirsOnly);
  }
  else
    dlg->setFileMode(QFileDialog::ExistingFile);

  dlg->setModal(true);
  dlg->move(QCursor::pos() - QPoint(150, 200));
}

QVariant TulipFileDescriptorEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  TulipFileDialog* dlg = static_cast<TulipFileDialog*>(w);

  int result = dlg->ok;

  if (result == QDialog::Rejected)
    return QVariant();

  if (dlg->fileMode() == QFileDialog::Directory) {
    return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor(dlg->directory().absolutePath(),TulipFileDescriptor::Directory));
  }
  else if (!dlg->selectedFiles().empty()) {
    return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor(dlg->selectedFiles()[0],TulipFileDescriptor::File));
  }

  return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor());
}

bool TulipFileDescriptorEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& v) const {
  TulipItemEditorCreator::paint(painter,option,v);
  QRect rect = option.rect;
  TulipFileDescriptor fileDesc = v.value<TulipFileDescriptor>();
  QFileInfo fileInfo(fileDesc.absolutePath);
  QIcon icon;
  QString text;

  if (fileInfo.isFile()) {
    icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    text = fileInfo.fileName();
  }
  else if (fileInfo.isDir()) {
    icon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);
    QDir d1 = fileInfo.dir();
    d1.cdUp();
    text = fileInfo.absoluteFilePath().remove(0,d1.absolutePath().length()-1);
  }

  int iconSize = rect.height()-4;
  painter->drawPixmap(rect.x() + 2, rect.y() + 2, iconSize, iconSize, icon.pixmap(iconSize));
  int textXPos = rect.x() + iconSize + 5;

  if (option.state.testFlag(QStyle::State_Selected) && option.showDecorationSelected) {
    painter->setPen(option.palette.highlightedText().color());
    painter->setBrush(option.palette.highlightedText());
  }
  else {
    painter->setPen(option.palette.text().color());
    painter->setBrush(option.palette.text());
  }


  painter->drawText(textXPos, rect.y() + 2, rect.width() - (textXPos - rect.x()), rect.height()-4,Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, QFileInfo(fileDesc.absolutePath).fileName());

  return true;
}

QWidget* NodeShapeEditorCreator::createWidget(QWidget*parent) const {
  QComboBox* combobox = new QComboBox(parent);
  std::list<std::string> glyphs(PluginLister::instance()->availablePlugins<Glyph>());

  for(std::list<std::string>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
    std::string glyphName(*it);
    int glyphIndex = GlyphManager::getInst().glyphId(glyphName);
    //Create the glyph entry
    combobox->addItem(GlyphRenderer::getInst().render(glyphIndex),tlpStringToQString(glyphName),glyphIndex);
  }

  return combobox;
}
void NodeShapeEditorCreator::setEditorData(QWidget* editor, const QVariant&data , bool, Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  combobox->setCurrentIndex(combobox->findData(data.value<NodeShape>().nodeShapeId));
}

QVariant NodeShapeEditorCreator::editorData(QWidget*editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<NodeShape>(NodeShape(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString NodeShapeEditorCreator::displayText(const QVariant & data) const {
  return tlpStringToQString(GlyphManager::getInst().glyphName(data.value<NodeShape>().nodeShapeId));
}

QSize NodeShapeEditorCreator::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QPixmap pixmap = GlyphRenderer::getInst().render(data.value<NodeShape>().nodeShapeId);
  QFontMetrics fontMetrics(option.font);
  return QSize(pixmap.width()+fontMetrics.boundingRect(displayText(data)).width()+20, pixmap.height());
}

bool NodeShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const {
  TulipItemEditorCreator::paint(painter,option,data);

  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  QPixmap pixmap = GlyphRenderer::getInst().render(data.value<NodeShape>().nodeShapeId);
  opt.icon = QIcon(pixmap);
  opt.decorationSize = pixmap.size();

  opt.features |= QStyleOptionViewItemV2::HasDisplay;
  opt.text = displayText(data);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, NULL);
  return true;
}

///EdgeExtremityShapeEditorCreator
QWidget* EdgeExtremityShapeEditorCreator::createWidget(QWidget* parent) const {
  QComboBox* combobox = new QComboBox(parent);
  combobox->addItem(QString("NONE"),EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
  std::list<std::string> glyphs(PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>());

  for(std::list<std::string>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
    std::string glyphName(*it);
    const tlp::Plugin& infos = PluginLister::pluginInformations(glyphName);
    int glyphIndex = infos.id();
    //Create the glyph entry
    combobox->addItem(EdgeExtremityGlyphRenderer::getInst().render(glyphIndex),tlpStringToQString(glyphName),glyphIndex);
  }

  return combobox;
}
void EdgeExtremityShapeEditorCreator::setEditorData(QWidget* editor, const QVariant& data,bool,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  combobox->setCurrentIndex(combobox->findData(data.value<EdgeExtremityShape>().edgeExtremityShapeId));
}

QVariant EdgeExtremityShapeEditorCreator::editorData(QWidget* editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<EdgeExtremityShape>(EdgeExtremityShape(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeExtremityShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(EdgeExtremityGlyphManager::getInst().glyphName(data.value<EdgeExtremityShape>().edgeExtremityShapeId));
}

bool EdgeExtremityShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const {
  TulipItemEditorCreator::paint(painter,option,data);

  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  QPixmap pixmap = EdgeExtremityGlyphRenderer::getInst().render(data.value<EdgeExtremityShape>().edgeExtremityShapeId);
  opt.icon = QIcon(pixmap);
  opt.decorationSize = pixmap.size();

  opt.features |= QStyleOptionViewItemV2::HasDisplay;
  opt.text = displayText(data);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, NULL);
  return true;
}

///EdgeShapeEditorCreator
QWidget* EdgeShapeEditorCreator::createWidget(QWidget* parent) const {
  QComboBox* combobox = new QComboBox(parent);

  for (int i = 0; i < GlGraphStaticData::edgeShapesCount; i++) {
    combobox->addItem(tlpStringToQString(GlGraphStaticData::edgeShapeName(GlGraphStaticData::edgeShapeIds[i])),QVariant(GlGraphStaticData::edgeShapeIds[i]));
  }

  return combobox;
}
void EdgeShapeEditorCreator::setEditorData(QWidget* editor, const QVariant& data, bool, Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  combobox->setCurrentIndex(combobox->findData(static_cast<int>(data.value<EdgeShape>())));
}

QVariant EdgeShapeEditorCreator::editorData(QWidget* editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<EdgeShape>(static_cast<EdgeShape>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(GlGraphStaticData::edgeShapeName(data.value<EdgeShape>()));
}

//TulipFontEditorCreator
QWidget* TulipFontEditorCreator::createWidget(QWidget*) const {
  return new TulipFontDialog(NULL);
}
void TulipFontEditorCreator::setEditorData(QWidget*editor, const QVariant&data,bool,tlp::Graph*) {
  TulipFont font =data.value<TulipFont>();
  TulipFontDialog* fontWidget = static_cast<TulipFontDialog*>(editor);
  fontWidget->selectFont(font);
}

QVariant TulipFontEditorCreator::editorData(QWidget* editor,tlp::Graph*) {
  TulipFontDialog* fontWidget = static_cast<TulipFontDialog*>(editor);
  return QVariant::fromValue<TulipFont>(fontWidget->font());
}

QString TulipFontEditorCreator::displayText(const QVariant & data) const {
  TulipFont font =data.value<TulipFont>();
  return font.fontName();
}

//TulipLabelPositionEditorCreator
QVector<QString> TulipLabelPositionEditorCreator::POSITION_LABEL = QVector<QString>()
    << QObject::trUtf8("Center")
    << QObject::trUtf8("Top")
    << QObject::trUtf8("Bottom")
    << QObject::trUtf8("Left")
    << QObject::trUtf8("Right");

QWidget* TulipLabelPositionEditorCreator::createWidget(QWidget* parent) const {
  QComboBox* result = new QComboBox(parent);
  foreach(QString s, POSITION_LABEL)
  result->addItem(s);
  return result;
}
void TulipLabelPositionEditorCreator::setEditorData(QWidget* w, const QVariant& var,bool,tlp::Graph*) {
  QComboBox* comboBox = static_cast<QComboBox*>(w);
  comboBox->setCurrentIndex((int)(var.value<LabelPosition>()));
}
QVariant TulipLabelPositionEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return QVariant::fromValue<LabelPosition>((LabelPosition)(static_cast<QComboBox*>(w)->currentIndex()));
}
QString TulipLabelPositionEditorCreator::displayText(const QVariant& v) const {
  return POSITION_LABEL[(int)(v.value<LabelPosition>())];
}

//GraphEditorCreator
QWidget* GraphEditorCreator::createWidget(QWidget *parent) const {
  return new QLabel(parent);
}

void GraphEditorCreator::setEditorData(QWidget* w, const QVariant& var, bool, tlp::Graph*) {
  Graph* g = var.value<Graph*>();

  if (g != NULL) {
    std::string name;
    g->getAttribute<std::string>("name",name);
    static_cast<QLabel*>(w)->setText(name.c_str());
  }
}

QVariant GraphEditorCreator::editorData(QWidget*,tlp::Graph*) {
  return QVariant();
}

QString GraphEditorCreator::displayText(const QVariant& var) const {
  Graph* g = var.value<Graph*>();

  if (g == NULL)
    return QString::null;

  std::string name;
  g->getAttribute<std::string>("name",name);
  return name.c_str();
}

//QStringEditorCreator
QWidget* QStringEditorCreator::createWidget(QWidget *parent) const {
  return new QLineEdit(parent);
}

void QStringEditorCreator::setEditorData(QWidget* w, const QVariant& var, bool, tlp::Graph*) {
  static_cast<QLineEdit*>(w)->setText(var.toString());
  static_cast<QLineEdit*>(w)->selectAll();
}

QVariant QStringEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return static_cast<QLineEdit*>(w)->text();
}

QString QStringEditorCreator::displayText(const QVariant& var) const {
  return var.toString();
}

QWidget *QStringListEditorCreator::createWidget(QWidget *) const {
  VectorEditionWidget* w = new VectorEditionWidget(NULL);
  w->setWindowFlags(Qt::Dialog);
  w->setWindowModality(Qt::ApplicationModal);
  return w;
}

//QStringListEditorCreator
void QStringListEditorCreator::setEditorData(QWidget *w, const QVariant &var, bool, Graph *) {
  QStringList strList = var.toStringList();
  QVector<QVariant> vect(strList.length());
  int i=0;
  foreach(QString s, strList) {
    vect[i++] = s;
  }
  static_cast<VectorEditionWidget*>(w)->setVector(vect,qMetaTypeId<QString>());
}

QVariant QStringListEditorCreator::editorData(QWidget* w, Graph*) {
  QVector<QVariant> vect = static_cast<VectorEditionWidget*>(w)->vector();
  QStringList lst;
  foreach(QVariant v, vect)
  lst.push_back(v.toString());
  return lst;
}

QString QStringListEditorCreator::displayText(const QVariant &var) const {
  return QStringListType::toString(var.toStringList()).c_str();
}
