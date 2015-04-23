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

#include <tulip/PythonInterpreter.h>
#include <tulip/PythonCodeEditor.h>

#include <QFile>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QTextBlock>
#include <QWebView>



#include "PythonScriptViewWidget.h"
#include "PythonScriptView.h"
#include "ui_PythonScriptViewWidget.h"

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

PythonScriptViewWidget::PythonScriptViewWidget(PythonScriptView *view, QWidget *parent) : QWidget(parent), _ui(new Ui::PythonScriptViewWidget),  _pythonScriptView(view) {
  _ui->setupUi(this);
  _ui->tabWidget->setTextColor(QColor(200, 200, 200));
  _ui->consoleOutputWidget->installEventFilter(&keyboardFocusEventFilter);
  _mainScriptToolBar = new QToolBar(_ui->mainScriptToolBarWidget);
  _newMainScriptAction = _mainScriptToolBar->addAction(QIcon(":/icons/doc_new.png"), "New main script");
  _loadMainScriptAction = _mainScriptToolBar->addAction(QIcon(":/icons/doc_import.png"), "Load main script from file");
  _saveMainScriptAction = _mainScriptToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save main script to file");

  _modulesToolBar = new QToolBar(_ui->modulesToolBarWidget);
  _newStringModuleAction = _modulesToolBar->addAction(QIcon(":/icons/doc_plus.png"), "New string module");
  _newFileModuleAction = _modulesToolBar->addAction(QIcon(":/icons/doc_new.png"), "New file module");
  _loadModuleAction = _modulesToolBar->addAction(QIcon(":/icons/doc_import.png"), "Import module from file");
  _saveModuleAction = _modulesToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save module to file");

  _ui->modulesTabWidget->clear();
  _ui->mainScriptsTabWidget->clear();
  QList<int> sizes;
  sizes.push_back(550);
  sizes.push_back(150);
  _ui->splitter->setSizes(sizes);
  _ui->splitter->setCollapsible(0, false);

  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(resizeToolBars()));
  connect(_ui->decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
  connect(_ui->consoleOutputWidget, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(scrollToEditorLine(const QUrl &)));
  connect(_ui->runScriptButton, SIGNAL(clicked()), view, SLOT(executeCurrentScript()));
  connect(_ui->pauseScriptButton, SIGNAL(clicked()), view, SLOT(pauseCurrentScript()));
  connect(_ui->stopScriptButton, SIGNAL(clicked()), view, SLOT(stopCurrentScript()));
  connect(_newMainScriptAction, SIGNAL(triggered()), view, SLOT(newScript()));
  connect(_loadMainScriptAction, SIGNAL(triggered()), view, SLOT(loadScript()));
  connect(_saveMainScriptAction, SIGNAL(triggered()), view, SLOT(saveScript()));
  connect(_newStringModuleAction, SIGNAL(triggered()), view, SLOT(newStringModule()));
  connect(_newFileModuleAction, SIGNAL(triggered()), view, SLOT(newFileModule()));
  connect(_loadModuleAction, SIGNAL(triggered()), view, SLOT(loadModule()));
  connect(_saveModuleAction, SIGNAL(triggered()), view, SLOT(saveModule()));
  connect(_ui->modulesTabWidget, SIGNAL(fileSaved(int)), view, SLOT(checkErrors()));
  connect(_ui->modulesTabWidget, SIGNAL(filesReloaded()), _ui->mainScriptsTabWidget, SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->mainScriptsTabWidget, SIGNAL(filesReloaded()), _ui->modulesTabWidget, SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->modulesTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this, SLOT(closeModuleTabRequested(int)));
  connect(_ui->mainScriptsTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this, SLOT(closeScriptTabRequested(int)));
}

PythonScriptViewWidget::~PythonScriptViewWidget() {
  // ensure all updated scripts have been saved
  for (int i = 0 ; i < _ui->mainScriptsTabWidget->count() ; ++i)
    closeScriptTabRequested(i);

  for (int i = 0 ; i < _ui->modulesTabWidget->count() ; ++i)
    closeModuleTabRequested(i);

  delete _ui;
}

void PythonScriptViewWidget::resizeEvent(QResizeEvent *e) {
  QWidget::resizeEvent(e);
  resizeToolBars();
}

void PythonScriptViewWidget::showEvent(QShowEvent *e) {
  QWidget::showEvent(e);
  resizeToolBars();
}

