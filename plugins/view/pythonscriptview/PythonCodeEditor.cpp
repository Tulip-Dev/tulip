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

#include "PythonInterpreter.h"
#include "PythonCodeEditor.h"

#include <QtGui/QPainter>
#include <QtGui/QTextBlock>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QToolTip>
#include <QtGui/QScrollBar>

#include <iostream>

#include <tulip/TlpTools.h>

using namespace std;

AutoCompletionList::AutoCompletionList(PythonCodeEditor *parent) : QListWidget(parent), codeEditor(parent) {
#if defined(__APPLE__)
  setWindowFlags(Qt::Popup|Qt::FramelessWindowHint);
#elif QT_VERSION >= 0x040500
  setWindowFlags(Qt::ToolTip);
#else
  setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
#endif
  setAttribute(Qt::WA_StaticContents);
  setFrameShape(StyledPanel);
  setFrameShadow(Plain);
  setFocusProxy(parent);
  activated = false;
  wasActivated = false;
}

void AutoCompletionList::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape) {
    e->accept();
    close();
  }
  else if (e->key() == Qt::Key_Down || e->key() == Qt::Key_Up ||
           e->key() == Qt::Key_Home || e->key() == Qt::Key_End ||
           e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown) {
    QListWidget::keyPressEvent(e);
  }
  else if (e->key() == Qt::Key_Left) {
    if (horizontalScrollBar()) {
      horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->sliderPosition()-2);
    }
  }
  else if (e->key() == Qt::Key_Right) {
    if (horizontalScrollBar()) {
      horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->sliderPosition()+2);
    }
  }
  else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
    e->accept();
    close();

    if (currentItem()) {
      PythonCodeEditor *editor = static_cast<PythonCodeEditor *>(parent());
      QTextCursor cursor = editor->textCursor();
      QString text = cursor.block().text();

      if (text != "") {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
        int start = cursor.positionInBlock();
#else
        int start = cursor.position() - cursor.block().position();
#endif


        int end = 0;
        bool endOk = false;

        for (int i = start ; i > 0 ; --i) {
          int j = 0;

          while (sepChar[j]) {
            if (text[i-1] == sepChar[j] || text[i-1] == '.') {
              end = i;
              endOk = true;
              break;
            }

            ++j;
          }

          if (endOk)
            break;
        }

        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, start - end);
        cursor.removeSelectedText();
      }

      QString textToInsert = currentItem()->text();
      int pos = textToInsert.indexOf("\" (");

      if (pos != -1) {
        textToInsert = textToInsert.mid(0, pos+1);
      }

      cursor.insertText(textToInsert);

      QString type = codeEditor->autoCompletionDb->getLastFoundType();

      if (type != "") {
        QVector<QString> types;
        types.push_back(type);
        vector<string> baseTypes = PythonInterpreter::getInstance()->getBaseTypesForType(type.toStdString());

        for (size_t i = 0 ; i < baseTypes.size() ; ++i) {
          types.push_back(baseTypes[i].c_str());
        }

        for (int i = 0 ; i < types.size() ; ++i) {
          QString funcName = types[i] + "." + textToInsert;

          if (codeEditor->apiDb->functionExists(funcName)) {
            QVector<QVector<QString> > params = codeEditor->apiDb->getParamTypesForMethodOrFunction(funcName);

            if (params.count() > 1 || params[0].count() > 0) {
              if (text.indexOf("class ") == -1)
                qApp->sendEvent(codeEditor, new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier, "("));
            }
            else {
              cursor.insertText("()");
            }

            break;
          }
        }
      }
    }
  }
  else {
    QCoreApplication::sendEvent(parent(), e);
  }
}

void AutoCompletionList::showEvent (QShowEvent * event) {
  QListWidget::showEvent(event);
  grabKeyboard();
  activated = true;
}

void AutoCompletionList::hideEvent (QHideEvent * event) {
  QListWidget::hideEvent(event);
  releaseKeyboard();
  activated = false;
}


bool AutoCompletionList::eventFilter(QObject *, QEvent *event) {
  if (!wasActivated && (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide)) {
    wasActivated = activated;
    hide();
  }
  else if (event->type() == QEvent::WindowActivate || event->type() == QEvent::Show) {
    if (wasActivated) {
      show();
      wasActivated = false;
    }
  }

  codeEditor->updateAutoCompletionListPosition();

  return false;
}

