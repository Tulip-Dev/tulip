#ifndef TULIPAGENTSERVICE_H
#define TULIPAGENTSERVICE_H

#include <QtCore/QObject>

class TulipAgentService: public QObject {
  Q_OBJECT
public:
  explicit TulipAgentService(QObject *parent=0);

  Q_PROPERTY(QString pid READ pid)
  QString pid() const;

public Q_SLOTS: // METHODS
  void ShowAboutPage();
  void ShowPluginsCenter();
  void ShowWelcomeScreen();
};

#endif // TULIPAGENTSERVICE_H