tlp::PythonEditorsTabWidget *PythonScriptViewWidget::getScriptsTabWidget() const {
  return _ui->mainScriptsTabWidget;
}

tlp::PythonEditorsTabWidget *PythonScriptViewWidget::getModulesTabWidget() const {
  return _ui->modulesTabWidget;
}

void PythonScriptViewWidget::resizeToolBars() {
  _modulesToolBar->resize(_ui->modulesToolBarWidget->size());
  _mainScriptToolBar->resize(_ui->mainScriptToolBarWidget->size());
}

void PythonScriptViewWidget::indicateErrors(const QMap<QString, QVector<int> > &errorLines) {
  _ui->mainScriptsTabWidget->indicateErrors(errorLines);
  _ui->modulesTabWidget->indicateErrors(errorLines);
}

void PythonScriptViewWidget::clearErrorIndicators() {
  _ui->mainScriptsTabWidget->clearErrorIndicators();
  _ui->modulesTabWidget->clearErrorIndicators();
}

QTextBrowser *PythonScriptViewWidget::consoleWidget() const {
  return _ui->consoleOutputWidget;
}

QToolButton *PythonScriptViewWidget::runScriptButton() const {
  return _ui->runScriptButton;
}

QToolButton *PythonScriptViewWidget::pauseScriptButton() const {
  return _ui->pauseScriptButton;
}

QToolButton *PythonScriptViewWidget::stopScriptButton() const {
  return _ui->stopScriptButton;
}

QLabel *PythonScriptViewWidget::scriptStatusLabel() const {
  return _ui->scriptStatusLabel;
}

QProgressBar *PythonScriptViewWidget::progressBar() const {
  return _ui->progressBar;
}

void PythonScriptViewWidget::setMainTabWidgetIndex(int idx) {
  _ui->tabWidget->setCurrentIndex(idx);
}

int PythonScriptViewWidget::numberOfScriptEditors() const {
  return _ui->mainScriptsTabWidget->count();
}

int PythonScriptViewWidget::numberOfModulesEditors() const {
  return _ui->modulesTabWidget->count();
}

void PythonScriptViewWidget::setCurrentScriptEditor(int idx) {
  _ui->mainScriptsTabWidget->setCurrentIndex(idx);
}

void PythonScriptViewWidget::setCurrentModuleEditor(int idx) {
  _ui->modulesTabWidget->setCurrentIndex(idx);
}

QString PythonScriptViewWidget::getScriptEditorTabText(int idx) const {
  return _ui->mainScriptsTabWidget->tabText(idx);
}

QString PythonScriptViewWidget::getModuleEditorTabText(int idx) const {
  return _ui->modulesTabWidget->tabText(idx);
}

void PythonScriptViewWidget::setScriptEditorTabText(int idx, const QString &tabText) {
  _ui->mainScriptsTabWidget->setTabText(idx, tabText);
}

void PythonScriptViewWidget::setModuleEditorTabText(int idx, const QString &tabText) {
  _ui->modulesTabWidget->setTabText(idx, tabText);
}

void PythonScriptViewWidget::setScriptEditorTabToolTip(int idx, const QString &tooltip) {
  _ui->mainScriptsTabWidget->setTabToolTip(idx, tooltip);
}

void PythonScriptViewWidget::setModuleEditorTabToolTip(int idx, const QString &tooltip) {
  _ui->modulesTabWidget->setTabToolTip(idx, tooltip);
}

int PythonScriptViewWidget::addMainScriptEditor(const QString &fileName) {
  int idx = _ui->mainScriptsTabWidget->addEditor(fileName);
  getMainScriptEditor(idx)->installEventFilter(_pythonScriptView);
  getMainScriptEditor(idx)->getAutoCompletionDb()->setGraph(_pythonScriptView->getGraph());
  return idx;
}

int PythonScriptViewWidget::addModuleEditor(const QString &fileName) {
  int idx = _ui->modulesTabWidget->addEditor(fileName);
  getModuleEditor(idx)->installEventFilter(_pythonScriptView);
  getModuleEditor(idx)->getAutoCompletionDb()->setGraph(_pythonScriptView->getGraph());
  return idx;
}

int PythonScriptViewWidget::getCurrentMainScriptEditorIndex() const {
  return _ui->mainScriptsTabWidget->currentIndex();
}

int PythonScriptViewWidget::getCurrentModuleEditorIndex() const {
  return _ui->modulesTabWidget->currentIndex();
}

