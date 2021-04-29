/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "tulip/CSVImportWizard.h"
#include "tulip/CSVImportConfigurationWidget.h"
#include "ui_CSVImportConfigurationWidget.h"
#include "ui_CSVPropertyDialog.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QStylePainter>

#include <tulip/CSVParser.h>
#include <tulip/CSVGraphImport.h>
#include <tulip/TlpQtTools.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

PropertyConfigurationWidget::PropertyConfigurationWidget(
    unsigned int propertyNumber, const QString &propertyName, bool propertyNameIsEditable,
    const std::string &propertyType, PropertyNameValidator *validator, QWidget *parent)
    : QWidget(parent), CSVColumn(QStringToTlpString(propertyName), propertyType),
      propertyNameValidator(validator), propertyEditButton(new QPushButton(this)),
      nameEditable(propertyNameIsEditable), propertyNumber(propertyNumber) {
  setLayout(new QVBoxLayout());
  layout()->setContentsMargins(0, 0, 0, 0);
  layout()->setSpacing(0);
  propertyEditButton->setText(propertyName);
  propertyEditButton->setEnabled(propertyNameIsEditable);
  propertyEditButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  layout()->addWidget(propertyEditButton);
  connect(propertyEditButton, SIGNAL(released()), this, SLOT(showPropertyCreationDialog()));
  layout()->setAlignment(propertyEditButton, Qt::AlignHCenter);

  setPropertyType(propertyType);
}

const string &PropertyConfigurationWidget::getPropertyType() const {
  // Return the real type
  return _type;
}

void PropertyConfigurationWidget::setPropertyType(const string &pType) {
  _type = pType.empty() ? "string" : pType;
  propertyEditButton->setText(
      QString("%1\n[%2]").arg(getPropertyName()).arg(propertyTypeToPropertyTypeLabel(_type)));
}

QString PropertyConfigurationWidget::getPropertyName() const {
  return tlpStringToQString(_name);
}
void PropertyConfigurationWidget::setPropertyName(const QString &pName) {
  _name = QStringToTlpString(pName);
  propertyEditButton->setText(QString("%1\n[%2]").arg(pName).arg(QString(_type.c_str())));
  propertyEditButton->setToolTip(QString("<center><b>Column #%1</b></center>name: %2<br/>type: "
                                         "%3<br>Click for more import options.")
                                     .arg(propertyNumber)
                                     .arg(pName)
                                     .arg(_type.c_str()));
}

unsigned int PropertyConfigurationWidget::getPropertyNumber() const {
  return propertyNumber;
}

void PropertyConfigurationWidget::toggleUsed() {
  _used = !_used;
  propertyEditButton->setEnabled(_used);
  emit stateChange(_used);
}

void PropertyConfigurationWidget::typeCBChanged(const QString &type) {
  bool enabled = type.contains(QString("Vector"));
  ui->separatorCB->setEnabled(enabled);
  // update ui->nameCB
  int nbItems = ui->nameCB->count();
  for (int i = 1; i < nbItems; ++i)
    ui->nameCB->removeItem(1);
  const std::set<std::string> &props =
      CSVImportConfigurationWidget::getPropsForTypename(propertyTypeLabelToPropertyType(type));
  for (const std::string &prop : props)
    ui->nameCB->addItem(tlpStringToQString(prop));
}

void PropertyConfigurationWidget::addException() {
  QTableWidget *w = ui->exceptionTableWidget;
  auto row = w->rowCount();
  w->insertRow(row);
  w->setItem(row, 0, new QTableWidgetItem(QString("edit the value")));
  QComboBox *actionCB = new QComboBox(w);
  actionCB->addItem(QString("Assign no value"));
  actionCB->addItem(QString("Ignore the row"));
  w->setCellWidget(row, 1, actionCB);
}

void PropertyConfigurationWidget::delCurrentException() {
  QTableWidget *w = ui->exceptionTableWidget;
  auto row = w->currentRow();
  if (row > -1)
    w->removeRow(row);
}

