/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef IMPORTCSVDATACONFIGURATIONWIDGET_H_
#define IMPORTCSVDATACONFIGURATIONWIDGET_H_
#include <tulip/tulipconf.h>
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QValidator>
#include <tulip/ImportCSVDataConfigurationWidgetData.h>
#include "tulip/CSVContentHandler.h"

class QComboBox;
class QLineEdit;
class QCheckBox;
class QTableWidget;
namespace tlp {
/**
 * Configuration widget for a property.
 */
class TLP_QT_SCOPE PropertyConfigurationWidget: public QWidget {
  Q_OBJECT
public:
  PropertyConfigurationWidget(unsigned int propertyNumber, const QString& propertyName, bool propertyNameIsEditable,
      int PropertyType, QWidget* parent = NULL);
  /**
   * Return the selected property type.
   */
  int getPropertyType() const;

  QString getPropertyName() const;
  bool getPropertyUsed() const;
  /**
   * Set the property name validator. Use to chek if entered graph name is valid.
   */
  void setPropertyNameValidator(QValidator* validator);
  unsigned int getPropertyNumber() const;

  QLineEdit *getNameLineEdit() {
    return propertyNameLineEdit;
  }
  QComboBox *getTypeComboBox() {
    return propertyTypeComboBox;
  }

  QCheckBox *getCheckBox() {
    return usedCheckBox;
  }

private:
  void fillPropertyTypeComboBox();
  QLineEdit *propertyNameLineEdit;
  QComboBox *propertyTypeComboBox;
  QCheckBox *usedCheckBox;
  bool nameEditable;
  unsigned int propertyNumber;

private slots:
  void nameEditFinished();
  void useStateChanged(int state);

  signals:
  void propertyNameChange(QString newName);
  void stateChange(bool state);
};

class PropertyNameValidator;

/*
 * Widget that encapsulate import and properties settings for ImportCSVDataWidget class.
 */
class TLP_QT_SCOPE ImportCSVDataConfigurationWidget: public QWidget, public Ui::ImportCSVDataConfigurationWidget, public CSVContentHandler {
  Q_OBJECT
public:
  enum PropertyType {
    AutoDetect,
    Double,
    Layout,
    String,
    Int,
    Color,
    Size,
    Bool,
    DoubleVector,
    StringVector,
    IntVector,
    CoordVector,
    ColorVector,
    SizeVector,
    BoolVector
  };

  enum PropertyOrientation {
    Column, Row
  };

  ImportCSVDataConfigurationWidget(QWidget *parent = NULL);
  virtual ~ImportCSVDataConfigurationWidget();

  QString getSeparator() const {
    return separatorLineEdit->text();
  }
  QString getCsvFile() const {
    return csvFileLineEdit->text();
  }

  bool useFirstRowAsPropertyName() const {
    return useFirstRowAsPropertyNameCheckBox->checkState() == Qt::Checked;
  }

  unsigned int numberOfPreviews() const {
    return 5;
  }

  PropertyOrientation getPropertyOrientation() const {
    if (colAsPropertiesRadioButton->isChecked()) {
      return Column;
    }
    else {
      return Row;
    }
  }

  /**
   * Empty the properties list.
   */
  void clearPropertiesTypeList();
  /**
   * Add a property to the current property list.
   */
  void addPropertyToPropertyList(const std::string& propertyName, bool isEditable, PropertyType propertyType =
      AutoDetect);

  QString getPropertyName(unsigned int propertyNumber) const;

  ImportCSVDataConfigurationWidget::PropertyType getPropertyType(unsigned int propertyNumber) const;

  /**
   * Return true if the property with the given number must be loaded in the graph.
   */
  bool useProperty(unsigned int propertyNumber);

  bool checkIfMoreThanOnePropertyHaveTheName(const QString& name);

  void begin();
  void token(unsigned int row, unsigned int column, const std::string& token);
  void end(unsigned int rowNumber,unsigned int columnNumber);

protected:

  /**
   * Build the property configuration widget from given parameters.
   * @param propertyNumber the property number.
   * @param propertyName
   * @param isEditable Is the property name editable?
   * @param defaultPropertyType the default property type.
   */
  virtual PropertyConfigurationWidget *createPropertyConfigurationWidget(unsigned int propertyNumber,
      const QString& propertyName, bool propertyNameIsEditable, ImportCSVDataConfigurationWidget::PropertyType defaultPropertyType, QWidget* parent);

  /**
   * Create and return the table widget preview widget.
   */
  virtual QTableWidget* buildPreviewWidget(QWidget* parent);

  std::vector<PropertyConfigurationWidget*> propertyWidgets;
  PropertyNameValidator *propertyNameValidator;
  QTableWidget* previewWidget;

private:
  void propertyNameChanged(unsigned int propertyNumber, QString newPropertyName);
  void propertyStateChanged(unsigned int propertyNumber, bool state);

private slots:
  void browseNewCsvFile();
  void lineEditEditionFinished();
  void useFirstRowAsPropertyNameStateChanged(int state);
  void propertyNameChanged(QString newName);
  void propertyOrientationButtonPressed(QAbstractButton * button);
  void propertyStateChanged(bool state);

  signals:
  void csvFileChange(QString newFile);
  void separatorChange(QString newSeparator);
  void useFirstRowAsPropertyNameChange(bool useFirstRowAsPropName);
  void propertiesOrientationChange(ImportCSVDataConfigurationWidget::PropertyOrientation orientation);

};
/*
 * Check if the property name already exist in the property list.
 */
class TLP_QT_SCOPE PropertyNameValidator: public QValidator {
public:
  PropertyNameValidator(ImportCSVDataConfigurationWidget *parent) :
  QValidator(parent), parent(parent) {
  }
  virtual ~PropertyNameValidator() {

  }

  /**
   * Validate the new property name. Check if any property does not have the same name
   */
  QValidator::State validate(QString & input, int & pos) const;

private:
  ImportCSVDataConfigurationWidget *parent;
};
}
#endif /* IMPORTCSVDATACONFIGURATIONWIDGET_H_ */
