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

#ifndef PYTHONPANEL_H
#define PYTHONPANEL_H

#include <QWidget>


namespace Ui {
class PythonPanel;
}

namespace tlp {
class GraphHierarchiesModel;
}

class QPropertyAnimation;
class QPlainTextEdit;
class OutputPanelButton;

class PythonPanel : public QWidget {


  Q_OBJECT
  Ui::PythonPanel *_ui;
  OutputPanelButton *_button;
  QPropertyAnimation *_animation;

public :

  explicit PythonPanel(QWidget *parent = 0);
  ~PythonPanel();
  void setModel(tlp::GraphHierarchiesModel* model);

  void setPanelButton(OutputPanelButton *button);

  QPlainTextEdit *consoleWidget();

protected:

  void dragEnterEvent(QDragEnterEvent *);
  void dropEvent(QDropEvent *);

private slots:

  void graphComboIndexChanged();

  void clearConsole();

  void newOutputInConsole();

  void stopButtonAnimation();

private:

  void startButtonAnimation();

};

#endif // PYTHONPANEL_H
