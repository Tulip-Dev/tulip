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
#ifndef Tulip_CHOOSECONTROLLERDIALOG_H
#define Tulip_CHOOSECONTROLLERDIALOG_H

#include "ChooseControllerDialogData.h"

class ChooseControllerDialog : public QDialog, public Ui::ChooseControllerDialogData {
    Q_OBJECT

public:
  ChooseControllerDialog( QWidget* parent = 0);

  void setControllersAvailable(const std::vector<std::string> &controllersName);
  void setDefaultCheckedControllerName(const std::string &controllerName);
  void setDefaultControllerName(const std::string &controllerName);
  std::string getCheckedControllerName();

public slots:

  void accept();

protected slots:

  void itemClicked(QListWidgetItem *);

private :

  std::string defaultControllerName;

};

#endif