FindReplaceDialog::FindReplaceDialog(QPlainTextEdit *editor) : QDialog(editor), editor(editor) {
  setupUi(this);
  connect(findButton, SIGNAL(clicked()), this, SLOT(doFind()));
  connect(replaceButton, SIGNAL(clicked()), this, SLOT(doReplace()));
  connect(replaceFindButton, SIGNAL(clicked()), this, SLOT(doReplaceFind()));
  connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(doReplaceAll()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
  connect(forwardRB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(backwardRB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(regexpCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(regexpCB, SIGNAL(toggled(bool)), this, SLOT(regexpToggled(bool)));
  connect(wholeWordCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(wrapSearchCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(caseSensitiveCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(textToFind, SIGNAL(textChanged(const QString &)), this, SLOT(textToFindChanged()));
}

void FindReplaceDialog::setTextToFind(const QString &text) {
  textToFind->setText(text);
}

void FindReplaceDialog::textToFindChanged() {
  replaceButton->setEnabled(false);
  replaceFindButton->setEnabled(false);
  QString text = textToFind->text();

  if (text == "") {
    findButton->setEnabled(false);
    replaceAllButton->setEnabled(false);
  }
  else {
    findButton->setEnabled(true);
    replaceAllButton->setEnabled(true);
  }
}

void FindReplaceDialog::regexpToggled(bool toggled) {
  wholeWordCB->setEnabled(!toggled);
}

void FindReplaceDialog::setFindMode(const bool findMode) {
  if (findMode) {
    textToFind->setFocus();
  }
  else {
    replaceText->setFocus();
  }
}

void FindReplaceDialog::setSearchResult(const bool result) {
  replaceButton->setEnabled(result);
  replaceFindButton->setEnabled(result);

  if (!result) {
    searchStatusLabel->setText("String Not Found");
  }
  else {
    searchStatusLabel->setText("");
  }
}

bool FindReplaceDialog::doFind() {
  QString text = textToFind->text();

  if (text == "")
    return false;


  QTextDocument::FindFlags findFlags;

  if (!forwardRB->isChecked()) {
    findFlags |= QTextDocument::FindBackward;
  }

  if (caseSensitiveCB->isChecked()) {
    findFlags |= QTextDocument::FindCaseSensitively;
  }

  if (wholeWordCB->isChecked()) {
    findFlags |= QTextDocument::FindWholeWords;
  }

  QTextCursor sel;

  if (!regexpCB->isChecked()) {
    sel = editor->document()->find(text, editor->textCursor(), findFlags);
  }
  else {
    sel = editor->document()->find(QRegExp(text), editor->textCursor(), findFlags);
  }

  bool ret = !sel.isNull();

  if (ret) {
    editor->setTextCursor(sel);
  }
  else if (wrapSearchCB->isChecked()) {
    QTextCursor cursor = editor->textCursor();

    if (!backwardRB->isChecked()) {
      cursor.movePosition(QTextCursor::Start);
    }
    else {
      cursor.movePosition(QTextCursor::End);
    }

    if (!regexpCB->isChecked()) {
      sel = editor->document()->find(text, cursor, findFlags);
    }
    else {
      sel = editor->document()->find(QRegExp(text), cursor, findFlags);
    }

    ret = !sel.isNull();

    if (ret) {
      editor->setTextCursor(sel);
    }
  }

  setSearchResult(ret);
  lastSearch = text;
  return ret;
}

bool FindReplaceDialog::doReplace() {
  QString text = textToFind->text();

  if (text == "")
    return false;

  QString selection = editor->textCursor().selectedText();

  if (!caseSensitiveCB->isChecked()) {
    selection = selection.toLower();
    text = text.toLower();
  }

  if (selection == text) {
    editor->textCursor().removeSelectedText();
    editor->textCursor().insertText(replaceText->text());
    return true;
  }

  return false;
}

void FindReplaceDialog::doReplaceFind() {
  if (doReplace())
    doFind();
}

void FindReplaceDialog::doReplaceAll() {
  QString text = textToFind->text();

  if (text == "")
    return;

  bool ret = doFind();

  if (ret) {
    int startLine = editor->textCursor().blockNumber();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    int startIndex = editor->textCursor().positionInBlock();
#else
    int startIndex = editor->textCursor().position() - editor->textCursor().block().position();
#endif
    int nbReplacements = 0;

    while(ret) {
      doReplace();
      ret = doFind();
      ++nbReplacements;
      int line = editor->textCursor().blockNumber();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
      int index = editor->textCursor().positionInBlock();
#else
      int index = editor->textCursor().position() - editor->textCursor().block().position();
#endif

      // when replacing a pattern P by a pattern following this regexp .*P.*
      // this can lead to an infinite loop, handle this case
      if (line == startLine && index >= startIndex)
        break;
    }

    searchStatusLabel->setText(QString::number(nbReplacements) + QString(" matches replaced"));
    resetSearch = true;
  }
  else {
    setSearchResult(ret);
  }
}

void FindReplaceDialog::hideEvent(QHideEvent *) {
  editor->setFocus();
}

APIDataBase*PythonCodeEditor::apiDb(NULL);
GragKeyboardFocusEventFilter PythonCodeEditor::keyboardFocusEventFilter;

PythonCodeEditor::PythonCodeEditor(QWidget *parent) : QPlainTextEdit(parent), highlighter(NULL), tooltipActive(false) {
  installEventFilter(&keyboardFocusEventFilter);
  setAutoIndentation(true);
  setIndentationGuides(true);
  setHighlightEditedLine(true);
  setFindReplaceActivated(true);
  setCommentShortcutsActivated(true);
  setIndentShortcutsActivated(true);
  setWordWrapMode(QTextOption::NoWrap);
  QTextCharFormat format = currentCharFormat();
#if defined(WIN32)
  currentFont.setFamily("Courier New");
  currentFont.setPointSize(8);
#elif defined(__APPLE__)
#ifdef QT_MAC_USE_COCOA
  currentFont.setFamily("Menlo");
#else
  currentFont.setFamily("Andale Mono");
#endif
  currentFont.setPointSize(12);
#else
  currentFont.setFamily("Monospace");
  currentFont.setPointSize(8);
#endif

  format.setFont(currentFont);
  setCurrentCharFormat(format);

  lineNumberArea = new LineNumberArea(this);

  updateTabStopWidth();

  updateLineNumberAreaWidth();

  parenHighlighter = new ParenMatcherHighlighter(document());
  highlighter = new PythonCodeHighlighter(document());

  if (!apiDb) {
    apiDb = APIDataBase::getInstance();
    apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/tulip.api");
    apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/Python-" + QString(PythonInterpreter::getInstance()->getPythonVersion().c_str()) + ".api");
    apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/tulipogl.api");
    apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/tulipgui.api");

    PythonInterpreter::getInstance()->setOutputEnabled(false);

    if (PythonInterpreter::getInstance()->runString("import PyQt4.QtGui")) {
      apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/tulipqt.api");
      apiDb->loadApiFile(QString(tlp::TulipShareDir.c_str()) + "/apiFiles/PyQt4.api");
    }

    PythonInterpreter::getInstance()->setOutputEnabled(true);
  }

  autoCompletionList = new AutoCompletionList(this);
  autoCompletionDb = new AutoCompletionDataBase(apiDb);

  // Hack to get a pointer on the main window
  // in order for the autocompletion dialog to catch
  // window activate/desactivate events
  QMainWindow *mainWindow = NULL;
  QWidget *parente = dynamic_cast<QWidget*>(this->parent());

  while (parente) {
    mainWindow = dynamic_cast<QMainWindow *>(parente);

    if (mainWindow)
      break;

    parente = dynamic_cast<QWidget *>(parente->parent());
  }

  if (mainWindow)
    mainWindow->installEventFilter(autoCompletionList);

  installEventFilter(autoCompletionList);

  findReplaceDialog = new FindReplaceDialog(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
  connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(resetExtraSelections()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightErrors()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchParens()));
  connect(this, SIGNAL(textChanged()), this, SLOT(updateAutoCompletionList()));
  connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightSelection()));

  shellWidget = false;
  moduleEditor = false;
}

PythonCodeEditor::~PythonCodeEditor() {
  delete autoCompletionDb;
  removeEventFilter(autoCompletionList);
}

QString PythonCodeEditor::getCleanCode() const {
  QString code = toPlainText().replace("\r\n", "\n");

  if (code[code.size()-1] != '\n')
    code += "\n";

  return  code;
}

QFontMetrics PythonCodeEditor::fontMetrics() const {
  return QFontMetrics(currentFont);
}

void PythonCodeEditor::indicateScriptCurrentError(int lineNumber) {
  currentErrorLines.append(lineNumber);
  emit cursorPositionChanged();
}

void PythonCodeEditor::clearErrorIndicator() {
  currentErrorLines.clear();
  emit cursorPositionChanged();
}

int PythonCodeEditor::lineNumberAreaWidth() const {
  int digits = 1;
  int max = qMax(1, blockCount());

  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
  return space;
}

void PythonCodeEditor::updateLineNumberAreaWidth() {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void PythonCodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(),
                           rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth();
}

void PythonCodeEditor::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);
  QRect cr = contentsRect();
  lineNumberArea->setGeometry(
    QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void PythonCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + static_cast<int>(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.setFont(currentFont);
      painter.drawText(0, top, lineNumberArea->width(),
                       fontMetrics().height(), Qt::AlignRight | Qt::AlignCenter,
                       number);
    }

    block = block.next();
    top = bottom;
    bottom = top + static_cast<int>(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

static float clamp(float f, float minV, float maxV) {
  return std::min(std::max(f, minV), maxV);
}

void PythonCodeEditor::updateTabStopWidth() {
  setTabStopWidth(3 * fontMetrics().width(QLatin1Char(' ')));
}

void PythonCodeEditor::zoomIn() {
  QTextCursor cursor = textCursor();
  selectAll();
  QTextCharFormat format = currentCharFormat();
  currentFont.setPointSize(clamp(currentFont.pointSize()+1, 6, 30));
  format.setFont(currentFont);
  setCurrentCharFormat(format);
  setTextCursor(cursor);
  updateTabStopWidth();
}

void PythonCodeEditor::zoomOut() {
  QTextCursor cursor = textCursor();
  selectAll();
  QTextCharFormat format = currentCharFormat();
  currentFont.setPointSize(clamp(currentFont.pointSize()-1, 6, 30));
  format.setFont(currentFont);
  setCurrentCharFormat(format);
  setTextCursor(cursor);
  updateTabStopWidth();
}

void PythonCodeEditor::showEvent(QShowEvent *event) {
  QPlainTextEdit::showEvent(event);
  setFocus();
  updateLineNumberAreaWidth();
  emit cursorPositionChanged();
}

void PythonCodeEditor::paintEvent(QPaintEvent *event) {
  QPlainTextEdit::paintEvent(event);

  QPainter painter(viewport());
  painter.setFont(currentFont);

  // draw tooltip

  if (isTooltipActive()) {
    QTextBlock tooltipBlock = document()->findBlockByNumber(toolTipPos.x());
    int top = static_cast<int>(blockBoundingGeometry(tooltipBlock).translated(contentOffset()).top());
    int left = static_cast<int>(blockBoundingGeometry(tooltipBlock).translated(contentOffset()).left());
    int bottom = top + static_cast<int>(blockBoundingRect(tooltipBlock).height());
    QString blockText = tooltipBlock.text();

    for (int i = 0 ; i < toolTipPos.y() ; ++i) {
      if (blockText[i] == '\t') {
        left += tabStopWidth();
      }
      else {
        left += fontMetrics().width(QLatin1Char(blockText[i].toAscii()));
      }


    }

    QStringList toolTipLines = toolTipText.split("\n");
    int height = toolTipLines.size() *  blockBoundingRect(tooltipBlock).height();
    int width = 0;

    for (int i = 0 ; i < toolTipLines.size() ; ++i) {
      int w = 0;

      for (int j = 0 ; j < toolTipLines[i].length() ; ++j) {
        w += fontMetrics().width(QLatin1Char(toolTipLines[i][j].toAscii()));
      }

      width = std::max(w, width);
    }

    QPoint tPos(left+4, top - toolTipLines.size()*(bottom - top) - 1);
#ifndef __APPLE__
    QRect tooltipRect(tPos, tPos + QPoint(width, height));
#else
    QRect tooltipRect(tPos, tPos + QPoint(width+2*fontMetrics().width(QLatin1Char(' ')), height));
#endif
    painter.drawRect(tooltipRect);
    painter.fillRect(tooltipRect, QColor(249,251,100,200));
#ifndef __APPLE__
    painter.drawText(tooltipRect, toolTipText);
#else
    painter.drawText(tooltipRect, Qt::AlignLeft, toolTipText);
#endif
  }

  // draw indentation guides

  if (!indentationGuides()) {
    return;
  }

  QTextBlock block = firstVisibleBlock();
  int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());

  int bottom = top + static_cast<int>(blockBoundingRect(block).height());

  QPen pen;
  pen.setStyle(Qt::DotLine);
  painter.setPen(pen);

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString text = block.text();
      int indentVal = 0;

      for (int i = 0 ; i < text.length() ; ++i) {
        if (text[i] == ' ')
          indentVal+=fontMetrics().width(QLatin1Char(' '));
        else if (text[i] == '\t')
          indentVal+=tabStopWidth();
        else
          break;
      }

      int i = 1;

      while (indentVal > tabStopWidth()) {
        painter.drawLine(contentOffset().x() + i*tabStopWidth() + 4 , top,
                         contentOffset().x() + i*tabStopWidth() + 4 , bottom);
        indentVal -= tabStopWidth();
        ++i;
      }
    }

    block = block.next();
    top = bottom;
    bottom = top + static_cast<int>(blockBoundingRect(block).height());
  }
}

static int matchLeftParenthesis(const QTextBlock &block, const std::pair<char, char> &parens, int dataStartIndex, int dec=0) {
  if (!block.isValid()) {
    return -1;
  }

  ParenInfoTextBlockData *data = static_cast<ParenInfoTextBlockData *>(block.userData());
  QVector<ParenInfo> infos = data->parens();

  for (int i = dataStartIndex ; i < infos.size() ; ++i) {
    if (infos.at(i).character == parens.second) {
      if (dec == 0)
        return infos.at(i).position;
      else
        --dec;
    }
    else if (infos.at(i).character == parens.first) {
      ++dec;
    }
  }

  return matchLeftParenthesis(block.next(), parens, 0, dec);
}

static int matchRightParenthesis(const QTextBlock &block, const std::pair<char, char> &parens, int dataStartIndex, int dec=0) {
  if (!block.isValid()) {
    return -1;
  }

  ParenInfoTextBlockData *data = static_cast<ParenInfoTextBlockData *>(block.userData());
  QVector<ParenInfo> infos = data->parens();

  int startIdx = (dataStartIndex == -1) ? infos.size()-1 : dataStartIndex;

  for (int i = startIdx ; i >= 0 ; --i) {
    if (infos.at(i).character == parens.first) {
      if (dec == 0)
        return infos.at(i).position;
      else
        --dec;
    }
    else if (infos.at(i).character == parens.second) {
      ++dec;
    }
  }

  return matchRightParenthesis(block.previous(), parens, -1, dec);
}

void PythonCodeEditor::matchParens() {
  blockSignals(true);
  QTextCharFormat format = textCursor().blockCharFormat();
  parenHighlighter->rehighlightBlock(textCursor().block());
  textCursor().setBlockCharFormat(format);

  blockSignals(false);
  ParenInfoTextBlockData *data = static_cast<ParenInfoTextBlockData *>(textCursor().block().userData());

  if (data) {
    int curPos = textCursor().position();
    QVector<ParenInfo> infos = data->parens();

    for (int i = 0; i < infos.size(); ++i) {
      ParenInfo info = infos.at(i);

      if (info.position == curPos - 1) {
        int match = -1;

        if (info.character == ')') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('(', ')'), i, -1);
        }
        else if (info.character == ']') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('[', ']'), i, -1);
        }
        else if (info.character == '}') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('{', '}'), i, -1);
        }

        if (match != -1) {
          createParenSelection(info.position);
          createParenSelection(match);
          return;
        }
      }

      if (info.position == curPos) {
        int match = -1;

        if (info.character == ')') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('(', ')'), i, -1);
        }
        else if (info.character == ']') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('[', ']'), i, -1);
        }
        else if (info.character == '}') {
          match = matchRightParenthesis(textCursor().block(), std::make_pair('{', '}'), i, -1);
        }

        if (match != -1) {
          createParenSelection(info.position);
          createParenSelection(match);
          return;
        }
      }

      if (info.position == curPos) {
        int match = -1;

        if (info.character == '(') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('(', ')'), i + 1);
        }
        else if (info.character == '[') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('[', ']'), i + 1);
        }
        else if (info.character == '{') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('{', '}'), i + 1);
        }

        if (match != -1) {
          createParenSelection(info.position);
          createParenSelection(match);
          return;
        }
      }

      if (info.position == curPos-1) {
        int match = -1;

        if (info.character == '(') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('(', ')'), i + 1);
        }
        else if (info.character == '[') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('[', ']'), i + 1);
        }
        else if (info.character == '{') {
          match = matchLeftParenthesis(textCursor().block(), std::make_pair('{', '}'), i + 1);
        }

        if (match != -1) {
          createParenSelection(info.position);
          createParenSelection(match);
        }
      }
    }
  }
}

