/**
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

#ifndef SOMWIDGET_H_
#define SOMWIDGET_H_

#include <QWidget>

namespace Ui {
class SOMWidget;
}

namespace tlp {
class SOMWidget:public QWidget {
  Q_OBJECT

  Ui::SOMWidget* _ui;

public:
  SOMWidget(QWidget *parent = NULL);
  ~SOMWidget();
};
}

#endif /* SOMWIDGET_H_ */
