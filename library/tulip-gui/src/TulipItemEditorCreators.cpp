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
#include <QCheckBox>
#include <QLabel>
#include <QStylePainter>
#include <QApplication>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QColorDialog>
#include <QMainWindow>
#include <QFileDialog>
#include <QHBoxLayout>

#include <tulip/TlpQtTools.h>
#include <tulip/ColorScaleButton.h>
#include <tulip/TulipFileDescriptorWidget.h>
#include <tulip/CoordEditor.h>
#include <tulip/StringEditor.h>
#include <tulip/GlyphRenderer.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/TulipFontDialog.h>
#include <tulip/GlyphManager.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/GlLabel.h>
#include <tulip/Perspective.h>
#include <tulip/TulipItemEditorCreators.h>

using namespace tlp;

class CustomComboBox : public QComboBox {

public:

  CustomComboBox(QWidget *parent=NULL) : QComboBox(parent), _popupWidth(0) {}

  void addItem(const QPixmap & icon, const QString & text, const QVariant & userData = QVariant(), const int extraWidth=20) {
    QFontMetrics fm = fontMetrics();
    _popupWidth = qMax(_popupWidth, icon.width() + fm.boundingRect(text).width() + extraWidth);
    QComboBox::addItem(icon, text, userData);
  }

  void addItem(const QString & text, const QVariant & userData = QVariant(), const int extraWidth=20) {
    QFontMetrics fm = fontMetrics();
    _popupWidth = qMax(_popupWidth, fm.boundingRect(text).width() + extraWidth);
    QComboBox::addItem(text, userData);
  }

  void showPopup() {
    QComboBox::showPopup();
    QWidget *popup = findChild<QFrame*>();

    if (_popupWidth > popup->width()) {
      popup->resize(_popupWidth, popup->height());
    }
  }

private:

  int _popupWidth;

};

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

QSize TulipItemEditorCreator::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  QString line = displayText(data);
  QFontMetrics fontMetrics(option.font);
  QRect textBB = fontMetrics.boundingRect(line);
  return QSize(textBB.width()+15, textBB.height()+5);
}

// this class is defined to properly catch the return status
// of a QColorDialog. calling QDialog::result() instead does not work
class TulipColorDialog :public QColorDialog {
public:
  TulipColorDialog(QWidget* w): QColorDialog(w),
    previousColor(), ok(QDialog::Rejected) {
  }
  ~TulipColorDialog() {}
  tlp::Color previousColor;
  int ok;
  void done(int res) {
    ok = res;
    QColorDialog::done(res);
  }
};

/*
  ColorEditorCreator
*/
QWidget* ColorEditorCreator::createWidget(QWidget *parent) const {
  TulipColorDialog *colorDialog = new TulipColorDialog(tlp::Perspective::instance()
      ? tlp::Perspective::instance()->mainWindow()
      : parent);
  colorDialog->setOptions(colorDialog->options() | QColorDialog::ShowAlphaChannel);
  colorDialog->setModal(true);
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
  TulipColorDialog* dlg = static_cast<TulipColorDialog*>(editor);

  dlg->previousColor = data.value<tlp::Color>();
  dlg->setCurrentColor(colorToQColor(dlg->previousColor));
  dlg->move(QCursor::pos() - QPoint(dlg->width()/2, dlg->height()/2));
}

QVariant ColorEditorCreator::editorData(QWidget *editor,tlp::Graph*) {
  TulipColorDialog* dlg = static_cast<TulipColorDialog*>(editor);

  if (dlg->ok == QDialog::Rejected)
    // restore the previous color
    return QVariant::fromValue<tlp::Color>(dlg->previousColor);

  return QVariant::fromValue<tlp::Color>(QColorToColor(dlg->currentColor()));
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
QWidget* CoordEditorCreator::createWidget(QWidget* parent) const {
  return new CoordEditor(Perspective::instance()
                         ? Perspective::instance()->mainWindow()
                         : parent, editSize);
}

void CoordEditorCreator::setEditorData(QWidget* w, const QVariant& v, bool, tlp::Graph*) {
  static_cast<CoordEditor*>(w)->setCoord(v.value<tlp::Coord>());
}

QVariant CoordEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return QVariant::fromValue<tlp::Coord>(static_cast<CoordEditor*>(w)->coord());
}