void PythonCodeEditor::resetExtraSelections() {
  if (selectedText() != "")
    return;

  QList<QTextEdit::ExtraSelection> selections;
  setExtraSelections(selections);
}

void PythonCodeEditor::highlightCurrentLine() {
  QList<QTextEdit::ExtraSelection> selections = extraSelections();

  if (highlightEditedLine() && !isReadOnly() && selectedText() == "") {
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::yellow).lighter(160);
    selection.format = textCursor().block().charFormat();
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selections.append(selection);
  }

  setExtraSelections(selections);
}

void PythonCodeEditor::highlightSelection() {
  QString text = selectedText();
  QList<QTextEdit::ExtraSelection> selections = extraSelections();

  if (text != "") {
    QTextDocument::FindFlags findFlags;
    findFlags |= QTextDocument::FindCaseSensitively;
    findFlags |= QTextDocument::FindWholeWords;
    QTextCursor cursor = document()->find(text, QTextCursor(document()->begin()), findFlags);

    while (!cursor.isNull()) {
      QTextEdit::ExtraSelection selection;
      QColor lineColor = QColor(Qt::yellow);
      selection.format = cursor.block().charFormat();
      selection.format.setBackground(lineColor);
      selection.cursor = cursor;
      selections.append(selection);
      cursor = document()->find(text, cursor, findFlags);
    }
  }

  setExtraSelections(selections);
}

