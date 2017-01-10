/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "ui_AboutTulipPage.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

using namespace tlp;

AboutTulipPage::AboutTulipPage(QWidget *parent) : QWidget(parent), _ui(new Ui::AboutTulipPageData()) {
  _ui->setupUi(this);

  QString title("Tulip ");
  title += TULIP_VERSION;
  _ui->logolabel->setPixmap(QPixmap(tlpStringToQString(TulipBitmapDir+"/logo.bmp")));
  _ui->TulipLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:600;\">"+title+"</span></p></body></html>");

  GlMainWidget::getFirstQGLWidget()->makeCurrent();
  QString tulipDependenciesInfo = "<p style=\"font-size:12pt\">"
                                  "This open source software is powered by:"
                                  "<ul>"
                                  "  <li> <b> Qt </b> " + tlpStringToQString(qVersion()) + ": <a href=\"https://wwww.qt.io\">https://wwww.qt.io</a></li>"
                                  "  <li> <b> OpenGL </b> " + QString::number(OpenGlConfigManager::getInst().getOpenGLVersion()) + " (from vendor " + tlpStringToQString(OpenGlConfigManager::getInst().getOpenGLVendor()) + "): <a href=\"https://wwww.opengl.org\">https://wwww.opengl.org</a> </li>"
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
                                  "  <li> <b> Python </b> " + TLP_PYTHON + ": <a href=\"https://wwww.python.org\">https://wwww.python.org</a> </li>"
#endif
                                  "</ul>"
                                  "</p>"
                                  ;
  GlMainWidget::getFirstQGLWidget()->doneCurrent();

  _ui->dependenciesInfo->setText(tulipDependenciesInfo);

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
