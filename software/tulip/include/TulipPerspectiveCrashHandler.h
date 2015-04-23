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
#ifndef TULIPPERSPECTIVECRASHHANDLER_H
#define TULIPPERSPECTIVECRASHHANDLER_H

#include <QDialog>
#include "TulipPerspectiveProcessHandler.h"

class FormPost;

namespace Ui {
class TulipPerspectiveCrashHandlerData;
}

class TulipPerspectiveCrashHandler : public QDialog {
  Q_OBJECT

  Ui::TulipPerspectiveCrashHandlerData *_ui;
  PerspectiveProcessInfos _perspectiveInfos;
  bool _isDetailedView;
  FormPost *_poster;

public:
  explicit TulipPerspectiveCrashHandler(QWidget *parent = 0);

protected slots:
  void reportPosted();

signals:

public slots:
  void setDetailedView(bool);
  void toggleDetailedView();
  void sendReport();
  void saveData();
  void setEnvData(const QString &plateform, const QString &arch, const QString &compiler, const QString &version, const QString &stackTrace);
  void setPerspectiveData(const PerspectiveProcessInfos& infos);
};

#endif // TULIPPERSPECTIVECRASHHANDLER_H
