/*
	  StringListSelectionWidgetInterface.h

   Created on: 3 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef STRINGLISTSELECTIONWIDGETINTERFACE_H_
#define STRINGLISTSELECTIONWIDGETINTERFACE_H_

#include <tulip/tulipconf.h>

#include <vector>
#include <string>

#include <QtGui/QWidget>

namespace tlp {

class TLP_QT_SCOPE StringsListSelectionWidgetInterface  {

public :

	virtual ~StringsListSelectionWidgetInterface() {}

	virtual void setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) = 0;

	virtual void setSelectedStringsList(const std::vector<std::string> &selectedStringsList) = 0;

	virtual void clearUnselectedStringsList() = 0;

	virtual void clearSelectedStringsList() = 0;

	virtual void setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) = 0;

	virtual std::vector<std::string> getSelectedStringsList() const = 0;

	virtual std::vector<std::string> getUnselectedStringsList() const = 0;

	virtual void selectAllStrings() = 0;

	virtual void unselectAllStrings() = 0;

};

}

#endif /* STRINGLISTSELECTIONWIDGETINTERFACE_H_ */
