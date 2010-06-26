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
** Form implementation generated from reading ui file 'ElementPropertiesWidget.ui'
**
** Created: lun sep 29 12:43:35 2003
**      by: The User Interface Compiler ($Id: ElementPropertiesWidgetUI.cpp,v 1.4 2008-04-21 12:16:28 pmary Exp $)
**
****************************************************************************/

#include "../include/tulip/ElementPropertiesWidgetUI.h"

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qwhatsthis.h>

#include <tulip/PropertyWidget.h>

using namespace tlp;

/* 
 *  Constructs a ElementPropertiesWidgetUI as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
ElementPropertiesWidgetUI::ElementPropertiesWidgetUI( QWidget* parent)
    : QWidget(parent)
{
    ElementPropertiesWidgetUILayout = new QGridLayout(this); 
    
    ElementPropertiesWidgetUILayout->setSpacing(6);
    label = new QLabel(this);
    /*label->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, label->sizePolicy().hasHeightForWidth() ) );*/
    label->setFrameShape( QLabel::NoFrame );
    label->setScaledContents( FALSE );
    label->setAlignment(Qt::AlignCenter);
    label->setIndent( -1 );

    ElementPropertiesWidgetUILayout->addWidget( label, 0, 0 );

    propertyTable = new PropertyWidget( this, "propertyTable" );
    /* propertyTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, propertyTable->sizePolicy().hasHeightForWidth() ) ); */
    // propertyTable->setVScrollBarMode( PropertyWidget::Auto );
    propertyTable->setRowCount( 7 );
    propertyTable->setColumnCount( 2 );
    propertyTable->setSelectionMode(QAbstractItemView::NoSelection);

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
    label->setText( tr( "Node" ) );
}

void ElementPropertiesWidgetUI::propertyTableValueChanged(int,int)
{
    qWarning( "ElementPropertiesWidgetUI::propertyTableValueChanged(int,int): Not implemented yet" );
}

