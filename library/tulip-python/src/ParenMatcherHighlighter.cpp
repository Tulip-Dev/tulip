/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <QRegularExpression>

#include "tulip/ParenMatcherHighlighter.h"

ParenInfoTextBlockData::ParenInfoTextBlockData() {}

QVector<ParenInfo> ParenInfoTextBlockData::parens() {
  return _parenInfo;
}

void ParenInfoTextBlockData::insert(const ParenInfo &parenInfo) {
  _parenInfo.append(parenInfo);
}

void ParenInfoTextBlockData::sortParenInfo() {
  std::sort(_parenInfo.begin(), _parenInfo.end());
}

void ParenMatcherHighlighter::highlightBlock(const QString &text) {
  ParenInfoTextBlockData *data = new ParenInfoTextBlockData;

  QString modifiedText = text;
  QRegularExpression dblQuotesRegexp("\"[^\"]*\"");
  QRegularExpression simpleQuotesRegexp("'[^']*'");
  QRegularExpressionMatch match;
  auto ml = modifiedText.length();
  int pos = modifiedText.indexOf(dblQuotesRegexp, 0, &match);

  while (pos != -1 && pos < ml) {
    auto l = std::min(pos + match.capturedLength(), ml);
    for (int i = pos; i < l; ++i) {
      modifiedText[i] = ' ';
    }

    pos = modifiedText.indexOf(dblQuotesRegexp, pos + match.capturedLength());
  }

  pos = modifiedText.indexOf(simpleQuotesRegexp, 0, &match);
  while (pos != -1 && pos < modifiedText.length()) {
    auto l = std::min(pos + match.capturedLength(), ml);
    for (int i = pos; i < l; ++i) {
      modifiedText[i] = ' ';
    }

    pos = modifiedText.indexOf(simpleQuotesRegexp, pos + match.capturedLength());
  }

  for (char paren : {'(', '[', '{', ')', ']', '}'}) {
    pos = modifiedText.indexOf(paren);

    while (pos != -1) {
      ParenInfo info;
      info.character = paren;
      info.position = currentBlock().position() + pos;
      data->insert(info);
      pos = modifiedText.indexOf(paren, pos + 1);
    }
  }
  data->sortParenInfo();
  setCurrentBlockUserData(data);
}
