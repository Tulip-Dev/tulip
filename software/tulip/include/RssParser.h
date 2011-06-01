#ifndef RSSPARSER_H
#define RSSPARSER_H

#include <QtXml/QXmlDefaultHandler>

/**
  @brief a Basic XML/RSS parser meant to parse RSS flux coming from the Tulip website.
  The RSS parser store items titles, links, publication dates and descriptions so that the welcome screen can display them.
  */
class RssParser: public QXmlDefaultHandler {
public:
  struct RssItem {
    QString title;
    QString link;
    QString description;
    QString pubDate;
  };

  RssParser();

  virtual bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
  virtual bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
  virtual bool characters(const QString &str);

  QList<RssItem> result() const;

private:
  QString _currentElement;
  QList<RssItem> _result;

  void fetchElement(const QString &name, const QString &value);
};

#endif // RSSPARSER_H