void PropertyConfigurationWidget::showPropertyCreationDialog() {
  QDialog dialog(this);
  ui = new Ui_CSVPropertyDialog();
  ui->setupUi(&dialog);
  ui->columnLabel->setText(QString("Column #%1").arg(propertyNumber + 1));
  propertyNameValidator->setCurrentIndex(propertyNumber);
  ui->nameCB->setValidator(propertyNameValidator);
  ui->typeCB->clear();
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(BooleanProperty::propertyTypename),
                      QVariant(QString::fromStdString(BooleanProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(DoubleProperty::propertyTypename),
                      QVariant(QString::fromStdString(DoubleProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(ColorProperty::propertyTypename),
                      QVariant(QString::fromStdString(ColorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(IntegerProperty::propertyTypename),
                      QVariant(QString::fromStdString(IntegerProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(LayoutProperty::propertyTypename),
                      QVariant(QString::fromStdString(LayoutProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(SizeProperty::propertyTypename),
                      QVariant(QString::fromStdString(SizeProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(StringProperty::propertyTypename),
                      QVariant(QString::fromStdString(StringProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(BooleanVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(BooleanVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(ColorVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(ColorVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(CoordVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(CoordVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(DoubleVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(DoubleVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(IntegerVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(IntegerVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(SizeVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(SizeVectorProperty::propertyTypename)));
  ui->typeCB->addItem(propertyTypeToPropertyTypeLabel(StringVectorProperty::propertyTypename),
                      QVariant(QString::fromStdString(StringVectorProperty::propertyTypename)));

  auto index = ui->typeCB->findData(QVariant(QString::fromStdString(_type)));
  if (index != -1) {
    ui->typeCB->setCurrentIndex(index);
  }
  ui->nameCB->addItem(tlpStringToQString(_name));
  ui->nameCB->setCurrentIndex(0);
  typeCBChanged(ui->typeCB->currentText());
  connect(ui->typeCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(typeCBChanged(QString)));

  if (_valueSeparator) {
    index = ui->separatorCB->findText(QString(QChar(_valueSeparator)));
    if (index != -1)
      ui->separatorCB->setCurrentIndex(index);
  }

  ui->exceptionTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  connect(ui->addExceptionButton, SIGNAL(released()), this, SLOT(addException()));
  connect(ui->delCurrentExceptionButton, SIGNAL(released()), this, SLOT(delCurrentException()));

  if (dialog.exec() == QDialog::Accepted) {
    _name = QStringToTlpString(ui->nameCB->currentText());
    setPropertyType(propertyTypeLabelToPropertyType(ui->typeCB->currentText()));
    _valueSeparator = ui->separatorCB->currentText()[0].toLatin1();
    clearExceptions();
    for (int i = 0; i < ui->exceptionTableWidget->rowCount(); ++i) {
      std::string value = QStringToTlpString(ui->exceptionTableWidget->item(i, 0)->text());
      auto action =
          static_cast<QComboBox *>(ui->exceptionTableWidget->cellWidget(i, 1))->currentIndex();
      CSVColumn::addException(value, CSVColumn::Action(action));
    }
  }
  delete ui;
}

CSVTableHeader::CSVTableHeader(QWidget *parent,
                               std::vector<PropertyConfigurationWidget *> &propertyWidgets)
    : QHeaderView(Qt::Horizontal, parent), widgets(propertyWidgets) {
  setSectionsClickable(true);
  connect(this, SIGNAL(sectionPressed(int)), this, SLOT(checkBoxPressed(int)));
}

void CSVTableHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
  painter->save();
  QHeaderView::paintSection(painter, rect, logicalIndex);
  painter->restore();
  QStyleOptionButton cb;
  cb.initFrom(parentWidget());
  cb.state = QStyle::State_Enabled |
             (widgets[logicalIndex]->isUsed() ? QStyle::State_On : QStyle::State_Off);
  auto cbRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &cb);
#ifdef __APPLE__
  // this a specific MacOSX hack because on this platform
  // style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &cb, painter)
  // executes always as if cb.rect.x() = cb.rect.y() = 0
  cb.rect = QRect(0, 0, cbRect.width(), cbRect.height());
  QPixmap pix(cbRect.width(), cbRect.height());
  QStylePainter pixpainter(&pix, parentWidget());
  pixpainter.fillRect(0, 0, cbRect.width(), cbRect.height(), cb.palette.brush(QPalette::Midlight));
  style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &cb, &pixpainter);
  pixpainter.end();
  painter->drawPixmap(rect.x() + (rect.width() - cbRect.width()) / 2,
                      rect.y() + (rect.height() - cbRect.height()) / 2, pix, 0, 0, cbRect.width(),
                      cbRect.height());
#else
  cb.rect =
      QRect(rect.x() + (rect.width() - cbRect.width()) / 2,
            rect.y() + (rect.height() - cbRect.height()) / 2, cbRect.width(), cbRect.height());
  style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &cb, painter);
#endif
}

void CSVTableHeader::checkBoxPressed(int logicalIndex) {
  widgets[logicalIndex]->toggleUsed();
  this->update();
}

CSVTableWidget::CSVTableWidget(QWidget *parent)
    : QTableWidget(parent), maxLineNumber(UINT_MAX), firstLineIndex(0), checkCommentsLines(true),
      nbCommentsLines(0) {
  horizontalHeader()->setMinimumSectionSize(50);
}

bool CSVTableWidget::begin() {
  clear();
  setColumnCount(0);
  setRowCount(0);
  nbCommentsLines = 0;
  checkCommentsLines = true;
  return true;
}

bool CSVTableWidget::line(unsigned int row, const vector<string> &lineTokens) {

  if ((row < firstLineIndex) || // Wait for the first line index
                                // If the maximum line number is reached ignore the token.
      (static_cast<unsigned>(rowCount()) >= maxLineNumber))
    return true;

  if (checkCommentsLines) {
    if (lineTokens[0][0] == '#')
      ++nbCommentsLines;
    else if (lineTokens[0].substr(0, 2) == "//")
      ++nbCommentsLines;
    else
      checkCommentsLines = false;
  }

  // Add a new row in the table
  int currentRow = rowCount();
  insertRow(currentRow);

  for (size_t column = 0; column < lineTokens.size(); ++column) {
    // Add a new column if needed
    if (uint(columnCount()) <= column) {
      insertColumn(column);
    }

    // Fill the table
    setItem(currentRow, column, new QTableWidgetItem(tlpStringToQString(lineTokens[column])));
  }

  return true;
}

bool CSVTableWidget::end(unsigned int, unsigned int) {
  return true;
}

static std::unordered_map<std::string, std::set<std::string>> typenameToProps;

CSVImportConfigurationWidget::CSVImportConfigurationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::CSVImportConfigurationWidget),
      validator(new PropertyNameValidator(propertyWidgets, this)), maxLineNumber(0), parser(NULL),
      firstLine(0), guessFirstLineIsHeader(true), keepPropertyWidgets(false) {
  ui->setupUi(this);

  // Import line number change
  connect(ui->useFirstLineAsPropertyNamecheckBox, SIGNAL(clicked(bool)), this,
          SLOT(useFirstLineAsHeaderUpdated()));
  connect(ui->limitPreviewLineNumberCheckBox, SIGNAL(clicked(bool)), this,
          SLOT(filterPreviewLineNumber(bool)));
  connect(ui->previewLineNumberSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(previewLineNumberChanged(int)));

  // Init the preview line number with the right value.
  if (ui->limitPreviewLineNumberCheckBox->isChecked()) {
    setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
  } else {
    ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
  }

  // set horizontal header
  ui->previewTableWidget->setHorizontalHeader(
      new CSVTableHeader(ui->previewTableWidget, propertyWidgets));
  ui->previewTableWidget->horizontalHeader()->setMinimumSectionSize(120);
  ui->previewTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CSVImportConfigurationWidget::~CSVImportConfigurationWidget() {
  delete ui;
  delete parser;
}

const std::set<std::string> &
CSVImportConfigurationWidget::getPropsForTypename(const std::string &type) {
  return typenameToProps[type];
}

void CSVImportConfigurationWidget::setFirstLineIndex(int fl) {
  firstLine = fl;
  ui->importLinesLabel->setText(QString("Import lines from %1 to").arg(fl + 1));
  ui->toLineSpinBox->setMinimum(fl + 1);
}

void CSVImportConfigurationWidget::setNewParser(CSVParser *newParser) {
  delete parser;
  parser = newParser;
  guessFirstLineIsHeader = true;
  updateWidget("Parsing file to guess column types...");
  guessFirstLineIsHeader = false;
  // Reset import range
  updateLineNumbers(true);
}

void CSVImportConfigurationWidget::updateWidget(const std::string &title) {
  if (parser) {
    setEnabled(true);
    SimplePluginProgressDialog progress(this);
    progress.showPreview(false);
    progress.setWindowTitle(QString(title.c_str()));
    progress.show();

    // if needed try to guess if the first line is used as header.
    if (guessFirstLineIsHeader) {
      setUseFirstLineAsPropertyName(true);
      // parse the first line only
      parser->parse(this, &progress, true);

      for (unsigned int i = 0; i < columnHeaderType.size(); ++i) {
        // If there is at least one column with a header type different
        // from StringProperty, then the first line is not the header
        if (columnHeaderType[i] != StringProperty::propertyTypename) {
          setUseFirstLineAsPropertyName(false);
          break;
        }
      }
    }
    parser->parse(this, &progress);
  } else {
    setEnabled(true);
  }
}

bool CSVImportConfigurationWidget::begin() {
  if (keepPropertyWidgets == false) {
    ui->previewTableWidget->begin();
    ui->previewTableWidget->setFirstLineIndex(getFirstLineIndex());
    clearPropertiesTypeList();
    // Clear initialized columns
    columnHeaderType.clear();
    columnType.clear();
    // insert a first row to display the propertyWidgets
    // (see line method below)
    ui->previewTableWidget->insertRow(0);
    ui->previewTableWidget->setRowHeight(0, 50);
  } else
    ui->previewTableWidget->setRowCount(1);
  return true;
}

bool CSVImportConfigurationWidget::line(unsigned int row, const vector<string> &lineTokens) {
  ui->previewTableWidget->line(row, lineTokens);

  if (keepPropertyWidgets)
    return true;

  // Wait for the first row
  if (row >= getFirstLineIndex()) {
    if (useFirstLineAsPropertyName()) {
      if (row == getFirstLineIndex())
        headerColumnCount = columnCount();
      else if (lineTokens.size() > headerColumnCount) {
        if (QMessageBox::warning(
                this, "Invalid number of row fields",
                QString(
                    "row %1: the number of fields (%2) is greater than the number of columns (%3)")
                    .arg(row + 1)
                    .arg(lineTokens.size())
                    .arg(headerColumnCount),
                QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Cancel)
          return false;
      }
    }

    for (size_t column = 0; column < lineTokens.size(); ++column) {
      // A new column was created set its label and its configuration widget.
      if (propertyWidgets.size() <= column) {
        QString columnName = generateColumnName(column);
        // Store the first token type
        columnHeaderType.push_back(guessDataType(lineTokens[column]));
        // Mark the column type as uninitialized
        columnType.push_back("");
        // Create the new column widget. The default type is string
        addPropertyToPropertyList(QStringToTlpString(columnName), true,
                                  StringProperty::propertyTypename);
      } else {
        // If the widget is not initialized do not use the default type
        string previousPropertyType = columnType[column];
        string propertyType = guessPropertyDataType(lineTokens[column], previousPropertyType);
        // Store the new type
        columnType[column] = propertyType;
      }
    }
  }

  return true;
}

bool CSVImportConfigurationWidget::end(unsigned int rowNumber, unsigned int) {
  maxLineNumber = rowNumber;

  // Force the table to correctly update.
  useFirstLineAsHeaderUpdated();
  // Avoid updating widget.
  if (rowNumber > 1) {
    ui->previewLineNumberSpinBox->blockSignals(true);
    ui->previewLineNumberSpinBox->setMaximum(rowNumber);
    ui->previewLineNumberSpinBox->blockSignals(false);
  }
  return true;
}

void CSVImportConfigurationWidget::setMaxPreviewLineNumber(unsigned int lineNumber) {
  if (useFirstLineAsPropertyName())
    ++lineNumber;
  ui->previewTableWidget->setMaxPreviewLineNumber(lineNumber);
}

void CSVImportConfigurationWidget::filterPreviewLineNumber(bool checked) {
  if (checked) {
    setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
  } else {
    ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
  }

  updateWidget();
  // Reset import range
  updateLineNumbers(true);
}

void CSVImportConfigurationWidget::previewLineNumberChanged(int maxLineNumber) {
  setMaxPreviewLineNumber(maxLineNumber);
  keepPropertyWidgets = true;
  updateWidget();
  keepPropertyWidgets = false;
  // Reset import range
  updateLineNumbers(false);
}

void CSVImportConfigurationWidget::toLineValueChanged(int) {
  updateWidget();
}

bool CSVImportConfigurationWidget::useFirstLineAsPropertyName() const {
  return ui->useFirstLineAsPropertyNamecheckBox->checkState() == Qt::Checked;
}

void CSVImportConfigurationWidget::setUseFirstLineAsPropertyName(bool useFirstLineAsHeader) const {
  ui->useFirstLineAsPropertyNamecheckBox->setChecked(useFirstLineAsHeader);
}

unsigned int CSVImportConfigurationWidget::rowCount() const {
  return ui->previewTableWidget->rowCount();
}
unsigned int CSVImportConfigurationWidget::columnCount() const {
  return ui->previewTableWidget->columnCount();
}

void CSVImportConfigurationWidget::updateTableHeaders() {
  QStringList itemsLabels;

  for (unsigned int i = 0; i < columnCount(); ++i) {
    // Update the column name
    QString columnName = generateColumnName(i);
    itemsLabels << ""; // columnName;
    propertyWidgets[i]->setPropertyName(columnName);
    // Update the column type.
    propertyWidgets[i]->setPropertyType(getColumnType(i));
  }

  ui->previewTableWidget->setHorizontalHeaderLabels(itemsLabels);
  itemsLabels.clear();
  // Ensure that the first visible row has the number 1.
  unsigned int currentRow = useFirstLineAsPropertyName() ? 0 : 1;

  for (unsigned int i = 0; i < rowCount(); ++i) {
    itemsLabels << QString::number(currentRow);
    ++currentRow;
  }

  ui->previewTableWidget->setVerticalHeaderLabels(itemsLabels);
}

QString CSVImportConfigurationWidget::generateColumnName(unsigned int col) const {
  if (useFirstLineAsPropertyName()) {
    QTableWidgetItem *item = ui->previewTableWidget->item(1, col);

    if (item != nullptr) {
      return item->text();
    } else {
      return QString("Column_") + QString::number(col + 1);
    }
  } else {
    return QString("Column_") + QString::number(col + 1);
  }
}

string CSVImportConfigurationWidget::getColumnType(unsigned int col) const {
  if (useFirstLineAsPropertyName()) {
    return columnType[col];
  } else {
    return combinePropertyDataType(columnType[col], columnHeaderType[col]);
  }
}

void CSVImportConfigurationWidget::useFirstLineAsHeaderUpdated() {
  if (useFirstLineAsPropertyName()) {
    ui->previewTableWidget->hideRow(1);
    ui->previewTableWidget->showRow(ui->previewTableWidget->rowCount() - 1);
  } else {
    ui->previewTableWidget->showRow(1);
    ui->previewTableWidget->hideRow(ui->previewTableWidget->rowCount() - 1);
  }

  if (keepPropertyWidgets == false) {
    updateTableHeaders();
    updateLineNumbers(false);
  }
}

void CSVImportConfigurationWidget::updateLineNumbers(bool resetValues) {
  blockSignals(true);
  ui->toLineSpinBox->blockSignals(true);
  bool wasAtMax = ui->toLineSpinBox->value() == ui->toLineSpinBox->maximum();
  // If the first line is not headers the maximum of line number to import increase.
  int rowNumber = useFirstLineAsPropertyName() ? maxLineNumber - 1 : maxLineNumber;

  // If the old value was at the maximum keep it
  if (wasAtMax) {
    ui->toLineSpinBox->setValue(rowNumber);
  }

  // Reset all the values of the spinbox.
  if (resetValues)
    ui->toLineSpinBox->setValue(rowNumber);

  // Reset min/max values
  ui->toLineSpinBox->setMaximum(rowNumber);

  ui->toLineSpinBox->blockSignals(false);
  blockSignals(false);
}

void CSVImportConfigurationWidget::clearPropertiesTypeList() {
  propertyWidgets.clear();
}

void CSVImportConfigurationWidget::addPropertyToPropertyList(const string &propertyName,
                                                             bool isEditable,
                                                             const std::string &propertyType) {

  PropertyConfigurationWidget *propertyConfigurationWidget = createPropertyConfigurationWidget(
      propertyWidgets.size(), QString::fromStdString(propertyName), isEditable, propertyType,
      ui->previewTableWidget);
  if (ui->previewTableWidget->rowCount() == 0) {
    ui->previewTableWidget->insertRow(0);
    ui->previewTableWidget->setRowHeight(0, 50);
  }
  propertyWidgets.push_back(propertyConfigurationWidget);

  int column = propertyWidgets.size() - 1;
  // Fill the first row
  ui->previewTableWidget->setItem(0, column, new QTableWidgetItem());
  ui->previewTableWidget->setCellWidget(0, column, propertyConfigurationWidget);
}

PropertyConfigurationWidget *CSVImportConfigurationWidget::createPropertyConfigurationWidget(
    unsigned int propertyNumber, const QString &propertyName, bool isEditable,
    const string &propertyType, QWidget *parent) {
  PropertyConfigurationWidget *propertyConfigurationWidget = new PropertyConfigurationWidget(
      propertyNumber, propertyName, isEditable, propertyType, validator, parent);
  propertyConfigurationWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  connect(propertyConfigurationWidget, SIGNAL(stateChange(bool)), this,
          SLOT(propertyStateChanged(bool)));
  propertyConfigurationWidget->installEventFilter(this);

  // rebuild typenameToProps if needed
  if (propertyNumber == 0) {
    typenameToProps.clear();

    auto itp = CSVImportWizard::getGraph()->getObjectProperties();
    while (itp->hasNext()) {
      auto prop = itp->next();
      typenameToProps[prop->getTypename()].insert(prop->getName());
    }
  }

  return propertyConfigurationWidget;
}

void CSVImportConfigurationWidget::propertyStateChanged(bool state) {
  PropertyConfigurationWidget *widget = qobject_cast<PropertyConfigurationWidget *>(sender());
  assert(widget != nullptr);

  for (int i = 0; i < ui->previewTableWidget->rowCount(); ++i) {
    QTableWidgetItem *item = ui->previewTableWidget->item(i, widget->getPropertyNumber());

    if (item) {
      item->setFlags(state ? Qt::ItemIsEnabled : Qt::NoItemFlags);
    }
  }
}

const vector<CSVColumn *> CSVImportConfigurationWidget::getPropertiesToImport() const {
  vector<CSVColumn *> properties(propertyWidgets.size());

  for (size_t i = 0; i < propertyWidgets.size(); ++i) {
    properties[i] = propertyWidgets[i];
  }

  return properties;
}

unsigned int CSVImportConfigurationWidget::getFirstImportedLineIndex() const {
  unsigned int firstLine = getFirstLineIndex();
  // Shift the line number if we use the first line as header.
  return useFirstLineAsPropertyName() ? ++firstLine : firstLine;
}

unsigned int CSVImportConfigurationWidget::getFirstLineIndex() const {
  return firstLine;
}

unsigned int CSVImportConfigurationWidget::getLastLineIndex() const {
  unsigned int lastLine = ui->toLineSpinBox->value();

  if (!useFirstLineAsPropertyName()) {
    --lastLine;
  }

  return lastLine;
}

CSVImportParameters CSVImportConfigurationWidget::getImportParameters() const {
  return CSVImportParameters(getFirstImportedLineIndex(), getLastLineIndex(),
                             getPropertiesToImport());
}

QValidator::State PropertyNameValidator::validate(QString &input, int &) const {
  if (input.isEmpty()) {
    return QValidator::Invalid;
  }

  // Only the property at the current index can have this name
  for (auto widget : widgets) {
    if ((widget->getPropertyName().compare(input) == 0) &&
        (currentIndex != widget->getPropertyNumber()))
      return QValidator::Invalid;
  }

  return QValidator::Acceptable;
}

const string &
CSVImportConfigurationWidget::guessPropertyDataType(const string &data,
                                                    const string &previousType) const {
  // If there is no data skip the token
  if (data.empty()) {
    return previousType;
  }

  const string &dataType = guessDataType(data);
  return combinePropertyDataType(previousType, dataType);
}

const string &CSVImportConfigurationWidget::combinePropertyDataType(const string &previousType,
                                                                    const string &newType) const {
  if (previousType.empty()) {
    return newType;
  } else if (previousType == newType) {
    return newType;
  } else if (newType.empty())
    return previousType;
  // If both types are numeric return the more generic numeric type : double
  else if (previousType == BooleanProperty::propertyTypename &&
           (newType == DoubleProperty::propertyTypename ||
            newType == IntegerProperty::propertyTypename))
    return newType;
  else if (previousType == IntegerProperty::propertyTypename) {
    if (newType == DoubleProperty::propertyTypename)
      return DoubleProperty::propertyTypename;

    if (newType == BooleanProperty::propertyTypename)
      return IntegerProperty::propertyTypename;
  } else if (previousType == DoubleProperty::propertyTypename &&
             (newType == BooleanProperty::propertyTypename ||
              newType == IntegerProperty::propertyTypename))
    return DoubleProperty::propertyTypename;

  return StringProperty::propertyTypename;
}

// need a constant
static const std::string emptyString;

const string &CSVImportConfigurationWidget::guessDataType(const string &data) const {
  char *ptr = const_cast<char *>(data.c_str());

  while (isspace(*ptr))
    ++ptr;

  if (!*ptr)
    return emptyString;

  bool b;

  if (BooleanType::fromString(b, std::string(ptr), true)) {
    // The type is boolean
    return BooleanProperty::propertyTypename;
  }

  // Qt framework is the best way to detect numerals in string.
  QString str = tlpStringToQString(ptr);
  bool ok = false;
  str.toInt(&ok);

  // The type is int
  if (ok)
    return IntegerProperty::propertyTypename;

  if (parser->decimalMark() == ',') {
    QLocale c(QLocale::French);
    c.toDouble(str, &ok);
  } else
    str.toDouble(&ok);

  // The type is double
  if (ok) {
    return DoubleProperty::propertyTypename;
  } else {
    // All the other cases are treated as string.
    return StringProperty::propertyTypename;
  }
}
