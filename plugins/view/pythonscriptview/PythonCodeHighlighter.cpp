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
#include "APIDataBase.h"

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
  qtApiFormat.setForeground(QColor(0, 110, 40));

  rule.pattern = QRegExp("def [A-Za-z0-9_]+(?=\\()");
  rule.format = functionFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("class [A-Za-z]+");
  rule.format = classFormat;
  highlightingRules.append(rule);

  rule.pattern = QRegExp("tlp\\.[A-Za-z0-9_.]+");
  rule.format = tlpApiFormat;
  highlightingRules.append(rule);

  keywordFormat.setForeground(Qt::darkBlue);
  keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;
  int i = 0;

  while (pythonKeywords[i]) {
    keywordPatterns << "\\b" + QString(pythonKeywords[i++]) + "\\b";
  }


  QStringList specialCharsPatterns;
  specialCharsPatterns << "\\+" << "-" << "=" << "\\(" << "\\)" << "\\[" << "\\]" << "," << "!" << "\\*" << "/"
                       << "\\{" << "\\}" << ":" << "\\." << ">" << "<" << "%" << "&" << "\\^" << "\\|";


  std::string builtinModName = "__builtin__";

  if (atof(PythonInterpreter::getInstance()->getPythonVersion().c_str()) >= 3.0) {
    builtinModName = "builtins";
  }

  if (PythonInterpreter::getInstance()->runString(std::string("import ") + builtinModName)) {
    std::vector<std::string> builtinDictContent = PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);
    QStringList builtinPatterns;

    for (size_t i = 0 ; i < builtinDictContent.size() ; ++i) {
      builtinPatterns << "\\b" + QString(builtinDictContent[i].c_str()) + "\\b";
    }

    builtinPatterns << "\\bself\\b";

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
    int nbQuotes = 0;

    for (int j = index - 1 ; j > 0 ; --j) {
      if (text[j] == '\'') {
        ++nbQuotes;
      }
    }

    int nbDblQuotes = 0;

    for (int j = index - 1 ; j > 0 ; --j) {
      if (text[j] == '"') {
        ++nbDblQuotes;
      }
    }

    if (nbQuotes%2 == 0 && nbDblQuotes%2 == 0) {
      int length = commentRegexp.matchedLength();
      setFormat(index, length, commentFormat);
      comments = true;
    }
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
    if (text[i] == '"' && (i == 0 || text[i-1] != '\\')) {
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
    if (text[i] == '\'' && (i == 0 || text[i-1] != '\\')) {
      if (quoteStartPos == -1) {
        quoteStartPos = i;
      }
      else {
        setFormat(quoteStartPos, i-quoteStartPos+1, quotationFormat);
        quoteStartPos = -1;
      }
    }
  }

  QRegExp qtApiRegexp("\\bQ[A-Za-z_.]+\\b");
  index = qtApiRegexp.indexIn(text);

  while (index >= 0) {
    int length = qtApiRegexp.matchedLength();
    QString expr = text.mid(index, length);

    if (APIDataBase::getInstance()->typeExists(expr) || APIDataBase::getInstance()->getFullTypeName(expr) != "") {
      setFormat(index, length, qtApiFormat);
    }
    else if (expr.indexOf(".") != -1) {
      QString type = expr.mid(0, expr.lastIndexOf("."));

      if (APIDataBase::getInstance()->getFullTypeName(type) != "")
        type = APIDataBase::getInstance()->getFullTypeName(type);

      QString entry = expr.mid(expr.lastIndexOf(".")+1);

      if (APIDataBase::getInstance()->dictEntryExists(type, entry)) {
        setFormat(index, length, qtApiFormat);
      }
    }

    index = qtApiRegexp.indexIn(text, index + length);
  }

}
