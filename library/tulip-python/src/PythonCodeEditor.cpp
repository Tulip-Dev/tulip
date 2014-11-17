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
#include "tulip/PythonCodeEditor.h"
#include "tulip/PythonCodeHighlighter.h"
#include "tulip/ParenMatcherHighlighter.h"

#include <QTextStream>
#include <QPainter>
#include <QTextBlock>
#include <QApplication>
#include <QMainWindow>
#include <QToolTip>
#include <QScrollBar>
#include <QMessageBox>
#include <QMimeData>
#include <QDesktopWidget>
#include <QFileInfo>

#include "ui_FindReplaceDialog.h"

using namespace std;
using namespace tlp;

class GragKeyboardFocusEventFilter : public QObject {
public :
  bool eventFilter(QObject *, QEvent *event) {
    if (event->type() == QEvent::ShortcutOverride) {
      event->accept();
      return true;
    }

    return false;
  }
};

static GragKeyboardFocusEventFilter keyboardFocusEventFilter;

static char sepChar[] = {' ', '\t', '=', '(', '[', '{' , ',', '*', '+', '/', '^', '-', 0};

namespace tlp {

class LineNumberArea : public QWidget {
public:
  LineNumberArea(PythonCodeEditor *editor) : QWidget(editor) {
    codeEditor = editor;
  }

  QSize sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

private:
  PythonCodeEditor *codeEditor;
};

}

AutoCompletionList::AutoCompletionList(PythonCodeEditor *parent) : QListWidget(parent), _codeEditor(parent) {
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
  installEventFilter(&keyboardFocusEventFilter);
  _activated = false;
  _wasActivated = false;
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
    insertSelectedItem();
  }
  else {
    QCoreApplication::sendEvent(_codeEditor, e);
  }
}

void AutoCompletionList::mouseDoubleClickEvent(QMouseEvent *) {
  insertSelectedItem();
}

void AutoCompletionList::insertSelectedItem() {
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

    QString type = _codeEditor->_autoCompletionDb->getLastFoundType();

    if (type != "") {
      QVector<QString> types;
      types.push_back(type);
      QVector<QString> baseTypes = PythonInterpreter::getInstance()->getBaseTypesForType(type);

      for (int i = 0 ; i < baseTypes.size() ; ++i) {
        types.push_back(baseTypes[i]);
      }

      for (int i = 0 ; i < types.size() ; ++i) {
        QString funcName = types[i] + "." + textToInsert;

        if (APIDataBase::getInstance()->functionExists(funcName)) {
          QVector<QVector<QString> > params = APIDataBase::getInstance()->getParamTypesForMethodOrFunction(funcName);

          if (params.count() > 1 || params[0].count() > 0) {
            if (text.indexOf("class ") == -1)
              qApp->sendEvent(_codeEditor, new QKeyEvent(QEvent::KeyPress, Qt::Key_ParenLeft, Qt::NoModifier, "("));
          }
          else {
            cursor.insertText("()");
          }

          break;
        }
      }
    }
  }

  _codeEditor->setFocus();
}

void AutoCompletionList::showEvent (QShowEvent * event) {
  QListWidget::showEvent(event);
  grabKeyboard();
  _activated = true;
}

void AutoCompletionList::hideEvent (QHideEvent * event) {
  QListWidget::hideEvent(event);
  releaseKeyboard();
  _codeEditor->setFocus();
  _activated = false;
}


bool AutoCompletionList::eventFilter(QObject *, QEvent *event) {
  if (!_wasActivated && (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide)) {
    _wasActivated = _activated;
    hide();
  }
  else if (event->type() == QEvent::WindowActivate || event->type() == QEvent::Show) {
    if (_wasActivated) {
      show();
      _wasActivated = false;
    }
  }

  if (isVisible())
    _codeEditor->updateAutoCompletionListPosition();

  return false;
}

