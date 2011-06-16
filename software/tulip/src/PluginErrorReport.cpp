#include "PluginErrorReport.h"

#include <QtCore/QFileInfo>

#include "ui_PluginErrorReport.h"

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent): QWidget(parent), _ui(new Ui::PluginErrorReportData()) {
  _ui->setupUi(this);

  QFileInfo fileInfo(fileName);
  _ui->fileName->setText("<html><head/><body><b>" + fileInfo.fileName() + "</b></body></head>");

  _ui->errorMsg->setText("<p><span style=\"font-size:small;\">" +
                         errorMsg +
                         "</span></p>");
}