void PythonCodeEditor::createParenSelection(int pos) {
  QList<QTextEdit::ExtraSelection> selections = extraSelections();
  QTextEdit::ExtraSelection selection;
  selection.format.setForeground(Qt::red);
  QTextCursor cursor = textCursor();
  cursor.setPosition(pos);
  cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
  selection.cursor = cursor;
  selections.append(selection);
  setExtraSelections(selections);
}

void PythonCodeEditor::highlightErrors() {
  QList<QTextEdit::ExtraSelection> selections = extraSelections();

  for (int i = 0 ; i < currentErrorLines.size() ; ++i) {
    QTextEdit::ExtraSelection selection;
    QTextBlock block = document()->findBlockByNumber(currentErrorLines.at(i));
    selection.format = block.charFormat();
    selection.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    selection.format.setUnderlineColor(Qt::red);
    selection.cursor = QTextCursor(block);
    selection.cursor.select(QTextCursor::LineUnderCursor);
    selections.append(selection);
  }

  setExtraSelections(selections);
}

void PythonCodeEditor::keyPressEvent (QKeyEvent * e) {
  Qt::KeyboardModifier modifier = Qt::ControlModifier;
#ifdef __APPLE__
  modifier = Qt::MetaModifier;
#endif

  if (commentShortcutsActivated() && e->modifiers() == modifier && e->key() == Qt::Key_D) {
    commentSelectedCode();
  }
  else if (commentShortcutsActivated() &&  e->modifiers() == (modifier | Qt::ShiftModifier) && e->key() == Qt::Key_D) {
    uncommentSelectedCode();
  }
  else if (indentShortcutsActivated() && e->modifiers() == modifier && e->key() == Qt::Key_I) {
    indentSelectedCode();
  }
  else if (indentShortcutsActivated() &&  e->modifiers() == (modifier | Qt::ShiftModifier) && e->key() == Qt::Key_I) {
    unindentSelectedCode();
  }
  else if (findReplaceActivated() && e->modifiers() == modifier && e->key() == Qt::Key_F) {
    QString selection = textCursor().selectedText();

    if (selection != "")
      findReplaceDialog->setTextToFind(selection);

    findReplaceDialog->show();
    findReplaceDialog->raise();
    findReplaceDialog->activateWindow();
    findReplaceDialog->setFindMode(true);
  }
  else if (findReplaceActivated() && e->modifiers() == modifier && e->key() == Qt::Key_R) {
    QString selection = textCursor().selectedText();

    if (selection != "")
      findReplaceDialog->setTextToFind(selection);

    findReplaceDialog->show();
    findReplaceDialog->raise();
    findReplaceDialog->activateWindow();
    findReplaceDialog->setFindMode(false);
  }
  else if ((e->key() == Qt::Key_Space && e->modifiers() == modifier) || e->text() == ".") {
    QPlainTextEdit::keyPressEvent(e);
    QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());

    if (!textBeforeCursor.contains('#'))
      showAutoCompletionList(e->text() == ".");
  }
  else {
    QPlainTextEdit::keyPressEvent(e);

    // Auto indentation
    if (autoIndentation() && e->key() == Qt::Key_Return) {
      QTextBlock block = textCursor().block().previous();

      if (block.isValid()) {
        QString text = block.text();

        for (int i = 0 ; i < text.length() ; ++i) {
          if (text[i].isSpace()) {
            textCursor().insertText(QString(text[i]));
          }
          else {
            break;
          }
        }

        text = text.trimmed();

        if (text.length() > 0 && text[text.length()-1] == ':')
          textCursor().insertText("\t");
      }
    }

    QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());
    QString textAfterCursor = textCursor().block().text().mid(textCursor().position() - textCursor().block().position());

    if (isTooltipActive()) {
      if (textBeforeCursor.indexOf(toolTipFunc + "(") == -1) {
        hideTooltip();
      }
    }

    if (e->text() == "(" && textAfterCursor.trimmed() == "") {

      QString cleanContext = textBeforeCursor.trimmed();

      if (cleanContext.lastIndexOf(' ') != -1) {
        cleanContext = cleanContext.mid(cleanContext.lastIndexOf(' ')+1);
      }

      if (cleanContext.lastIndexOf('=') != -1) {
        cleanContext = cleanContext.mid(cleanContext.lastIndexOf('=')+1);
      }

      QString type = "";
      QString funcName = "";

      if (cleanContext.indexOf(".") != -1) {
        type = autoCompletionDb->findTypeForExpr(cleanContext.mid(0, cleanContext.lastIndexOf('.')), getEditedFunctionName());
        funcName = cleanContext.mid(cleanContext.lastIndexOf('.')+1, cleanContext.lastIndexOf('(') - cleanContext.lastIndexOf('.')-1);
      }
      else {
        type = autoCompletionDb->findTypeForExpr(cleanContext.mid(0, cleanContext.lastIndexOf('(')), getEditedFunctionName());

        if (type.lastIndexOf(".") != -1) {
          funcName = type.mid(type.lastIndexOf('.')+1);
          type = type.mid(0, type.lastIndexOf('.'));
        }

      }

      if (type != "") {

        QVector<QVector<QString> > params = autoCompletionDb->getParamTypesForMethodOrFunction(type, funcName);
        QString retType = autoCompletionDb->getReturnTypeForMethodOrFunction(type, funcName);

        if (retType != "") {
          retType = " -> " + retType;
        }

        QSet<QString> toolTipTxts;
        QString toolTipTxt = "";

        for (int i = 0 ; i < params.size() ; ++i) {
          toolTipTxt = "";
          bool optArgs = false;
          toolTipTxt += (funcName + "(");

          for (int j = 0 ; j < params[i].size() ; ++j) {

            if (!optArgs && params[i][j].indexOf('=') != -1) {
              optArgs = true;

              if (toolTipTxt.indexOf(',') != -1) {
                toolTipTxt = toolTipTxt.mid(0, toolTipTxt.length() - 2);
                toolTipTxt += "[, ";
              }
              else {
                toolTipTxt += "[";
              }
            }

            toolTipTxt += params[i][j];

            if (j != params[i].size() - 1) {
              toolTipTxt += ", ";
            }
          }

          if (optArgs) {
            toolTipTxt += "]";
          }

          toolTipTxt += ")"+retType;
          toolTipTxts.insert(toolTipTxt);

        }

        toolTipTxt = "";
        int i = 0;
        foreach(QString txt, toolTipTxts) {
          toolTipTxt += txt;

          if (i != toolTipTxts.size() - 1) {
            toolTipTxt += "\n";
          }

          ++i;
        }

        toolTipFunc = funcName;
        showTooltip(textCursor().blockNumber(), textBeforeCursor.indexOf(funcName), toolTipTxt);
      }
    }
    else if (e->text() == ")") {
      hideTooltip();
    }
  }
}

