/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

class PythonPanel : public QWidget {

  Q_OBJECT
  Ui::PythonPanel *_ui;

public:
  explicit PythonPanel(QWidget *parent = nullptr);
  ~PythonPanel() override;
  void setModel(tlp::GraphHierarchiesModel *model);
  void decreaseFontSize();
  void increaseFontSize();

protected:
  void dragEnterEvent(QDragEnterEvent *) override;
  void dropEvent(QDropEvent *) override;

private slots:

  void graphComboIndexChanged();
  void beginCurrentLinesExecution();
  void endCurrentLinesExecution();
};

#endif // PYTHONPANEL_H
