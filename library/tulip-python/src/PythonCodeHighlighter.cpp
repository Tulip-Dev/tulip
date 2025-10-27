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

#include "tulip/PythonInterpreter.h"
#include "tulip/PythonCodeHighlighter.h"
#include "tulip/APIDataBase.h"

#include <QTextDocument>

using namespace tlp;

PythonCodeHighlighter::PythonCodeHighlighter(QTextDocument *parent, bool /*darkBackground*/)
    : QSyntaxHighlighter(parent), _shellMode(false) {

  QTextCharFormat builtinFormat;
  builtinFormat.setForeground(QColor(0, 87, 187));

  HighlightingRule rule;

  _commentFormat.setForeground(QColor("#388E3C"));
  _functionFormat.setFontWeight(QFont::Bold);
  _functionFormat.setForeground(QColor("#0D47F1"));
  _tlpApiFormat.setForeground(QColor("#E65100"));
  _classFormat.setFontWeight(QFont::Bold);
  _classFormat.setForeground(QColor("#7E57C2"));
  _qtApiFormat.setForeground(QColor("#00D09A"));
  _keywordFormat.setForeground(QColor("#00A8FF"));
  _numberFormat.setForeground(QColor("#00B8D4"));
  _quotationFormat.setForeground(QColor("#E040FB"));

  rule.pattern = QRegularExpression("def [A-Za-z_][A-Za-z0-9_]+(?=\\()");
  rule.format = _functionFormat;
  _highlightingRules.append(rule);

  rule.pattern = QRegularExpression("class [A-Za-z_][A-Za-z0-9_]+");
  rule.format = _classFormat;
  _highlightingRules.append(rule);

  rule.pattern = QRegularExpression("tlp.*\\.[A-Za-z0-9_.]+");
  rule.format = _tlpApiFormat;
  _highlightingRules.append(rule);

  rule.pattern = QRegularExpression("^[ \t]*@.*$");
  rule.format = builtinFormat;
  _highlightingRules.append(rule);

  _keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;
  int i = 0;

  while (PythonInterpreter::pythonKeywords[i]) {
    keywordPatterns << "\\b" + QString(PythonInterpreter::pythonKeywords[i++]) + "\\b";
  }

  QStringList specialCharsPatterns;
  specialCharsPatterns << "\\+"
                       << "-"
                       << "="
                       << "\\("
                       << "\\)"
                       << "\\["
                       << "\\]"
                       << ","
                       << "!"
                       << "\\*"
                       << "/"
                       << "\\{"
                       << "\\}"
                       << ":"
                       << "\\."
                       << ">"
                       << "<"
                       << "%"
                       << "&"
                       << "\\^"
                       << "\\|";

  QString builtinModName = "builtins";

  if (PythonInterpreter::getInstance()->runString(QString("import ") + builtinModName)) {
    QVector<QString> builtinDictContent =
        PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);
    QStringList builtinPatterns;

    for (int i = 0; i < builtinDictContent.size(); ++i) {
      builtinPatterns << "\\b" + builtinDictContent[i] + "\\b";
    }

    builtinPatterns << "\\bself\\b";

    for (const QString &pattern : builtinPatterns) {
      rule.pattern = QRegularExpression(pattern);
      rule.format = builtinFormat;
      _highlightingRules.append(rule);
    }
  }

  for (const QString &pattern : keywordPatterns) {
    rule.pattern = QRegularExpression(pattern);
    rule.format = _keywordFormat;
    _highlightingRules.append(rule);
  }

  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);

  for (const QString &pattern : specialCharsPatterns) {
    rule.pattern = QRegularExpression(pattern);
    rule.format = format;
    _highlightingRules.append(rule);
  }

  rule.pattern = QRegularExpression("\\b[0-9]+[lL]?\\b");
  rule.format = _numberFormat;
  _highlightingRules.append(rule);
  rule.pattern = QRegularExpression("\\b0[xX][0-9A-Fa-f]+[lL]?\\b");
  _highlightingRules.append(rule);
  rule.pattern = QRegularExpression("\\b[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b");
  _highlightingRules.append(rule);
}

