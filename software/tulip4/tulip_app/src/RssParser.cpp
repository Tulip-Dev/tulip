#include "RssParser.h"

#include <iostream>
using namespace std;

RssParser::RssParser(): QXmlDefaultHandler() {
  result.clear();
}

bool RssParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts) {
  if (localName == "item")
    result.push_back(RssItem());
  _currentElement = localName;
  return true;
}

bool RssParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName) {
  _currentElement = "";
  return true;
}

bool RssParser::characters(const QString &str) {
  if (result.size() == 0)
    return true;

  fetchElement("title",str);
  fetchElement("link",str);
  fetchElement("description",str);
  fetchElement("pubDate",str);

  return true;
}

QList<RssParser::RssItem> RssParser::result() const {
  return result;
}

void RssParser::fetchElement(const QString &name, const QString &value) {
  if (name != _currentElement)
    return;

  RssItem item = result[result.size()-1];
  if (name == "title")
    item.title = value;
  else if (name == "link")
    item.link = value;
  else if (name == "description")
    item.description = value;
  else if (name == "pubDate")
    item.pubDate = value;

  result[result.size()-1] = item;
}
