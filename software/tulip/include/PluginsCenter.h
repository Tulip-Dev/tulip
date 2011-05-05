#ifndef PLUGINSCENTER_
#define PLUGINSCENTER_

#include <QtGui/QWidget>
#include "ui_PluginsCenter.h"

class PluginsCenter : public QWidget {
public:
  explicit PluginsCenter(QWidget *parent=0);

private:
  Ui::PluginsCenterData *_ui;
};

#endif // PLUGINSCENTER_