void PythonCodeEditor::wheelEvent(QWheelEvent * event) {
  if (!autoCompletionList->isVisible()) {
    QPlainTextEdit::wheelEvent(event);
  }
}

void PythonCodeEditor::mouseDoubleClickEvent(QMouseEvent * event) {
  if (!autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseDoubleClickEvent(event);
  }
}

void PythonCodeEditor::mouseMoveEvent(QMouseEvent * event) {
  if (!autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseMoveEvent(event);
  }
}

void PythonCodeEditor::mousePressEvent(QMouseEvent * event) {
  if (!autoCompletionList->isVisible()) {
    QPlainTextEdit::mousePressEvent(event);
  }
}

void PythonCodeEditor::mouseReleaseEvent(QMouseEvent * event) {
  if (!autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseReleaseEvent(event);
  }
}

void PythonCodeEditor::analyseScriptCode(const bool wholeText) {
  QString moduleName = "";

  if (moduleEditor) {
    QFileInfo fileInfo(getFileName());
    moduleName = fileInfo.fileName().replace(".py", "");
  }

  if (!wholeText) {
    autoCompletionDb->analyseCurrentScriptCode(toPlainText(), textCursor().blockNumber(), shellWidget, moduleName);
  }
  else {
    autoCompletionDb->analyseCurrentScriptCode(toPlainText(), document()->blockCount(), shellWidget, moduleName);
  }
}

