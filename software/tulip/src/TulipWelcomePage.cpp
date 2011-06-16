#include "TulipWelcomePage.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlSimpleReader>

#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>

#include "PerspectiveItemWidget.h"
#include "RssParser.h"
#include "ui_TulipWelcomePage.h"

static const QString RSS_URL = "http://tulip.labri.fr/TulipDrupal/?q=newsFeed.xml";
static const int RSS_LIMIT = 3;

TulipWelcomePage::TulipWelcomePage(QWidget *parent): QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);

  //Finalize Ui
  connect(_ui->websiteLabel,SIGNAL(linkActivated(QString)),this,SLOT(openLink(QString)));
  _ui->openProjectButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));

  // Fetch RSS
  _ui->rssScroll->setVisible(false);
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(rssReply(QNetworkReply*)));
  manager->get(QNetworkRequest(QUrl(RSS_URL)));

  // Recent documents list
  QList<QString> recentDocs = TulipSettings::instance().recentDocuments();
  if (recentDocs.size() > 0) {
    QString txt;

    for (QList<QString>::iterator it = recentDocs.begin(); it != recentDocs.end(); ++it)
      txt += trUtf8(("<p><span><img src=\":/tulip/app/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
                     *it + "\">" + *it + "</a>" +
                     "</span></p><p/>").toStdString().c_str());

    _ui->recentDocumentsLabel->setText(txt);
  }

  // Perspectives list
  QVBoxLayout *perspectivesLayout = new QVBoxLayout(_ui->perspectivesFrame);
  perspectivesLayout->setContentsMargins(6,6,6,6);
  tlp::PluginManager localPluginManager; // This manager is used to list locally loaded plugins. We won't need to rebuild the list until tulip restarts
  QList<tlp::PluginInformations *> localPlugins = localPluginManager.pluginsList(tlp::PluginManager::Local);
  tlp::PluginInformations *info;
  foreach(info,localPlugins) {
    if (info->type() != "Perspective")
      continue;
    PerspectiveItemWidget *item = new PerspectiveItemWidget(info);
    perspectivesLayout->addWidget(item);
    connect(item,SIGNAL(selected()),this,SLOT(perspectiveSelected()));
  }
  perspectivesLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Maximum,QSizePolicy::Expanding));
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

void TulipWelcomePage::perspectiveSelected() {
  PerspectiveItemWidget *item = static_cast<PerspectiveItemWidget *>(sender());
  emit openPerspective(item->perspectiveId());
}
