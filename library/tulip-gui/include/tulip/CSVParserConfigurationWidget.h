/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
///@cond DOXYGEN_HIDDEN

#ifndef CSVPARSERCONFIGURATIONWIDGET_H
#define CSVPARSERCONFIGURATIONWIDGET_H

#include <QWidget>

#include <tulip/tulipconf.h>

namespace Ui {
class CSVParserConfigurationWidget;
}

namespace tlp {

class CSVParser;

class TLP_QT_SCOPE CSVParserConfigurationWidget : public QWidget {
  Q_OBJECT
public:
  CSVParserConfigurationWidget(QWidget *parent = NULL);
  ~CSVParserConfigurationWidget();
  /**
    * @brief Generate a csv parser in function of the parameters in the widget. User define the range of line to import.
    * The user takes the ownership of the widget.
    **/
  CSVParser * buildParser(unsigned int firstLine=0, unsigned int lastLine=UINT_MAX)const;

  std::string getFile()const;
  bool isValid()const;
  QString getSeparator()const;
  char getTextSeparator()const;
  std::string getEncoding()const;
  bool invertMatrix()const;
  bool getMergeSeparator() const;
  void initWithLastOpenedFile();

public slots:
  void setFileToOpen(const QString& fileToOpen);
protected:
  void fillEncodingComboBox();

protected slots:
  void changeFileNameButtonPressed();
  void encodingChanged();
  void changeSeparator(int index);

private:

  QString getSeparator(int index)const;

  Ui::CSVParserConfigurationWidget *ui;
  static QString lastOpenedFile;

signals:
  void parserChanged();
};
}
#endif // CSVPARSERCONFIGURATIONWIDGET_H
///@endcond
