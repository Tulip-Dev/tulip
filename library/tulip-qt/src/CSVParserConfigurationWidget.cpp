#include <tulip/TlpQtTools.h>

#include <QtCore/QTextCodec>
#include <QtGui/QFileDialog>

#include "tulip/CSVParserConfigurationWidget.h"
#include "tulip/CSVParserConfigurationWidgetData.h"
#include "tulip/CSVParser.h"

using namespace tlp;
using namespace std;
CSVParserConfigurationWidget::CSVParserConfigurationWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CSVParserConfigurationWidget)
{
    ui->setupUi(this);
    //Fill the encoding combo box
    fillEncodingComboBox();
    //Set the default encoding to UTF8
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(QString("UTF-8")));
    connect(ui->encodingComboBox,SIGNAL(currentIndexChanged ( int)),this,SIGNAL(parserChanged()));

    //File change
    connect(ui->fileChooserPushButton,SIGNAL(clicked(bool)),this,SLOT(changeFileNameButtonPressed()));

    //Invert rows and column
    connect(ui->switchRowColumnCheckBox,SIGNAL(stateChanged ( int )),this,SIGNAL(parserChanged()));

    //Separator and text delimiters.
    connect(ui->separatorComboBox,SIGNAL(currentIndexChanged ( int)),this,SIGNAL(parserChanged()));
    connect(ui->textDelimiterComboBox,SIGNAL(currentIndexChanged ( int)),this,SIGNAL(parserChanged()));
}

CSVParserConfigurationWidget::~CSVParserConfigurationWidget()
{
    delete ui;
}

void CSVParserConfigurationWidget::changeEvent(QEvent *e)
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

CSVParser * CSVParserConfigurationWidget::buildParser()const{
    CSVParser *parser = NULL;
    if(fileIsValid()){
        parser = new CSVSimpleParser(getFile(),getSeparator(),getTextSeparator(),getEncoding(),true);
        if(invertMatrix()){
            parser = new CSVInvertMatrixParser(parser);
        }
    }
    return parser;
}

void CSVParserConfigurationWidget::fillEncodingComboBox(){
    QList<QByteArray> codecs = QTextCodec::availableCodecs ();
    ui->encodingComboBox->clear();
    QStringList list;
    for(QList<QByteArray>::iterator it = codecs.begin() ; it != codecs.end();++it){
        list.push_back(QString(*it));
    }
    list.sort();
    ui->encodingComboBox->addItems(list);
}


string CSVParserConfigurationWidget::getSeparator()const{
    QString selectedText = ui->separatorComboBox->currentText();
    if(selectedText.compare("Tab")==0){
        return string("\t");
    }else if(selectedText.compare("Space")==0){
        return string(" ");
    }else{
        return QStringToTlpString(selectedText);
    }
}
void CSVParserConfigurationWidget::changeFileNameButtonPressed(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CSV file"), QString(), tr(
            "CSV files (*.txt *.csv)"));
    setFileToOpen(fileName);
}

void CSVParserConfigurationWidget::setFileToOpen(const QString& fileToOpen){
    if(QFile::exists(fileToOpen)){
        ui->fileLineEdit->setText(fileToOpen);
        emit parserChanged();
    }
}


void CSVParserConfigurationWidget::encodingChanged(){
    emit parserChanged();
}

string CSVParserConfigurationWidget::getFile()const{
    return QStringToTlpString(ui->fileLineEdit->text());
}
bool CSVParserConfigurationWidget::fileIsValid()const{
    return QFile::exists(ui->fileLineEdit->text());
}
string CSVParserConfigurationWidget::getEncoding()const{
    return QStringToTlpString(ui->encodingComboBox->currentText());
}
char CSVParserConfigurationWidget::getTextSeparator()const{   
    return QStringToTlpString(ui->textDelimiterComboBox->currentText()).at(0);
}
bool CSVParserConfigurationWidget::invertMatrix()const{
    return ui->switchRowColumnCheckBox->checkState() == Qt::Checked;
}
