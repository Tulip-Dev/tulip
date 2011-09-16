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

  tlp::PluginInformations *pluginInformations() {
    return _pluginInformations;
  }

public slots:
  void hideNavigationBar();

signals:
  void goBack();
  void fetch();
  void fetch(tlp::PluginInformations*);
  void remove();
  void remove(tlp::PluginInformations*);

protected slots:
  void fetchButtonClicked();
  void removeButtonClicked();

private:
  Ui::DetailedPluginInformationsWidgetData *_ui;
  tlp::PluginInformations *_pluginInformations;

};

#endif // DETAILEDPLUGININFORMATIONSWIDGET_H