void CoordEditorCreator::setPropertyToEdit(tlp::PropertyInterface* prop) {
  editSize = (dynamic_cast<tlp::SizeProperty*>(prop) != NULL);
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
  NumericPropertyEditorCreator
*/
QWidget* NumericPropertyEditorCreator::createWidget(QWidget* parent) const {
  return new QComboBox(parent);
}

void NumericPropertyEditorCreator::setEditorData(QWidget* w, const QVariant& val, bool isMandatory, tlp::Graph* g) {
  if (g == NULL) {
    w->setEnabled(false);
    return;
  }

  NumericProperty* prop = val.value<NumericProperty*>();
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<NumericProperty>* model = NULL;

  if (isMandatory)
    model = new GraphPropertiesModel<NumericProperty>(g,false,combo);
  else
    model = new GraphPropertiesModel<NumericProperty>(QObject::trUtf8("Select a property"),g,false,combo);

  combo->setModel(model);
  combo->setCurrentIndex(model->rowOf(prop));
}

QVariant NumericPropertyEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  QComboBox* combo = static_cast<QComboBox*>(w);
  GraphPropertiesModel<NumericProperty>* model = static_cast<GraphPropertiesModel<NumericProperty> *>(combo->model());
  return model->data(model->index(combo->currentIndex(),0),TulipModel::PropertyRole);
}

QString NumericPropertyEditorCreator::displayText(const QVariant& v) const {
  NumericProperty *prop = v.value<NumericProperty*>();

  if (prop==NULL)
    return "";

  return prop->getName().c_str();
}

/*
  ColorScaleEditorCreator
*/

QWidget* ColorScaleEditorCreator::createWidget(QWidget* parent) const {
  return new ColorScaleButton(ColorScale(), parent);
}

bool ColorScaleEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& var) const {
  TulipItemEditorCreator::paint(painter,option,var);
  ColorScaleButton::paintScale(painter,option.rect,var.value<ColorScale>());
  return true;
}

void ColorScaleEditorCreator::setEditorData(QWidget* w, const QVariant& var,bool,tlp::Graph*) {
  static_cast<ColorScaleButton*>(w)->editColorScale(var.value<ColorScale>());
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
  TulipFileDescriptor previousFileDescriptor;

  void done(int res) {
    ok = res;
    QFileDialog::done(res);
  }
};

/*
  TulipFileDescriptorEditorCreator
  */
QWidget* TulipFileDescriptorEditorCreator::createWidget(QWidget* parent) const {
  QFileDialog* dlg = new TulipFileDialog(Perspective::instance()
                                         ? Perspective::instance()->mainWindow()
                                         : parent);
#if defined(__APPLE__)
  dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#else
  dlg->setOption(QFileDialog::DontUseNativeDialog, false);
#endif
  dlg->setMinimumSize(300,400);
  return dlg;
}

void TulipFileDescriptorEditorCreator::setEditorData(QWidget* w, const QVariant& v, bool, tlp::Graph*) {
  TulipFileDescriptor desc = v.value<TulipFileDescriptor>();
  TulipFileDialog* dlg = static_cast<TulipFileDialog*>(w);
  dlg->previousFileDescriptor = desc;

  // force the dialog initial directory
  // only if there is a non empty absolute path
  if (!desc.absolutePath.isEmpty())
    dlg->setDirectory(QFileInfo(desc.absolutePath).absolutePath());
  // or if we are in gui testing mode
  // where we must ensure that choosing the file is relative to
  // the current directory to allow to run the gui tests
  // from any relative unit_test/gui directory
  else if (inGuiTestingMode())
    dlg->setDirectory(QDir::currentPath());

  if (desc.type == TulipFileDescriptor::Directory) {
    dlg->setFileMode(QFileDialog::Directory);
    dlg->setOption(QFileDialog::ShowDirsOnly);
  }
  else
    dlg->setFileMode(desc.mustExist ? QFileDialog::ExistingFile : QFileDialog::AnyFile);

  dlg->setModal(true);
  dlg->move(QCursor::pos() - QPoint(150, 200));
}

