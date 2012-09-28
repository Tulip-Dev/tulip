#include "%ProjectName:c%.h"

#include <tulip/Interactor.h>

#include <iostream>

using namespace std;
using namespace tlp;

PLUGIN(%ProjectName:c%)

//  INTERACTORPLUGINVIEWEXTENSION(%ProjectName:c%ViewInteractor,"%ProjectName:c%Interactor","InteractorNavigation","%PluginName%", "%Author%","%Date%","%Stability%","%Version%");

%ProjectName:c%::%ProjectName:c%(const tlp::PluginContext*):configWidget(NULL) {
}

void %ProjectName:c%::setState(const tlp::DataSet& dataSet){
if(!configWidget)
  configWidget = new %ProjectName:c%ConfigWidget(this);

NodeLinkDiagramComponent::setState(dataSet);
DataSet data;
if(dataSet.exist("data")){
  dataSet.get("data",data);
}
configWidget->setData(data);
}

tlp::DataSet %ProjectName:c%::state() const {
DataSet dataSet = NodeLinkDiagramComponent::state();

DataSet data;
dataSet.get<DataSet>("data",data);
configWidget->getData(&data);
dataSet.set<DataSet>("data",data);
}
//==================================================
QList<QWidget*> %ProjectName:c%::configurationWidgets() const {
QList<QWidget*> widgetList = NodeLinkDiagramComponent::configurationWidgets();
widgetList.push_back(configWidget);
return widgetList;
}
