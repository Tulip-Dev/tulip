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

#ifndef RSSPARSER_H
#define RSSPARSER_H

#include <QXmlDefaultHandler>

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