FindReplaceDialog::FindReplaceDialog(QPlainTextEdit *editor, QWidget *parent) : QDialog(parent), _ui(new Ui::FindReplaceDialogData), _editor(editor) {
  _ui->setupUi(this);
  connect(_ui->findButton, SIGNAL(clicked()), this, SLOT(doFind()));
  connect(_ui->replaceButton, SIGNAL(clicked()), this, SLOT(doReplace()));
  connect(_ui->replaceFindButton, SIGNAL(clicked()), this, SLOT(doReplaceFind()));
  connect(_ui->replaceAllButton, SIGNAL(clicked()), this, SLOT(doReplaceAll()));
  connect(_ui->closeButton, SIGNAL(clicked()), this, SLOT(hide()));
  connect(_ui->forwardRB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->backwardRB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->regexpCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->regexpCB, SIGNAL(toggled(bool)), this, SLOT(regexpToggled(bool)));
  connect(_ui->wholeWordCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->wrapSearchCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->caseSensitiveCB, SIGNAL(toggled(bool)), this, SLOT(setResetSearch()));
  connect(_ui->textToFind, SIGNAL(textChanged(const QString &)), this, SLOT(textToFindChanged()));
}

FindReplaceDialog::~FindReplaceDialog() {
  delete _ui;
}

void FindReplaceDialog::setTextToFind(const QString &text) {
  _ui->textToFind->setText(text);
}

void FindReplaceDialog::textToFindChanged() {
  _ui->replaceButton->setEnabled(false);
  _ui->replaceFindButton->setEnabled(false);
  QString text = _ui->textToFind->text();

  if (text == "") {
    _ui->findButton->setEnabled(false);
    _ui->replaceAllButton->setEnabled(false);
  }
  else {
    _ui->findButton->setEnabled(true);
    _ui->replaceAllButton->setEnabled(true);
  }
}

void FindReplaceDialog::regexpToggled(bool toggled) {
  _ui->wholeWordCB->setEnabled(!toggled);
}

void FindReplaceDialog::setFindMode(const bool findMode) {
  if (findMode) {
    _ui->textToFind->setFocus();
  }
  else {
    _ui->replaceText->setFocus();
  }
}

void FindReplaceDialog::setSearchResult(const bool result) {
  _ui->replaceButton->setEnabled(result);
  _ui->replaceFindButton->setEnabled(result);

  if (!result) {
    _ui->searchStatusLabel->setText("String Not Found");
  }
  else {
    _ui->searchStatusLabel->setText("");
  }
}

bool FindReplaceDialog::doFind() {
  QString text = _ui->textToFind->text();

  if (text == "")
    return false;


  QTextDocument::FindFlags findFlags;

  if (!_ui->forwardRB->isChecked()) {
    findFlags |= QTextDocument::FindBackward;
  }

  if (_ui->caseSensitiveCB->isChecked()) {
    findFlags |= QTextDocument::FindCaseSensitively;
  }

  if (_ui->wholeWordCB->isChecked()) {
    findFlags |= QTextDocument::FindWholeWords;
  }

  QTextCursor sel;

  if (!_ui->regexpCB->isChecked()) {
    sel = _editor->document()->find(text, _editor->textCursor(), findFlags);
  }
  else {
    sel = _editor->document()->find(QRegExp(text), _editor->textCursor(), findFlags);
  }

  bool ret = !sel.isNull();

  if (ret) {
    _editor->setTextCursor(sel);
  }
  else if (_ui->wrapSearchCB->isChecked()) {
    QTextCursor cursor = _editor->textCursor();

    if (!_ui->backwardRB->isChecked()) {
      cursor.movePosition(QTextCursor::Start);
    }
    else {
      cursor.movePosition(QTextCursor::End);
    }

    if (!_ui->regexpCB->isChecked()) {
      sel = _editor->document()->find(text, cursor, findFlags);
    }
    else {
      sel = _editor->document()->find(QRegExp(text), cursor, findFlags);
    }

    ret = !sel.isNull();

    if (ret) {
      _editor->setTextCursor(sel);
    }
  }

  setSearchResult(ret);
  _lastSearch = text;
  return ret;
}

