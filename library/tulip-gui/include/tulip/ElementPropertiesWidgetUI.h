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
/****************************************************************************
** Form interface generated from reading ui file 'ElementPropertiesWidget.ui'
**
** Created: lun sep 29 12:43:34 2003
**      by: The User Interface Compiler ($Id: ElementPropertiesWidgetUI.h,v 1.3 2008-04-02 13:12:28 pmary Exp $)
**
****************************************************************************/

#ifndef ELEMENTPROPERTIESDATA_H
#define ELEMENTPROPERTIESDATA_H

#include <QtGui/qwidget.h>

class QGridLayout;
class QLabel;

namespace tlp {

class TulipTableWidget;

class ElementPropertiesWidgetUI : public QWidget
{
    Q_OBJECT

public:
    ElementPropertiesWidgetUI( QWidget* parent = 0);
    ~ElementPropertiesWidgetUI();

    QLabel* label;
    TulipTableWidget* propertyTable;

protected:
    QGridLayout* ElementPropertiesWidgetUILayout;

protected slots:
    virtual void languageChange();

    virtual void propertyTableValueChanged(int, int);

};

}

#endif // ELEMENTPROPERTIESDATA_H
