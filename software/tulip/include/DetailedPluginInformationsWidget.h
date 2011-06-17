#ifndef DETAILEDPLUGININFORMATIONSWIDGET_H
#define DETAILEDPLUGININFORMATIONSWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
  class DetailedPluginInformationsWidgetData;
}

namespace tlp {
  class PluginInformations;
}

class DetailedPluginInformationsWidget: public QWidget {
  Q_OBJECT
public:
  explicit DetailedPluginInformationsWidget(tlp::PluginInformations *infos,QWidget *parent = 0);

signals:
  void goBack();

private:
  Ui::DetailedPluginInformationsWidgetData *_ui;

};

#endif // DETAILEDPLUGININFORMATIONSWIDGET_H