bool FindReplaceDialog::doReplace() {
  QString text = _ui->textToFind->text();

  if (text == "")
    return false;

  QString selection = _editor->textCursor().selectedText();

  if (!_ui->caseSensitiveCB->isChecked()) {
    selection = selection.toLower();
    text = text.toLower();
  }

  if (selection == text) {
    _editor->textCursor().removeSelectedText();
    _editor->textCursor().insertText(_ui->replaceText->text());
    return true;
  }

  return false;
}

void FindReplaceDialog::doReplaceFind() {
  if (doReplace())
    doFind();
}

void FindReplaceDialog::doReplaceAll() {
  QString text = _ui->textToFind->text();

  if (text == "")
    return;

  bool ret = doFind();

  if (ret) {
    int startLine = _editor->textCursor().blockNumber();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    int startIndex = _editor->textCursor().positionInBlock();
#else
    int startIndex = editor->textCursor().position() - editor->textCursor().block().position();
#endif
    int nbReplacements = 0;

    while(ret) {
      doReplace();
      ret = doFind();
      ++nbReplacements;
      int line = _editor->textCursor().blockNumber();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
      int index = _editor->textCursor().positionInBlock();
#else
      int index = editor->textCursor().position() - editor->textCursor().block().position();
#endif

      // when replacing a pattern P by a pattern following this regexp .*P.*
      // this can lead to an infinite loop, handle this case
      if (line == startLine && index >= startIndex)
        break;
    }

    _ui->searchStatusLabel->setText(QString::number(nbReplacements) + QString(" matches replaced"));
    _resetSearch = true;
  }
  else {
    setSearchResult(ret);
  }
}

void FindReplaceDialog::hideEvent(QHideEvent *) {
  _editor->setFocus();
}

PythonCodeEditor::PythonCodeEditor(QWidget *parent) : QPlainTextEdit(parent), _highlighter(NULL), _tooltipActive(false) {
  installEventFilter(&keyboardFocusEventFilter);
  setAutoIndentation(true);
  setIndentationGuides(true);
  setHighlightEditedLine(true);
  setFindReplaceActivated(true);
  setCommentShortcutsActivated(true);
  setIndentShortcutsActivated(true);
  setWordWrapMode(QTextOption::NoWrap);
  setFocusPolicy(Qt::ClickFocus);
  QTextCharFormat format = currentCharFormat();
#if defined(WIN32)
  _currentFont.setFamily("Courier New");
  _currentFont.setPointSize(8);
#elif defined(__APPLE__)
  _currentFont.setFamily("Menlo");
  _currentFont.setPointSize(12);
#else
  _currentFont.setFamily("Monospace");
  _currentFont.setPointSize(8);
#endif

  setStyleSheet("QFrame { background-color: white; }"
                "QPlainTextEdit { selection-background-color: #C0C0C0; }");

  format.setFont(_currentFont);
  setCurrentCharFormat(format);

  _lineNumberArea = new LineNumberArea(this);

  updateTabStopWidth();

  updateLineNumberAreaWidth();

  _parenHighlighter = new ParenMatcherHighlighter(document());
  _highlighter = new PythonCodeHighlighter(document());

  _autoCompletionList = new AutoCompletionList(this);
  _autoCompletionDb = new AutoCompletionDataBase(APIDataBase::getInstance());

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
    mainWindow->installEventFilter(_autoCompletionList);

  installEventFilter(_autoCompletionList);

  _findReplaceDialog = new FindReplaceDialog(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
  connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(resetExtraSelections()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightErrors()));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchParens()));
  connect(this, SIGNAL(textChanged()), this, SLOT(updateAutoCompletionList()));
  connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightSelection()));

  _shellWidget = false;
  _moduleEditor = false;
}

PythonCodeEditor::~PythonCodeEditor() {
  delete _autoCompletionDb;
  removeEventFilter(_autoCompletionList);
  delete _autoCompletionList;
}

QString PythonCodeEditor::getCleanCode() const {
  QString code = toPlainText().replace("\r\n", "\n");

  if (code.size() && code[code.size()-1] != '\n')
    code += "\n";

  return  code;
}


