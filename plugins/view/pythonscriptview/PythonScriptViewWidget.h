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

#ifndef PYTHONSCRIPTVIEWWIDGET_H_
#define PYTHONSCRIPTVIEWWIDGET_H_

#include <QWidget>
#include <QUrl>

namespace Ui {
class PythonScriptViewWidget;
}

namespace tlp {
class PythonCodeEditor;
class Graph;
class PythonEditorsTabWidget;
}

class QToolBar;
class QAction;
class PythonScriptView;
class QTextBrowser;
class QToolButton;
class QLabel;
class QProgressBar;

class PythonScriptViewWidget : public QWidget {

  Q_OBJECT

  Ui::PythonScriptViewWidget *_ui;
  PythonScriptView *_pythonScriptView;
  QToolBar *_mainScriptToolBar;
  QToolBar *_modulesToolBar;
  QAction *_newMainScriptAction;
  QAction *_loadMainScriptAction;
  QAction *_saveMainScriptAction;
  QAction *_newStringModuleAction;
  QAction *_newFileModuleAction;
  QAction *_loadModuleAction;
  QAction *_saveModuleAction;


public :

  PythonScriptViewWidget(PythonScriptView *view, QWidget *parent=NULL);
  ~PythonScriptViewWidget();
  void showEvent(QShowEvent *);
  void resizeEvent(QResizeEvent *);

  void setMainTabWidgetIndex(int idx);

  tlp::PythonEditorsTabWidget *getScriptsTabWidget() const;
  tlp::PythonEditorsTabWidget *getModulesTabWidget() const;

  int addMainScriptEditor(const QString &fileName="");
  int addModuleEditor(const QString &fileName="");

  int numberOfScriptEditors() const;
  int numberOfModulesEditors() const;

  int getCurrentMainScriptEditorIndex() const;
  int getCurrentModuleEditorIndex() const;
  tlp::PythonCodeEditor *getMainScriptEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentMainScriptEditor() const;
  tlp::PythonCodeEditor *getModuleEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentModuleEditor() const;

  void setCurrentScriptEditor(int idx);
  void setCurrentModuleEditor(int idx);

  QString getScriptEditorTabText(int idx) const;
  QString getModuleEditorTabText(int idx) const;
  void setScriptEditorTabText(int idx, const QString &tabText);
  void setModuleEditorTabText(int idx, const QString &tabText);

  void setScriptEditorTabToolTip(int idx, const QString &tooltip);
  void setModuleEditorTabToolTip(int idx, const QString &tooltip);

  void setGraph(tlp::Graph *graph);

  void indicateErrors(const QMap<QString, QVector<int> > &errorLines);

  void clearErrorIndicators();

  QTextBrowser *consoleWidget() const;
  QToolButton *runScriptButton() const;
  QToolButton *pauseScriptButton() const;
  QToolButton *stopScriptButton() const;
  QLabel *scriptStatusLabel() const;
  QProgressBar *progressBar() const;

public slots :

  void decreaseFontSize();
  void increaseFontSize();
  void resizeToolBars();
  void currentTabChanged(int index);
  void closeEditorTabRequested(tlp::PythonEditorsTabWidget* tabWidget, int index);
  void closeModuleTabRequested(int index);
  void closeScriptTabRequested(int index);
  void scrollToEditorLine(const QUrl & link);

};

#endif /* PYTHONSCRIPTVIEWWIDGET_H_ */