void PythonCodeHighlighter::highlightBlock(const QString &text) {

  if (_shellMode) {
    if (currentBlock().blockNumber() > 2 && !text.startsWith(">>>") && !text.startsWith("...")) {
      return;
    }
  }

  QRegularExpressionMatch match;
  for (const HighlightingRule &rule : _highlightingRules) {
    int index = text.indexOf(rule.pattern, 0, &match);

    while (index >= 0) {
      int length = match.capturedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(rule.pattern, index + length, &match);
    }
  }

  int quoteStartPos = -1;

  for (int i = 0; i < text.length(); ++i) {
    if (text[i] == '"' && (i == 0 || text[i - 1] != '\\')) {
      // don't treat multiline strings here (enclosed in """)
      if ((i + 1) < text.length() && (i + 2) < text.length() && text[i + 1] == '"' &&
          text[i + 2] == '"') {
        continue;
      }

      if ((i - 1) > 0 && (i + 1) < text.length() && text[i - 1] == '"' && text[i + 1] == '"') {
        continue;
      }

      if ((i - 1) > 0 && (i - 2) > 0 && text[i - 1] == '"' && text[i - 2] == '"') {
        continue;
      }

      if (quoteStartPos == -1) {
        quoteStartPos = i;
        setFormat(quoteStartPos, 1, _quotationFormat);
      } else {
        setFormat(quoteStartPos, i - quoteStartPos + 1, _quotationFormat);
        quoteStartPos = -1;
      }
    } else if (quoteStartPos != -1) {
      setFormat(quoteStartPos, i - quoteStartPos + 1, _quotationFormat);
    }
  }

  quoteStartPos = -1;

  for (int i = 0; i < text.length(); ++i) {
    if (text[i] == '\'' && (i == 0 || text[i - 1] != '\\')) {
      // don't treat multiline strings here (enclosed in ''')
      if ((i + 1) < text.length() && (i + 2) < text.length() && text[i + 1] == '\'' &&
          text[i + 2] == '\'') {
        continue;
      }

      if ((i - 1) > 0 && (i + 1) < text.length() && text[i - 1] == '\'' && text[i + 1] == '\'') {
        continue;
      }

      if ((i - 1) > 0 && (i - 2) > 0 && text[i - 1] == '\'' && text[i - 2] == '\'') {
        continue;
      }

      if (quoteStartPos == -1) {
        quoteStartPos = i;
        setFormat(quoteStartPos, 1, _quotationFormat);
      } else {
        setFormat(quoteStartPos, i - quoteStartPos + 1, _quotationFormat);
        quoteStartPos = -1;
      }
    } else if (quoteStartPos != -1) {
      setFormat(quoteStartPos, i - quoteStartPos + 1, _quotationFormat);
    }
  }

  QRegularExpression qtApiRegexp("\\bQ[A-Za-z_.]+\\b");
  int index = text.indexOf(qtApiRegexp, 0, &match);

  while (index >= 0) {
    int length = match.capturedLength();
    QString expr = text.mid(index, length);

    if (APIDataBase::getInstance()->typeExists(expr) ||
        !APIDataBase::getInstance()->getFullTypeName(expr).isEmpty()) {
      setFormat(index, length, _qtApiFormat);
    } else if (expr.indexOf(".") != -1) {
      QString type = expr.mid(0, expr.lastIndexOf("."));

      if (!APIDataBase::getInstance()->getFullTypeName(type).isEmpty())
        type = APIDataBase::getInstance()->getFullTypeName(type);

      QString entry = expr.mid(expr.lastIndexOf(".") + 1);

      if (APIDataBase::getInstance()->dictEntryExists(type, entry)) {
        setFormat(index, length, _qtApiFormat);
      }
    }

    index = text.indexOf(qtApiRegexp, index + length, &match);
  }

  setCurrentBlockState(0);

  static QRegularExpression triSingleQuote("'''");
  static QRegularExpression triDoubleQuote("\"\"\"");

  // highlight multi-line strings
  bool isInMultilne = highlightMultilineString(text, triSingleQuote, 1, _quotationFormat);

  if (!isInMultilne)
    highlightMultilineString(text, triDoubleQuote, 2, _quotationFormat);

  QRegularExpression commentRegexp("#[^\n]*");
  index = text.indexOf(commentRegexp, 0, &match);

  while (index >= 0 && currentBlockState() == 0) {
    int nbQuotes = 0;

    for (int j = index - 1; j >= 0; --j) {
      if (text[j] == '\'') {
        ++nbQuotes;
      }
    }

    int nbDblQuotes = 0;

    for (int j = index - 1; j >= 0; --j) {
      if (text[j] == '"') {
        ++nbDblQuotes;
      }
    }

    int length = match.capturedLength();

    if (nbQuotes % 2 == 0 && nbDblQuotes % 2 == 0) {
      if (previousBlockState() <= 0 ||
          (previousBlockState() == 1 && text.indexOf(triSingleQuote) < index) ||
          (previousBlockState() == 2 && text.indexOf(triDoubleQuote) < index))
        setFormat(index, length, _commentFormat);
    }

    index = text.indexOf(commentRegexp, index + length, &match);
  }
}

bool PythonCodeHighlighter::highlightMultilineString(const QString &text,
                                                     const QRegularExpression &delimiter,
                                                     const int inState,
                                                     const QTextCharFormat &style) {
  int start = -1;
  int add = -1;
  int commentPos = -1;
  QRegularExpressionMatch match;

  if (previousBlockState() == inState) {
    start = 0;
    add = 0;
  } else {
    start = text.indexOf(delimiter, 0, &match);
    add = match.capturedLength();
    commentPos = text.indexOf('#');
  }

  if (commentPos < 0 || commentPos > start) {

    while (start >= 0) {
      int end = text.indexOf(delimiter, start + add, &match);
      int length;

      if (end >= add) {
        length = end - start + add + match.capturedLength();
        setCurrentBlockState(0);
      } else {
        setCurrentBlockState(inState);
        length = text.length() - start + add;
      }

      setFormat(start, length, style);
      start = text.indexOf(delimiter, start + length, &match);
      add = match.capturedLength();
    }
  }

  return (currentBlockState() == inState);
}
