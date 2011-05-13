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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/Interactor.h>
namespace tlp {
typedef FactoryInterface<Interactor, InteractorContext*> InteractorFactory;
};
%End

namespace tlp {
class Interactor : QObject, tlp::WithParameter, tlp::WithDependency /Abstract/ {
%TypeHeaderCode
#include <tulip/Interactor.h>
%End
public:
	Interactor();

	virtual ~Interactor();

	virtual void setView(tlp::View *view) = 0;

	virtual void install(QWidget *) = 0;

	virtual void remove() = 0;

	virtual void setConfigurationWidgetText(const QString &text);

	virtual QWidget *getConfigurationWidget();

	virtual bool isCompatible(const std::string &viewName) = 0;

	int getPriority();

	void setPriority(int number);

	virtual tlp::InteractorAction* getAction() = 0;

	virtual void compute(tlp::GlMainWidget *) = 0;

	virtual void draw(tlp::GlMainWidget *) = 0;
};

class InteractorAction : QAction {
%TypeHeaderCode
#include <tulip/Interactor.h>
%End
public:
	InteractorAction(tlp::Interactor *interactor,const QIcon &icon, const QString &text);
	tlp::Interactor *getInteractor();
};

class InteractorContext {
%TypeHeaderCode
#include <tulip/Interactor.h>
%End
public :
	InteractorContext();

};

class InteractorFactory : tlp::AbstractPluginInfo {
%TypeHeaderCode
#include <tulip/Interactor.h>
%End
public:
	virtual ~InteractorFactory();
	
	virtual tlp::Interactor *createPluginObject(tlp::InteractorContext *ic) = 0 /Factory/;

	virtual std::string getMajor() const;
	
	virtual std::string getMinor() const;
	
	virtual std::string getTulipMajor() const;
	
	virtual std::string getTulipMinor() const;
	
	void registerPlugin();
%MethodCode
	tlp::InteractorLister::registerPlugin(sipCpp);
%End
};
};
