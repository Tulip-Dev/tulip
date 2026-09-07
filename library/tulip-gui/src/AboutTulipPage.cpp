/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif
#include <sip.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <ogdf/basic/internal/config.h>
#include <tulip/AboutTulipPage.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipRelease.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/PythonIDEInterface.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>

#include "ui_AboutTulipPage.h"

#include <QFile>
#include <QOpenGLContext>

using namespace tlp;

AboutTulipPage::AboutTulipPage(QWidget *parent)
    : QWidget(parent), _ui(new Ui::AboutTulipPageWidget()) {
  _ui->setupUi(this);

  QString title("Tulip ");
  title += TULIP_VERSION;

  _ui->logolabel->setPixmap(QPixmap(tlpStringToQString(TulipBitmapDir + "/welcomelogo.bmp")));
  _ui->TulipLabel->setText(
      "<html>"
      "  <head/>"
      "  <body>"
      "    <p align=\"center\"><span style=\" font-size:18pt; font-weight:600;\">" +
      title + "</span></p>" +
      "  </body>"
      "</html>");

  bool openGL_OK = GlOffscreenRenderer::getInstance()->isValid();

  if (openGL_OK)
    GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();

  QString tulipDependenciesInfo =
      "<p style=\"font-size:12pt\">"
      "This open source software is powered by:"
      "<ul>"
      "  <li> <b> Qt </b> " +
      tlpStringToQString(qVersion()) +
      ": <a href=\"https://www.qt.io\" style=\"color:" HTML_LINK_COLOR ";\">www.qt.io</a></li>"
      "  <li> <b> OpenGL </b> " +
      (openGL_OK ? QString::number(OpenGlConfigManager::getOpenGLVersion()) : QString("?.?")) +
      ": <a href=\"https://www.opengl.org\" style=\"color:" HTML_LINK_COLOR
      ";\">www.opengl.org</a> </li>"
      "<li><b>OGDF</b> v" +
      OGDF_VERSION +
      ": <a href=\"https://github.com/ogdf/ogdf\" style=\"color: " HTML_LINK_COLOR
      ";\">github.com/ogdf/ogdf</a> </li>" +
      (PythonIDEInterface::exists()
           ? QString("  <li> <b> Python </b> ") + PythonIDEInterface::compiledVersion(true) +
                 ": <a href=\"https://www.python.org\" style=\"color:" HTML_LINK_COLOR
                 ";\">www.python.org</a> </li>"
                 "  <li> <b> SIP </b> " +
                 SIP_VERSION_STR +
                 ": <a href=\"https://github.com/Python-SIP/sip\"  style=\"color:" HTML_LINK_COLOR
                 ";\">github.com/Python-SIP/sip</a></li>"
           : "") +
      "<li><b>Font Awesome</b> " + TulipFontAwesome::getVersion().c_str() +
      ": <a href=\"https://fontawesome.com\" style=\"color:" HTML_LINK_COLOR
      ";\">fontawesome.com</a></li>"
      "<li><b>Material Design Icons</b> " +
      TulipMaterialDesignIcons::getVersion().c_str() +
      ": <a href=\"https://materialdesignicons.com\" style=\"color:" HTML_LINK_COLOR
      ";\">materialdesignicons.com</a></li>"
      "<li><b>Color Brewer</b> "
      ": <a href=\"https://colorbrewer2.org\" style=\"color:" HTML_LINK_COLOR
      ";\">colorbrewer2.org</a></li>"
      "</ul>"
      "</p>";

  if (openGL_OK)
    GlOffscreenRenderer::getInstance()->doneOpenGLContextCurrent();

  _ui->dependenciesInfo->setText(tulipDependenciesInfo);
  connect(_ui->aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));

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
    in.setAutoDetectUnicode(true);
    _ui->authorsTextEdit->setText(in.readAll());
  }

  if (licenseFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&licenseFile);
    in.setAutoDetectUnicode(true);
    _ui->licenseTextEdit->setText(in.readAll());
  }

  // relook some html links
  for (auto label : {_ui->tutorialsLabel, _ui->bugsLabel, _ui->websiteLabel}) {
    auto txt = label->text();
    auto pos = txt.indexOf(" href=");
    txt.insert(pos, " style=\"color:" HTML_LINK_COLOR "\"");
    label->setText(txt);
  }
}

AboutTulipPage::~AboutTulipPage() {
  delete _ui;
}
