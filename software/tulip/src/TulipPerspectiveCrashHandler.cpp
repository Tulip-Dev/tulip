#include "TulipPerspectiveCrashHandler.h"

#include "ui_TulipPerspectiveCrashHandler.h"

#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include "FormPost.h"

TulipPerspectiveCrashHandler::TulipPerspectiveCrashHandler(QWidget *parent)
  : QDialog(parent), _ui(new Ui::TulipPerspectiveCrashHandlerData), _isDetailedView(false) {
  _ui->setupUi(this);
  setDetailedView(false);
#ifndef USE_CRASH_HANDLING
  _ui->errorReportTitle->setVisible(false);
  _ui->sendReportButton->setVisible(false);
  _ui->detailsLink->setVisible(false);
  _ui->detailsIcon->setVisible(false);
  adjustHeight();
#else
  _poster = 0;
#endif
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
  if (_reportPath.isNull())
    return;

#ifdef USE_CRASH_HANDLING
  _poster = new FormPost;
  _poster->addField("platform",_ui->plateformValue->text());
  _poster->addField("compiler",_ui->compilerValue->text());
  _poster->addField("arch",_ui->archValue->text());
  _poster->addField("perspective_name",_ui->perspectiveNameValue->text());
  _poster->addField("tulip_version",_ui->versionValue->text());
  _poster->addFile("dump_file",_reportPath,"application/octet-stream");
  connect(_poster->postData("http://tulip.labri.fr/crash_report/send_report.php"),SIGNAL(finished()),this,SLOT(reportPosted()));
  _ui->sendReportButton->setText(trUtf8("Sending report"));
  _ui->sendReportButton->setEnabled(false);
#endif
}

void TulipPerspectiveCrashHandler::reportPosted() {
  _ui->sendReportButton->setText(trUtf8("Report sent"));
#ifdef USE_CRASH_HANDLING
  _ui->errorReportTitle->setText(trUtf8("<b>Report has been sent. Thank you for supporting Tulip !</b>"));
  sender()->deleteLater();
  _poster->deleteLater();
#endif
}

void TulipPerspectiveCrashHandler::saveData() {

}

#ifdef USE_CRASH_HANDLING
void TulipPerspectiveCrashHandler::setEnvData(const QString &plateform, const QString &arch, const QString &compiler, const QString &version, const QString &dumpFile, const QString &stackTrace) {
  _ui->plateformValue->setText(plateform);
  _ui->archValue->setText(arch);
  _ui->compilerValue->setText(compiler);
  _ui->versionValue->setText(version);
  _ui->dumpEdit->setPlainText(stackTrace);
  _reportPath=dumpFile;
}
#endif

void TulipPerspectiveCrashHandler::setPerspectiveData(PerspectiveProcessInfos infos) {
  _perspectiveInfos = infos;
  _ui->perspectiveNameValue->setText(infos.name);
  QString args;
  QString a;
  foreach(a,infos.args.keys())
  args += "--" + a + "=" + infos.args[a].toString() + " ";
  _ui->perspectiveArgumentsValue->setText(args);
}
