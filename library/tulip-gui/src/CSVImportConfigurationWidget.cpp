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
#include "tulip/CSVImportConfigurationWidget.h"
#include "ui_CSVImportConfigurationWidget.h"

#include <QComboBox>
#include <QLineEdit>

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

PropertyConfigurationWidget::PropertyConfigurationWidget(unsigned int propertyNumber, const QString& propertyName,
    bool propertyNameIsEditable, const std::string& PropertyType, QWidget* parent) :
  QWidget(parent), propertyNameLineEdit(new QLineEdit(this)), propertyTypeComboBox(new QComboBox(this)), usedCheckBox(
    new QCheckBox("", this)), nameEditable(propertyNameIsEditable), propertyNumber(propertyNumber) {
  setLayout(new QVBoxLayout());
  layout()->setContentsMargins(0, 0, 0, 0);
  layout()->setSpacing(0);
  layout()->addWidget(usedCheckBox);
  layout()->setAlignment(usedCheckBox, Qt::AlignHCenter);
  connect(usedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useStateChanged(int)));
  usedCheckBox->setCheckState(Qt::Checked);
  propertyNameLineEdit->setText(propertyName);
  propertyNameLineEdit->setEnabled(propertyNameIsEditable);
  propertyNameLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  layout()->addWidget(propertyNameLineEdit);
  layout()->setAlignment(propertyNameLineEdit, Qt::AlignHCenter);
  fillPropertyTypeComboBox();
  propertyTypeComboBox->setCurrentIndex(0);

  if(!PropertyType.empty()) {
    setPropertyType(PropertyType);
  }

  propertyTypeComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  layout()->addWidget(propertyTypeComboBox);
  layout()->setAlignment(propertyTypeComboBox, Qt::AlignHCenter);
}

