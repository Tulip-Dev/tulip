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

#ifndef PATHFINDERCONFIGURATIONWIDGET_H_
#define PATHFINDERCONFIGURATIONWIDGET_H_

#include "ui_PathFinderConfiguration.h"
#include <QtGui/QWidget>

class PathFinderConfigurationWidget: public QWidget, public Ui::PathFinderConfigurationData {
  Q_OBJECT
public:
  PathFinderConfigurationWidget(QWidget *parent=0);
  virtual ~PathFinderConfigurationWidget();
};

#endif /* PATHFINDERCONFIGURATIONWIDGET_H_ */
