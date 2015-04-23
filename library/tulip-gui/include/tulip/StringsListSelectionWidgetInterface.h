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


#ifndef STRINGLISTSELECTIONWIDGETINTERFACE_H_
#define STRINGLISTSELECTIONWIDGETINTERFACE_H_

#include <tulip/tulipconf.h>

#include <vector>
#include <string>

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
///@endcond
