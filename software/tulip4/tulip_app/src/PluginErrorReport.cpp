#include "PluginErrorReport.h"

#include <QtCore/QFileInfo>

#include "ui_PluginErrorReport.h"

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent): QWidget(parent), _ui(new Ui::PluginErrorReportData()) {
  _ui->setupUi(this);

  QFileInfo fileInfo(fileName);
  _ui->fileName->setText(fileInfo.fileName());

  _ui->errorMsg->setText("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\"font-size:small;\">" +
                         errorMsg +
                         "</span></p></body></html>");
}
