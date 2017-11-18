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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
  Q_OBJECT

  Ui::PreferencesDialog *_ui;

public:
  explicit PreferencesDialog(QWidget *parent = 0);
  ~PreferencesDialog();

public slots:
  void writeSettings();
  void readSettings();
  void cellChanged(int row, int column);
  void randomSeedCheckChanged(int);
  void showGraphDefaultsContextMenu(const QPoint &);

private slots:
  void usetlpbformat(int state);
  void resetToTulipDefaults(int row = -1, int updateMode = 0);
};

#endif // PREFERENCESDIALOG_H
