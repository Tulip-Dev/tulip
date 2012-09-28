#ifndef Tulip_%ProjectName:c%_VIEW_H
#define Tulip_%ProjectName:c%_VIEW_H

#include <tulip/NodeLinkDiagramComponent.h>

#include "%ProjectName:c%ConfigWidget.h"

class %ProjectName:c% : public tlp::NodeLinkDiagramComponent {
  Q_OBJECT
public:
  PLUGININFORMATIONS("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

%ProjectName:
  c%(const tlp::PluginContext* context = NULL);
virtual ~%ProjectName:
  c%() {}

  tlp::DataSet state() const;
  void setState(const tlp::DataSet&);
  QList<QWidget*> configurationWidgets() const;

private :
%ProjectName:
  c%ConfigWidget *configWidget;
};

#endif
