#include "TulipWelcomePage.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlSimpleReader>

#include <tulip/TulipSettings.h>

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
    QString txt = trUtf8("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; } a {color: black; }</style></head><body>");

    for (QList<QString>::iterator it = recentDocs.begin(); it != recentDocs.end(); ++it)
      txt += trUtf8(("<p style=\" -qt-block-indent:0; text-indent:0px;\"><span><img src=\":/tulip/app/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
                     *it + "\">" + *it + "</a>" +
                     "</span></p><p/>").toStdString().c_str());

    txt += trUtf8("</body></html>");
    _ui->recentDocumentsLabel->setText(txt);
  }
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
      QString text = "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\"color:#626262; font-size:large;\">";
      text += it->title;
      text += "</span></p><p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:medium;\"> </p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\"color:#000000;\">";
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
