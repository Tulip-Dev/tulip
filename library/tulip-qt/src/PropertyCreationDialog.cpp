#include "tulip/PropertyCreationDialog.h"
#include "PropertyCreationDialogData.h"

#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

using namespace tlp;

PropertyCreationDialog::PropertyCreationDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PropertyCreationDialog),_createPropertyButton(NULL),_graph(NULL),_createdProperty(NULL)
{
    initGui();
}

PropertyCreationDialog::PropertyCreationDialog(Graph* graph,QWidget *parent ):
        QDialog(parent),
        ui(new Ui::PropertyCreationDialog),_graph(graph),_createdProperty(NULL)
{
    initGui();
}

void PropertyCreationDialog::initGui(){
    ui->setupUi(this);
    QStringList labels;
    //Init properties list
    labels<<propertyTypeToPropertyTypeLabel("color");
    labels<<propertyTypeToPropertyTypeLabel("int");
    labels<<propertyTypeToPropertyTypeLabel("layout");
    labels<<propertyTypeToPropertyTypeLabel("double");
    labels<<propertyTypeToPropertyTypeLabel("bool");
    labels<<propertyTypeToPropertyTypeLabel("size");
    labels<<propertyTypeToPropertyTypeLabel("string");
    labels<<propertyTypeToPropertyTypeLabel("vector<bool>");
    labels<<propertyTypeToPropertyTypeLabel("vector<color>");
    labels<<propertyTypeToPropertyTypeLabel("vector<coord>");
    labels<<propertyTypeToPropertyTypeLabel("vector<double>");
    labels<<propertyTypeToPropertyTypeLabel("vector<int>");
    labels<<propertyTypeToPropertyTypeLabel("vector<size>");
    labels<<propertyTypeToPropertyTypeLabel("vector<string>");
    ui->propertyTypeComboBox->addItems(labels);
    _createPropertyButton = ui->buttonBox->addButton(tr("Create"),QDialogButtonBox::AcceptRole);
    ui->errorIconLabel->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16,16));
    connect(ui->propertyNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(checkValidity()));
    checkValidity();
}

PropertyCreationDialog::~PropertyCreationDialog()
{
    delete ui;
}

void PropertyCreationDialog::setGraph(Graph* graph){
    _graph = graph;
    checkValidity();
}

void PropertyCreationDialog::accept(){
    bool error = false;
    if(_graph == NULL){
        QMessageBox::warning(this, "Fail to create property", "The parent graph is invalid",
                             QMessageBox::Ok, QMessageBox::Ok);
        error = true;
    }
    QString propertyName = ui->propertyNameLineEdit->text();
    if(propertyName.isEmpty()){
        QMessageBox::warning(this, "Fail to create property", "You can't create a property with an empty name",
                             QMessageBox::Ok, QMessageBox::Ok);
        error = true;
    }
    if (_graph->existLocalProperty(QStringToTlpString(propertyName))) {
        QMessageBox::warning(this, "Fail to create property", "A property with the same name already exist", QMessageBox::Ok,
                             QMessageBox::Ok);
        error = true;
    }

    if(!error){
        _graph->push();
        _createdProperty = _graph->getProperty(QStringToTlpString(propertyName),propertyTypeLabelToPropertyType(ui->propertyTypeComboBox->currentText()));
    }
    QDialog::accept();
}

PropertyInterface* PropertyCreationDialog::createNewProperty(tlp::Graph* graph,QWidget* parent){
    PropertyCreationDialog *dialog = new PropertyCreationDialog(graph,parent);
    PropertyInterface* result = NULL;
    if(dialog->exec() == QDialog::Accepted){
        result = dialog->createdProperty();
    }
    dialog->deleteLater();
    return result;
}

void PropertyCreationDialog::checkValidity(){
    QString propertyName = ui->propertyNameLineEdit->text();
    if(_graph == NULL){
        ui->errorLabel->setText(tr("You need to specify a parent graph"));
        ui->errorNotificationWidget->setVisible(true);
        _createPropertyButton->setEnabled(false);
        return;
    }else if(propertyName.isEmpty()){
        ui->errorLabel->setText(tr("You can't create a property with an empty name"));
        _createPropertyButton->setEnabled(false);
        ui->errorNotificationWidget->setVisible(true);
        return;
    } else if (_graph->existLocalProperty(QStringToTlpString(propertyName))) {
        ui->errorLabel->setText(tr("A property with the same name already exist"));
        _createPropertyButton->setEnabled(false);
        ui->errorNotificationWidget->setVisible(true);
        return;
    }
    ui->errorNotificationWidget->setVisible(false);
    _createPropertyButton->setEnabled(true);
}
