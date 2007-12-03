/****************************************************************************
** Form implementation generated from reading ui file 'ElementPropertiesWidget.ui'
**
** Created: lun sep 29 12:43:35 2003
**      by: The User Interface Compiler ($Id: ElementPropertiesWidgetUI.cpp,v 1.1.2.2 2007-12-03 09:13:39 pmary Exp $)
**
****************************************************************************/

#include "../include/tulip/ElementPropertiesWidgetUI.h"

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qwhatsthis.h>
#include "tulip/Qt3ForTulip.h"

#include <tulip/PropertyWidget.h>

using namespace tlp;

/* 
 *  Constructs a ElementPropertiesWidgetUI as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
ElementPropertiesWidgetUI::ElementPropertiesWidgetUI( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ElementPropertiesWidgetUI" );
    ElementPropertiesWidgetUILayout = new QGridLayout( this, 1, 1, 0, 6, "ElementPropertiesWidgetUILayout"); 

    label = new QLabel( this, "label" );
    label->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, label->sizePolicy().hasHeightForWidth() ) );
    label->setFrameShape( QLabel::NoFrame );
    label->setScaledContents( FALSE );
    label->setAlignment( int( Qt::AlignCenter ) );
    label->setIndent( -1 );

    ElementPropertiesWidgetUILayout->addWidget( label, 0, 0 );

    propertyTable = new PropertyWidget( this, "propertyTable" );
    /* propertyTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, propertyTable->sizePolicy().hasHeightForWidth() ) ); */
    propertyTable->setVScrollBarMode( PropertyWidget::Auto );
    propertyTable->setNumRows( 7 );
    propertyTable->setNumCols( 2 );
    propertyTable->setSelectionMode( PropertyWidget::NoSelection );

    ElementPropertiesWidgetUILayout->addWidget( propertyTable, 1, 0 );
    languageChange();
    resize( QSize(220, 352).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    //    connect( propertyTable, SIGNAL( valueChanged(int,int) ), this, SLOT( propertyTableValueChanged(int,int) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ElementPropertiesWidgetUI::~ElementPropertiesWidgetUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ElementPropertiesWidgetUI::languageChange()
{
    setCaption( tr( "Element Properties", "test" ) );
    label->setText( tr( "Node" ) );
}

void ElementPropertiesWidgetUI::propertyTableValueChanged(int,int)
{
    qWarning( "ElementPropertiesWidgetUI::propertyTableValueChanged(int,int): Not implemented yet" );
}

