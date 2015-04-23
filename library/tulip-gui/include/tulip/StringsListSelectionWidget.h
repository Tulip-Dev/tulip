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


#ifndef STRINGLISTSELECTIONWIDGET_H_
#define STRINGLISTSELECTIONWIDGET_H_

#include <vector>

#include <tulip/StringsListSelectionWidgetInterface.h>

#include <QWidget>

namespace tlp {

/** \brief A widget for selecting a set of strings
 *
 * This widget allow to select a subset of strings from an initial input strings list.
 * The look of the widget can be set via the ListType parameter :
 *    -> SIMPLE_LIST : the widget contains only one strings list, the selection of strings is done via the checkboxes located on the left of the items list
 *    -> DOUBLE_LIST : the widget contains two lists, the left one contains the unselected string list and the right one the selected strings list. To select
 *                     a string (resp. unselect a string), it has to be moved from the list on the left to the list on the right (resp. from the list on the right to
 *                     the list on the left) via the buttons located between the two lists or by drag'n drop.
 */
class TLP_QT_SCOPE StringsListSelectionWidget: public QWidget,
  public StringsListSelectionWidgetInterface {

public:

  enum ListType {
    SIMPLE_LIST, DOUBLE_LIST
  };

  /**
   * Default constructor (for qt designer)
   * \param parent the widget's parent
   * \param listType this parameter defines the widget's look (see class description)
   * \param maxSelectedStringsListSize the maximum number of strings that can be selected (if 0, no size restriction)
   */
  StringsListSelectionWidget(QWidget *parent = NULL, const ListType listType =
                               DOUBLE_LIST, const unsigned int maxSelectedStringsListSize = 0);

  /**
   * This constructor creates the widget and initializes the unselected strings list
   * \param unselectedStringsList a vector containing the set of strings that can be selected
   * \param parent the widget's parent
   * \param listType this parameter defines the widget's look (see class description)
   * \param maxSelectedStringsListSize the maximum number of strings that can be selected (if 0, no size restriction)
   */
  StringsListSelectionWidget(
    const std::vector<std::string> &unselectedStringsList,
    QWidget *parent = NULL, const ListType listType = DOUBLE_LIST,
    const unsigned int maxSelectedStringsListSize = 0);

  /**
   * Method which sets the look of the widget
   * \param listType this parameter defines the widget's look (see class description)
   */
  void setListType(const ListType listType);

  /**
   * Method which sets the unselected strings list
   * \param unselectedStringsList a vector containing a set of strings to be unselected
   */
  void setUnselectedStringsList(
    const std::vector<std::string> &unselectedStringsList);

  /**
   * Method which sets the selected strings list
   * \param selectedStringsList a vector containing a set of strings to be selected
   */
  void setSelectedStringsList(
    const std::vector<std::string> &selectedStringsList);

  /**
   * Method which empty the unselected strings list
   */
  void clearUnselectedStringsList();

  /**
   * Method which empty the selected strings list
   */
  void clearSelectedStringsList();

  /**
   * Method which sets the label text value of the unselected strings list
   * (this method does nothing if listType = SIMPLE_LIST)
   */
  void setUnselectedStringsListLabel(
    const std::string &unselectedStringsListLabel);

  /**
   * Method which sets the label text value of the selected strings list
   * (this method does nothing if listType = SIMPLE_LIST)
   */
  void setSelectedStringsListLabel(
    const std::string &selectedStringsListLabel);

  /**
   * Method which sets the maximum size of the selected strings list
   */
  void setMaxSelectedStringsListSize(
    const unsigned int maxSelectedStringsListSize);

  /**
   * Method which returns the selected strings as a vector
   */
  std::vector<std::string> getSelectedStringsList() const;

  /**
   * Method which returns the unselected strings as a vector
   */
  std::vector<std::string> getUnselectedStringsList() const;

  /**
   * Method which returns both of the selected and unselected strings as a vector
   */
  std::vector<std::string> getCompleteStringsList() const;

  /**
   * Method which selects all strings
   */
  void selectAllStrings();

  /**
   * Method which unselect all strings
   */
  void unselectAllStrings();

private:

  ListType listType;
  StringsListSelectionWidgetInterface *stringsListSelectionWidget;

};

}

#endif /* STRINGLISTSELECTIONWIDGET_H_ */
///@endcond
