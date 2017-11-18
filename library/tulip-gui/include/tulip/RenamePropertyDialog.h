/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
///@cond DOXYGEN_HIDDEN

#ifndef RENAMEPROPERTYDIALOG_H
#define RENAMEPROPERTYDIALOG_H

#include <tulip/tulipconf.h>

class QWidget;

namespace tlp {
class PropertyInterface;

/**
  * @brief Provide a dialog that allow user to rename a property.
  *
  * The easiest way to use this class is to use the renameProperty static function.
  *
  **/

class TLP_QT_SCOPE RenamePropertyDialog {
public:
  /**
    * @brief This is a convenience static function to rename a property using a dialog. If the user
    *presses Cancel or an error occurs, it returns false.
    *
    * The function creates a modal property rename dialog with the given property, and parent
    *widget.
    *
    * @param toRenameProp The property to rename.
    **/
  static bool renameProperty(tlp::PropertyInterface *toRenameProp, QWidget *parent = NULL);
};
}
#endif // RENAMEPROPERTYDIALOG_H
///@endcond
