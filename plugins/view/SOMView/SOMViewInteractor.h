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

#ifndef SOMVIEWINTERACTOR_H_
#define SOMVIEWINTERACTOR_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>

namespace tlp {

class SOMViewInteractor: public tlp::NodeLinkDiagramComponentInteractor {
public:
  SOMViewInteractor(const QString &iconPath, const QString &text);

  virtual bool isCompatible(const std::string &viewName) const;

};

class SOMViewNavigation: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewNavigation", "Dubois Jonathan", "02/04/2009", "Standard navigation with scale", "1.0","Navigation")

public:

  SOMViewNavigation(tlp::PluginContext *);

  virtual void construct();
};

class SOMViewSelection: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewSelection", "Dubois Jonathan", "02/04/2009", "Standard selection with scale","1.0","Selection")

public:

  SOMViewSelection(tlp::PluginContext *);

  virtual void construct();

};

class SOMViewProperties:public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewProperties", "Dubois Jonathan", "02/04/2009", "Standard show element info with scale","1.0","Informations")

public :

  SOMViewProperties(tlp::PluginContext *);

  virtual void construct();

};

class SOMViewThreshold: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewThreshold", "Dubois Jonathan", "02/04/2009", "Threshold selection on SOM", "1.0","Informations")

public:

  SOMViewThreshold(tlp::PluginContext *);

  virtual void construct();

};
}
#endif /* SOMVIEWINTERACTOR_H_ */
