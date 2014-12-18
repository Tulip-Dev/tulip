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

#include "tulip/PythonInterpreter.h"
#include "tulip/PythonCodeHighlighter.h"
#include "tulip/APIDataBase.h"

#include <QTextDocument>

using namespace tlp;

PythonCodeHighlighter::PythonCodeHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent), _shellMode(false) {

  HighlightingRule rule;

  _commentFormat.setForeground(Qt::darkGreen);
  _functionFormat.setFontWeight(QFont::Bold);
  _functionFormat.setForeground(Qt::darkCyan);
  _tlpApiFormat.setForeground(QColor(128,128,0));
  _classFormat.setFontWeight(QFont::Bold);
  _classFormat.setForeground(Qt::blue);
  _qtApiFormat.setForeground(QColor(0, 110, 40));

  rule.pattern = QRegExp("def [A-Za-z_][A-Za-z0-9_]+(?=\\()");
  rule.format = _functionFormat;
  _highlightingRules.append(rule);

  rule.pattern = QRegExp("class [A-Za-z_][A-Za-z0-9_]+");
  rule.format = _classFormat;
  _highlightingRules.append(rule);

  rule.pattern = QRegExp("tlp.*\\.[A-Za-z0-9_.]+");
  rule.format = _tlpApiFormat;
  _highlightingRules.append(rule);

  _keywordFormat.setForeground(Qt::darkBlue);
  _keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;
  int i = 0;

  while (PythonInterpreter::pythonKeywords[i]) {
    keywordPatterns << "\\b" + QString(PythonInterpreter::pythonKeywords[i++]) + "\\b";
  }


  QStringList specialCharsPatterns;
  specialCharsPatterns << "\\+" << "-" << "=" << "\\(" << "\\)" << "\\[" << "\\]" << "," << "!" << "\\*" << "/"
                       << "\\{" << "\\}" << ":" << "\\." << ">" << "<" << "%" << "&" << "\\^" << "\\|";


  QString builtinModName = "__builtin__";

  if (PythonInterpreter::getInstance()->getPythonVersion() >= 3.0) {
    builtinModName = "builtins";
  }

  if (PythonInterpreter::getInstance()->runString(QString("import ") + builtinModName)) {
    QVector<QString> builtinDictContent = PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);
    QStringList builtinPatterns;

    for (int i = 0 ; i < builtinDictContent.size() ; ++i) {
      builtinPatterns << "\\b" + builtinDictContent[i] + "\\b";
    }

    builtinPatterns << "\\bself\\b";

    QTextCharFormat builtinFormat;
    builtinFormat.setForeground(QColor(0,87,187));
    foreach (const QString &pattern, builtinPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = builtinFormat;
      _highlightingRules.append(rule);
    }
  }

  foreach (const QString &pattern, keywordPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = _keywordFormat;
    _highlightingRules.append(rule);
  }

  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);

  foreach (const QString &pattern, specialCharsPatterns) {
    rule.pattern = QRegExp(pattern);
    rule.format = format;
    _highlightingRules.append(rule);
  }

  _numberFormat.setForeground(Qt::darkCyan);
  rule.pattern = QRegExp("\\b[0-9]+[lL]?\\b");
  rule.format = _numberFormat;
  _highlightingRules.append(rule);
  rule.pattern = QRegExp("\\b0[xX][0-9A-Fa-f]+[lL]?\\b");
  _highlightingRules.append(rule);
  rule.pattern = QRegExp("\\b[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b");
  _highlightingRules.append(rule);

  _quotationFormat.setForeground(Qt::darkMagenta);

}

