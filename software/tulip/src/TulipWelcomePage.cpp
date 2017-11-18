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

#include "TulipWelcomePage.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>

#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>

#include "PerspectiveItemWidget.h"
#include "ui_TulipWelcomePage.h"

static const QString RSS_URL = "http://tulip.labri.fr/TulipDrupal/?q=newsFeed.xml";
static const unsigned RSS_LIMIT = 3;

using namespace tlp;

TulipWelcomePage::TulipWelcomePage(QWidget *parent)
    : QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);

  // Finalize Ui
  connect(_ui->websiteLabel, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));
  _ui->openProjectButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
  connect(_ui->openProjectButton, SIGNAL(clicked()), this, SIGNAL(openProject()));

  // Fetch RSS
  _ui->rssScroll->setVisible(false);
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(rssReply(QNetworkReply *)));
  manager->get(QNetworkRequest(QUrl(RSS_URL)));

  // Recent documents list
  TulipSettings::instance().checkRecentDocuments();
  QStringList recentDocs = TulipSettings::instance().recentDocuments();

  if (!recentDocs.empty()) {
    QString txt;

    foreach (const QString &txt2, recentDocs)
      txt += "<p><span><img src=\":/tulip/gui/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
             txt2 + "\">" + txt2 + "</a>" + "</span></p><p/>";

    _ui->recentDocumentsLabel->setText(txt);
  }

  std::list<std::string> perspectives =
      PluginLister::instance()->availablePlugins<tlp::Perspective>();

  for (std::list<std::string>::iterator it = perspectives.begin(); it != perspectives.end(); ++it) {
    _ui->perspectivesFrame->layout()->addWidget(new PerspectiveItemWidget(it->c_str()));
  }

  _ui->perspectivesFrame->layout()->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
}

TulipWelcomePage::~TulipWelcomePage() {
  delete _ui;
}

void TulipWelcomePage::rssReply(QNetworkReply *reply) {
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
        QLabel *label = new QLabel(text, 0);
        label->setMinimumWidth(1);
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(label, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));
        rssLayout->addWidget(label);
      }
    }

    if (xmlReader.hasError()) {
      _ui->rssError->setVisible(true);
      _ui->rssScroll->setVisible(false);
    }
  }
}

void TulipWelcomePage::openLink(const QString &link) {
  QDesktopServices::openUrl(link);
}

void TulipWelcomePage::recentFileLinkActivated(const QString &link) {
  if (!QFileInfo(link).exists())
    QMessageBox::critical(this, trUtf8("Error"),
                          trUtf8("Selected recent project does not exist anymore"));
  else
    emit openFile(link);
}
