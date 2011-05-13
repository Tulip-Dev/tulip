#ifndef PLUGINERRORREPORT_H
#define PLUGINERRORREPORT_H

#include <QtGui/QWidget>

namespace Ui {
class PluginErrorReportData;
}

/**
  @brief This widget provides with maintenance actions related to a plugin load errror.
  When a plugin crash at load time, the user shall have the following choices:
  @list
  @li Uninstall the plugin (removes the file permanantly)
  @li Reinstall it from a distant repository
  @list
  */
class PluginErrorReport: public QWidget {
  Q_OBJECT
public:
  explicit PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent=0);

private:
  Ui::PluginErrorReportData *_ui;
};
#endif // PLUGINERRORREPORT_H
