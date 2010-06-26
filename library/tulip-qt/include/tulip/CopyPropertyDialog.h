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
#ifndef COPYPROPERTYDIALOG_H
#define COPYPROPERTYDIALOG_H

#include <tulip/tulipconf.h>

#include <string>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif

#include "CopyPropertyDialogData.h"
#include "tulip/Iterator.h"

class TLP_QT_SCOPE CopyPropertyDialog : public QDialog, public Ui::CopyPropertyDialogData
{ 
  Q_OBJECT;

public:
  CopyPropertyDialog(QWidget *parent = 0);
  void setProperties(std::string& srcProp,
		     std::vector<std::string>& localProps,
		     std::vector<std::string>& inheritedProps);
  enum destType {NEW, LOCAL, INHERITED};
  bool getDestinationProperty(destType&, std::string& prop);
  
public slots:  
  void setNew( bool );
  void setLocal( bool );
  void setInherited( bool );
};

#endif // COPYPROPERTYDIALOG_H
