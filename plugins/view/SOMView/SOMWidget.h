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

#ifndef SOMWIDGET_H_
#define SOMWIDGET_H_

#include "ui_SOMWidget.h"

class SOMWidget:public QWidget, public Ui::SOMWidget {
  Q_OBJECT
public:
  SOMWidget(QWidget *parent = NULL);
  virtual ~SOMWidget();
};


#endif /* SOMWIDGET_H_ */
