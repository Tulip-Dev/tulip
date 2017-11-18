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

#ifndef TULIPLINEEDIT_H
#define TULIPLINEEDIT_H

#include <QLineEdit>

namespace tlp {

class TulipLineEdit : public QLineEdit {
public:
  TulipLineEdit(QWidget *parent = NULL) : QLineEdit(parent) {}

#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
  // the method below exists only since Qt 4.7
  // so we define it to ensure compatibility with
  // older versions
  void setPlaceholderText(const QString &) {}
#endif
};
}

#endif

///@endcond
