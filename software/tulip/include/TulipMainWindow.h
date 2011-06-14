#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>
#include <QtCore/QVariant>

namespace Ui {
class TulipMainWindowData;
}
class PluginsCenter;

/**
  @brief The window used in the tulip_app process to display the Welcome, plugins center and getting help screens.
  This class is basically a controller for the 3 underleying screens that manage their own logic.
  */
class TulipMainWindow: public QMainWindow {
  Q_OBJECT

public:
  TulipMainWindow(QWidget *parent=0);
  virtual ~TulipMainWindow();

  PluginsCenter *pluginsCenter() const;

public slots:
  void startApp();
  void closeApp();

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void systemTrayMessageClicked();
  void pageChooserClicked();
  void pageSwitched(int);

private:
  enum SystemTrayMessage {
    NoMessage,
    PluginErrorMessage
  };

  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
  SystemTrayMessage _currentTrayMessage;

  QVector<QObject *> _pageChoosers;

  void runPerspectiveProcess(const QString &perspective,const QString &file, const QVariantMap &parameters);

// Methods and properties coming from the D-Bus service implementation
public: // PROPERTIES
    Q_PROPERTY(qlonglong pid READ pid)
    qlonglong pid() const;
public Q_SLOTS: // METHODS
    void AddPluginRepository(const QString &url);
    void CreatePerspective(const QString &name);
    void CreatePerspective(const QString &name, const QVariantMap &parameters);
    QStringList GetCompatiblePerspectives(const QString &file);
    void OpenProject(const QString &file);
    void OpenProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters);
    void RemovePluginRepository(const QString &url);
    void ShowAboutPage();
    void ShowOpenProjectWindow();
    void ShowPluginsCenter();
    void ShowWelcomeScreen();
Q_SIGNALS: // SIGNALS
    void Terminate();
};

#endif // TULIPMAINWINDOW_H
