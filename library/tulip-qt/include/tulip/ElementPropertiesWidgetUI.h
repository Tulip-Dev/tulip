/****************************************************************************
** Form interface generated from reading ui file 'ElementPropertiesWidget.ui'
**
** Created: lun sep 29 12:43:34 2003
**      by: The User Interface Compiler ($Id: ElementPropertiesWidgetUI.h,v 1.2 2008-03-03 15:30:57 pmary Exp $)
**
****************************************************************************/

#ifndef ELEMENTPROPERTIESDATA_H
#define ELEMENTPROPERTIESDATA_H

#include <QtGui/qwidget.h>
#include "tulip/Qt3ForTulip.h"

class QGridLayout;
class QLabel;

namespace tlp {

class TulipTableWidget;

class ElementPropertiesWidgetUI : public QWidget
{
    Q_OBJECT

public:
    ElementPropertiesWidgetUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
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