PythonCodeEditor *PythonScriptViewWidget::getCurrentMainScriptEditor() const {
  return _ui->mainScriptsTabWidget->getCurrentEditor();
}

PythonCodeEditor *PythonScriptViewWidget::getCurrentModuleEditor() const {
  return _ui->modulesTabWidget->getCurrentEditor();
}

PythonCodeEditor *PythonScriptViewWidget::getMainScriptEditor(int idx) const {
  return _ui->mainScriptsTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonScriptViewWidget::getModuleEditor(int idx) const {
  return _ui->modulesTabWidget->getEditor(idx);
}

void PythonScriptViewWidget::decreaseFontSize() {
  _ui->mainScriptsTabWidget->decreaseFontSize();
  _ui->modulesTabWidget->decreaseFontSize();
}

void PythonScriptViewWidget::increaseFontSize() {
  _ui->mainScriptsTabWidget->increaseFontSize();
  _ui->modulesTabWidget->increaseFontSize();
}

void PythonScriptViewWidget::currentTabChanged(int index) {
  static int lastTabIndex = 0;
  static QList<int> lastSizes = _ui->splitter->sizes();

  if (lastTabIndex < 2) {
    lastSizes = _ui->splitter->sizes();
  }

  QList<int> sizes;

  if (index >= 2) {
    sizes.push_back(height());
    sizes.push_back(0);
    _ui->runScriptButton->setEnabled(false);
  }
  else {
    _ui->runScriptButton->setEnabled(true);
    sizes = lastSizes;
  }

  _ui->splitter->setSizes(sizes);

  if (index >= 2) {
    _ui->scriptControlFrame->hide();
  }
  else {
    _ui->scriptControlFrame->show();
  }

  lastTabIndex = index;
}

void PythonScriptViewWidget::closeEditorTabRequested(PythonEditorsTabWidget* tabWidget, int idx) {
  QString curTabText = tabWidget->tabText(idx);

  if (curTabText == "")
    return;

  if (!curTabText.contains("no file") && curTabText[curTabText.size() -1] == '*') {
    PythonCodeEditor* editor = tabWidget->getEditor(idx);

    if (QMessageBox::question(QApplication::activeWindow(),
                              QString("Save edited Python code"),
                              QString("The code of ") +
                              editor->getFileName() + QString("\n has been edited but has not been saved.\nDo you want to save it ?"),
                              QMessageBox::Save | QMessageBox::Discard,
                              QMessageBox::Save) ==
        QMessageBox::Save)
      editor->saveCodeToFile();
  }
}

void PythonScriptViewWidget::closeModuleTabRequested(int idx) {
  closeEditorTabRequested(_ui->modulesTabWidget, idx);
}

void PythonScriptViewWidget::closeScriptTabRequested(int idx) {
  closeEditorTabRequested(_ui->mainScriptsTabWidget, idx);
}

void PythonScriptViewWidget::setGraph(tlp::Graph *graph) {
  for (int i = 0 ; i < _ui->mainScriptsTabWidget->count() ; ++i) {
    getMainScriptEditor(i)->getAutoCompletionDb()->setGraph(graph);
  }

  for (int i = 0 ; i < _ui->modulesTabWidget->count() ; ++i) {
    getModuleEditor(i)->getAutoCompletionDb()->setGraph(graph);
  }

}

void PythonScriptViewWidget::scrollToEditorLine(const QUrl & link) {
  QString linkStr = QUrl::fromPercentEncoding(link.toEncoded());
#ifdef WIN32
  linkStr.replace("\\", "/");
#endif
  QStringList strList = linkStr.split(":");
  QString file = strList.at(0);

  for (int i = 1 ; i < strList.size() - 1 ; ++i) {
    file += (":" + strList.at(i));
  }

  int line = strList.at(strList.size() - 1).toInt()-1;

  if (file == "<unnamed script>") {
    _ui->tabWidget->setCurrentIndex(0);
    getCurrentMainScriptEditor()->scrollToLine(line);
    return;
  }

// Qt5 on windows sets the drive letter as lowercase when converting the url to a string
// Resets it to uppercase as it was originally
#if defined(WIN32) && (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  file[0] = file[0].toUpper();
#endif

  for (int i = 0 ; i < _ui->mainScriptsTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = getMainScriptEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(0);
      _ui->mainScriptsTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }

  for (int i = 0 ; i < _ui->modulesTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = getModuleEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(1);
      _ui->modulesTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }

}
