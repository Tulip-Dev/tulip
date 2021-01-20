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

#include "tulip/TulipSettings.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/PropertyTypes.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/GlyphManager.h>
#include <tulip/TulipRelease.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#ifdef __APPLE__
#include <QProcess>
#include <QSysInfo>
#endif

using namespace tlp;
using namespace std;

TulipSettings *TulipSettings::_instance = nullptr;

static const QString TS_RemoteLocations = "app/remote_locations";
static const QString TS_RecentDocuments = "app/recent_documents";
static const QString TS_PluginsToRemove = "app/pluginsToRemove";
static const QString TS_DefaultColor = "graph/defaults/color/";
static const QString TS_DefaultLabelColor = "graph/defaults/color/labels";
static const QString TS_DefaultSize = "graph/defaults/size/";
static const QString TS_DefaultShape = "graph/defaults/shape/";
static const QString TS_DefaultSelectionColor = "graph/defaults/selectioncolor/";
// add a specific key which must never exists in user settings
static const QString TS_DefaultOfDefault = "never written in user file";
static const QString TS_FavoriteAlgorithms = "app/algorithms/favorites";

static const QString TS_FirstRun = "app/tulip/firstRun";
static const QString TS_FirstRunMM = QString("app/") + TULIP_MM_VERSION + "/firstRun";

static const QString TS_ProxyEnabled = "app/proxy/enabled";
static const QString TS_ProxyType = "app/proxy/type";
static const QString TS_ProxyHost = "app/proxy/host";
static const QString TS_ProxyPort = "app/proxy/port";
static const QString TS_ProxyUseAuth = "app/proxy/user";
static const QString TS_ProxyUsername = "app/proxy/useAuth";
static const QString TS_ProxyPassword = "app/proxy/passwd";

static const QString TS_AutomaticDisplayDefaultViews = "graph/auto/defaultViews";
static const QString TS_AutomaticPerfectAspectRatio = "graph/auto/ratio";
static const QString TS_AutomaticCentering = "graph/auto/center";
static const QString TS_ViewOrtho = "graph/auto/ortho";
static const QString TS_AutomaticMapMetric = "graph/auto/colors";

static const QString TS_ResultPropertyStored = "graph/auto/result";

static const QString TS_LogPluginCall = "graph/auto/log";
static const QString TS_UseTlpbFileFormat = "graph/auto/usetlpb";
static const QString TS_SeedForRandomSequence = "graph/auto/seed";

static const QString TS_WarnUserAboutGraphicsCard = "app/warn_about_graphics_card";
static const QString TS_DisplayInDarkMode = "app/gui/display_in_dark_mode";
static const QString TS_ShowStatusBar = "app/gui/show_status_bar";
static const QString TS_LoggerAnchored = "app/gui/logger_anchored";

static const QString TS_RecentPerspectiveFiles = "perspective/recent_files";

TulipSettings::TulipSettings() : QSettings("TulipSoftware", "Tulip") {}

TulipSettings &TulipSettings::instance() {
  if (!_instance) {
    _instance = new TulipSettings;
    TulipViewSettings::instance().addListener(_instance);
    GlDefaultSelectionColorManager::setManager(_instance);
  }

  return *_instance;
}

void TulipSettings::synchronizeSettings() {
  instance().sync();
}

void TulipSettings::synchronizeViewSettings() {
  TulipViewSettings::setDefaultColor(NODE, defaultColor(NODE));
  TulipViewSettings::setDefaultColor(EDGE, defaultColor(EDGE));
  TulipViewSettings::setDefaultSize(NODE, defaultSize(NODE));
  TulipViewSettings::setDefaultSize(EDGE, defaultSize(EDGE));
  TulipViewSettings::setDefaultShape(NODE, defaultShape(NODE));
  TulipViewSettings::setDefaultShape(EDGE, defaultShape(EDGE));
  TulipViewSettings::setDefaultLabelColor(defaultLabelColor());
}

QStringList TulipSettings::recentDocuments() {
  return instance().value(TS_RecentDocuments).toStringList();
}

