#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtCore/QSettings>

#include <tulip/TlpNetworkAccess.h>

namespace tlp {
NetworkProxyConfiguration getNetworkProxyConfiguration() {
  NetworkProxyConfiguration npc;
  QSettings settings("TulipSoftware","Tulip");

  // manage compatibility
  if (settings.contains("NetworkProxy") ||
      !settings.contains("PluginsManager"))
    settings.beginGroup("NetworkProxy");
  else
    settings.beginGroup("PluginsManager");

  npc.proxyEnabled=settings.value("proxyEnable",false).toBool();
  npc.address=settings.value("proxyAddress","").toString();
  npc.port=settings.value("proxyPort",0).toUInt();
  npc.authenticationEnabled=settings.value("proxyUsernamePasswordEnable",false).toBool();
  npc.login=settings.value("proxyUsername","").toString();
  npc.passwd=settings.value("proxyPassword","").toString();
  settings.endGroup();

  return npc;
}

void setNetworkProxyConfiguration(const NetworkProxyConfiguration& npc) {
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("NetworkProxy");
  settings.setValue("proxyEnable", npc.proxyEnabled);
  settings.setValue("proxyAddress", npc.address);
  settings.setValue("proxyPort", npc.port);
  settings.setValue("proxyUsernamePasswordEnable", npc.authenticationEnabled);
  settings.setValue("proxyUsername", npc.login);
  settings.setValue("proxyPassword", npc.passwd);
  settings.endGroup();
}

static QNetworkAccessManager* networkAccessManager = NULL;

QNetworkAccessManager* getNetworkAccessManager() {
  if (!networkAccessManager) {
    networkAccessManager = new QNetworkAccessManager();
    NetworkProxyConfiguration npc = getNetworkProxyConfiguration();

    if (npc.proxyEnabled) {
      networkAccessManager->setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy,
                                     npc.address,
                                     npc.port,
                                     npc.login,
                                     npc.passwd));
    }
  }

  return networkAccessManager;
}

}