QVariant TulipFileDescriptorEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  TulipFileDialog* dlg = static_cast<TulipFileDialog*>(w);

  int result = dlg->ok;

  if (result == QDialog::Rejected)
    return QVariant::fromValue<TulipFileDescriptor>(dlg->previousFileDescriptor);

  if (dlg->fileMode() == QFileDialog::Directory) {
    return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor(dlg->directory().absolutePath(),TulipFileDescriptor::Directory));
  }
  else if (!dlg->selectedFiles().empty()) {
    return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor(dlg->selectedFiles()[0],TulipFileDescriptor::File));
  }

  return QVariant::fromValue<TulipFileDescriptor>(TulipFileDescriptor());
}

class QImageIconPool {

public:

  const QIcon &getIconForImageFile(const QString &file) {
    if (iconPool.contains(file)) {
      return iconPool[file];
    }
    else {
      QImage imageFile(file);

      if (!imageFile.isNull()) {
        iconPool[file] = QPixmap::fromImage(imageFile.scaled(32,32));
        return iconPool[file];
      }
    }

    return nullIcon;
  }

private:

  QMap<QString, QIcon> iconPool;
  QIcon nullIcon;
};

static QImageIconPool imageIconPool;

bool TulipFileDescriptorEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& v) const {
  TulipItemEditorCreator::paint(painter,option,v);
  QRect rect = option.rect;
  TulipFileDescriptor fileDesc = v.value<TulipFileDescriptor>();
  QFileInfo fileInfo(fileDesc.absolutePath);
  QIcon icon;
  QString text;

  const QIcon &imageIcon = imageIconPool.getIconForImageFile(fileInfo.absoluteFilePath());

  if (!imageIcon.isNull()) {
    icon = imageIcon;
    text = fileInfo.fileName();
  }
  else if (fileInfo.isFile()) {
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

QSize TulipFileDescriptorEditorCreator::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  TulipFileDescriptor fileDesc = data.value<TulipFileDescriptor>();
  QFileInfo fileInfo(fileDesc.absolutePath);
  QString text;

  if (fileInfo.isDir()) {
    QDir d1 = fileInfo.dir();
    d1.cdUp();
    text = fileInfo.absoluteFilePath().remove(0,d1.absolutePath().length()-1);
  }
  else {
    text = fileInfo.fileName();
  }

  const int pixmapWidth = 32;

  QFontMetrics fontMetrics(option.font);

  return QSize(pixmapWidth+fontMetrics.boundingRect(text).width(), pixmapWidth);
}

QWidget* NodeShapeEditorCreator::createWidget(QWidget*parent) const {
  CustomComboBox* combobox = new CustomComboBox(parent);
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
  combobox->setCurrentIndex(combobox->findData(data.value<NodeShape::NodeShapes>()));
}

QVariant NodeShapeEditorCreator::editorData(QWidget*editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<NodeShape::NodeShapes>(static_cast<NodeShape::NodeShapes>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString NodeShapeEditorCreator::displayText(const QVariant & data) const {
  return tlpStringToQString(GlyphManager::getInst().glyphName(data.value<NodeShape::NodeShapes>()));
}

QSize NodeShapeEditorCreator::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QPixmap pixmap = GlyphRenderer::getInst().render(data.value<NodeShape::NodeShapes>());
  QFontMetrics fontMetrics(option.font);
  return QSize(pixmap.width()+fontMetrics.boundingRect(displayText(data)).width()+20, pixmap.height());
}

bool NodeShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const {
  TulipItemEditorCreator::paint(painter,option,data);

  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  QPixmap pixmap = GlyphRenderer::getInst().render(data.value<NodeShape::NodeShapes>());
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
  CustomComboBox* combobox = new CustomComboBox(parent);
  combobox->addItem(QString("NONE"),EdgeExtremityShape::None);
  std::list<std::string> glyphs(PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>());

  for(std::list<std::string>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
    std::string glyphName(*it);
    const tlp::Plugin& infos = PluginLister::pluginInformation(glyphName);
    int glyphIndex = infos.id();
    //Create the glyph entry
    combobox->addItem(EdgeExtremityGlyphRenderer::getInst().render(glyphIndex),tlpStringToQString(glyphName),glyphIndex);
  }

  return combobox;
}
void EdgeExtremityShapeEditorCreator::setEditorData(QWidget* editor, const QVariant& data,bool,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  combobox->setCurrentIndex(combobox->findData(data.value<EdgeExtremityShape::EdgeExtremityShapes>()));
}

QVariant EdgeExtremityShapeEditorCreator::editorData(QWidget* editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<EdgeExtremityShape::EdgeExtremityShapes>(static_cast<EdgeExtremityShape::EdgeExtremityShapes>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeExtremityShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(EdgeExtremityGlyphManager::getInst().glyphName(data.value<EdgeExtremityShape::EdgeExtremityShapes>()));
}

bool EdgeExtremityShapeEditorCreator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant& data) const {
  TulipItemEditorCreator::paint(painter,option,data);

  QStyleOptionViewItemV4 opt = option;
  opt.features |= QStyleOptionViewItemV2::HasDecoration;
  QPixmap pixmap = EdgeExtremityGlyphRenderer::getInst().render(data.value<EdgeExtremityShape::EdgeExtremityShapes>());
  opt.icon = QIcon(pixmap);
  opt.decorationSize = pixmap.size();

  opt.features |= QStyleOptionViewItemV2::HasDisplay;
  opt.text = displayText(data);

  QStyle *style = QApplication::style();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, NULL);
  return true;
}

QSize EdgeExtremityShapeEditorCreator::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QVariant data = index.model()->data(index);
  static QPixmap pixmap = EdgeExtremityGlyphRenderer::getInst().render(data.value<EdgeExtremityShape::EdgeExtremityShapes>());
  QFontMetrics fontMetrics(option.font);
  return QSize(pixmap.width()+fontMetrics.boundingRect(displayText(data)).width()+40, pixmap.height());
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
  combobox->setCurrentIndex(combobox->findData(static_cast<int>(data.value<EdgeShape::EdgeShapes>())));
}

