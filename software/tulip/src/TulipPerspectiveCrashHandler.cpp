/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "TulipPerspectiveCrashHandler.h"

#include "ui_TulipPerspectiveCrashHandler.h"

#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include "FormPost.h"

static const QString SEPARATOR = "=======================\n";

TulipPerspectiveCrashHandler::TulipPerspectiveCrashHandler(QWidget *parent)
  : QDialog(parent), _ui(new Ui::TulipPerspectiveCrashHandlerData), _isDetailedView(false) {
  _ui->setupUi(this);
  setDetailedView(false);
  adjustHeight();
  connect(_ui->detailsLink,SIGNAL(linkActivated(QString)),this,SLOT(toggleDetailedView()));
  connect(_ui->sendReportButton,SIGNAL(clicked()),this,SLOT(sendReport()));
  connect(_ui->saveButton,SIGNAL(clicked()),this,SLOT(saveData()));
}

void TulipPerspectiveCrashHandler::setDetailedView(bool f) {
  _isDetailedView = f;
  _ui->detailsFrame->setVisible(f);
  adjustHeight();
}

void TulipPerspectiveCrashHandler::adjustHeight() {
  setFixedSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
  int w = width();
  adjustSize();
  resize(w,height());
}

void TulipPerspectiveCrashHandler::toggleDetailedView() {
  setDetailedView(!_isDetailedView);
}

void TulipPerspectiveCrashHandler::sendReport() {
  _poster = new FormPost;

  _poster->addField("summary","[ Tulip " + _ui->versionValue->text() + " ] Crash report from perspective: " + _ui->perspectiveNameValue->text() + " " + _ui->perspectiveArgumentsValue->text());

  QString description = + "System:\n\n";
  description += "Plateform: " + _ui->plateformValue->text() + "\n";
  description += "Architecture: " + _ui->archValue->text() + "\n";
  description += "Compiler: " + _ui->compilerValue->text() + "\n";
  description += "Tulip version: " + _ui->versionValue->text() + "\n";
  description += SEPARATOR;
  description += "Perspective:\n";
  description += "Name: " + _ui->perspectiveNameValue->text() + "\n";
  description += "Arguments: " + _ui->perspectiveArgumentsValue->text() + "\n";
  description += SEPARATOR;
  description += "Stack trace:\n";
  description += _ui->dumpEdit->toPlainText();

  _poster->addField("description",description);
  _poster->addField("platform",_ui->compilerValue->text());
  _poster->addField("os",_ui->plateformValue->text());
  _poster->addField("os_build",_ui->archValue->text());
  _poster->addField("steps_to_reproduce",_ui->commentsEdit->toPlainText());

  connect(_poster->postData("http://tulip.labri.fr/crash_report/mantis.php"),SIGNAL(finished()),this,SLOT(reportPosted()));

  _ui->sendReportButton->setText(trUtf8("Sending report..."));
  _ui->detailsFrame->setEnabled(false);
  _ui->sendReportButton->setEnabled(false);
  _ui->commentsEdit->setEnabled(false);
}

void TulipPerspectiveCrashHandler::reportPosted() {
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());

  if (reply->error() == QNetworkReply::NoError) {
    QString bugLink = _poster->response();
    _ui->sendReportButton->setText(trUtf8("Report sent"));
    _ui->errorReportTitle->setText(trUtf8("<b>Report has been sent. Thank you for supporting Tulip !</b><br/><br/>You can follow your report at this URL: ") + "<a href=\"" + bugLink + "\">" + bugLink + "</a>");
  }
  else {
    _ui->sendReportButton->setText(trUtf8("Error while sending report"));
    _ui->errorReportTitle->setText("<i>" + reply->errorString() + "</i>");
    _ui->sendReportButton->setEnabled(true);
  }

  sender()->deleteLater();
  _poster->deleteLater();
}

void TulipPerspectiveCrashHandler::saveData() {

}

void TulipPerspectiveCrashHandler::setEnvData(const QString &plateform, const QString &arch, const QString &compiler, const QString &version, const QString &stackTrace) {
  _ui->plateformValue->setText(plateform);
  _ui->archValue->setText(arch);
  _ui->compilerValue->setText(compiler);
  _ui->versionValue->setText(version);
  _ui->dumpEdit->setPlainText(stackTrace);
}

void TulipPerspectiveCrashHandler::setPerspectiveData(PerspectiveProcessInfos infos) {
  _perspectiveInfos = infos;
  _ui->perspectiveNameValue->setText(infos.name);
  QString args;
  QString a;
  foreach(a,infos.args.keys())
  args += "--" + a + "=" + infos.args[a].toString() + " ";
  _ui->perspectiveArgumentsValue->setText(args);
}
