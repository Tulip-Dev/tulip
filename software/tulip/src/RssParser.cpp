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

#include "RssParser.h"

#include <iostream>
using namespace std;

RssParser::RssParser(): QXmlDefaultHandler() {
  _result.clear();
}

bool RssParser::startElement(const QString &, const QString &localName, const QString &, const QXmlAttributes &) {
  if (localName == "item")
    _result.push_back(RssItem());

  _currentElement = localName;
  return true;
}

bool RssParser::endElement(const QString &, const QString &, const QString &) {
  _currentElement = "";
  return true;
}

bool RssParser::characters(const QString &str) {
  if (_result.size() == 0)
    return true;

  fetchElement("title",str);
  fetchElement("link",str);
  fetchElement("description",str);
  fetchElement("pubDate",str);

  return true;
}

QList<RssParser::RssItem> RssParser::result() const {
  return _result;
}

void RssParser::fetchElement(const QString &name, const QString &value) {
  if (name != _currentElement)
    return;

  RssItem item = _result[_result.size()-1];

  if (name == "title")
    item.title = value;
  else if (name == "link")
    item.link = value;
  else if (name == "description")
    item.description = value;
  else if (name == "pubDate")
    item.pubDate = value;

  _result[_result.size()-1] = item;
}