void PropertyConfigurationWidget::fillPropertyTypeComboBox() {
  propertyTypeComboBox->clear();
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(BooleanProperty::propertyTypename),QVariant(QString::fromStdString(BooleanProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(DoubleProperty::propertyTypename),QVariant(QString::fromStdString(DoubleProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(ColorProperty::propertyTypename),QVariant(QString::fromStdString(ColorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(IntegerProperty::propertyTypename),QVariant(QString::fromStdString(IntegerProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(LayoutProperty::propertyTypename),QVariant(QString::fromStdString(LayoutProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(SizeProperty::propertyTypename),QVariant(QString::fromStdString(SizeProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(StringProperty::propertyTypename),QVariant(QString::fromStdString(StringProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(BooleanVectorProperty::propertyTypename),QVariant(QString::fromStdString(BooleanVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(ColorVectorProperty::propertyTypename),QVariant(QString::fromStdString(ColorVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(CoordVectorProperty::propertyTypename),QVariant(QString::fromStdString(CoordVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(DoubleVectorProperty::propertyTypename),QVariant(QString::fromStdString(DoubleVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(IntegerVectorProperty::propertyTypename),QVariant(QString::fromStdString(IntegerVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(SizeVectorProperty::propertyTypename),QVariant(QString::fromStdString(SizeVectorProperty::propertyTypename)));
  propertyTypeComboBox->addItem(propertyTypeToPropertyTypeLabel(StringVectorProperty::propertyTypename),QVariant(QString::fromStdString(StringVectorProperty::propertyTypename)));

}

string PropertyConfigurationWidget::getPropertyType() const {
  //Return the real type in the
  return QStringToTlpString(propertyTypeComboBox->itemData(propertyTypeComboBox->currentIndex()).toString());
}

void PropertyConfigurationWidget::setPropertyType(const string& propertyType) {
  int index = propertyTypeComboBox->findData(QVariant(QString::fromStdString(propertyType)));

  if(index != -1) {
    propertyTypeComboBox->setCurrentIndex(index);
  }
}
QString PropertyConfigurationWidget::getPropertyName() const {
  return propertyNameLineEdit->text();
}
bool PropertyConfigurationWidget::getPropertyUsed() const {
  return usedCheckBox->checkState() == Qt::Checked;
}
unsigned int PropertyConfigurationWidget::getPropertyNumber() const {
  return propertyNumber;
}
void PropertyConfigurationWidget::useStateChanged(int state) {
  if (state == Qt::Checked) {
    propertyNameLineEdit->setEnabled(nameEditable);
    propertyTypeComboBox->setEnabled(true);
    emit stateChange(true);
  }
  else {
    propertyNameLineEdit->setEnabled(false);
    propertyTypeComboBox->setEnabled(false);
    emit stateChange(false);
  }
}

void PropertyConfigurationWidget::setPropertyNameValidator(QValidator* validator) {
  propertyNameLineEdit->setValidator(validator);
}


CSVTableWidget::CSVTableWidget(QWidget* parent):QTableWidget(parent),maxLineNumber(UINT_MAX),firstLineIndex(0), checkCommentsLines(true), nbCommentsLines(0) {

}

bool CSVTableWidget::begin() {
  clear();
  setColumnCount(0);
  setRowCount(0);
  nbCommentsLines = 0;
  checkCommentsLines = true;
  //Force the table view to redraw
  QApplication::processEvents();
  return true;
}

bool CSVTableWidget::line(unsigned int row,const vector<string>& lineTokens) {

  //Wait for the first line index
  if(row < firstLineIndex) {
    return true;
  }

  //If the maximum line number is reach ignore the token.
  if(static_cast<unsigned>(rowCount()) >= maxLineNumber) {
    return true;
  }

  if (checkCommentsLines) {
    if (lineTokens[0][0] == '#')
      ++nbCommentsLines;
    else if (lineTokens[0].substr(0, 2) == "//")
      ++nbCommentsLines;
    else
      checkCommentsLines = false;
  }

  //Add a new row in the table
  int currentRow = rowCount();
  insertRow(currentRow);

  for(size_t column = 0 ; column < lineTokens.size() ; ++column) {
    //Add a new column if needed
    if(uint(columnCount()) <= column ) {
      insertColumn(column);
    }

    //Fill the table
    setItem(currentRow,column,new QTableWidgetItem(tlpStringToQString(lineTokens[column])));
  }

  return true;
}


bool CSVTableWidget::end(unsigned int, unsigned int) {
  return true;
}

CSVImportConfigurationWidget::CSVImportConfigurationWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CSVImportConfigurationWidget),validator(new PropertyNameValidator(propertyWidgets,this)),maxLineNumber(0),parser(NULL), firstLine(0), guessFirstLineIsHeader(true) {
  ui->setupUi(this);

  //Import line number change
  //connect(ui->toLineSpinBox,SIGNAL(valueChanged(int)),this,SLOT(toLineValueChanged(int)));
  connect(ui->useFirstLineAsPropertyNamecheckBox,SIGNAL(clicked(bool)),this,SLOT(useFirstLineAsHeaderUpdated()));
  connect(ui->limitPreviewLineNumberCheckBox,SIGNAL(clicked(bool)),this,SLOT(filterPreviewLineNumber(bool)));
  connect(ui->previewLineNumberSpinBox,SIGNAL(valueChanged(int)),this,SLOT(previewLineNumberChanged(int)));

  //Init the preview line number with the right value.
  if(ui->limitPreviewLineNumberCheckBox->isChecked()) {
    ui->previewTableWidget->setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
  }
  else {
    ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
  }

  // ensure the table columns are vertically aligned
  // with the PropertyConfigurationWidget(s)
#if QT_VERSION >= 0x050000
  ui->previewTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  ui->previewTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

CSVImportConfigurationWidget::~CSVImportConfigurationWidget() {
  delete ui;
  delete parser;
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
  //Reset import range
  updateLineNumbers(true);
}

void CSVImportConfigurationWidget::updateWidget(const std::string& title) {
  if(parser) {
    setEnabled(true);
    SimplePluginProgressDialog progress(this);
    progress.showPreview(false);
    progress.setWindowTitle(QString(title.c_str()));
    progress.show();

    // if neede try to guess if the first line is used as header.
    if (guessFirstLineIsHeader) {
      setUseFirstLineAsPropertyName(true);
      // parse the first line only
      parser->parse(this,&progress, true);

      for(unsigned int i = 0 ; i< columnHeaderType.size() ; ++i) {
        // If there is at least one column with a header type different
        // from StringProperty, then the first line is not the header
        if (columnHeaderType[i] != StringProperty::propertyTypename) {
          setUseFirstLineAsPropertyName(false);
          break;
        }
      }
    }

    parser->parse(this,&progress);
  }
  else {
    setEnabled(true);
  }
}

bool CSVImportConfigurationWidget::begin() {
  ui->previewTableWidget->begin();
  ui->previewTableWidget->setFirstLineIndex(getFirstLineIndex());
  clearPropertiesTypeList();
  //Clear initialized columns
  columnHeaderType.clear();
  columnType.clear();
  return true;
}

bool CSVImportConfigurationWidget::line(unsigned int row,const vector<string>& lineTokens) {

  ui->previewTableWidget->line(row,lineTokens);

  //Wait for the first row
  if(row >= getFirstLineIndex()) {
    if(useFirstLineAsPropertyName()) {
      if (row == getFirstLineIndex())
        headerColumnCount = columnCount();
      else if (lineTokens.size() > headerColumnCount) {
        if (QMessageBox::warning(this, "Invalid number of row fields",
                                 QString("row %1: the number of fields (%2) is greater than the number of columns (%3)").arg(row  + 1).arg(columnCount()).arg(headerColumnCount),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok) == QMessageBox::Cancel)
          return false;
      }
    }

    for(size_t column = 0 ; column < lineTokens.size() ; ++column) {
      //A new column was created set its label and it's configuration widget.
      if(propertyWidgets.size()<=column) {
        QString columnName = generateColumnName(column);
        ui->previewTableWidget->setHorizontalHeaderItem(column,new QTableWidgetItem(columnName));
        //Store the first token type
        columnHeaderType.push_back(guessDataType(lineTokens[column]));
        //Mark the colun type as uninitialised
        columnType.push_back("");
        //Create the new column widget. The default type is String property
        addPropertyToPropertyList(QStringToTlpString(columnName),true,StringProperty::propertyTypename);
      }
      else {
        //If the widget is not initialized do not use the default type
        string previousPropertyType = columnType[column];
        string propertyType = guessPropertyDataType(lineTokens[column],previousPropertyType);
        //Store the new type
        columnType[column]=propertyType;
      }
    }
  }

  return true;
}

bool CSVImportConfigurationWidget::end(unsigned int rowNumber, unsigned int) {
  maxLineNumber = rowNumber;

  //Force the table to correctly update.
  useFirstLineAsHeaderUpdated();
  //Avoid updating widget.
  ui->previewLineNumberSpinBox->blockSignals(true);
  ui->previewLineNumberSpinBox->setMaximum(rowNumber);
  ui->previewLineNumberSpinBox->blockSignals(false);
  return true;
}

void CSVImportConfigurationWidget::filterPreviewLineNumber(bool checked) {
  if(checked) {
    ui->previewTableWidget->setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
  }
  else {
    ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
  }

  updateWidget();
  //Reset import range
  updateLineNumbers(true);
}

void CSVImportConfigurationWidget::previewLineNumberChanged(int maxLineNumber) {
  ui->previewTableWidget->setMaxPreviewLineNumber(maxLineNumber);
  updateWidget();
  //Reset import range
  updateLineNumbers(true);
}

void CSVImportConfigurationWidget::toLineValueChanged(int) {
  updateWidget();
}

bool CSVImportConfigurationWidget::useFirstLineAsPropertyName()const {
  return ui->useFirstLineAsPropertyNamecheckBox->checkState()==Qt::Checked;
}

void CSVImportConfigurationWidget::setUseFirstLineAsPropertyName(bool useFirstLineAsHeader)const {
  ui->useFirstLineAsPropertyNamecheckBox->setChecked(useFirstLineAsHeader);
}

unsigned int CSVImportConfigurationWidget::rowCount()const {
  return ui->previewTableWidget->rowCount();
}
unsigned int CSVImportConfigurationWidget::columnCount()const {
  return ui->previewTableWidget->columnCount();
}

void CSVImportConfigurationWidget::updateTableHeaders() {
  QStringList itemsLabels;

  for(unsigned int i=0 ; i< columnCount(); ++i) {
    //Update the column name
    QString columnName = generateColumnName(i);
    itemsLabels<<columnName;
    propertyWidgets[i]->getNameLineEdit()->setText(columnName);
    //Update the column type.
    propertyWidgets[i]->setPropertyType(getColumnType(i));
  }

  ui->previewTableWidget->setHorizontalHeaderLabels (itemsLabels);
  itemsLabels.clear();
  //Ensure that the first visible row has the number 1.
  unsigned int currentRow = useFirstLineAsPropertyName()?0:1;

  for(unsigned int i = 0 ; i< rowCount(); ++i) {
    itemsLabels<<QString::number(currentRow);
    ++currentRow;
  }

  ui->previewTableWidget->setVerticalHeaderLabels(itemsLabels);
}

QString CSVImportConfigurationWidget::generateColumnName(unsigned int col)const {
  if(useFirstLineAsPropertyName()) {
    QTableWidgetItem *item = ui->previewTableWidget->item(0,col);

    if(item!=NULL) {
      return item->text();
    }
    else {
      return QString("Column_")+QString::number(col);
    }
  }
  else {
    return QString("Column_")+QString::number(col);
  }
}

string CSVImportConfigurationWidget::getColumnType(unsigned int col)const {
  if(useFirstLineAsPropertyName()) {
    return columnType[col];
  }
  else {
    return combinePropertyDataType(columnType[col] , columnHeaderType[col]);
  }
}

void CSVImportConfigurationWidget::useFirstLineAsHeaderUpdated() {
  if(useFirstLineAsPropertyName()) {
    ui->previewTableWidget->hideRow(0);
  }
  else {
    ui->previewTableWidget->showRow(0);
  }

  updateTableHeaders();
  updateLineNumbers(false);
}

void CSVImportConfigurationWidget::updateLineNumbers(bool resetValues) {
  blockSignals(true);
  ui->toLineSpinBox->blockSignals(true);
  bool wasAtMax = ui->toLineSpinBox->value()==ui->toLineSpinBox->maximum();
  //If the first line is not headers the maximum of line number to import increase.
  int rowNumber = useFirstLineAsPropertyName()?maxLineNumber-1:maxLineNumber;

  //If the old value was at the maximum keep it
  if(wasAtMax) {
    ui->toLineSpinBox->setValue(rowNumber);
  }

  //Reset all the values of the spinbox.
  if(resetValues)
    ui->toLineSpinBox->setValue(rowNumber);

  //Reset min/max values
  ui->toLineSpinBox->setMaximum(rowNumber);

  ui->toLineSpinBox->blockSignals(false);
  blockSignals(false);
}


void CSVImportConfigurationWidget::clearPropertiesTypeList() {
  for (vector<PropertyConfigurationWidget*>::iterator it = propertyWidgets.begin(); it != propertyWidgets.end(); ++it) {
    ui->gridLayout->removeWidget(*it);
    (*it)->deleteLater();
  }

  propertyWidgets.clear();
}

void CSVImportConfigurationWidget::addPropertyToPropertyList(const string& propertyName, bool isEditable,
    const std::string& propertyType) {

  PropertyConfigurationWidget *propertyConfigurationWidget = createPropertyConfigurationWidget(propertyWidgets.size(),
      QString::fromStdString(propertyName), isEditable, propertyType, ui->scrollAreaWidgetContents);
  ui->gridLayout->addWidget(propertyConfigurationWidget,0,propertyWidgets.size());
  propertyWidgets.push_back(propertyConfigurationWidget);
}

PropertyConfigurationWidget *CSVImportConfigurationWidget::createPropertyConfigurationWidget(
  unsigned int propertyNumber, const QString& propertyName, bool isEditable,
  const string& propertyType, QWidget* parent) {
  PropertyConfigurationWidget *propertyConfigurationWidget = new PropertyConfigurationWidget(propertyNumber,
      propertyName, isEditable, propertyType, parent);
  propertyConfigurationWidget->setPropertyNameValidator(validator);
  propertyConfigurationWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  connect(propertyConfigurationWidget, SIGNAL(stateChange(bool)), this, SLOT(propertyStateChanged(bool )));
  propertyConfigurationWidget->installEventFilter(this);
  return propertyConfigurationWidget;
}

void CSVImportConfigurationWidget::propertyStateChanged(bool state) {
  PropertyConfigurationWidget *widget = qobject_cast<PropertyConfigurationWidget*>(sender());
  assert(widget != NULL);

  for(int i = 0 ; i < ui->previewTableWidget->rowCount(); ++ i) {
    QTableWidgetItem* item = ui->previewTableWidget->item(i,widget->getPropertyNumber());

    if(item) {
      item->setFlags(state?Qt::ItemIsEnabled:Qt::NoItemFlags);
    }
  }
}

vector<CSVColumn> CSVImportConfigurationWidget::getPropertiesToImport()const {
  vector<CSVColumn> properties(propertyWidgets.size());

  for(size_t i = 0 ; i < propertyWidgets.size(); ++i) {
    properties[i] = CSVColumn(QStringToTlpString(propertyWidgets[i]->getNameLineEdit()->text()),propertyWidgets[i]->getPropertyUsed(),propertyWidgets[i]->getPropertyType());
  }

  return properties;
}

unsigned int CSVImportConfigurationWidget::getFirstImportedLineIndex()const {
  unsigned int firstLine =getFirstLineIndex();
  //Shift the line number if we use the first line as header.
  return useFirstLineAsPropertyName()?++firstLine:firstLine;

}

unsigned int CSVImportConfigurationWidget::getFirstLineIndex()const {
  return firstLine;
}

unsigned int CSVImportConfigurationWidget::getLastLineIndex()const {
  unsigned int lastLine = ui->toLineSpinBox->value();

  if(!useFirstLineAsPropertyName()) {
    --lastLine;
  }

  return lastLine;
}

CSVImportParameters CSVImportConfigurationWidget::getImportParameters()const {
  return CSVImportParameters(getFirstImportedLineIndex(),getLastLineIndex(),getPropertiesToImport());
}

QValidator::State PropertyNameValidator::validate(QString & input, int&) const {
  if(input.isEmpty()) {
    return QValidator::Invalid;
  }

  //Only one property can have the same name
  unsigned int count = 0;

  for(vector<PropertyConfigurationWidget*>::const_iterator it = widgets.begin() ; it != widgets.end(); ++it) {
    if((*it)->getPropertyName().compare(input)==0) {
      ++count;
    }
  }

  return count<=1?QValidator::Acceptable:QValidator::Invalid;
}

const string& CSVImportConfigurationWidget::guessPropertyDataType(const string& data,const string& previousType) const {
  //If there is no data skip the token
  if(data.empty()) {
    return previousType;
  }

  string dataType = guessDataType(data);
  return combinePropertyDataType(previousType,dataType);
}

const string& CSVImportConfigurationWidget::combinePropertyDataType(const string& previousType, const string& newType) const {
  if(previousType.empty()) {
    return newType;
  }
  else if(previousType == newType) {
    return newType;
  }
  else if (newType.empty())
    return previousType;
  //If both types are numeric return the more generic numeric type : double
  else if (previousType == BooleanProperty::propertyTypename &&
           (newType == DoubleProperty::propertyTypename ||
            newType == IntegerProperty::propertyTypename))
    return newType;
  else if (previousType == IntegerProperty::propertyTypename) {
    if (newType == DoubleProperty::propertyTypename)
      return DoubleProperty::propertyTypename;

    if (newType == BooleanProperty::propertyTypename)
      return IntegerProperty::propertyTypename;
  }
  else if (previousType == DoubleProperty::propertyTypename &&
           (newType == BooleanProperty::propertyTypename ||
            newType == IntegerProperty::propertyTypename))
    return DoubleProperty::propertyTypename;

  return StringProperty::propertyTypename;
}

// need a constant
static const std::string emptyString;

const string& CSVImportConfigurationWidget::guessDataType(const string& data) const {
  char *ptr = const_cast<char *>(data.c_str());

  while(isspace(*ptr))
    ++ptr;

  if (!*ptr)
    return emptyString;

  bool b;

  if(BooleanType::fromString(b, std::string(ptr),true)) {
    //The type is boolean
    return BooleanProperty::propertyTypename;
  }

  // Qt framework is the best way to detect numerals in string.
  QString str = tlpStringToQString(ptr);
  bool ok=false;
  str.toInt(&ok);

  //The type is int
  if(ok)
    return IntegerProperty::propertyTypename;

  if (parser->decimalMark() == ',') {
    QLocale c(QLocale::French);
    c.toDouble(str, &ok);
  }
  else
    str.toDouble(&ok);

  //The type is double
  if(ok) {
    return DoubleProperty::propertyTypename;
  }
  else {
    //All the other cases are treated as string.
    return StringProperty::propertyTypename;
  }
}

bool CSVImportConfigurationWidget::eventFilter(QObject *obj, QEvent *evt) {
  if(evt->type() == QEvent::Resize) {
    PropertyConfigurationWidget* columnWidget = qobject_cast<PropertyConfigurationWidget*>(obj);

    if(columnWidget!=NULL) {
      columnSizeChanged(columnWidget->getPropertyNumber());
    }
  }

  return QWidget::eventFilter(obj,evt);
}

void CSVImportConfigurationWidget::columnSizeChanged(unsigned int i) {
  //    assert(_columns.size()>i);
  PropertyConfigurationWidget* widget = propertyWidgets[i];

  if(widget!=NULL) {
    //QRect widgetRect = ui->horizontalLayout_4->;
    ui->previewTableWidget->setColumnWidth(i,widget->width());
  }
}
