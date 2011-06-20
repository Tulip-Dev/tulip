#ifndef TULIPPERSPECTIVEPROCESSHANDLER_H
#define TULIPPERSPECTIVEPROCESSHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QProcess>

#include <QtGui/QPushButton>


// Some utility class used in crash handler dialog designer form
class SelectionButton: public QPushButton {
public:
  explicit SelectionButton(QWidget *parent=0);
  void paintEvent(QPaintEvent *e);
};

struct PerspectiveProcessInfos {
  PerspectiveProcessInfos() {}

  PerspectiveProcessInfos(const QString name, const QVariantMap &args, const QString &file):
    name(name), args(args), file(file) {
  }

  QString name;
  QVariantMap args;
  QString file;
  QString projectPath;
};

class TulipPerspectiveProcessHandler: public QObject {
  Q_OBJECT

  QMap<QProcess *, PerspectiveProcessInfos> _processInfos;

  static TulipPerspectiveProcessHandler *_instance;
  TulipPerspectiveProcessHandler();

public:
  static TulipPerspectiveProcessHandler &instance();

public slots:
  void createPerspective(const QString &perspective, const QString &file, const QVariantMap &parameters);
  void enableCrashHandling(qlonglong perspectivePid, const QString &perspectiveProjectPath);

protected slots:
  void perspectiveCrashed(QProcess::ProcessError);
  void perspectiveFinished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif // TULIPPERSPECTIVEPROCESSHANDLER_H
