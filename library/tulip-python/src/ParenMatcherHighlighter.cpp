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

#include <algorithm>

#include "tulip/ParenMatcherHighlighter.h"

ParenInfoTextBlockData::ParenInfoTextBlockData() {}

QVector<ParenInfo> ParenInfoTextBlockData::parens() {
  return _parenInfos;
}

void ParenInfoTextBlockData::insert(const ParenInfo &parenInfo) {
  _parenInfos.append(parenInfo);
}

void ParenInfoTextBlockData::sortParenInfos() {
  std::sort(_parenInfos.begin(), _parenInfos.end());
}

ParenMatcherHighlighter::ParenMatcherHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
  _leftParensToMatch.append('(');
  _leftParensToMatch.append('[');
  _leftParensToMatch.append('{');
  _rightParensToMatch.append(')');
  _rightParensToMatch.append(']');
  _rightParensToMatch.append('}');
}

void ParenMatcherHighlighter::highlightBlock(const QString &text) {
  ParenInfoTextBlockData *data = new ParenInfoTextBlockData;

  QString modifiedText = text;
  QRegExp dblQuotesRegexp("\"[^\"]*\"");
  QRegExp simpleQuotesRegexp("'[^']*'");

  int pos = dblQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos ; i < pos + dblQuotesRegexp.matchedLength() ; ++i) {
      modifiedText[i] = ' ';
    }

    pos = dblQuotesRegexp.indexIn(modifiedText, pos + dblQuotesRegexp.matchedLength());
  }

  pos = simpleQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos ; i < pos + simpleQuotesRegexp.matchedLength() ; ++i) {
      modifiedText[i] = ' ';
    }

    pos = simpleQuotesRegexp.indexIn(modifiedText, pos + simpleQuotesRegexp.matchedLength());
  }

  for (int i = 0 ; i < _leftParensToMatch.size() ; ++i) {
    int leftPos = modifiedText.indexOf(_leftParensToMatch.at(i));

    while (leftPos != -1) {
      ParenInfo info;
      info.character = _leftParensToMatch.at(i);
      info.position = currentBlock().position() + leftPos;
      data->insert(info);
      leftPos = modifiedText.indexOf(_leftParensToMatch.at(i), leftPos+1);
    }
  }

  for (int i = 0 ; i < _rightParensToMatch.size() ; ++i) {
    int rightPos = modifiedText.indexOf(_rightParensToMatch.at(i));

    while (rightPos != -1) {
      ParenInfo info;
      info.character = _rightParensToMatch.at(i);
      info.position = currentBlock().position() + rightPos;
      data->insert(info);
      rightPos = modifiedText.indexOf(_rightParensToMatch.at(i), rightPos+1);
    }
  }

  data->sortParenInfos();
  setCurrentBlockUserData(data);
}
