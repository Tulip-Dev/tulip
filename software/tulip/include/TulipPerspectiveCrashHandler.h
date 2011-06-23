#ifndef TULIPPERSPECTIVECRASHHANDLER_H
#define TULIPPERSPECTIVECRASHHANDLER_H

#include <QtGui/QDialog>
#include "TulipPerspectiveProcessHandler.h"

#ifdef USE_GOOGLE_BREAKPAD
class FormPost;
#endif

namespace Ui {
class TulipPerspectiveCrashHandlerData;
}

class TulipPerspectiveCrashHandler : public QDialog {
  Q_OBJECT

  Ui::TulipPerspectiveCrashHandlerData *_ui;
  PerspectiveProcessInfos _perspectiveInfos;
  bool _isDetailedView;
  QString _reportPath;

#ifdef USE_GOOGLE_BREAKPAD
  FormPost *_poster;
#endif

  void adjustHeight();

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
#ifdef USE_GOOGLE_BREAKPAD
  void setEnvData(const QString &plateform, const QString &arch, const QString &compiler, const QString &version, const QString &dumpFile);
#endif
  void setPerspectiveData(PerspectiveProcessInfos infos);
};

#endif // TULIPPERSPECTIVECRASHHANDLER_H
