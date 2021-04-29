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

#ifndef DOUBLESTRINGSLISTSELECTIONWIDGET_H_
#define DOUBLESTRINGSLISTSELECTIONWIDGET_H_

#include <QWidget>

#include "StringsListSelectionWidgetInterface.h"

namespace Ui {
class DoubleStringsListSelectionData;
}

namespace tlp {

class TLP_QT_SCOPE DoubleStringsListSelectionWidget : public QWidget,
                                                      public StringsListSelectionWidgetInterface {

  Q_OBJECT

  Ui::DoubleStringsListSelectionData *_ui;

public:
  DoubleStringsListSelectionWidget(QWidget *parent = nullptr,
                                   const unsigned int maxSelectedStringsListSize = 0);
  ~DoubleStringsListSelectionWidget() override;

  void setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) override;

  void setSelectedStringsList(const std::vector<std::string> &selectedStringsList) override;

  void clearUnselectedStringsList() override;

  void clearSelectedStringsList() override;

  void setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) override;

  std::vector<std::string> getSelectedStringsList() const override;

  std::vector<std::string> getUnselectedStringsList() const override;

  void selectAllStrings() override;

  void unselectAllStrings() override;

  void setUnselectedStringsListLabel(const std::string &unselectedStringsListLabel);

  void setSelectedStringsListLabel(const std::string &selectedStringsListLabel);

private slots:

  void pressButtonSelectAll();
  void pressButtonUnselectAll();
  void pressButtonAdd();
  void pressButtonRem();
  void pressButtonUp();
  void pressButtonDown();

private:
  void qtWidgetsConnection();
};
} // namespace tlp

#endif /* DOUBLESTRINGSLISTSELECTIONWIDGET_H_ */
///@endcond
