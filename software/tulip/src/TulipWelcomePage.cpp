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

#include "TulipWelcomePage.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlSimpleReader>

#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>

#include "PerspectiveItemWidget.h"
#include "RssParser.h"
#include "ui_TulipWelcomePage.h"

static const QString RSS_URL = "http://tulip.labri.fr/TulipDrupal/?q=newsFeed.xml";
static const int RSS_LIMIT = 3;

using namespace tlp;

TulipWelcomePage::TulipWelcomePage(QWidget *parent): QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);

  //Finalize Ui
  connect(_ui->websiteLabel,SIGNAL(linkActivated(QString)),this,SLOT(openLink(QString)));
  _ui->openProjectButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
  connect(_ui->openProjectButton,SIGNAL(clicked()),this,SIGNAL(openProject()));

  // Fetch RSS
  _ui->rssScroll->setVisible(false);
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(rssReply(QNetworkReply*)));
  manager->get(QNetworkRequest(QUrl(RSS_URL)));

  // Recent documents list
  TulipSettings::instance().checkRecentDocuments();
  QList<QString> recentDocs = TulipSettings::instance().recentDocuments();

  if (recentDocs.size() > 0) {
    QString txt;

    for (QList<QString>::iterator it = recentDocs.begin(); it != recentDocs.end(); ++it)
      txt += trUtf8(("<p><span><img src=\":/tulip/gui/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
                     *it + "\">" + *it + "</a>" +
                     "</span></p><p/>").toUtf8().data());

    _ui->recentDocumentsLabel->setText(txt);
  }


  std::list<std::string> perspectives = PluginLister::instance()->availablePlugins<tlp::Perspective>();

  for (std::list<std::string>::iterator it = perspectives.begin(); it != perspectives.end(); ++it) {
    _ui->perspectivesFrame->layout()->addWidget(new PerspectiveItemWidget(it->c_str()));
  }

  _ui->perspectivesFrame->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Expanding));
}

TulipWelcomePage::~TulipWelcomePage() {
}

void TulipWelcomePage::rssReply(QNetworkReply *reply) {
  sender()->deleteLater();
  QXmlSimpleReader rssReader;
  QXmlInputSource *rssSource = new QXmlInputSource(reply);
  RssParser *parser = new RssParser;
  rssReader.setContentHandler(parser);

  if (rssReader.parse(rssSource)) {
    _ui->rssError->setVisible(false);
    _ui->rssScroll->setVisible(true);

    QVBoxLayout *rssLayout = new QVBoxLayout;
    rssLayout->setContentsMargins(0,0,0,0);
    rssLayout->setSpacing(30);
    _ui->rssScroll->widget()->setLayout(rssLayout);

    QList<RssParser::RssItem> rssItems = parser->result();
    int i=0;

    for (QList<RssParser::RssItem>::iterator it = rssItems.begin(); it != rssItems.end(); ++it) {
      if (i++ >= RSS_LIMIT)
        break;

      QString text = "<p><span style=\"color:#626262; font-size:large;\">";
      text += it->title;
      text += "</span></p><p><span>";
      text += it->description;
      text += "</span></p></body></html>";
      QLabel *label = new QLabel(text,0);
      label->setMinimumWidth(1);
      label->setWordWrap(true);
      label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
      connect(label,SIGNAL(linkActivated(QString)),this,SLOT(openLink(QString)));
      rssLayout->addWidget(label);
    }
  }

  delete rssSource;
  delete parser;
}

void TulipWelcomePage::openLink(const QString &link) {
  QDesktopServices::openUrl(link);
}

void TulipWelcomePage::recentFileLinkActivated(const QString& link) {
  if (!QFileInfo(link).exists())
    QMessageBox::critical(this,trUtf8("Error"),trUtf8("Selected recent project does not exist anymore"));
  else
    emit openFile(link);
}
