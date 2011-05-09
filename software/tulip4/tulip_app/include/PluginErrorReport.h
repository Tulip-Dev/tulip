#ifndef PLUGINERRORREPORT_H
#define PLUGINERRORREPORT_H

#include <QtGui/QWidget>

namespace Ui {
class PluginErrorReportData;
}

class PluginErrorReport: public QWidget {
  Q_OBJECT
public:
  explicit PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent=0);

private:
  Ui::PluginErrorReportData *_ui;
};
#endif // PLUGINERRORREPORT_H
