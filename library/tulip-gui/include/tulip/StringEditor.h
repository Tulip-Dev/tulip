/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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


#ifndef STRINGEDITOR_H
#define STRINGEDITOR_H

#include <QString>
#include <QTextEdit>
#include <QDialog>

#include <tulip/tulipconf.h>

namespace tlp {
/**
* @brief Simple String edition widget.
**/
class TLP_QT_SCOPE StringEditor : public QDialog {
  QString currentString;
  QTextEdit* edit;

public:
  explicit StringEditor(QWidget *parent = NULL);

  QString getString() const;
  void setString(const QString& qstr);

  // redefinition to ensure it is shown in the center of its parent
  void showEvent(QShowEvent* ev);

public slots:
  // redefinition to ensure to catch the end of input
  void done(int r);
};
}

#endif // STRINGEDITOR_H
///@endcond
