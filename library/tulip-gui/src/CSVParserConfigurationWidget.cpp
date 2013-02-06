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
#include <tulip/TlpQtTools.h>

#include <QtCore/QTextCodec>
#include <QtGui/QFileDialog>
#include <QtGui/QKeyEvent>

#include "tulip/CSVParserConfigurationWidget.h"
#include "ui_CSVParserConfigurationWidget.h"
#include "tulip/CSVParser.h"

using namespace tlp;
using namespace std;

CSVParserConfigurationWidget::CSVParserConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSVParserConfigurationWidget) {
    ui->setupUi(this);
    //Fill the encoding combo box
    fillEncodingComboBox();
    //Set the default encoding to UTF8
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(QString("UTF-8")));

    connect(ui->encodingComboBox,SIGNAL(currentIndexChanged ( int)),this,SIGNAL(parserChanged()));

    //Invert rows and column
    connect(ui->switchRowColumnCheckBox,SIGNAL(stateChanged ( int )),this,SIGNAL(parserChanged()));

    //Separator and text delimiters.
    connect(ui->separatorComboBox,SIGNAL(currentIndexChanged ( int)),this,SLOT(changeSeparator(int)));
    connect(ui->textDelimiterComboBox,SIGNAL(currentIndexChanged ( int)),this,SIGNAL(parserChanged()));
    connect(ui->mergesep,SIGNAL(stateChanged(int)),this,SIGNAL(parserChanged()));
    connect(ui->othersep,SIGNAL(textEdited(const QString &)), this, SIGNAL(parserChanged()));

    //File change
    connect(ui->fileChooserPushButton,SIGNAL(clicked(bool)),this,SLOT(changeFileNameButtonPressed()));

}

CSVParserConfigurationWidget::~CSVParserConfigurationWidget() {
    delete ui;
}

CSVParser * CSVParserConfigurationWidget::buildParser(unsigned int firstLine, unsigned int lastLine)const {
    CSVParser *parser = NULL;

    if(isValid()) {
        parser = new CSVSimpleParser(getFile(),getSeparator(),getMergeSeparator(),getTextSeparator(),getEncoding(),firstLine,lastLine);

        if(invertMatrix()) {
            parser = new CSVInvertMatrixParser(parser);
        }
    }

    return parser;
}

void CSVParserConfigurationWidget::fillEncodingComboBox() {
    QList<QByteArray> codecs = QTextCodec::availableCodecs ();
    ui->encodingComboBox->clear();
    QStringList list;

    for(QList<QByteArray>::const_iterator it = codecs.constBegin() ; it != codecs.constEnd(); ++it) {
        list.push_back(QString(*it));
    }

    list.sort();
    ui->encodingComboBox->addItems(list);
}

void CSVParserConfigurationWidget::changeSeparator(int index) {
    if(ui->separatorComboBox->itemText(index)=="Other") {
        if(!ui->othersep->isEnabled())
            ui->othersep->setEnabled(true);
        else if(!ui->othersep->text().isEmpty())
            emit(parserChanged());
    }
    else {
        ui->othersep->setEnabled(false);
        emit(parserChanged());
    }
}

char CSVParserConfigurationWidget::getSeparator(int index) const {
    QString text = ui->separatorComboBox->itemText(index);

    if(text == "Tab") {
        return '\t';
    }
    else if(text == "Space") {
        return ' ';
    }
    else if(text=="Other") {
        return ui->othersep->text().isEmpty()?' ':ui->othersep->text().at(0).toAscii();
    }
    else {
        return text.at(0).toAscii();
    }
}

char CSVParserConfigurationWidget::getSeparator()const {
    return getSeparator(ui->separatorComboBox->currentIndex());
}

void CSVParserConfigurationWidget::changeFileNameButtonPressed() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CSV file"), QString(), tr(
                                                        "CSV files (*.csv);;Text files (*.txt);;All files (*)"));
    setFileToOpen(fileName);
}

void CSVParserConfigurationWidget::setFileToOpen(const QString& fileToOpen) {
    if(QFile::exists(fileToOpen)) {
        ui->fileLineEdit->setText(fileToOpen);

        //Try to autodetect separator
        if(QFile::exists(ui->fileLineEdit->text())) {
            QFile file(ui->fileLineEdit->text());

            if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                //Read the first line
                QByteArray firstLine = file.readLine();

                if(!firstLine.isEmpty()) {
                    QString line(firstLine);
                    //Search for the best matching separator in the default list
                    QVector<int> separatorOccurence(ui->separatorComboBox->count());

                    for(int i = 0 ; i< ui->separatorComboBox->count() ; ++i) {
                        char separator = getSeparator(i);
                        //Count the number of occurence for this separator
                        separatorOccurence[i] = line.count(separator);
                    }

                    int currentMaxOccurence = -1;

                    for(int i = 0 ; i< ui->separatorComboBox->count() ; ++i) {
                        if(separatorOccurence[i] > currentMaxOccurence) {
                            currentMaxOccurence = separatorOccurence[i];
                            //Set as separator the one with the greatest occurence number.
                            ui->separatorComboBox->setCurrentIndex(i);
                        }
                    }
                }

                file.close();
            }
        }

        emit parserChanged();
    }
}

void CSVParserConfigurationWidget::encodingChanged() {
    emit parserChanged();
}

string CSVParserConfigurationWidget::getFile()const {
    return QStringToTlpString(ui->fileLineEdit->text());
}
bool CSVParserConfigurationWidget::isValid()const {
    return QFile::exists(ui->fileLineEdit->text());
}
string CSVParserConfigurationWidget::getEncoding()const {
    return QStringToTlpString(ui->encodingComboBox->currentText());
}
char CSVParserConfigurationWidget::getTextSeparator() const {
    return ui->textDelimiterComboBox->currentText().at(0).toAscii();
}

bool CSVParserConfigurationWidget::getMergeSeparator() const {
    return ui->mergesep->isChecked();
}

bool CSVParserConfigurationWidget::invertMatrix()const {
    return ui->switchRowColumnCheckBox->isChecked();
}
