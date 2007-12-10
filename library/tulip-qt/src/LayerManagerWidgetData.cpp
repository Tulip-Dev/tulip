#include "../include/tulip/LayerManagerWidgetData.h"

#include <qvariant.h>
/*
 *  Constructs a GWOverviewWidgetData as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
LayerManagerWidgetData::LayerManagerWidgetData(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QDialog(parent, name, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
LayerManagerWidgetData::~LayerManagerWidgetData()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void LayerManagerWidgetData::languageChange()
{
    retranslateUi(this);
}

void LayerManagerWidgetData::updateView()
{
    qWarning("LayerManagerWidgetData::updateView(): Not implemented yet");
}

void LayerManagerWidgetData::backColor()
{
    qWarning("LayerManagerWidgetData::backColor(): Not implemented yet");
}

