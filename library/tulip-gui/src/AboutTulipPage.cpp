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
#include <ogdf/basic/internal/config.h>
#include <tulip/AboutTulipPage.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TulipRelease.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/PythonVersionChecker.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>

#include "ui_AboutTulipPage.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QOpenGLContext>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>

namespace tlp {
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
extern QString getSipVersion();
#endif
extern QString getTulipGitRevision();
} // namespace tlp

static const QString TulipRepoUrl = "https://github.com/Tulip-Dev/tulip";
static const QString RSS_URL = "https://tulip.labri.fr/TulipDrupal/?q=newsFeed.xml";
static const unsigned RSS_LIMIT = 3;

using namespace tlp;

AboutTulipPage::AboutTulipPage(QWidget *parent)
    : QWidget(parent), _ui(new Ui::AboutTulipPageWidget()) {
  _ui->setupUi(this);

  QString title("Tulip ");
  title += TULIP_VERSION;
  QString git_rev(getTulipGitRevision());

  if (!git_rev.isEmpty())
    title += "<br/>(Git commit: <a href=\"" + TulipRepoUrl + "/commit/" + git_rev + "\">" +
             "<span style=\"color: #0d47f1;\">" + git_rev.mid(0, 7) + "</span></a>)";

  _ui->logolabel->setPixmap(QPixmap(tlpStringToQString(TulipBitmapDir + "/welcomelogo.bmp")));
  _ui->TulipLabel->setText(
      "<html>"
      "  <head/>"
      "  <body>"
      "    <p align=\"center\"><span style=\" font-size:18pt; font-weight:600;\">" +
      title + "</span></p>" +
      (!git_rev.isEmpty() ? (QString("    <p align=\"center\"><a href=\"") + TulipRepoUrl + "\">" +
                             "<span style=\"color: #0d47f1;\">" + TulipRepoUrl + "</span></a></p>")
                          : QString()) +
      "  </body>"
      "</html>");

  bool openGL_OK = GlOffscreenRenderer::getInstance()->getOpenGLContext()->isValid();

  if (openGL_OK)
    GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();

  /*  QString tulipDependenciesInfo =
      "<p style=\"font-size:12pt\">"
      "This open source software is powered by:"
      "<ul>"
      "<li><a href=\"https://www.qt.io\"><span style=\"color: #0d47f1;\">"
      "<b>Qt</b></span></a> " + tlpStringToQString(qVersion()) +
      "</li>"
      "<li><a href=\"https://www.opengl.org\"><span style=\"color: #0d47f1;\">"
      "<b>OpenGL</b></span></a> " +
      (openGL_OK ? QString::number(OpenGlConfigManager::getOpenGLVersion()) : QString("?.?")) +
      " (from vendor " +
      (openGL_OK ? tlpStringToQString(OpenGlConfigManager::getOpenGLVendor())
                 : QString("unknown")) +
      ") </li>"
      "<li><a href=\"http://ogdf.net/\"><span style=\"color: #0d47f1;\">"
      "<b>OGDF</b></span></a> v" + OGDF_VERSION + "</li>"
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
      "<li><a href=\"https://www.python.org\"><span style=\"color: #0d47f1;\">"
      "<b> Python </b></span></a> " + PythonVersionChecker::compiledVersion() +
      "</li>"
      "<li> <a href=\"https://www.riverbankcomputing.com/software/sip\"><span style=\"color:
#0d47f1;\">"
      "<b>SIP</b></span></a> " + getSipVersion() + "</li>"
#endif
      "</ul>"
      "</p>";*/
  QString tulipDependenciesInfo =
      "<p style=\"font-size:12pt\">"
      "This open source software is powered by:"
      "<ul>"
      "  <li> <b> Qt </b> " +
      tlpStringToQString(qVersion()) +
      ": <a href=\"https://www.qt.io\"><span style=\"color: "
      "#0d47f1;\">www.qt.io</span></a></li>"
      "  <li> <b> OpenGL </b> " +
      (openGL_OK ? QString::number(OpenGlConfigManager::getOpenGLVersion()) : QString("?.?")) +
      ": <a href=\"https://www.opengl.org\"><span style=\"color: "
      "#0d47f1;\">www.opengl.org</span></a> </li>"
      "<li><b>OGDF</b> v" +
      OGDF_VERSION +
      ": <a "
      "href=\"http://ogdf.net/\"><span style=\"color: #0d47f1;\">ogdf.net</span></a> </li>"
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
      "  <li> <b> Python </b> " +
      PythonVersionChecker::compiledVersion() +
      ": <a href=\"https://www.python.org\"><span style=\"color: "
      "#0d47f1;\">www.python.org</span></a> </li>"
      "  <li> <b> SIP </b> " +
      getSipVersion() +
      ": <a "
      "href=\"https://www.riverbankcomputing.com/software/sip\"><span style=\"color: "
      "#0d47f1;\">www.riverbankcomputing.com/software/sip</span></a></li>"
#endif
      "<li><b>Font Awesome</b> " +
      TulipFontAwesome::getVersion().c_str() +
      ": <a href=\"http://fontawesome.com\"><span style=\"color: #0d47f1;\">"
      "fontawesome.com</span></a></li>"
      "<li><b>Material Design Icons</b> " +
      TulipMaterialDesignIcons::getVersion().c_str() +
      ": <a href=\"https://materialdesignicons.com\"><span style=\"color: #0d47f1;\">"
      "materialdesignicons.com</span></a></li>"
      "<li><b>Color Brewer</b> "
      ": <a href=\"http://colorbrewer2.org\"><span style=\"color: #0d47f1;\">"
      "colorbrewer2.org</span></a></li>"
      "</ul>"
      "</p>";

  if (openGL_OK)
    GlOffscreenRenderer::getInstance()->doneOpenGLContextCurrent();

  _ui->dependenciesInfo->setText(tulipDependenciesInfo);
  connect(_ui->aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
  connect(_ui->dependenciesInfo, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));
  connect(_ui->TulipLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));
  connect(_ui->websiteLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));

  // Fetch RSS
  _ui->rssScroll->setVisible(false);
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(rssReply(QNetworkReply *)));
  manager->get(QNetworkRequest(QUrl(RSS_URL)));

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