void TulipSettings::checkRecentDocuments() {
  QList<QVariant> recentDocumentsValue = instance().value(TS_RecentDocuments).toList();

  for (const QVariant &doc : recentDocumentsValue) {
    if (!QFileInfo(doc.toString()).exists())
      recentDocumentsValue.removeAll(doc);
  }

  _instance->setValue(TS_RecentDocuments, recentDocumentsValue);
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = instance().value(TS_RecentDocuments).toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  _instance->setValue(TS_RecentDocuments, recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = instance().value(TS_RemoteLocations).toStringList();

  if (!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  _instance->setValue(TS_RemoteLocations, remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = instance().value(TS_RemoteLocations).toStringList();

  if (remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  _instance->setValue(TS_RemoteLocations, remoteLocations);
}

const QStringList TulipSettings::remoteLocations() {
  return instance().value(TS_RemoteLocations).toStringList();
}

const QStringList TulipSettings::pluginsToRemove() {
  return instance().value(TS_PluginsToRemove).toStringList();
}

void TulipSettings::markPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = instance().value(TS_PluginsToRemove).toStringList();

  if (!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  _instance->setValue(TS_PluginsToRemove, markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = instance().value(TS_PluginsToRemove).toStringList();

  if (markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  _instance->setValue(TS_PluginsToRemove, markedPlugins);
}

QString TulipSettings::elementKey(const QString &configEntry, tlp::ElementType elem) {
  return configEntry + (elem == tlp::NODE ? "node" : "edge");
}

tlp::Color TulipSettings::defaultColor(tlp::ElementType elem, bool tulipDefault) {
  QString val = instance()
                    .value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultColor, elem),
                           (elem == tlp::NODE ? "(255, 95, 95)" : "(180,180,180)"))
                    .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultColor(tlp::ElementType elem, const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  instance().setValue(elementKey(TS_DefaultColor, elem), value);
  TulipViewSettings::setDefaultColor(elem, color);
}

Color TulipSettings::defaultLabelColor(bool tulipDefault) {
  QString val = instance()
                    .value(tulipDefault ? TS_DefaultOfDefault : TS_DefaultLabelColor, "(0, 0, 0)")
                    .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultLabelColor(const Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  instance().setValue(TS_DefaultLabelColor, value);
  TulipViewSettings::setDefaultLabelColor(color);
}

tlp::Size TulipSettings::defaultSize(tlp::ElementType elem, bool tulipDefault) {
  QString val = instance()
                    .value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultSize, elem),
                           (elem == tlp::NODE ? "(1,1,1)" : "(0.125,0.125,0.5)"))
                    .toString();
  Size result;
  SizeType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultSize(tlp::ElementType elem, const tlp::Size &size) {
  QString value = tlp::SizeType::toString(size).c_str();
  instance().setValue(elementKey(TS_DefaultSize, elem), value);
  TulipViewSettings::setDefaultSize(elem, size);
}

int TulipSettings::defaultShape(tlp::ElementType elem, bool tulipDefault) {
  return instance()
      .value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultShape, elem),
             (elem == tlp::NODE ? int(NodeShape::Circle) : int(EdgeShape::Polyline)))
      .toInt();
}

void TulipSettings::setDefaultShape(tlp::ElementType elem, int shape) {
  instance().setValue(elementKey(TS_DefaultShape, elem), shape);
  TulipViewSettings::setDefaultShape(elem, shape);
}

tlp::Color TulipSettings::defaultSelectionColor(bool tulipDefault) {
  QString val =
      instance()
          .value(tulipDefault ? TS_DefaultOfDefault : TS_DefaultSelectionColor, "(23, 81, 228)")
          .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

tlp::Color TulipSettings::getDefaultSelectionColor(bool flag) {
  return instance().defaultSelectionColor(flag);
}

void TulipSettings::setDefaultSelectionColor(const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  instance().setValue(TS_DefaultSelectionColor, value);
}

QSet<QString> TulipSettings::favoriteAlgorithms() {
  auto ls = instance().value(TS_FavoriteAlgorithms, QStringList()).toStringList();
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
  return ls.toSet();
#else
  return QSet<QString>(ls.begin(), ls.end());
#endif
}

void TulipSettings::addFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.insert(name);
  instance().setFavoriteAlgorithms(favAlgs);
}

void TulipSettings::removeFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.remove(name);
  instance().setFavoriteAlgorithms(favAlgs);
}

bool TulipSettings::isProxyEnabled() {
  return instance().value(TS_ProxyEnabled).toBool();
}

void TulipSettings::setProxyEnabled(bool f) {
  instance().setValue(TS_ProxyEnabled, f);
}

QNetworkProxy::ProxyType TulipSettings::proxyType() {
  return static_cast<QNetworkProxy::ProxyType>(instance().value(TS_ProxyType).toInt());
}

void TulipSettings::setProxyType(QNetworkProxy::ProxyType t) {
  instance().setValue(TS_ProxyType, int(t));
}

QString TulipSettings::proxyHost() {
  return instance().value(TS_ProxyHost).toString();
}

void TulipSettings::setProxyHost(const QString &h) {
  instance().setValue(TS_ProxyHost, h);
}

unsigned int TulipSettings::proxyPort() {
  return instance().value(TS_ProxyPort).toUInt();
}

void TulipSettings::setProxyPort(unsigned int p) {
  instance().setValue(TS_ProxyPort, p);
}

bool TulipSettings::isUseProxyAuthentification() {
  return instance().value(TS_ProxyUseAuth).toBool();
}

void TulipSettings::setUseProxyAuthentification(bool f) {
  instance().setValue(TS_ProxyUseAuth, f);
}

QString TulipSettings::proxyUsername() {
  return instance().value(TS_ProxyUsername).toString();
}

void TulipSettings::setProxyUsername(const QString &s) {
  instance().setValue(TS_ProxyUsername, s);
}

QString TulipSettings::proxyPassword() {
  return instance().value(TS_ProxyPassword).toString();
}

void TulipSettings::setProxyPassword(const QString &s) {
  instance().setValue(TS_ProxyPassword, s);
}

void TulipSettings::applyProxySettings() {
  if (isProxyEnabled()) {
    QNetworkProxy proxy(proxyType(), proxyHost(), static_cast<qint16>(proxyPort()));

    if (isUseProxyAuthentification()) {
      proxy.setUser(proxyUsername());
      proxy.setPassword(proxyPassword());
    }
    QNetworkProxy::setApplicationProxy(proxy);
  } else
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

bool TulipSettings::isFirstRun() {
  return instance().contains(TS_FirstRun) == false;
}

void TulipSettings::setFirstRun(bool f) {
  instance().setValue(TS_FirstRun, f);
}

bool TulipSettings::isFirstTulipMMRun() {
  return instance().contains(TS_FirstRunMM) == false;
}

bool TulipSettings::userHasLaunchedTulipMM(const std::string &mmVersion) {
  return instance().contains(QString("app/") + mmVersion.c_str() + "/firstRun");
}

void TulipSettings::setFirstTulipMMRun(bool f) {
  instance().setValue(TS_FirstRunMM, f);
}

bool TulipSettings::displayDefaultViews() {
  return instance().value(TS_AutomaticDisplayDefaultViews, true).toBool();
}

void TulipSettings::setDisplayDefaultViews(bool f) {
  instance().setValue(TS_AutomaticDisplayDefaultViews, f);
}

bool TulipSettings::isAutomaticMapMetric() {
  return instance().value(TS_AutomaticMapMetric, false).toBool();
}

void TulipSettings::setAutomaticMapMetric(bool f) {
  instance().setValue(TS_AutomaticMapMetric, f);
}

bool TulipSettings::isAutomaticRatio() {
  return instance().value(TS_AutomaticPerfectAspectRatio, false).toBool();
}

void TulipSettings::setAutomaticRatio(bool f) {
  instance().setValue(TS_AutomaticPerfectAspectRatio, f);
}

bool TulipSettings::isAutomaticCentering() {
  return instance().value(TS_AutomaticCentering, true).toBool();
}

void TulipSettings::setAutomaticCentering(bool f) {
  instance().setValue(TS_AutomaticCentering, f);
}

bool TulipSettings::isViewOrtho() {
  return instance().value(TS_ViewOrtho, true).toBool();
}

void TulipSettings::setViewOrtho(bool f) {
  instance().setValue(TS_ViewOrtho, f);
}

void TulipSettings::setFavoriteAlgorithms(const QSet<QString> &lst) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
  instance().setValue(TS_FavoriteAlgorithms, static_cast<QStringList>(lst.toList()));
#else
  instance().setValue(TS_FavoriteAlgorithms, QStringList(lst.begin(), lst.end()));
#endif
}

bool TulipSettings::isResultPropertyStored() {
  return instance().value(TS_ResultPropertyStored, false).toBool();
}

void TulipSettings::setResultPropertyStored(bool f) {
  instance().setValue(TS_ResultPropertyStored, f);
}

unsigned int TulipSettings::logPluginCall() {
  return instance().value(TS_LogPluginCall, NoLog).toUInt();
}

void TulipSettings::setLogPluginCall(unsigned int val) {
  instance().setValue(TS_LogPluginCall, val);
}

bool TulipSettings::isUseTlpbFileFormat() {
  return instance().value(TS_UseTlpbFileFormat, true).toBool();
}

void TulipSettings::setUseTlpbFileFormat(bool f) {
  instance().setValue(TS_UseTlpbFileFormat, f);
}

unsigned int TulipSettings::seedOfRandomSequence() {
  return instance().value(TS_SeedForRandomSequence, tlp::getSeedOfRandomSequence()).toUInt();
}

void TulipSettings::setSeedOfRandomSequence(unsigned int seed) {
  instance().setValue(TS_SeedForRandomSequence, seed);
}

void TulipSettings::initSeedOfRandomSequence() {
  tlp::setSeedOfRandomSequence(seedOfRandomSequence());
}

bool TulipSettings::warnUserAboutGraphicsCard() {
  return instance().value(TS_WarnUserAboutGraphicsCard, true).toBool();
}

void TulipSettings::setWarnUserAboutGraphicsCard(bool f) {
  instance().setValue(TS_WarnUserAboutGraphicsCard, f);
}

bool TulipSettings::isDisplayInDarkMode() {
  if (!instance().contains(TS_DisplayInDarkMode)) {
#ifdef __APPLE__
    QString osVersion = QSysInfo::productVersion();
    // Dark mode exists since MacOS 10.14
    if (osVersion >= "10.14") {
      QProcess process;
      process.start("defaults", {"read", "-g", "AppleInterfaceStyle"});
      process.waitForFinished(-1);
      QString output(process.readAllStandardOutput());
      return output.contains("Dark", Qt::CaseInsensitive);
    }
#endif
#ifdef _WIN32
    QSettings settings(
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1) == 0;
#endif
    return false;
  }

  return instance().value(TS_DisplayInDarkMode, false).toBool();
}

void TulipSettings::setDisplayInDarkMode(bool f) {
  if (f != isDisplayInDarkMode())
    instance().setValue(TS_DisplayInDarkMode, f);
}

bool TulipSettings::showStatusBar() {
  return instance().value(TS_ShowStatusBar, true).toBool();
}

void TulipSettings::setShowStatusBar(bool f) {
  instance().setValue(TS_ShowStatusBar, f);
}

bool TulipSettings::loggerAnchored() {
  return instance().value(TS_LoggerAnchored, true).toBool();
}

void TulipSettings::setLoggerAnchored(bool f) {
  instance().setValue(TS_LoggerAnchored, f);
}

QStringList TulipSettings::recentPerspectiveFiles() {
  return instance().value(TS_RecentPerspectiveFiles).toStringList();
}

void TulipSettings::setRecentPerspectiveFiles(QStringList &files) {
  instance().value(TS_RecentPerspectiveFiles, files);
}

void TulipSettings::treatEvent(const Event &message) {
  const ViewSettingsEvent *sev = dynamic_cast<const ViewSettingsEvent *>(&message);

  if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_COLOR_MODIFIED) {
    setDefaultColor(sev->getElementType(), sev->getColor());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SIZE_MODIFIED) {
    setDefaultSize(sev->getElementType(), sev->getSize());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SHAPE_MODIFIED) {
    setDefaultShape(sev->getElementType(), sev->getShape());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_LABEL_COLOR_MODIFIED) {
    setDefaultLabelColor(sev->getColor());
  }
}
