/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 02/09

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 */

#ifndef STRINGLISTSELECTIONWIDGET_H_
#define STRINGLISTSELECTIONWIDGET_H_

#include <vector>
#include "StringListSelectionWidgetData.h"

namespace tlp {

/** \brief A widget which allow to select a subset of strings from an input string list
 *   This Widget allows the user to select a subset of strings from an input string list
 *   The widget is composed of an input string list on the left and an output string list on the right
 *   The user user can move an item from one list to another by using the "left" and "right" button
 *    (located in the middle of the widget) or by drag'n drop
 *   The output string list can also be ordering the way the user want it by using the
 *    "up" and "down" button on the right of the widget
 */
class TLP_QT_SCOPE StringListSelectionWidget : public QWidget, public Ui::StringListSelectionData {

  Q_OBJECT

public :

  /**
   * Default constructor, create an empty String List Selection Widget
   * Use setInputStringList and setOutputStringList to initialize the widget the way you want
   *
   * \param maxOutputListSize specify the maximum number of items the user can pick (if set to 0, there is no restriction on the output list size)
   */
  StringListSelectionWidget(QWidget *parent = 0, const unsigned int maxOutputListSize = 0);

  /**
   * This constructor will create a new String List Selection Widget with the input string list initialized
   *  with the content of the vector passed as first parameter
   *
   *  \param inputStringList a vector containing the strings to insert in the input string list
   *  \param maxOutputListSize specify the maximum number of items the user can pick (if set to 0, there is no restriction on the output list size)
   */
  StringListSelectionWidget(const std::vector<std::string> &inputStringList, QWidget *parent = 0, const unsigned int maxOutputListSize = 0);

  /**
   * Method to fill the content of the input string list
   *
   * \param inputStringList a vector containing the strings to insert in the input string list
   */
  void setInputStringList(const std::vector<std::string> &inputStringList);

  /**
   * Method to fill the content of the output string list
   *
   * \param outputStringList a vector containing the strings to insert in the output string list
   */
  void setOutputStringList(const std::vector<std::string> &outputStringList);

  /**
   * Methods which empty the contents of the input string list
   */
  void clearInputStringList();

  /**
   * Methods which empty the contents of the output string list
   */
  void clearOutputStringList();

  /**
   * Method to set the text of the label located at the top of the input string list
   *
   * \param inputStringListLabel the string to display in the input string list label
   */
  void setInputStringListLabel(const std::string &inputStringListLabel);

  /**
   * Method to set the text of the label located at the top of the input string list
   *
   * \param inputStringListLabel the string to display in the input string list label
   */
  void setOutputStringListLabel(const std::string &outputStringListLabel);

  /**
   * Method to set the maximum size of the output string list
   *
   * \param maxOutputListSize the maximum number of items the output list can contain (if set to 0, there is no restriction on the output list size)
   */
  void setMaxOutputListSize(const unsigned int maxOutputListSize);

  /**
   * Method which returns the content of the output string list
   */
  std::vector<std::string> getOutputStringList() const;

  /**
   * Method which returns the content of the input string list.
   */
  std::vector<std::string> getInputStringList() const;

public slots:

  void pressButtonAdd();
  void pressButtonRem();
  void pressButtonUp();
  void pressButtonDown();

private :

  void qtWidgetsConnection();

};

}

#endif /* STRINGLISTSELECTIONWIDGET_H_ */