QFontMetrics PythonCodeEditor::fontMetrics() const {
  return QFontMetrics(_currentFont);
}

void PythonCodeEditor::indicateScriptCurrentError(int lineNumber) {
  _currentErrorLines.append(lineNumber);
  emit cursorPositionChanged();
}

void PythonCodeEditor::clearErrorIndicator() {
  _currentErrorLines.clear();
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
    _lineNumberArea->scroll(0, dy);
  else
    _lineNumberArea->update(0, rect.y(), _lineNumberArea->width(),
                            rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth();
}

void PythonCodeEditor::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);
  QRect cr = contentsRect();
  _lineNumberArea->setGeometry(
    QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void PythonCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(_lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + static_cast<int>(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.setFont(_currentFont);
      painter.drawText(0, top, _lineNumberArea->width(),
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
  _currentFont.setPointSize(clamp(_currentFont.pointSize()+1, 6, 30));
  format.setFont(_currentFont);
  setCurrentCharFormat(format);
  setTextCursor(cursor);
  updateTabStopWidth();
}

void PythonCodeEditor::zoomOut() {
  QTextCursor cursor = textCursor();
  selectAll();
  QTextCharFormat format = currentCharFormat();
  _currentFont.setPointSize(clamp(_currentFont.pointSize()-1, 6, 30));
  format.setFont(_currentFont);
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
  painter.setFont(_currentFont);

  // draw tooltip

  if (isTooltipActive()) {
    QTextBlock tooltipBlock = document()->findBlockByNumber(_toolTipPos.x());
    int top = static_cast<int>(blockBoundingGeometry(tooltipBlock).translated(contentOffset()).top());
    int left = static_cast<int>(blockBoundingGeometry(tooltipBlock).translated(contentOffset()).left());
    int bottom = top + static_cast<int>(blockBoundingRect(tooltipBlock).height());
    QString blockText = tooltipBlock.text();

    for (int i = 0 ; i < _toolTipPos.y() ; ++i) {
      if (blockText[i] == '\t') {
        left += tabStopWidth();
      }
      else {
        left += fontMetrics().width(QLatin1Char(blockText[i].toLatin1()));
      }


    }

    QStringList toolTipLines = _toolTipText.split("\n");
    int height = toolTipLines.size() *  blockBoundingRect(tooltipBlock).height();
    int width = 0;

    for (int i = 0 ; i < toolTipLines.size() ; ++i) {
      int w = 0;

      for (int j = 0 ; j < toolTipLines[i].length() ; ++j) {
        w += fontMetrics().width(QLatin1Char(toolTipLines[i][j].toLatin1()));
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
    painter.drawText(tooltipRect, _toolTipText);
#else
    painter.drawText(tooltipRect, Qt::AlignLeft, _toolTipText);
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
  _parenHighlighter->rehighlightBlock(textCursor().block());
  _highlighter->rehighlightBlock(textCursor().block());
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

  for (int i = 0 ; i < _currentErrorLines.size() ; ++i) {
    QTextEdit::ExtraSelection selection;
    QTextBlock block = document()->findBlockByNumber(_currentErrorLines.at(i));
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

#ifdef __APPLE__
  Qt::KeyboardModifiers modifier = Qt::MetaModifier;
#else
  Qt::KeyboardModifiers modifier = Qt::ControlModifier;
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
      _findReplaceDialog->setTextToFind(selection);

    _findReplaceDialog->show();
    _findReplaceDialog->raise();
    _findReplaceDialog->activateWindow();
    _findReplaceDialog->setFindMode(true);
  }
  else if (findReplaceActivated() && e->modifiers() == modifier && e->key() == Qt::Key_R) {
    QString selection = textCursor().selectedText();

    if (selection != "")
      _findReplaceDialog->setTextToFind(selection);

    _findReplaceDialog->show();
    _findReplaceDialog->raise();
    _findReplaceDialog->activateWindow();
    _findReplaceDialog->setFindMode(false);
  }
  else if ((e->key() == Qt::Key_Space && e->modifiers() == modifier) || e->text() == ".") {
    if (e->text() == ".")
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
      if (textBeforeCursor.indexOf(_toolTipFunc + "(") == -1) {
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
        type = _autoCompletionDb->findTypeForExpr(cleanContext.mid(0, cleanContext.lastIndexOf('.')), getEditedFunctionName());
        funcName = cleanContext.mid(cleanContext.lastIndexOf('.')+1, cleanContext.lastIndexOf('(') - cleanContext.lastIndexOf('.')-1);
      }
      else {
        type = _autoCompletionDb->findTypeForExpr(cleanContext.mid(0, cleanContext.lastIndexOf('(')), getEditedFunctionName());

        if (type.lastIndexOf(".") != -1) {
          funcName = type.mid(type.lastIndexOf('.')+1);
          type = type.mid(0, type.lastIndexOf('.'));
        }

      }

      if (type != "") {

        QVector<QVector<QString> > params = _autoCompletionDb->getParamTypesForMethodOrFunction(type, funcName);
        QString retType = _autoCompletionDb->getReturnTypeForMethodOrFunction(type, funcName);

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

        _toolTipFunc = funcName;
        showTooltip(textCursor().blockNumber(), textBeforeCursor.indexOf(funcName), toolTipTxt);
      }
    }
    else if (e->text() == ")") {
      hideTooltip();
    }
  }
}

void PythonCodeEditor::wheelEvent(QWheelEvent * event) {
  if (!_autoCompletionList->isVisible()) {
    QPlainTextEdit::wheelEvent(event);
  }
}

void PythonCodeEditor::mouseDoubleClickEvent(QMouseEvent * event) {
  if (!_autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseDoubleClickEvent(event);
  }
}

void PythonCodeEditor::mouseMoveEvent(QMouseEvent * event) {
  if (!_autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseMoveEvent(event);
  }
}

void PythonCodeEditor::mousePressEvent(QMouseEvent * event) {
  if (!_autoCompletionList->isVisible()) {
    QPlainTextEdit::mousePressEvent(event);
  }
}

void PythonCodeEditor::mouseReleaseEvent(QMouseEvent * event) {
  if (!_autoCompletionList->isVisible()) {
    QPlainTextEdit::mouseReleaseEvent(event);
  }
}

void PythonCodeEditor::analyseScriptCode(const bool wholeText) {
  QString moduleName = "";

  if (_moduleEditor) {
    QFileInfo fileInfo(getFileName());
    moduleName = fileInfo.fileName().replace(".py", "");
  }

  if (!wholeText) {
    _autoCompletionDb->analyseCurrentScriptCode(toPlainText(), textCursor().blockNumber(), _shellWidget, moduleName);
  }
  else {
    _autoCompletionDb->analyseCurrentScriptCode(toPlainText(), document()->blockCount(), _shellWidget, moduleName);
  }
}

void PythonCodeEditor::showAutoCompletionList(bool dotContext) {
  analyseScriptCode();
  _autoCompletionList->show();
  updateAutoCompletionList(dotContext);

  if (_autoCompletionList->count() == 0)
    _autoCompletionList->hide();
}

void PythonCodeEditor::updateAutoCompletionListPosition() {

  if (!_autoCompletionList->isVisible())
    return;

  int left = static_cast<int>(blockBoundingGeometry(textCursor().block()).translated(contentOffset()).left());
  int top = static_cast<int>(blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top());
  int bottom = top + static_cast<int>(blockBoundingRect(textCursor().block()).height());
  QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());
  int pos = lineNumberAreaWidth() + left + 1;
  int stop = 0;

  for (int i = textBeforeCursor.length() ; i >=0 ;  --i) {
    if (textBeforeCursor[i] == '\t' || textBeforeCursor[i] == ' ' || textBeforeCursor[i] == '.' ||
        textBeforeCursor[i] == '(' || textBeforeCursor[i] == '[') {
      stop = i+1;
      break;
    }
  }

  for (int i = 0 ; i < stop ; ++i) {
    if (textBeforeCursor[i] == '\t') {
      pos += tabStopWidth();
    }
    else {
      pos += fontMetrics().width(QLatin1Char(textBeforeCursor[i].toLatin1()));
    }
  }

  if (mapToGlobal(QPoint(0, bottom+_autoCompletionList->height())).y() > QApplication::desktop()->screenGeometry().height())
    _autoCompletionList->move(mapToGlobal(QPoint(pos, top-_autoCompletionList->height())));
  else
    _autoCompletionList->move(mapToGlobal(QPoint(pos, bottom)));
}

void PythonCodeEditor::updateAutoCompletionList(bool dotContext) {

  if (!_autoCompletionList->isVisible())
    return;

  updateAutoCompletionListPosition();

  QString textBeforeCursor = textCursor().block().text().mid(0, textCursor().position() - textCursor().block().position());

  _autoCompletionList->clear();

  QString textBeforeCursorTrimmed = textBeforeCursor.trimmed();

  // string litteral edition : don't show autocompletion list
  if (dotContext && (textBeforeCursorTrimmed.count("\"")%2==1 || textBeforeCursorTrimmed.count("\'")%2==1))
    return;

  QSet<QString> stringList = _autoCompletionDb->getAutoCompletionListForContext(textBeforeCursorTrimmed, getEditedFunctionName(), dotContext);

  foreach(QString s, stringList) {
    _autoCompletionList->addItem(s);
  }
  _autoCompletionList->sortItems();

  if (_autoCompletionList->count() == 0)
    _autoCompletionList->hide();
  else
    _autoCompletionList->setCurrentRow(0);

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

void PythonCodeEditor::showTooltip(int line, int col, const QString &text) {
  if (text == "")
    return;

  _tooltipActive = true;
  _toolTipPos = QPoint(line, col);
  _toolTipText = text;
  viewport()->update();
}

void PythonCodeEditor::hideTooltip() {
  _tooltipActive = false;
  _toolTipFunc = "";
  viewport()->update();
}

bool PythonCodeEditor::isTooltipActive() const {
  return _tooltipActive;
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

void PythonCodeEditor::insertFromMimeData(const QMimeData * source) {
  textCursor().insertText(source->text());
}

bool PythonCodeEditor::loadCodeFromFile(const QString &filePath) {
  QFile file(filePath);

  if (!file.exists())
    return false;

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QFileInfo fileInfo(file);

  QString scriptCode;

  while (!file.atEnd()) {
    scriptCode += QString::fromUtf8(file.readLine().data());
  }

  file.close();

  _lastSavedTime = fileInfo.lastModified();

  if (filePath == getFileName() && !toPlainText().isEmpty()) {
    if (scriptCode != getCleanCode() && QMessageBox::question(NULL, "File changed on disk", QString("The file ") + filePath + " has been modified by another editor. Do you want to reload it ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
      setPlainText(scriptCode);
    }
    else {
      return false;
    }
  }
  else {
    setFileName(filePath);
    setPlainText(scriptCode);
  }

  return true;
}

bool PythonCodeEditor::saveCodeToFile() {
  QFile file(getFileName());
  QFileInfo fileInfo(file);

  if (getFileName() == fileInfo.absoluteFilePath() && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << getCleanCode();
    file.close();
    QFileInfo fileInfo(file);
    _lastSavedTime = fileInfo.lastModified();
    return true;
  }

  return false;
}

void PythonCodeEditor::scrollToLine(int line) {
  QTextBlock block = document()->findBlockByLineNumber(line);
  setTextCursor(QTextCursor(block));
  centerCursor();
}

void PythonCodeEditor::setPlainText(const QString &text) {
  QPlainTextEdit::setPlainText(text);
  QTextCursor cursor = textCursor();
  selectAll();
  QTextCharFormat format = currentCharFormat();
  format.setFont(_currentFont);
  setCurrentCharFormat(format);
  setTextCursor(cursor);
  updateTabStopWidth();
}