void PythonCodeEditor::showAutoCompletionList(bool dotContext) {
  analyseScriptCode();
  autoCompletionList->show();
  updateAutoCompletionList(dotContext);

  if (autoCompletionList->count() == 0)
    autoCompletionList->hide();
}

void PythonCodeEditor::updateAutoCompletionListPosition() {

  if (!autoCompletionList->isVisible())
    return;

  int left = static_cast<int>(blockBoundingGeometry(textCursor().block()).translated(contentOffset()).left());
  int top = static_cast<int>(blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top());
  int bottom = top + static_cast<int>(blockBoundingRect(textCursor().block()).height());
  QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());
  int pos = lineNumberAreaWidth() + left + 1;
  int stop = 0;

  for (int i = textBeforeCursor.length() ; i >=0 ;  --i) {
    if (textBeforeCursor[i] == '\t' || textBeforeCursor[i] == ' ' || textBeforeCursor[i] == '.') {
      stop = i+1;
      break;
    }
  }

  for (int i = 0 ; i < stop ; ++i) {
    if (textBeforeCursor[i] == '\t') {
      pos += tabStopWidth();
    }
    else {
      pos += fontMetrics().width(QLatin1Char(textBeforeCursor[i].toAscii()));
    }
  }

  autoCompletionList->move(mapToGlobal(QPoint(pos, bottom)));
}

