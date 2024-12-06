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
#include <vector>
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif
#include <sip.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <ogdf/basic/internal/config.h>
#include <zipconf.h>
#include <tulip/AboutTulipPage.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipRelease.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/PythonIDEInterface.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/YajlFacade.h>
#include <tulip/Delaunay.h>

#include "ui_AboutTulipPage.h"

#include <QFile>
#include <QUrl>
#include <QOpenGLContext>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>

static const QString RSS_URL = "https://sourceforge.net/p/auber/news/feed.rss";
static const unsigned RSS_LIMIT = 3;

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
      ": <a href=\"https://ogdf.net/\" style=\"color: " HTML_LINK_COLOR ";\">ogdf.net</a> </li>" +
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
    in.setAutoDetectUnicode(true);
    _ui->authorsTextEdit->setText(in.readAll());
  }

  if (licenseFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&licenseFile);
    in.setAutoDetectUnicode(true);
    _ui->licenseTextEdit->setText(in.readAll());
  }

  // relook some html links
  std::vector<QLabel *> labels{_ui->tutorialsLabel, _ui->forumsLabel, _ui->screenshotsLabel,
                               _ui->bugsLabel, _ui->websiteLabel};
  for (auto label : labels) {
    auto txt = label->text();
    auto pos = txt.indexOf(" href=");
    txt.insert(pos, " style=\"color:" HTML_LINK_COLOR "\"");
    label->setText(txt);
  }
}

AboutTulipPage::~AboutTulipPage() {
  delete _ui;
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
      if (xmlReader.name() == QString("item")) {
        QString title, description;

        ++i;
        _ui->rssError->setVisible(false);
        _ui->rssScroll->setVisible(true);
        QXmlStreamReader::TokenType p(xmlReader.readNext());

        while (xmlReader.name() != QString("item") && p != QXmlStreamReader::EndElement) {
          xmlReader.readNextStartElement();

          if (xmlReader.name() == QString("title"))
            title = xmlReader.readElementText();

          if (xmlReader.name() == QString("description"))
            description = xmlReader.readElementText();
        }

        // relook html link
        auto pos = description.indexOf(" href=");
        if (pos != -1)
          description.insert(pos, " style=\"color:" HTML_LINK_COLOR "\"");
        QString text("<p><span style=\"color:#626262; font-size:large;\">");
        text += title + "</span></p><p><span>" + description + "</span></p>";
        QLabel *label = new QLabel(text, nullptr);
        label->setOpenExternalLinks(true);
        label->setMinimumWidth(1);
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        rssLayout->addWidget(label);
      }
    }

    if (xmlReader.hasError()) {
      _ui->rssError->setVisible(true);
      _ui->rssScroll->setVisible(false);
    }
  }
}
