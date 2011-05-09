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

#ifndef DOUBLESTRINGSLISTRELATIONDIALOG_H_
#define DOUBLESTRINGSLISTRELATIONDIALOG_H_

#include <QtGui/QDialog>

#include <tulip/Color.h>

#include "ui_DoubleStringsListRelationDialog.h"

namespace tlp {

class DoubleStringsListRelationDialog : public QDialog, public Ui::DoubleStringsListRelationDialogData {

	Q_OBJECT

public :

  DoubleStringsListRelationDialog(const std::vector<std::string> &firstValues,const std::vector<Color> &secondValues,QWidget *parent = 0);
  void getResult(std::vector<std::pair<std::string,Color> > &result);


private slots:

  void upButtonClicked();
  void downButtonClicked();
  void scrollBarValueChanged(int value);

private :

};

}

#endif /* DOUBLESTRINGSLISTRELATIONDIALOG_H_ */
