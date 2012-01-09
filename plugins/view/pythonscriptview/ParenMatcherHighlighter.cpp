/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "ParenMatcherHighlighter.h"

ParenInfoTextBlockData::ParenInfoTextBlockData() {}

QVector<ParenInfo> ParenInfoTextBlockData::parens() {
  return parenInfos;
}

void ParenInfoTextBlockData::insert(const ParenInfo &parenInfo) {
  parenInfos.append(parenInfo);
}

void ParenInfoTextBlockData::sortParenInfos() {
  std::sort(parenInfos.begin(), parenInfos.end());
}

ParenMatcherHighlighter::ParenMatcherHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
  leftParensToMatch.append('(');
  leftParensToMatch.append('[');
  leftParensToMatch.append('{');
  rightParensToMatch.append(')');
  rightParensToMatch.append(']');
  rightParensToMatch.append('}');
}

void ParenMatcherHighlighter::highlightBlock(const QString &text) {
  ParenInfoTextBlockData *data = new ParenInfoTextBlockData;

  for (int i = 0 ; i < leftParensToMatch.size() ; ++i) {
    int leftPos = text.indexOf(leftParensToMatch.at(i));

    while (leftPos != -1) {
      ParenInfo info;
      info.character = leftParensToMatch.at(i);
      info.position = currentBlock().position() + leftPos;
      data->insert(info);
      leftPos = text.indexOf(leftParensToMatch.at(i), leftPos+1);
    }
  }

  for (int i = 0 ; i < rightParensToMatch.size() ; ++i) {
    int rightPos = text.indexOf(rightParensToMatch.at(i));

    while (rightPos != -1) {
      ParenInfo info;
      info.character = rightParensToMatch.at(i);
      info.position = currentBlock().position() + rightPos;
      data->insert(info);
      rightPos = text.indexOf(rightParensToMatch.at(i), rightPos+1);
    }
  }

  data->sortParenInfos();
  setCurrentBlockUserData(data);
}
