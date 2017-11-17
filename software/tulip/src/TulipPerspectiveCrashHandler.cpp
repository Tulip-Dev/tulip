/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkReply>
#include <QFile>
#include <QScreen>

#include "FormPost.h"
#include <tulip/TulipProject.h>

static const QString SEPARATOR = "=======================\n";

TulipPerspectiveCrashHandler::TulipPerspectiveCrashHandler(QWidget *parent)
  : QDialog(parent), _ui(new Ui::TulipPerspectiveCrashHandlerData), _isDetailedView(false) {
  _ui->setupUi(this);
  setDetailedView(false);
  connect(_ui->detailsLink,SIGNAL(linkActivated(QString)),this,SLOT(toggleDetailedView()));
  connect(_ui->sendReportButton,SIGNAL(clicked()),this,SLOT(sendReport()));
  //connect(_ui->saveButton,SIGNAL(clicked()),this,SLOT(saveData()));
  _ui->saveButton->hide();
  _ui->commentsEdit->moveCursor(QTextCursor::End);
  QPixmap px(":/tulip/gui/icons/logo32x32.png");
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  // take care of the devicePixelRatio
  // before setting the label pixmap
  px.setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
#endif
  _ui->icon->setPixmap(px);

}

TulipPerspectiveCrashHandler::~TulipPerspectiveCrashHandler() {
  delete _ui;
}

void TulipPerspectiveCrashHandler::setDetailedView(bool f) {
  _isDetailedView = f;
  _ui->detailsLink->setText(f ? "<a href=\"Hide details\">Hide details</a>" :
                            "<a href=\"Show details\">View details</a> <span style=\"font-size:small\"><i>(sent with your comments)</i></span>");
  _ui->stackedWidget->setCurrentIndex(int(f));
}

void TulipPerspectiveCrashHandler::toggleDetailedView() {
  setDetailedView(!_isDetailedView);
}

void TulipPerspectiveCrashHandler::sendReport() {
  _poster = new FormPost;

  _poster->addField("summary","[ Tulip " + _ui->versionValue->text() + " (" + _ui->plateformValue->text() + ") ] Crash report from perspective: " + _ui->perspectiveNameValue->text() + " " + _ui->perspectiveArgumentsValue->text());

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

  connect(_poster->postData("http://tulip.labri.fr/devel/tulip_crash_report.php"),SIGNAL(finished()),this,SLOT(reportPosted()));

  _ui->sendReportButton->setText(trUtf8("Sending report..."));
  _ui->detailsFrame->setEnabled(false);
  _ui->sendReportButton->setEnabled(false);
  _ui->commentsEdit->setEnabled(false);
}

void TulipPerspectiveCrashHandler::reportPosted() {
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());

  if (reply->error() == QNetworkReply::NoError) {
    _ui->sendReportButton->setText(trUtf8("Report sent"));
    _ui->errorReportTitle->setText(trUtf8("<b>Report has been sent. Thank you for supporting Tulip !"));
  }
  else {
    _ui->sendReportButton->setText(trUtf8("Error while sending report"));
    _ui->errorReportTitle->setText("<i>" + reply->errorString() + "</i>");
    _ui->sendReportButton->setEnabled(true);
  }

  sender()->deleteLater();
  _poster->deleteLater();
}

//void TulipPerspectiveCrashHandler::saveData() {
//  tlp::TulipProject* project = tlp::TulipProject::restoreProject(_perspectiveInfo.projectPath);

//  if (!project->isValid())
//    QMessageBox::critical(this,trUtf8("Error while saving data"),trUtf8("The perspective data could not be retrieved."));
//  else {
//    QString outputPath = QFileDialog::getSaveFileName(this,trUtf8("Save project"),QDir::homePath(),trUtf8("Tulip project (*.tlpx)"));

//    if (!outputPath.isEmpty()) {
//      project->write(outputPath);
//    }
//  }

//  delete project;
//  _ui->saveButton->setText(trUtf8("Data saved"));
//  _ui->saveButton->setEnabled(false);
//}

void TulipPerspectiveCrashHandler::setEnvData(const QString &plateform, const QString &arch, const QString &compiler, const QString &version, const QString &stackTrace) {
  _ui->plateformValue->setText(plateform);
  _ui->archValue->setText(arch);
  _ui->compilerValue->setText(compiler);
  _ui->versionValue->setText(version);
  _ui->dumpEdit->setPlainText(stackTrace);
}

void TulipPerspectiveCrashHandler::setPerspectiveData(const PerspectiveProcessInfo &info) {
  _perspectiveInfo = info;
  _ui->perspectiveNameValue->setText(info.name);
  QString args;

  foreach(const QString &a,info.args.keys())
    args += "--" + a + "=" + info.args[a].toString() + " ";

  _ui->perspectiveArgumentsValue->setText(args);
}
