#ifndef FACEBOOKIMPORT_H
#define FACEBOOKIMPORT_H

#include <tulip/ImportModule.h>
#include <tulip/TulipPluginHeaders.h>

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtGui/QWidget>

namespace Ui {
    class FacebookConnectWidget;
}

class FacebookConnectWidget : public QWidget {

    Q_OBJECT

public :

    FacebookConnectWidget(QWidget *parent=0);

    Ui::FacebookConnectWidget *_ui;

    std::string getAvatarsDlPath() const;

public slots:

    void pickDirectory();

    void activateDownloadPath(bool activate);

};

class FacebookImport : public QObject, public tlp::ImportModule {

    Q_OBJECT

public :

  PLUGININFORMATIONS("Facebook Import","Antoine LAMBERT","19/01/2013","Import the social network from a Facebook account.","1.0","Social networks")

  FacebookImport(const tlp::PluginContext*  context);

  ~FacebookImport() {}

  std::string icon() const;

  // import plugins must implement bool importGraph()
  bool importGraph();

public slots:

  void urlChanged(const QUrl & url);

signals:

  void authentificationDone();

private:

  QString accessToken;

};

#endif // FACEBOOKIMPORT_H
