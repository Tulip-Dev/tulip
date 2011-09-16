#include "PluginErrorReport.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "ui_PluginErrorReport.h"

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent): QWidget(parent), _ui(new Ui::PluginErrorReportData()) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");
  _ui->setupUi(this);

  QFileInfo fileInfo(fileName);
  _ui->fileName->setText("<html><head/><body><b>" + fileInfo.fileName() + "</b></body></head>");

  _ui->errorMsg->setText("<p><span style=\"font-size:small;\">" +
                         errorMsg +
                         "</span></p>");

}

void PluginErrorReport::focusInEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: rgb(232, 238, 244); border: 1px solid #C9C9C9; }");
  repaint();
}

void PluginErrorReport::focusOutEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");
  repaint();
}