QVariant EdgeShapeEditorCreator::editorData(QWidget* editor,Graph*) {
  QComboBox* combobox = static_cast<QComboBox*>(editor);
  return QVariant::fromValue<EdgeShape::EdgeShapes>(static_cast<EdgeShape::EdgeShapes>(combobox->itemData(combobox->currentIndex()).toInt()));
}

QString EdgeShapeEditorCreator::displayText(const QVariant &data) const {
  return tlpStringToQString(GlGraphStaticData::edgeShapeName(data.value<EdgeShape::EdgeShapes>()));
}

//TulipFontEditorCreator
QWidget* TulipFontEditorCreator::createWidget(QWidget* parent) const {
  return new TulipFontDialog(Perspective::instance()
                             ? Perspective::instance()->mainWindow()
                             : parent);
}
void TulipFontEditorCreator::setEditorData(QWidget*editor, const QVariant&data,bool,tlp::Graph*) {
  TulipFont font =data.value<TulipFont>();
  TulipFontDialog* fontWidget = static_cast<TulipFontDialog*>(editor);
  fontWidget->selectFont(font);
  fontWidget->move(QCursor::pos() - QPoint(fontWidget->width()/2,
                   fontWidget->height()/2));

}

QVariant TulipFontEditorCreator::editorData(QWidget* editor,tlp::Graph*) {
  TulipFontDialog* fontWidget = static_cast<TulipFontDialog*>(editor);
  return QVariant::fromValue<TulipFont>(fontWidget->getSelectedFont());
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
  comboBox->setCurrentIndex((int)(var.value<LabelPosition::LabelPositions>()));
}
QVariant TulipLabelPositionEditorCreator::editorData(QWidget* w,tlp::Graph*) {
  return QVariant::fromValue<LabelPosition::LabelPositions>(static_cast<LabelPosition::LabelPositions>(static_cast<QComboBox*>(w)->currentIndex()));
}
QString TulipLabelPositionEditorCreator::displayText(const QVariant& v) const {
  int pos = (int)(v.value<LabelPosition::LabelPositions>());

  if (pos < MIN_LABEL_POSITION || pos > MAX_LABEL_POSITION) {
    qCritical() << QObject::trUtf8("Invalid value found as label position");
    return QObject::trUtf8("Invalid label position");
  }
  else
    return POSITION_LABEL[pos];
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

//EdgeSetEditorCreator
QWidget* EdgeSetEditorCreator::createWidget(QWidget *parent) const {
  return new QLabel(parent);
}

void EdgeSetEditorCreator::setEditorData(QWidget* w, const QVariant& var, bool, tlp::Graph*) {
  std::set<tlp::edge> eset = var.value<std::set<tlp::edge> >();

  std::stringstream ss;
  tlp::EdgeSetType::write(ss, eset);
  static_cast<QLabel*>(w)->setText(ss.str().c_str());
}

QVariant EdgeSetEditorCreator::editorData(QWidget*,tlp::Graph*) {
  return QVariant();
}

QString EdgeSetEditorCreator::displayText(const QVariant& var) const {
  std::set<tlp::edge> eset = var.value<std::set<tlp::edge> >();

  std::stringstream ss;
  tlp::EdgeSetType::write(ss, eset);

  return ss.str().c_str();
}

QWidget* QVectorBoolEditorCreator::createWidget(QWidget*) const {
  VectorEditor* w = new VectorEditor(NULL);
  w->setWindowFlags(Qt::Dialog);
  w->setWindowModality(Qt::ApplicationModal);
  return w;
}

void QVectorBoolEditorCreator::setEditorData(QWidget* editor, const QVariant& v,bool,tlp::Graph*) {
  QVector<QVariant> editorData;
  QVector<bool> vect = v.value<QVector<bool> >();

  for (int i=0; i < vect.size(); ++i)  {
    editorData.push_back(QVariant::fromValue<bool>(vect[i]));
  }

  static_cast<VectorEditor*>(editor)->setVector(editorData,qMetaTypeId<bool>());

  static_cast<VectorEditor*>(editor)->move(QCursor::pos());
}

QVariant QVectorBoolEditorCreator::editorData(QWidget* editor,tlp::Graph*) {
  QVector<bool> result;
  QVector<QVariant> editorData = static_cast<VectorEditor*>(editor)->vector();
  foreach(QVariant v, editorData)
  result.push_back(v.value<bool>());
  return QVariant::fromValue<QVector<bool> >(result);
}

QString QVectorBoolEditorCreator::displayText(const QVariant &data) const {
  std::vector<bool> v = data.value<QVector<bool> >().toStdVector();

  if (v.empty())
    return QString();

  // use a DataTypeSerializer if any
  DataTypeSerializer* dts =
    DataSet::typenameToSerializer(std::string(typeid(v).name()));

  if (dts) {
    DisplayVectorDataType<bool> dt(&v);

    std::stringstream sstr;
    dts->writeData(sstr, &dt);

    std::string str = sstr.str();

    if (str.size() > 45)
      str.replace(str.begin() + 41, str.end(), " ...)");

    return QString::fromUtf8(str.c_str());
  }

  if (v.size() == 1)
    return QString("1 element");

  return QString::number(v.size()) + QObject::trUtf8(" elements");
}

//QStringEditorCreator
QWidget* QStringEditorCreator::createWidget(QWidget *parent) const {
  StringEditor* editor =
    new StringEditor(tlp::Perspective::instance()
                     ? tlp::Perspective::instance()->mainWindow()
                     : parent);
  editor->setWindowTitle(QString("Set ") + propName.c_str() + " value");
  editor->setMinimumSize(QSize(250, 250));
  return editor;
}

void QStringEditorCreator::setEditorData(QWidget* editor, const QVariant& var,
    bool, tlp::Graph*) {
  static_cast<StringEditor*>(editor)->setString(var.toString());
}

QVariant QStringEditorCreator::editorData(QWidget* editor, tlp::Graph*) {
  return static_cast<StringEditor*>(editor)->getString();
}

QString QStringEditorCreator::displayText(const QVariant& var) const {
  QString qstr = var.toString();

  if (qstr.size() > 45) {
    qstr.truncate(41);
    qstr.append(" ...");
  }

  return qstr;
}

void QStringEditorCreator::setPropertyToEdit(tlp::PropertyInterface* prop) {
  propName = prop->getName();
}

QWidget *QStringListEditorCreator::createWidget(QWidget *) const {
  VectorEditor* w = new VectorEditor(NULL);
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
  static_cast<VectorEditor*>(w)->setVector(vect,qMetaTypeId<QString>());
}

QVariant QStringListEditorCreator::editorData(QWidget* w, Graph*) {
  QVector<QVariant> vect = static_cast<VectorEditor*>(w)->vector();
  QStringList lst;
  foreach(QVariant v, vect)
  lst.push_back(v.toString());
  return lst;
}

QString QStringListEditorCreator::displayText(const QVariant &var) const {
  return QStringListType::toString(var.toStringList()).c_str();
}
