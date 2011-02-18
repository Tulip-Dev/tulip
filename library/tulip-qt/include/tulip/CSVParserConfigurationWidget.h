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
#ifndef CSVPARSERCONFIGURATIONWIDGET_H
#define CSVPARSERCONFIGURATIONWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <tulip/CSVParser.h>
#include <tulip/CSVContentHandler.h>

namespace Ui {
    class CSVParserConfigurationWidget;
}

namespace tlp{


    class CSVParserConfigurationWidgetEditableComboBox : public QComboBox {
        Q_OBJECT
    public :
            CSVParserConfigurationWidgetEditableComboBox(QWidget * parent = 0 );
    protected:
            //Qt workaround avoiding QDialog to close automatically when pressing return or enter.
            void keyPressEvent(QKeyEvent *e);
    };

class TLP_QT_SCOPE CSVParserConfigurationWidget : public QWidget {
    Q_OBJECT    
public:
    CSVParserConfigurationWidget(QWidget *parent = 0);
    ~CSVParserConfigurationWidget();    
    CSVParser * buildParser()const;

    std::string getFile()const;
    bool isValid()const;
    std::string getSeparator()const;
    char getTextSeparator()const;
    std::string getEncoding()const;
    bool invertMatrix()const;

public slots:
    void setFileToOpen(const QString& fileToOpen);
protected:
    void fillEncodingComboBox();
    void changeEvent(QEvent *e);

protected slots:
    void changeFileNameButtonPressed();
    void encodingChanged();

private:
    Ui::CSVParserConfigurationWidget *ui;

signals:
    void parserChanged();
};
}
#endif // CSVPARSERCONFIGURATIONWIDGET_H
