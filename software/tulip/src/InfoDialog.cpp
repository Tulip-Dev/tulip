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
#include <tulip/TulipRelease.h>
#include "InfoDialog.h"

using namespace std;

/*
 *  Constructs a InfoDialog which is a child of 'parent'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
InfoDialog::InfoDialog( QWidget* parent)
    : QDialog( parent) {
  setupUi(this);
  QString tmp="Tulip ";
  tmp+=TULIP_RELEASE;
  TextLabel2->setText(tmp);
}

/*
 *  Destroys the object and frees any allocated resources
 */
InfoDialog::~InfoDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