void AboutTulipPage::rssReply(QNetworkReply *reply) {
  sender()->deleteLater();
  QXmlStreamReader xmlReader(reply);
  unsigned i = 0;
  QVBoxLayout *rssLayout = new QVBoxLayout;
  rssLayout->setContentsMargins(0, 0, 0, 0);
  rssLayout->setSpacing(30);
  _ui->rssScroll->widget()->setLayout(rssLayout);

  while (!xmlReader.atEnd() && i < RSS_LIMIT) {
    if (xmlReader.readNextStartElement()) {
      QString title, description;

      if (xmlReader.name() == "item") {
        ++i;
        _ui->rssError->setVisible(false);
        _ui->rssScroll->setVisible(true);
        QXmlStreamReader::TokenType p(xmlReader.readNext());

        while (xmlReader.name() != "item" && p != QXmlStreamReader::EndElement) {
          xmlReader.readNextStartElement();

          if (xmlReader.name() == "title")
            title = xmlReader.readElementText();

          if (xmlReader.name() == "description")
            description = xmlReader.readElementText();
        }

        QString text("<p><span style=\"color:#626262; font-size:large;\">");
        text += title + "</span></p><p><span>" + description + "</span></p>";
        QLabel *label = new QLabel(text, nullptr);
        label->setMinimumWidth(1);
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(label, SIGNAL(linkActivated(const QString &)), this,
                SLOT(openUrlInBrowser(const QString &)));
        rssLayout->addWidget(label);
      }
    }

    if (xmlReader.hasError()) {
      _ui->rssError->setVisible(true);
      _ui->rssScroll->setVisible(false);
    }
  }
}
