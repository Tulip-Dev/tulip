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

#include <tulip/GLInteractor.h>
#include <tulip/GlMainWidget.h>

class SOMViewInteractor: public tlp::GLInteractorComposite {
public:
	SOMViewInteractor(const QString &iconPath, const QString &text);
	virtual ~SOMViewInteractor();

    virtual bool isCompatible(const std::string &viewName) const {
        return (viewName == "Self Organizing Map");
	}

};

class SOMViewNavigation: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewNavigation", "Dubois Jonathan", "02/04/2009", "Standard navigation with scale", "1.0","Navigation")

public:

  SOMViewNavigation(tlp::PluginContext *);
	~SOMViewNavigation();

	virtual void construct();

  QWidget *configurationWidget() const;

  unsigned int priority() const {return 3;}
};

class SOMViewSelection: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewSelection", "Dubois Jonathan", "02/04/2009", "Standard selection with scale","1.0","Selection")

public:

  SOMViewSelection(tlp::PluginContext *);
	~SOMViewSelection();

	virtual void construct();

  QWidget *configurationWidget() const;

  unsigned int priority() const {return 2;}

};

class SOMViewProperties:public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewProperties", "Dubois Jonathan", "02/04/2009", "Standard show element info with scale","1.0","Informations")

public :

  SOMViewProperties(tlp::PluginContext *);
	~SOMViewProperties();

	virtual void construct();

  QWidget *configurationWidget() const;

  unsigned int priority() const {return 1;}
};

class SOMViewThreshold: public SOMViewInteractor {

  PLUGININFORMATIONS("SOMViewThreshold", "Dubois Jonathan", "02/04/2009", "Threshold selection on SOM", "1.0","Informations")

public:

  SOMViewThreshold(tlp::PluginContext *);
	~SOMViewThreshold();

	virtual void construct();

  QWidget *configurationWidget() const;

  unsigned int priority() const {return 0;}
};

#endif /* SOMVIEWINTERACTOR_H_ */
