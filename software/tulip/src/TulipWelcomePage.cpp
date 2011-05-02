#include "TulipWelcomePage.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlSimpleReader>

#include "RssParser.h"

//FIXME remove me
#include <iostream>
#include <QtGui/QPushButton>
using namespace std;

static const QString RSS_URL = "http://tulip.labri.fr/TulipDrupal/?q=newsFeed.xml";
static const int RSS_LIMIT = 3;

TulipWelcomePage::TulipWelcomePage(QWidget *parent): QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);

  // Fetch RSS
  _ui->rssScroll->setVisible(false);
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(rssReply(QNetworkReply*)));
  manager->get(QNetworkRequest(QUrl(RSS_URL)));
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
      QString text = "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-size:8.25pt; font-weight:400; font-style:normal;\"><p style=\" -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#616161;\">. ";
      text += it->title;
      text += "</span></p><p/><p style=\" -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; color:#000000;\">";
      text += it->description;
      text += "</span></p></body></html>";
      QLabel *label = new QLabel(text,0);
      label->setMinimumWidth(1);
      label->setWordWrap(true);
      label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
      rssLayout->addWidget(label);
    }
  }
  delete rssSource;
  delete parser;
}
