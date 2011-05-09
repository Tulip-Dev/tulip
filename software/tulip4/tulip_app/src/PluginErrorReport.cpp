#include "PluginErrorReport.h"

#include <QtCore/QFileInfo>

#include "ui_PluginErrorReport.h"

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent): QWidget(parent), _ui(new Ui::PluginErrorReportData()) {
  _ui->setupUi(this);

  QFileInfo fileInfo(fileName);
  _ui->fileName->setText("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10.25pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">" +
                         fileInfo.fileName() +
                         "  </span></p></body></html>");

  _ui->errorMsg->setText("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:7pt;\">" +
                         errorMsg +
                         "</span></p></body></html>");
}
