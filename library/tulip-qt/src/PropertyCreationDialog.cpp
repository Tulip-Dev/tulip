#include "tulip/PropertyCreationDialog.h"
#include "ui_PropertyCreationDialog.h"

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
        QMessageBox::warning(this, "Fail to create property", "A property with same name already exist", QMessageBox::Ok,
                             QMessageBox::Ok);
        error = true;
    }

    if(!error){
        _graph->push();

        QString propertyType = ui->propertyTypeComboBox->currentText();
        if (propertyType == trUtf8("Selection"))
            _createdProperty = _graph->getLocalProperty<BooleanProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("Metric"))
            _createdProperty =_graph->getLocalProperty<DoubleProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("Layout"))
            _createdProperty =_graph->getLocalProperty<LayoutProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("String"))
            _createdProperty =_graph->getLocalProperty<StringProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("Integer"))
            _createdProperty =_graph->getLocalProperty<IntegerProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("Size"))
            _createdProperty =_graph->getLocalProperty<SizeProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("Color"))
            _createdProperty =_graph->getLocalProperty<ColorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("BooleanVector"))
            _createdProperty =_graph->getLocalProperty<BooleanVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("DoubleVector"))
            _createdProperty =_graph->getLocalProperty<DoubleVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("CoordVector"))
            _createdProperty =_graph->getLocalProperty<CoordVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("StringVector"))
            _createdProperty =_graph->getLocalProperty<StringVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("IntegerVector"))
            _createdProperty =_graph->getLocalProperty<IntegerVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("SizeVector"))
            _createdProperty =_graph->getLocalProperty<SizeVectorProperty> (QStringToTlpString(propertyName));
        if (propertyType == trUtf8("ColorVector"))
            _createdProperty =_graph->getLocalProperty<ColorVectorProperty> (QStringToTlpString(propertyName));
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
        ui->errorLabel->setText(tr("A property with same name already exist"));
        _createPropertyButton->setEnabled(false);
        ui->errorNotificationWidget->setVisible(true);
        return;
    }
    ui->errorNotificationWidget->setVisible(false);
    _createPropertyButton->setEnabled(true);
}
