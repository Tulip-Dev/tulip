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
typedef std::pair<QWidget *, std::string> std_pair_0101QWidget_0100std_string;
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlMainView.h>
%End

typedef std::pair<QWidget *, std::string> std_pair_0101QWidget_0100std_string;

namespace tlp {

class View : QObject, tlp::WithParameter, tlp::WithDependency /Abstract/ {
%TypeHeaderCode
#include <tulip/View.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::NodeLinkDiagramComponent*>(sipCpp)) {
   		sipType = sipType_tlp_NodeLinkDiagramComponent;
   } else if (dynamic_cast<tlp::GlMainView*>(sipCpp)) {
   		sipType = sipType_tlp_GlMainView;
   } else if (dynamic_cast<tlp::AbstractView*>(sipCpp)) {
   		sipType = sipType_tlp_AbstractView;
   } else {
   		sipType = NULL;
   } 
%End

public:
	virtual QWidget *construct(QWidget *parent) = 0;

	virtual void setData(tlp::Graph *graph, tlp::DataSet dataSet) = 0;

	virtual void getData(tlp::Graph **graph, tlp::DataSet *dataSet) = 0;

	virtual tlp::Graph *getGraph() = 0;
	
	virtual std::list<std_pair_0101QWidget_0100std_string> getConfigurationWidget() = 0;
	
	virtual void setInteractors(const std::list<tlp::Interactor *> &interactors) = 0;

	virtual std::list<tlp::Interactor *> getInteractors() = 0;
	
	
	virtual void setActiveInteractor(tlp::Interactor *interactor) = 0;

	virtual void createPicture(const std::string &pictureName,int width, int height) = 0;

public slots:
	virtual void setGraph(tlp::Graph *graph) = 0;
	
	virtual void draw() = 0;

	virtual void refresh() = 0;

	virtual void init() = 0;

signals:

	void elementSelected(unsigned int eltId, bool isNode);
	
	void requestChangeGraph(tlp::View *, tlp::Graph *);

};

class ViewContext {
%TypeHeaderCode
#include <tulip/View.h>
%End
};

class ViewFactory : tlp::AbstractPluginInfo {
%TypeHeaderCode
#include <tulip/View.h>
%End  	
  	public:
    	virtual ~ViewFactory();
    
    	virtual tlp::View *createPluginObject(tlp::ViewContext *gc) = 0 /Factory/;
    
    	virtual  std::string getMajor();
    
    	virtual  std::string getMinor();
    
    	virtual  std::string getTulipMajor();
    
    	virtual  std::string getTulipMinor();

    	void registerPlugin();
%MethodCode
	sipCpp->initFactory();
	sipCpp->factory->registerPlugin(sipCpp);
%End
  };
};
