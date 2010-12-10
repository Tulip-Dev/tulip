#ifndef CSVPARSERCONFIGURATIONWIDGET_H
#define CSVPARSERCONFIGURATIONWIDGET_H

#include <QtGui/QWidget>
#include <tulip/CSVParser.h>
#include <tulip/CSVContentHandler.h>

namespace Ui {
    class CSVParserConfigurationWidget;
}

namespace tlp{

class TLP_QT_SCOPE CSVParserConfigurationWidget : public QWidget {
    Q_OBJECT
public:
    CSVParserConfigurationWidget(QWidget *parent = 0);
    ~CSVParserConfigurationWidget();    
    CSVParser * buildParser()const;

    std::string getFile()const;
    bool fileIsValid()const;
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
