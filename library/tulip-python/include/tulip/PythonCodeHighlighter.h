/*
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

#ifndef PYTHONCODEHIGHLIGHTER_H_
#define PYTHONCODEHIGHLIGHTER_H_

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class PythonCodeHighlighter : public QSyntaxHighlighter {

  struct HighlightingRule {
    QRegExp pattern;
    QTextCharFormat format;
  };

  QVector<HighlightingRule> _highlightingRules;
  QTextCharFormat _keywordFormat;
  QTextCharFormat _classFormat;
  QTextCharFormat _commentFormat;
  QTextCharFormat _quotationFormat;
  QTextCharFormat _functionFormat;
  QTextCharFormat _numberFormat;
  QTextCharFormat _tlpApiFormat;
  QTextCharFormat _qtApiFormat;

  bool _shellMode;

public:

  explicit PythonCodeHighlighter(QTextDocument *parent = 0);

  void setShellMode(const bool shellMode) {
    _shellMode = shellMode;
  }

protected:

  void highlightBlock(const QString &text);

  bool highlightMultilineString(const QString &text, const QRegExp &delimiter, const int inState, const QTextCharFormat &style);

};

#endif /* PYTHONCODEHIGHLIGHTER_H_ */
