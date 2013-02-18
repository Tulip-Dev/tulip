//-*-c++-*-
/*
 Author: Ludwig Fiolka
 Email : ludwig.fiolka@inria.fr
 Last modification : Apr 6, 2010
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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
