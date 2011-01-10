#ifndef CSVIMPORTCONFIGURATIONWIDGET_H
#define CSVIMPORTCONFIGURATIONWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QValidator>
#include <QtGui/QTableWidget>
#include <tulip/CSVGraphImport.h>
#include <tulip/CSVContentHandler.h>
#include <vector>


class QComboBox;
class QCheckBox;
class QValidator;
class QLineEdit;
namespace Ui {
    class CSVImportConifgurationWidget;
}

namespace tlp{

/**
 * @brief Configuration widget for a property.
 */
class TLP_QT_SCOPE PropertyConfigurationWidget: public QWidget {
    Q_OBJECT
public:
    PropertyConfigurationWidget(unsigned int propertyNumber, const QString& propertyName, bool propertyNameIsEditable,
                                const std::string& PropertyType, QWidget* parent = NULL);
    /**
   * Return the selected property type.
   */
    std::string getPropertyType() const;

    QString getPropertyName() const;
    bool getPropertyUsed() const;
    /**
   *  @brief Set the property name validator. Use to chek if entered graph name is valid.
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

/**
 * @brief Check if the property name already exist in the property list.
 **/
class TLP_QT_SCOPE PropertyNameValidator: public QValidator {
public:
  PropertyNameValidator(const std::vector<PropertyConfigurationWidget*>& widgets,QObject*parent=NULL) :
  QValidator(parent), widgets(widgets) {
  }
  virtual ~PropertyNameValidator() {

  }

  /**
   * Validate the new property name. Check if any property does not have the same name
   */
  QValidator::State validate(QString & input, int & pos) const;

private:
  const std::vector<PropertyConfigurationWidget*>& widgets;
};

/**
* @brief Simple table preview of CSV file. Load in a QTableWidget the data send by a CSVContentHandler.
**/
  class TLP_QT_SCOPE CSVTableWidget : public QTableWidget, public CSVContentHandler{
  public:
      CSVTableWidget(QWidget* parent=NULL);
      void begin();
      void token(unsigned int row, unsigned int column, const std::string& token);
      void end(unsigned int rowNumber, unsigned int columnNumber);
      /**
        * @brief Limit the line number of the preview. Need to parse the file again to take this limit in account.
        **/
      void setMaxPreviewLineNumber(unsigned int lineNumber){
          maxLineNumber = lineNumber;
      }

      /**
        * @brief Get the preview line number.
        **/
      unsigned int getMaxPreviewLineNumber()const{
          return maxLineNumber;
      }

  private:
      unsigned int maxLineNumber;
  };


/**
  * @brief Widget generating a CSVImportParameters object configuring the CSV import process.
  *
  * Use a CSV parser to fill this widget with previews and CSV file statistics like number of rows and columns.
  **/
class TLP_QT_SCOPE CSVImportConfigurationWidget : public QWidget, public CSVContentHandler {
    Q_OBJECT        
public:
    CSVImportConfigurationWidget(QWidget *parent = 0);
    ~CSVImportConfigurationWidget();
    void begin();
    void token(unsigned int row, unsigned int column, const std::string& token);
    void end(unsigned int rowNumber, unsigned int columnNumber);

    /**
      * @brief Update the widget contents with the new file parser.
      **/
    void setNewParser(tlp::CSVParser *parser);

  /**
  * @brief Get the import parameters.
  *
  * Use this object to configure import process of the CSVImportGraph object.
  **/
    CSVImportParameters getImportParameters()const;


protected:

    void updateWidget();

    std::vector<CSVColumn> getPropertiesToImport()const;
    void changeEvent(QEvent *e);

    void updateLineNumbers(bool resetValues);    

    bool useFirstLineAsPropertyName()const;
    unsigned int rowCount()const;
    unsigned int columnCount()const;

    unsigned int getFirstLineIndex()const;
    unsigned int getLastLineIndex()const;

    /**
     * Empty the properties list.
     */
    void clearPropertiesTypeList();
    /**
     * Add a property to the current property list.
     */
    void addPropertyToPropertyList(const std::string& propertyName, bool isEditable, const std::string& propertyType =std::string("Auto detect"));

    /**
     * @brief Creates a property configuration widget.
     *
     * @param propertyNumber The property number.
     * @param propertyName The name of the property.
     * @param propertyNameIsEditable Whether the property's name is editable.
     * @param propertyType The type of the property.
     * @param parent This widget's parent.
     * @return :PropertyConfigurationWidget*
     **/
    virtual PropertyConfigurationWidget *createPropertyConfigurationWidget(unsigned int propertyNumber,
                                                                           const QString& propertyName, bool propertyNameIsEditable, const std::string& propertyType, QWidget* parent);

    QString genrateColumnName(unsigned int col)const;

    std::vector<PropertyConfigurationWidget*> propertyWidgets;

protected slots:

    void filterPreviewLineNumber(bool filter);
    void previewLineNumberChanged(int value);

    void fromLineValueChanged(int value);
    void toLineValueChanged(int value);

    void updateTableHeaders();

    void useFirstLineAsHeaderUpdated();
    void propertyNameChanged(QString propertyName);
    void propertyStateChanged(bool activated);



signals:
    void fileInfoChanged();

private:    
    Ui::CSVImportConifgurationWidget *ui;
    PropertyNameValidator* validator;
    unsigned int maxLineNumber;
    tlp::CSVParser* parser;
};

}
#endif // CSVIMPORTCONFIGURATIONWIDGET_H
