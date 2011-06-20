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

  tlp::PluginInformations *pluginInformations() { return _pluginInformations; }

signals:
  void goBack();
  void fetch();
  void remove();

private:
  Ui::DetailedPluginInformationsWidgetData *_ui;
  tlp::PluginInformations *_pluginInformations;

};

#endif // DETAILEDPLUGININFORMATIONSWIDGET_H
