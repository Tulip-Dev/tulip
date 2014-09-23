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


#ifndef STRINGSLISTSELECTIONDIALOG_H
#define STRINGSLISTSELECTIONDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>
#include <tulip/StringsListSelectionWidget.h>

namespace Ui {
class StringsListSelectionDialog;
}

namespace tlp {

/**
  * @brief Provide a dialog that allow user to select a list of strings.
  *
  * The easiest way to use this class is to use the static function.
  * @code
  * QString title;
  * QWidget* parent;
  * vector<string> listOfStrings; // must be initialized
  * vector<string> selectedStrings;
  * bool choosen = StringsListSelectionDialog::choose(title, parent, listOfStrings, selectedStrings);
  * @endcode
  *
  *
  **/
class TLP_QT_SCOPE StringsListSelectionDialog : public QDialog {
  Q_OBJECT

public:
  /**
    * @brief Constructs a dialog with the given parent.
    **/
  explicit StringsListSelectionDialog(QWidget *parent = NULL);

  /**
   * set the look of the widget
   * \param listType this parameter defines the widget's look (see class description)
   */
  void setListType(const StringsListSelectionWidget::ListType listType);

  /**
   * sets the maximum size of the selected strings list
   */
  void setMaxSelectedStringsListSize(const unsigned int maxSize);

  /**
   * sets the strings list and the already selected strings
   */
  void setStringsList(const std::vector<std::string>& strList,
                      std::vector<std::string>& selList);

  /**
    * @brief Constructs a strings list selection dialog with the given parent.
    * \param parent the widget's parent
    * \param listType this parameter defines the widget's look (see class description)
    * \param maxSelectedStringsListSize the maximum number of strings that can be selected (if 0, no size restriction)
    **/
  explicit StringsListSelectionDialog(QString title,
                                      QWidget *parent = NULL,
                                      const StringsListSelectionWidget::ListType listType = StringsListSelectionWidget::SIMPLE_LIST,
                                      const unsigned int maxSelectedStringsListSize = 0);
  ~StringsListSelectionDialog();

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
  static bool choose(QString title, const std::vector<std::string>& strList,
                     std::vector<std::string>& selList,
                     QWidget* parent=NULL,
                     const StringsListSelectionWidget::ListType listType =
                       StringsListSelectionWidget::SIMPLE_LIST,
                     const unsigned int maxSize = 0);
private:

  Ui::StringsListSelectionDialog *ui;
};
}

#endif // STRINGSLISTSELECTIONDIALOG_H
///@endcond
