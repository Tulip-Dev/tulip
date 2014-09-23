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
#include <tulip/StringsListSelectionDialog.h>
#include "ui_StringsListSelectionDialog.h"

using namespace std;

namespace tlp {
StringsListSelectionDialog::StringsListSelectionDialog(QWidget *parent): QDialog(parent),
  ui(new Ui::StringsListSelectionDialog) {
  ui->setupUi(this);
}

/**
 * set the look of the widget
 * \param listType this parameter defines the widget's look (see class description)
 */
void StringsListSelectionDialog::setListType(const StringsListSelectionWidget::ListType listType) {
  ui->stringsListSelectionWidget->setListType(listType);
}

/**
 * sets the maximum size of the selected strings list
 */
void StringsListSelectionDialog::setMaxSelectedStringsListSize(const unsigned int maxSize) {
  ui->stringsListSelectionWidget->setMaxSelectedStringsListSize(maxSize);
}


/**
 * sets the strings list and the already selected strings
 */
void StringsListSelectionDialog::setStringsList(const std::vector<std::string>& strList, std::vector<std::string>& selList) {
  ui->stringsListSelectionWidget->setUnselectedStringsList(strList);
  ui->stringsListSelectionWidget->setSelectedStringsList(selList);
}

/**
  * @brief Constructs a strings list selection dialog with the given parent.
  * \param parent the widget's parent
  * \param listType this parameter defines the widget's look (see class description)
  * \param maxSelectedStringsListSize the maximum number of strings that can be selected (if 0, no size restriction)
  **/
StringsListSelectionDialog::StringsListSelectionDialog(QString title,
    QWidget *parent,
    const StringsListSelectionWidget::ListType listType,
    const unsigned int maxSize)
  : QDialog(parent), ui(new Ui::StringsListSelectionDialog) {
  ui->setupUi(this);
  setWindowTitle(title);
  setListType(listType);
  setMaxSelectedStringsListSize(maxSize);
}

StringsListSelectionDialog::~StringsListSelectionDialog() {
  if (ui)
    delete ui;
}

/**
  * @brief This is a convenience static function that create a dialog
  * to select some strings in a list.
  * The function creates a modal dialog with the given title and parent widget.
  *
  * @param title the string to display in the dialog title bar,
  * @param strList the list of strings to choose,
  * @param selList on input it indicates the already selected strings, then on output the user selected strings,
  * @param listType the type of display for the strings to choose,
  * @param maxSize the maximum number of strings to select. 0 means this number is not limited.
  **/
bool StringsListSelectionDialog::choose(QString title,
                                        const std::vector<std::string>& strList,
                                        std::vector<std::string>& selList,
                                        QWidget* parent,
                                        const StringsListSelectionWidget::ListType listType,
                                        const unsigned int maxSize) {
  StringsListSelectionDialog dialog(title, parent, listType, maxSize);
  dialog.setStringsList(strList, selList);
  int result = dialog.exec();

  if (result == QDialog::Accepted) {
    selList = dialog.ui->stringsListSelectionWidget->getSelectedStringsList();
    return true;
  }

  return false;
}
}