void PythonCodeEditor::updateAutoCompletionList(bool dotContext) {

  if (!autoCompletionList->isVisible())
    return;

  updateAutoCompletionListPosition();

  QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());

  autoCompletionList->clear();

  QString textBeforeCursorTrimmed = textBeforeCursor.trimmed();

  QSet<QString> stringList = autoCompletionDb->getAutoCompletionListForContext(textBeforeCursorTrimmed, getEditedFunctionName(), dotContext);
  foreach(QString s, stringList) {
    autoCompletionList->addItem(s);
  }
  autoCompletionList->sortItems();


  if (autoCompletionList->count() == 0)
    autoCompletionList->hide();
  else
    autoCompletionList->setCurrentRow(0);
}

QString PythonCodeEditor::getEditedFunctionName() const {

  QString funcName = "global";
  QString className = "";
  QRegExp funcRegexp("^def [A-Za-z_][A-Za-z0-9_]*\\(.*\\)[ \t]*:$");
  QRegExp classRegexp("^class [A-Za-z_][A-Za-z0-9_]*.*:$");

  QTextBlock block = textCursor().block();
  QString currentLine = block.text();
  QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());

  if (textBeforeCursor.startsWith("\t") || textBeforeCursor.startsWith(" ")) {
    while (1) {
      block = block.previous();
      currentLine = block.text();

      if (currentLine.startsWith('#') || currentLine == "")
        continue;

      if (funcName == "global" && funcRegexp.indexIn(currentLine.trimmed()) != -1) {
        funcName = currentLine.trimmed();
        funcName = funcName.mid(4, funcName.indexOf('(')-4);

        if (currentLine.indexOf("(self") == -1) {
          break;
        }
      }

      if (classRegexp.indexIn(currentLine.trimmed()) != -1) {
        className = currentLine.trimmed();

        if (className.indexOf('(') != -1)
          className = className.mid(6, className.indexOf('(')-6);
        else
          className = className.mid(6, className.indexOf(':')-6);

        break;
      }

      if (!(currentLine.startsWith("\t") || currentLine.startsWith(" "))) {
        break;
      }
    }
  }

  if (funcName != "" && className != "")
    funcName = className + "." + funcName;

  return funcName;
}

void PythonCodeEditor::setCursorPosition(int line, int col) {
  QTextCursor cursor = textCursor();
  cursor.setPosition(document()->findBlockByNumber(line).position()+col);
  setTextCursor(cursor);
}

void PythonCodeEditor::getCursorPosition(int &line, int &col) const {
  QTextCursor cursor = textCursor();
  line = cursor.blockNumber();
  col = textCursor().position() - textCursor().block().position();
}

void PythonCodeEditor::setSelection(int startLine, int startCol, int endLine, int endCol) {
  setCursorPosition(startLine, startCol);
  QTextCursor cursor = textCursor();
  cursor.setPosition(document()->findBlockByNumber(endLine).position()+endCol, QTextCursor::KeepAnchor);
  setTextCursor(cursor);
}

