/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef ADDRESS_SELECTION_DIALOG_H
#define ADDRESS_SELECTION_DIALOG_H

#include <QDialog>

class QWidget;
class QString;
class QShowEvent;

namespace Ui {
class AddressSelectionDialogData;
}

namespace tlp {
class AddressSelectionDialog : public QDialog {

  Ui::AddressSelectionDialogData *_ui;

public:
  AddressSelectionDialog(QWidget *parent = nullptr);
  ~AddressSelectionDialog() override;

  void setBaseAddress(const QString &address);

  void addResultToList(const QString &result);

  void clearList();

  int getPickedResultIdx();

  bool rememberAddressChoice() const;

  void showEvent(QShowEvent *showEvt) override;
};
} // namespace tlp

#endif // ADDRESS_SELECTION_DIALOG_H