void PythonCodeHighlighter::highlightBlock(const QString &text) {

  if (_shellMode) {
    if (currentBlock().blockNumber() > 2 && !text.startsWith(">>>") && !text.startsWith("...")) {
      return;
    }
  }

  foreach (const HighlightingRule &rule, _highlightingRules) {
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
      // don't treat multiline strings here (enclosed in """)
      if ((i+1) < text.length() && (i+2) < text.length() && text[i+1] == '"' && text[i+2] == '"') {
        continue;
      }

      if ((i-1) > 0 && (i+1) < text.length() && text[i-1] == '"' && text[i+1] == '"') {
        continue;
      }

      if ((i-1) > 0 && (i-2) > 0 && text[i-1] == '"' && text[i-2] == '"') {
        continue;
      }

      if (quoteStartPos == -1) {
        quoteStartPos = i;
      }
      else {
        setFormat(quoteStartPos, i-quoteStartPos+1, _quotationFormat);
        quoteStartPos = -1;
      }
    }
  }

  quoteStartPos = -1;

  for (int i = 0 ; i < text.length() ; ++i) {
    if (text[i] == '\'' && (i == 0 || text[i-1] != '\\')) {
      // don't treat multiline strings here (enclosed in ''')
      if ((i+1) < text.length() && (i+2) < text.length() && text[i+1] == '\'' && text[i+2] == '\'') {
        continue;
      }

      if ((i-1) > 0 && (i+1) < text.length() && text[i-1] == '\'' && text[i+1] == '\'') {
        continue;
      }

      if ((i-1) > 0 && (i-2) > 0 && text[i-1] == '\'' && text[i-2] == '\'') {
        continue;
      }

      if (quoteStartPos == -1) {
        quoteStartPos = i;
      }
      else {
        setFormat(quoteStartPos, i-quoteStartPos+1, _quotationFormat);
        quoteStartPos = -1;
      }
    }
  }

  QRegExp qtApiRegexp("\\bQ[A-Za-z_.]+\\b");
  int index = qtApiRegexp.indexIn(text);

  while (index >= 0) {
    int length = qtApiRegexp.matchedLength();
    QString expr = text.mid(index, length);

    if (APIDataBase::getInstance()->typeExists(expr) || APIDataBase::getInstance()->getFullTypeName(expr) != "") {
      setFormat(index, length, _qtApiFormat);
    }
    else if (expr.indexOf(".") != -1) {
      QString type = expr.mid(0, expr.lastIndexOf("."));

      if (APIDataBase::getInstance()->getFullTypeName(type) != "")
        type = APIDataBase::getInstance()->getFullTypeName(type);

      QString entry = expr.mid(expr.lastIndexOf(".")+1);

      if (APIDataBase::getInstance()->dictEntryExists(type, entry)) {
        setFormat(index, length, _qtApiFormat);
      }
    }

    index = qtApiRegexp.indexIn(text, index + length);
  }

  setCurrentBlockState(0);

  static QRegExp triSingleQuote("'''");
  static QRegExp triDoubleQuote("\"\"\"");

  // highlight multi-line strings
  bool isInMultilne = highlightMultilineString(text, triSingleQuote, 1, _quotationFormat);

  if (!isInMultilne)
    highlightMultilineString(text, triDoubleQuote, 2, _quotationFormat);

  QRegExp commentRegexp("#[^\n]*");
  index = commentRegexp.indexIn(text);

  while (index >= 0 && currentBlockState() == 0) {
    int nbQuotes = 0;

    for (int j = index - 1 ; j >= 0 ; --j) {
      if (text[j] == '\'') {
        ++nbQuotes;
      }
    }

    int nbDblQuotes = 0;

    for (int j = index - 1 ; j >= 0 ; --j) {
      if (text[j] == '"') {
        ++nbDblQuotes;
      }
    }

    int length = commentRegexp.matchedLength();

    if (nbQuotes%2 == 0 && nbDblQuotes%2 == 0) {
      if (previousBlockState() <= 0 ||
          (previousBlockState() == 1 && triSingleQuote.indexIn(text) < index) ||
          (previousBlockState() == 2 && triDoubleQuote.indexIn(text) < index))
        setFormat(index, length, _commentFormat);
    }

    index = commentRegexp.indexIn(text, index+length);
  }
}

bool PythonCodeHighlighter::highlightMultilineString(const QString &text, const QRegExp &delimiter, const int inState, const QTextCharFormat &style) {
  int start = -1;
  int add = -1;
  int length = 0;
  int commentPos = -1;

  if (previousBlockState() == inState) {
    start = 0;
    add = 0;
  }
  else {
    start = delimiter.indexIn(text);
    add = delimiter.matchedLength();
    commentPos = text.indexOf('#');
  }

  if (commentPos < 0 || commentPos > start) {

    while (start >= 0) {
      int end = delimiter.indexIn(text, start + add);

      if (end >= add) {
        length = end - start + add + delimiter.matchedLength();
        setCurrentBlockState(0);
      }
      else {
        setCurrentBlockState(inState);
        length = text.length() - start + add;
      }

      setFormat(start, length, style);
      start = delimiter.indexIn(text, start + length);
      add = delimiter.matchedLength();
    }

  }

  return (currentBlockState() == inState);
}