void PythonCodeEditor::getSelection(int &lineFrom, int &indexFrom, int &lineTo, int &indexTo) const {
  QTextCursor cursor = textCursor();
  QTextBlock blockStart = document()->findBlock(cursor.selectionStart());
  QTextBlock blockEnd = document()->findBlock(cursor.selectionEnd());
  lineFrom = blockStart.blockNumber();
  indexFrom = cursor.selectionStart() - blockStart.position();
  lineTo = blockEnd.blockNumber();
  indexTo = cursor.selectionEnd() - blockEnd.position();
}

void PythonCodeEditor::removeSelectedText() {
  textCursor().removeSelectedText();
}

int PythonCodeEditor::lines() const {
  return document()->blockCount();
}

int PythonCodeEditor::lineLength(int lineNumber) const {
  return document()->findBlockByNumber(lineNumber).text().length();
}

bool PythonCodeEditor::hasSelectedText() const {
  return textCursor().selectedText() != "";
}

QString PythonCodeEditor::selectedText() const {
  return textCursor().selectedText();
}

void PythonCodeEditor::insertAt(QString text, int line, int col) {
  setCursorPosition(line, col);
  textCursor().insertText(text);
}

void PythonCodeEditor::insertFromMimeData(const QMimeData * source) {
  textCursor().insertText(source->text());
}

void PythonCodeEditor::showTooltip(int line, int col, const QString &text) {
  if (text == "")
    return;

  tooltipActive = true;
  toolTipPos = QPoint(line, col);
  toolTipText = text;
  viewport()->update();
}

void PythonCodeEditor::hideTooltip() {
  tooltipActive = false;
  toolTipFunc = "";
  viewport()->update();
}

bool PythonCodeEditor::isTooltipActive() const {
  return tooltipActive;
}

void PythonCodeEditor::commentSelectedCode() {
  if (hasSelectedText()) {
    int lineFrom = 0;
    int indexFrom = 0;
    int lineTo = 0;
    int indexTo = 0;
    getSelection(lineFrom, indexFrom, lineTo, indexTo);

    for (int i = lineFrom ; i <= lineTo ; ++i) {
      insertAt("#", i, 0);
    }

    setSelection(lineFrom, 0, lineTo, lineLength(lineTo));
  }
  else {
    QTextCursor currentCursor = textCursor();
    insertAt("#", currentCursor.blockNumber(), 0);
    setTextCursor(currentCursor);
  }
}

void PythonCodeEditor::uncommentSelectedCode() {
  if (hasSelectedText()) {
    int lineFrom = 0;
    int indexFrom = 0;
    int lineTo = 0;
    int indexTo = 0;
    getSelection(lineFrom, indexFrom, lineTo, indexTo);

    for (int i = lineFrom ; i <= lineTo ; ++i) {
      QString lineTxt = document()->findBlockByNumber(i).text();

      for (int j = 0 ; j < lineTxt.length() ; ++j) {
        if (lineTxt[j].isSpace()) {
          continue;
        }
        else {
          setSelection(i, j, i, j+1);
          break;
        }
      }

      if (selectedText() == "#") {
        removeSelectedText();
      }
    }

    setSelection(lineFrom, 0, lineTo, lineLength(lineTo));
  }
  else {
    QTextCursor currentCursor = textCursor();
    QString lineTxt = currentCursor.block().text();

    for (int j = 0 ; j < lineTxt.length() ; ++j) {
      if (lineTxt[j].isSpace()) {
        continue;
      }
      else {
        setSelection(currentCursor.blockNumber(), j, currentCursor.blockNumber(), j+1);
        break;
      }
    }

    if (selectedText() == "#") {
      removeSelectedText();
    }

    setTextCursor(currentCursor);
  }
}

void PythonCodeEditor::indentSelectedCode() {
  if (hasSelectedText()) {
    int lineFrom = 0;
    int indexFrom = 0;
    int lineTo = 0;
    int indexTo = 0;
    getSelection(lineFrom, indexFrom, lineTo, indexTo);

    for (int i = lineFrom ; i <= lineTo ; ++i) {
      insertAt("\t", i, 0);
    }

    setSelection(lineFrom, 0, lineTo, lineLength(lineTo));
  }
  else {
    QTextCursor currentCursor = textCursor();
    insertAt("\t", currentCursor.blockNumber(), 0);
    setTextCursor(currentCursor);
  }
}
void PythonCodeEditor::unindentSelectedCode() {
  if (hasSelectedText()) {
    int lineFrom = 0;
    int indexFrom = 0;
    int lineTo = 0;
    int indexTo = 0;
    getSelection(lineFrom, indexFrom, lineTo, indexTo);

    for (int i = lineFrom ; i <= lineTo ; ++i) {
      setSelection(i, 0, i, 1);

      if (selectedText() == "\t") {
        removeSelectedText();
      }
      else {
        break;
      }
    }

    setSelection(lineFrom, 0, lineTo, lineLength(lineTo));
  }
  else {
    QTextCursor currentCursor = textCursor();

    setSelection(currentCursor.blockNumber(), 0, currentCursor.blockNumber(), 1);

    if (selectedText() == "\t") {
      removeSelectedText();
    }

    setTextCursor(currentCursor);
  }
}

void PythonCodeEditor::setGraph(tlp::Graph *graph) {
  autoCompletionDb->setGraph(graph);
}
