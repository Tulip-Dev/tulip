#include "tulip/CSVImportConfigurationWidget.h"
#include "tulip/CSVImportConfigurationWidgetData.h"
#include <tulip/TlpQtTools.h>
#include <tulip/QtProgress.h>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QScrollBar>
#include <limits.h>

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
    connect(propertyNameLineEdit, SIGNAL(editingFinished()), this, SLOT(nameEditFinished()));
    fillPropertyTypeComboBox();
    propertyTypeComboBox->setCurrentIndex(0);
    propertyTypeComboBox->setCurrentIndex(propertyTypeComboBox->findText(tlpStringToQString(PropertyType)));
    propertyTypeComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout()->addWidget(propertyTypeComboBox);
    layout()->setAlignment(propertyTypeComboBox, Qt::AlignHCenter);
}

void PropertyConfigurationWidget::fillPropertyTypeComboBox() {
    QStringList lst = PropertyTools::getPropertyTypeLabelsList();
    propertyTypeComboBox->addItem("Auto detect");
    propertyTypeComboBox->addItems(lst);
}

string PropertyConfigurationWidget::getPropertyType() const {
    QString propertyType = propertyTypeComboBox->currentText();
    if(propertyType.compare("Auto detect")==0){
        return string("");
    }else{
        return PropertyTools::getPropertyTypeFromPropertyTypeLabel(QStringToTlpString(propertyType));
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
void PropertyConfigurationWidget::nameEditFinished() {
    emit propertyNameChange(propertyNameLineEdit->text());
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


CSVTableWidget::CSVTableWidget(QWidget* parent):QTableWidget(parent),maxLineNumber(UINT_MAX){

}

void CSVTableWidget::begin(){
    clear();
    setColumnCount(0);
    setRowCount(0);
}

void CSVTableWidget::token(unsigned int row, unsigned int column, const string& token){
    if(static_cast<unsigned int>(rowCount()) <= row){
        //If the maximum line number is reach ignore the token.
        if(static_cast<unsigned>(rowCount()) >= maxLineNumber)
            return;
        insertRow(row);
    }
    if(static_cast<unsigned int>(columnCount()) <= column){
        insertColumn(column);
        //Set the column name

    }
    setItem(row,column,new QTableWidgetItem(tlpStringToQString(token)));
}

void CSVTableWidget::end(unsigned int, unsigned int){

}

CSVImportConfigurationWidget::CSVImportConfigurationWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CSVImportConifgurationWidget),validator(new PropertyNameValidator(propertyWidgets,this)),maxLineNumber(0),parser(NULL)
{
    ui->setupUi(this);

    //Import line number change
    connect(ui->fromLineSpinBox,SIGNAL(valueChanged(int)),this,SLOT(fromLineValueChanged(int)));
    connect(ui->toLineSpinBox,SIGNAL(valueChanged(int)),this,SLOT(toLineValueChanged(int)));
    connect(ui->useFirstLineAsPropertyNamecheckBox,SIGNAL(clicked(bool)),this,SLOT(useFirstLineAsHeaderUpdated()));
    connect(ui->limitPreviewLineNumberCheckBox,SIGNAL(clicked(bool)),this,SLOT(filterPreviewLineNumber(bool)));
    connect(ui->previewLineNumberSpinBox,SIGNAL(valueChanged(int)),this,SLOT(previewLineNumberChanged(int)));
    //Init the preview line number with the right value.
    if(ui->limitPreviewLineNumberCheckBox->isChecked()){
        ui->previewTableWidget->setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
    }else{
    ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
    }
}

CSVImportConfigurationWidget::~CSVImportConfigurationWidget()
{
    delete ui;
    delete parser;
}

void CSVImportConfigurationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CSVImportConfigurationWidget::setNewParser(CSVParser *newParser){

    delete parser;
    parser = newParser;
    updateWidget();


}

void CSVImportConfigurationWidget::updateWidget(){
    if(parser){
        setEnabled(true);
        QtProgress progress(this,"Generating previews",NULL);
        progress.setComment("Generating preview");
        parser->parse(this,&progress);
    }else{
        setEnabled(true);
    }
}

void CSVImportConfigurationWidget::begin(){
    ui->previewTableWidget->begin();
    clearPropertiesTypeList();
}

void CSVImportConfigurationWidget::token(unsigned int row, unsigned int column, const string& token){

    ui->previewTableWidget->token(row,column,token);
    //A new column was created set its label and it's configuration widget.
    if(row == 0){
        QString columnName = genrateColumnName(column);
        ui->previewTableWidget->setHorizontalHeaderItem(column,new QTableWidgetItem(columnName));
        addPropertyToPropertyList(QStringToTlpString(columnName),true);
    }
}

void CSVImportConfigurationWidget::end(unsigned int rowNumber, unsigned int){
    maxLineNumber = rowNumber;
    updateLineNumbers(true);
    //Force the table to correctly update.
    useFirstLineAsHeaderUpdated();
    //Avoid updating widget.
    ui->previewLineNumberSpinBox->blockSignals(true);
    ui->previewLineNumberSpinBox->setMaximum(rowNumber);
    ui->previewLineNumberSpinBox->blockSignals(false);
}

void CSVImportConfigurationWidget::filterPreviewLineNumber(bool checked){
    if(checked){
        ui->previewTableWidget->setMaxPreviewLineNumber(ui->previewLineNumberSpinBox->value());
    }else{
        ui->previewTableWidget->setMaxPreviewLineNumber(UINT_MAX);
    }
    updateWidget();
}

void CSVImportConfigurationWidget::previewLineNumberChanged(int maxLineNumber){
    ui->previewTableWidget->setMaxPreviewLineNumber(maxLineNumber);
    updateWidget();
}

void CSVImportConfigurationWidget::fromLineValueChanged(int value){
    ui->toLineSpinBox->setMinimum(value);
    emit fileInfoChanged();
}

void CSVImportConfigurationWidget::toLineValueChanged(int value){
    ui->fromLineSpinBox->setMaximum(value);
    emit fileInfoChanged();
}

bool CSVImportConfigurationWidget::useFirstLineAsPropertyName()const{
    return ui->useFirstLineAsPropertyNamecheckBox->checkState()==Qt::Checked;
}

unsigned int CSVImportConfigurationWidget::rowCount()const{
    return ui->previewTableWidget->rowCount();
}
unsigned int CSVImportConfigurationWidget::columnCount()const{
    return ui->previewTableWidget->columnCount();
}
void CSVImportConfigurationWidget::updateTableHeaders(){
    QStringList itemsLabels;
    for(unsigned int i=0 ; i< columnCount();++i){
        QString columnName = genrateColumnName(i);
        itemsLabels<<columnName;
        propertyWidgets[i]->getNameLineEdit()->setText(columnName);
    }
    ui->previewTableWidget->setHorizontalHeaderLabels (itemsLabels);
    itemsLabels.clear();
    //Ensure that the first visible row has the number 1.
    unsigned int currentRow = useFirstLineAsPropertyName()?0:1;
    for(unsigned int i = 0 ; i< rowCount(); ++i){
        itemsLabels<<QString::number(currentRow);
        ++currentRow;
    }
    ui->previewTableWidget->setVerticalHeaderLabels(itemsLabels);
}
QString CSVImportConfigurationWidget::genrateColumnName(unsigned int col)const{
    if(useFirstLineAsPropertyName()){
        return ui->previewTableWidget->item(0,col)->text();
    }else{
        return QString("Property_")+QString::number(col);
    }
}

void CSVImportConfigurationWidget::useFirstLineAsHeaderUpdated(){
    if(useFirstLineAsPropertyName()){
        ui->previewTableWidget->hideRow(0);
    }else{
        ui->previewTableWidget->showRow(0);
    }
    updateTableHeaders();
    updateLineNumbers(false);    
    emit fileInfoChanged();
}

void CSVImportConfigurationWidget::updateLineNumbers(bool resetValues){
    blockSignals(true);
    bool wasAtMax = ui->toLineSpinBox->value()==ui->toLineSpinBox->maximum();
    //If the first line is not headers the maximum of line number to import increase.
    int rowNumber = useFirstLineAsPropertyName()?maxLineNumber-1:maxLineNumber;
    //No need to set the from line number maximum as it will be automatically set when updating the to
    ui->toLineSpinBox->setMaximum(rowNumber);
    //If the old value was at the maximum keep it
    if(wasAtMax){
        ui->toLineSpinBox->setValue(rowNumber);
    }
    //Reset all the values of the spinbox.
    if(resetValues){
        ui->fromLineSpinBox->setMinimum(1);
        ui->fromLineSpinBox->setValue(1);
        ui->toLineSpinBox->setValue(rowNumber);
    }
    blockSignals(false);    
}


void CSVImportConfigurationWidget::clearPropertiesTypeList() {
    for (vector<PropertyConfigurationWidget*>::iterator it = propertyWidgets.begin(); it != propertyWidgets.end(); ++it) {
        ui->propertiesListScrollAreaWidgetContents->layout()->removeWidget(*it);
        (*it)->deleteLater();
    }
    propertyWidgets.clear();
    ui->propertiesListScrollArea->setVisible(false);
}

void CSVImportConfigurationWidget::addPropertyToPropertyList(const string& propertyName, bool isEditable,
                                                             const std::string& propertyType) {
    if (!ui->propertiesListScrollArea->isVisible())
        ui->propertiesListScrollArea->setVisible(true);
    PropertyConfigurationWidget *propertyConfigurationWidget = createPropertyConfigurationWidget(propertyWidgets.size(),
                                                                                                 QString::fromStdString(propertyName), isEditable, propertyType, ui->propertiesListScrollAreaWidgetContents);
    ui->propertiesListScrollAreaWidgetContents->layout()->addWidget(propertyConfigurationWidget);
    //Update propertiesListScrollArea height to ensure that properties configuration widgets are fully visible.
    //The height is computed from the minimum property widget configuration height , the scroll bar height and a small spacing (10).
    ui->propertiesListScrollArea->setMinimumHeight(propertyConfigurationWidget->sizeHint().height()
                                                   + ui->propertiesListScrollArea->horizontalScrollBar()->sizeHint().height() + 10);
    propertyWidgets.push_back(propertyConfigurationWidget);
}
PropertyConfigurationWidget *CSVImportConfigurationWidget::createPropertyConfigurationWidget(
        unsigned int propertyNumber, const QString& propertyName, bool isEditable,
        const string& propertyType, QWidget* parent) {
    PropertyConfigurationWidget *propertyConfigurationWidget = new PropertyConfigurationWidget(propertyNumber,
                                                                                               propertyName, isEditable, propertyType, parent);
    propertyConfigurationWidget->setPropertyNameValidator(validator);
    propertyConfigurationWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect(propertyConfigurationWidget, SIGNAL(propertyNameChange(QString)), this, SLOT(propertyNameChanged(QString)));
    connect(propertyConfigurationWidget, SIGNAL(stateChange(bool)), this, SLOT(propertyStateChanged(bool )));
    return propertyConfigurationWidget;
}
void CSVImportConfigurationWidget::propertyNameChanged(QString newName){
    //Update headers
    PropertyConfigurationWidget *widget = qobject_cast<PropertyConfigurationWidget*>(sender());
    assert(widget != NULL);

    QTableWidgetItem * item = ui->previewTableWidget->horizontalHeaderItem( widget->getPropertyNumber());
    if(item == NULL){
        item = new QTableWidgetItem(newName);
        ui->previewTableWidget->setHorizontalHeaderItem(widget->getPropertyNumber(),item);
    }else{
        item->setText(newName);
    }

    //Tell other widget property name changed.
    emit fileInfoChanged();
}

void CSVImportConfigurationWidget::propertyStateChanged(bool state){
    PropertyConfigurationWidget *widget = qobject_cast<PropertyConfigurationWidget*>(sender());
    assert(widget != NULL);    
    ui->previewTableWidget->setColumnHidden ( widget->getPropertyNumber(), !state );
}

vector<CSVColumn> CSVImportConfigurationWidget::getPropertiesToImport()const{
    vector<CSVColumn> properties(propertyWidgets.size());
    for(size_t i = 0 ; i < propertyWidgets.size(); ++i){        
        properties[i] = CSVColumn(QStringToTlpString(propertyWidgets[i]->getNameLineEdit()->text()),propertyWidgets[i]->getPropertyUsed(),propertyWidgets[i]->getPropertyType());
    }
    return properties;
}

unsigned int CSVImportConfigurationWidget::getFirstLineIndex()const{
    unsigned int firstLine =ui->fromLineSpinBox->value();
    //If we don't use first line as header add it in the lines to import.
    if(!useFirstLineAsPropertyName()){
        --firstLine;
    }
    return firstLine;
}
unsigned int CSVImportConfigurationWidget::getLastLineIndex()const{
    unsigned int lastLine = ui->toLineSpinBox->value();
    if(!useFirstLineAsPropertyName()){
        --lastLine;
    }
    return lastLine;
}

CSVImportParameters CSVImportConfigurationWidget::getImportParameters()const{
    return CSVImportParameters(getFirstLineIndex(),getLastLineIndex(),getPropertiesToImport());
}

QValidator::State PropertyNameValidator::validate(QString & input, int&) const {
    if(input.isEmpty()){
        return QValidator::Invalid;
    }
    //Only one property can have the same name
    unsigned int count = 0;
    for(vector<PropertyConfigurationWidget*>::const_iterator it = widgets.begin() ; it != widgets.end();++it){
        if((*it)->getPropertyName().compare(input)==0){
            ++count;
        }
    }
    return count<=1?QValidator::Acceptable:QValidator::Invalid;
}
