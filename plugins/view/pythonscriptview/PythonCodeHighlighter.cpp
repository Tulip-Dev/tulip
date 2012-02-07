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

#include "PythonCodeHighlighter.h"
#include "PythonInterpreter.h"

#include <QtGui/QTextDocument>



PythonCodeHighlighter::PythonCodeHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent), shellMode(false) {

  HighlightingRule rule;

  commentFormat.setForeground(Qt::darkGreen);
  functionFormat.setFontWeight(QFont::Bold);
  functionFormat.setForeground(Qt::darkCyan);
  tlpApiFormat.setForeground(QColor(128,128,0));
  classFormat.setFontWeight(QFont::Bold);
  classFormat.setForeground(Qt::blue);

  rule.pattern = QRegExp("def [A-Za-z0-9_]+(?=\\()");
  rule.format = functionFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("class [A-Za-z]+");
  rule.format = classFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("tlp\\.[A-Za-z0-9_]+(?=\\()");
  rule.format = tlpApiFormat;
  highlightingRules.append(rule);

  keywordFormat.setForeground(Qt::darkBlue);
  keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;
  keywordPatterns << "\\bdef\\b" << "\\bclass\\b" << "\\bfrom\\b"
                  << "\\bin\\b" << "\\band\\b" << "\\bor\\b" << "\\bnot\\b"
                  << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belif\\b"
                  << "\\bimport\\b" << "\\bTrue\\b" << "\\bFalse\\b" << "\\bpass\\b"
                  << "\\belse\\b" << "\\bNone\\b" << "\\bprint\\b" << "\\bglobal\\b"
                  << "\\breturn\\b" << "\\bbreak\\b" << "\\bcontinue\\b" << "\\bas\\b"
                  << "\\blambda\\b" << "\\bdel\\b";

  QStringList specialCharsPatterns;
  specialCharsPatterns << "\\+" << "-" << "=" << "\\(" << "\\)" << "\\[" << "\\]" << "," << "!" << "\\*" << "/"
                       << "\\{" << "\\}" << ":" << "\\." << ">" << "<" << "%" << "&" << "\\^" << "\\|";


  if (PythonInterpreter::getInstance()->runString("import __builtin__")) {
    std::vector<std::string> builtinDictContent = PythonInterpreter::getInstance()->getObjectDictEntries("__builtin__");
    QStringList builtinPatterns;

    for (size_t i = 0 ; i < builtinDictContent.size() ; ++i) {
      builtinPatterns << "\\b" + QString(builtinDictContent[i].c_str()) + "\\b";
    }

    QTextCharFormat builtinFormat;
    builtinFormat.setForeground(QColor(0,87,187));
    foreach (const QString &pattern, builtinPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = builtinFormat;
      highlightingRules.append(rule);
    }
  }

  foreach (const QString &pattern, keywordPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
  }

  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);

  foreach (const QString &pattern, specialCharsPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = format;
    highlightingRules.append(rule);
  }

  numberFormat.setForeground(Qt::darkCyan);
  rule.pattern = QRegExp("\\b[0-9]+[.]*[O-9]*\\b");
  rule.format = numberFormat;
  highlightingRules.append(rule);

  quotationFormat.setForeground(Qt::darkMagenta);

}

void PythonCodeHighlighter::highlightBlock(const QString &text) {

  if (shellMode) {
    if (currentBlock().blockNumber() > 2 && !text.startsWith(">>>") && !text.startsWith("...")) {
      return;
    }
  }

  QRegExp commentRegexp("#[^\n]*");
  bool comments = false;
  int index = commentRegexp.indexIn(text);

  if (index >= 0) {
    int length = commentRegexp.matchedLength();
    setFormat(index, length, commentFormat);
    comments = true;
  }

  if (comments)
    return;

  foreach (const HighlightingRule &rule, highlightingRules) {
    QRegExp expression(rule.pattern);
    int index = expression.indexIn(text);

    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = expression.indexIn(text, index + length);
    }
  }

  int quoteStartPos = -1;

  for (int i = 0 ; i < text.length() ; ++i) {
    if (text[i] == '"') {
      if (quoteStartPos == -1) {
        quoteStartPos = i;
      }
      else {
        setFormat(quoteStartPos, i-quoteStartPos+1, quotationFormat);
        quoteStartPos = -1;
      }
    }
  }

  quoteStartPos = -1;

  for (int i = 0 ; i < text.length() ; ++i) {
    if (text[i] == '\'') {
      if (quoteStartPos == -1) {
        quoteStartPos = i;
      }
      else {
        setFormat(quoteStartPos, i-quoteStartPos+1, quotationFormat);
        quoteStartPos = -1;
      }
    }
  }


}
