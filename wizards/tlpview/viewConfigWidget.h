#ifndef Tulip_GEOVIEWCONFIGURATIONWIDGET_H
#define Tulip_GEOVIEWCONFIGURATIONWIDGET_H

#include <QtGui/QWidget>

#include <tulip/DataSet.h>

#include "%ProjectName:c%ConfigWidget.h"

class %ProjectName:c%;

class %ProjectName:c%ConfigWidget : public QWidget, public Ui::%ProjectName:c%ConfigWidgetData {
  Q_OBJECT

public:

%ProjectName:
c%ConfigWidget(%ProjectName:c% *view);
  void setData(tlp::DataSet dataSet);
  void getData(tlp::DataSet *dataSet);

protected slots:

//your slots here

protected :

%ProjectName:
  c% *view;

};

#endif
