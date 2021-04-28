/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_H_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_H_

#include <QWidget>

#include "StringsListSelectionWidgetInterface.h"

namespace Ui {
class SimpleStringsListSelectionData;
}

class QListWidgetItem;

namespace tlp {

class TLP_QT_SCOPE SimpleStringsListSelectionWidget
    : public QWidget,
      public StringsListSelectionWidgetInterface {

  Q_OBJECT

  Ui::SimpleStringsListSelectionData *_ui;

public:
  SimpleStringsListSelectionWidget(
      QWidget *parent = nullptr,
      const unsigned int maxSelectedStringsListSize = 0, bool orderable = true);
  ~SimpleStringsListSelectionWidget() override;

  void setUnselectedStringsList(
      const std::vector<std::string> &unselectedStringsList) override;

  void setSelectedStringsList(
      const std::vector<std::string> &selectedStringsList) override;

  void clearUnselectedStringsList() override;

  void clearSelectedStringsList() override;

  void setMaxSelectedStringsListSize(
      const unsigned int maxSelectedStringsListSize) override;

  std::vector<std::string> getSelectedStringsList() const override;

  std::vector<std::string> getUnselectedStringsList() const override;

  void selectAllStrings() override;

  void unselectAllStrings() override;

private slots:

  void pressButtonSelectAll();
  void pressButtonUnselectAll();
  void listItemClicked(QListWidgetItem *item);
  void pressButtonUp();
  void pressButtonDown();

private:
  unsigned int maxSelectedStringsListSize;
};
} // namespace tlp

#endif /* SIMPLESTRINGSLISTSELECTIONWIDGET_H_ */
///@endcond
