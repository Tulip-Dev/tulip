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
#include <tulip/AboutTulipPage.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TulipRelease.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/PythonVersionChecker.h>

#include "ui_AboutTulipPage.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>

namespace tlp {
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
extern QString getSipVersion();
#endif
extern QString getTulipGitRevision();
}

const QString TulipRepoUrl = "https://github.com/Tulip-Dev/tulip";

using namespace tlp;

AboutTulipPage::AboutTulipPage(QWidget *parent)
    : QWidget(parent), _ui(new Ui::AboutTulipPageData()) {
  _ui->setupUi(this);

  QString title("Tulip ");
  title += TULIP_VERSION;
  QString git_rev(getTulipGitRevision());

  if (!git_rev.isEmpty())
    title += "<br/>(Git rev. " + git_rev.mid(0, 7) + ")";

  _ui->logolabel->setPixmap(QPixmap(tlpStringToQString(TulipBitmapDir + "/logo.bmp")));
  _ui->TulipLabel->setText(
      "<html>"
      "  <head/>"
      "  <body>"
      "    <p align=\"center\"><span style=\" font-size:24pt; font-weight:600;\">" +
      title + "</span></p>"
              "    <p align=\"center\"><a href=\"" +
      TulipRepoUrl + "\">" + TulipRepoUrl + "</a></p>"
                                            "  </body>"
                                            "</html>");

  bool openGL_OK = GlMainWidget::getFirstQGLWidget()->isValid();

  if (openGL_OK)
    GlMainWidget::getFirstQGLWidget()->makeCurrent();

  QString tulipDependenciesInfo =
      "<p style=\"font-size:12pt\">"
      "This open source software is powered by:"
      "<ul>"
      "  <li> <b> Qt </b> " +
      tlpStringToQString(qVersion()) + ": <a href=\"https://www.qt.io\">https://www.qt.io</a></li>"
                                       "  <li> <b> OpenGL </b> " +
      (openGL_OK ? QString::number(OpenGlConfigManager::getInst().getOpenGLVersion())
                 : QString("?.?")) +
      " (from vendor " +
      (openGL_OK ? tlpStringToQString(OpenGlConfigManager::getInst().getOpenGLVendor())
                 : QString("unknown")) +
      "): <a href=\"https://www.opengl.org\">https://www.opengl.org</a> </li>"
      "  <li> <b>OGDF</b> v2015.05 (Baobab) aka the Open Graph Drawing Framework : <a "
      "href=\"http://www.ogdf.net\">http://www.ogdf.net</a> </li>"
#ifdef TULIP_BUILD_PYTHON_COMPONENTS

      "  <li> <b> Python </b> " +
      PythonVersionChecker::compiledVersion() +
      ": <a href=\"https://www.python.org\">https://www.python.org</a> </li>"
      "  <li> <b> SIP </b> " +
      getSipVersion() + ": <a "
                        "href=\"https://www.riverbankcomputing.com/software/sip/\">https://"
                        "www.riverbankcomputing.com/software/sip</a> </li>"
#endif
                        "</ul>"
                        "</p>";

  if (openGL_OK)
    GlMainWidget::getFirstQGLWidget()->doneCurrent();

  _ui->dependenciesInfo->setText(tulipDependenciesInfo);
  connect(_ui->aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
  connect(_ui->dependenciesInfo, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));
  connect(_ui->TulipLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));

  QPixmap qp(QString((TulipBitmapDir + "/samplePictures/1221.png").c_str()));
  _ui->sample_1221->setPixmap(qp.scaled(230, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  qp = QPixmap(QString((TulipBitmapDir + "/samplePictures/1861.jpg").c_str()));
  _ui->sample_1861->setPixmap(qp.scaled(230, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  qp = QPixmap(QString((TulipBitmapDir + "/samplePictures/1531.png").c_str()));
  _ui->sample_1531->setPixmap(qp.scaled(230, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  QFile authorsFile(tlpStringToQString(TulipShareDir + "AUTHORS"));
  QFile licenseFile(tlpStringToQString(TulipShareDir + "COPYING.LESSER"));

  if (authorsFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&authorsFile);
    in.setCodec("UTF-8");
    _ui->authorsTextEdit->setText(in.readAll());
  }

  if (licenseFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&licenseFile);
    in.setCodec("UTF-8");
    _ui->licenseTextEdit->setText(in.readAll());
  }
}

AboutTulipPage::~AboutTulipPage() {
  delete _ui;
}

void AboutTulipPage::openUrlInBrowser(const QString &url) {
  QDesktopServices::openUrl(QUrl(url));
}
