/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef TULIPCOMBOBOX_H
#define TULIPCOMBOBOX_H

#include <tulip/tulipconf.h>
#include <QComboBox>

namespace tlp {

// There is a Qt bug when displaying a QComboBox popup with many items
// in a QGraphicsView; this class implements a simple workaround using a QMenu
class TLP_QT_SCOPE TulipComboBox : public QComboBox {
public:
  explicit TulipComboBox(QWidget *parent = nullptr) : QComboBox(parent) {}

  void showPopup() override;
};

} // namespace tlp

#endif // TULIPCOMBOBOX_H
///@endcond
