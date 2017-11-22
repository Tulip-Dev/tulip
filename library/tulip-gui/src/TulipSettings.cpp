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
// add a specific key which which must never exists in user settings
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

static const QString TS_RunningTimeComputed = "graph/auto/time";
static const QString TS_LogPluginCall = "graph/auto/log";
static const QString TS_UseTlpbFileFormat = "graph/auto/usetlpb";
static const QString TS_SeedForRandomSequence = "graph/auto/seed";

static const QString TS_WarnUserAboutGraphicsCard = "app/warn_about_graphics_card";
static const QString TS_ShowStatusBar = "app/gui/show_status_bar";
static const QString TS_LoggerAnchored = "app/gui/logger_anchored";

TulipSettings::TulipSettings() : QSettings("TulipSoftware", "Tulip") {}

TulipSettings &TulipSettings::instance() {
  if (!_instance) {
    _instance = new TulipSettings;
    TulipViewSettings::instance().addListener(_instance);
    GlDefaultSelectionColorManager::setManager(_instance);
  }

  return *_instance;
}

void TulipSettings::synchronizeViewSettings() {
  TulipViewSettings::instance().setDefaultColor(NODE, defaultColor(NODE));
  TulipViewSettings::instance().setDefaultColor(EDGE, defaultColor(EDGE));
  TulipViewSettings::instance().setDefaultSize(NODE, defaultSize(NODE));
  TulipViewSettings::instance().setDefaultSize(EDGE, defaultSize(EDGE));
  TulipViewSettings::instance().setDefaultShape(NODE, defaultShape(NODE));
  TulipViewSettings::instance().setDefaultShape(EDGE, defaultShape(EDGE));
  TulipViewSettings::instance().setDefaultLabelColor(defaultLabelColor());
}

QStringList TulipSettings::recentDocuments() const {
  return value(TS_RecentDocuments).toStringList();
}

void TulipSettings::checkRecentDocuments() {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocuments).toList();

  foreach (const QVariant &doc, recentDocumentsValue) {
    if (!QFileInfo(doc.toString()).exists())
      recentDocumentsValue.removeAll(doc);
  }

  setValue(TS_RecentDocuments, recentDocumentsValue);
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocuments).toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  setValue(TS_RecentDocuments, recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocations).toStringList();

  if (!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue(TS_RemoteLocations, remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocations).toStringList();

  if (remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue(TS_RemoteLocations, remoteLocations);
}

const QStringList TulipSettings::remoteLocations() const {
  return value(TS_RemoteLocations).toStringList();
}

const QStringList TulipSettings::pluginsToRemove() const {
  return value(TS_PluginsToRemove).toStringList();
}

void TulipSettings::markPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemove).toStringList();

  if (!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue(TS_PluginsToRemove, markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemove).toStringList();

  if (markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue(TS_PluginsToRemove, markedPlugins);
}

QString TulipSettings::elementKey(const QString &configEntry, tlp::ElementType elem) {
  return configEntry + (elem == tlp::NODE ? "node" : "edge");
}

tlp::Color TulipSettings::defaultColor(tlp::ElementType elem, bool tulipDefault) {
  QString val = value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultColor, elem),
                      (elem == tlp::NODE ? "(255, 95, 95)" : "(180,180,180)"))
                    .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultColor(tlp::ElementType elem, const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(elementKey(TS_DefaultColor, elem), value);
  TulipViewSettings::instance().setDefaultColor(elem, color);
}

Color TulipSettings::defaultLabelColor(bool tulipDefault) {
  QString val =
      value(tulipDefault ? TS_DefaultOfDefault : TS_DefaultLabelColor, "(0, 0, 0)").toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultLabelColor(const Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultLabelColor, value);
  TulipViewSettings::instance().setDefaultLabelColor(color);
}

tlp::Size TulipSettings::defaultSize(tlp::ElementType elem, bool tulipDefault) {
  QString val = value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultSize, elem),
                      (elem == tlp::NODE ? "(1,1,1)" : "(0.125,0.125,0.5)"))
                    .toString();
  Size result;
  SizeType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultSize(tlp::ElementType elem, const tlp::Size &size) {
  QString value = tlp::SizeType::toString(size).c_str();
  setValue(elementKey(TS_DefaultSize, elem), value);
  TulipViewSettings::instance().setDefaultSize(elem, size);
}

int TulipSettings::defaultShape(tlp::ElementType elem, bool tulipDefault) {
  return value(elementKey(tulipDefault ? TS_DefaultOfDefault : TS_DefaultShape, elem),
               (elem == tlp::NODE ? int(NodeShape::Circle) : int(EdgeShape::Polyline)))
      .toInt();
}

void TulipSettings::setDefaultShape(tlp::ElementType elem, int shape) {
  setValue(elementKey(TS_DefaultShape, elem), shape);
  TulipViewSettings::instance().setDefaultShape(elem, shape);
}

tlp::Color TulipSettings::defaultSelectionColor(bool tulipDefault) {
  QString val =
      value(tulipDefault ? TS_DefaultOfDefault : TS_DefaultSelectionColor, "(23, 81, 228)")
          .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void TulipSettings::setDefaultSelectionColor(const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultSelectionColor, value);
}

QSet<QString> TulipSettings::favoriteAlgorithms() const {
  return value(TS_FavoriteAlgorithms, QStringList()).toStringList().toSet();
}

void TulipSettings::addFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.insert(name);
  setFavoriteAlgorithms(favAlgs);
}

void TulipSettings::removeFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.remove(name);
  setFavoriteAlgorithms(favAlgs);
}

bool TulipSettings::isProxyEnabled() const {
  return value(TS_ProxyEnabled).toBool();
}

void TulipSettings::setProxyEnabled(bool f) {
  setValue(TS_ProxyEnabled, f);
}

QNetworkProxy::ProxyType TulipSettings::proxyType() const {
  return static_cast<QNetworkProxy::ProxyType>(value(TS_ProxyType).toInt());
}

void TulipSettings::setProxyType(QNetworkProxy::ProxyType t) {
  setValue(TS_ProxyType, int(t));
}

QString TulipSettings::proxyHost() const {
  return value(TS_ProxyHost).toString();
}

void TulipSettings::setProxyHost(const QString &h) {
  setValue(TS_ProxyHost, h);
}

unsigned int TulipSettings::proxyPort() const {
  return value(TS_ProxyPort).toUInt();
}

void TulipSettings::setProxyPort(unsigned int p) {
  setValue(TS_ProxyPort, p);
}

bool TulipSettings::isUseProxyAuthentification() const {
  return value(TS_ProxyUseAuth).toBool();
}

void TulipSettings::setUseProxyAuthentification(bool f) {
  setValue(TS_ProxyUseAuth, f);
}

QString TulipSettings::proxyUsername() const {
  return value(TS_ProxyUsername).toString();
}

void TulipSettings::setProxyUsername(const QString &s) {
  setValue(TS_ProxyUsername, s);
}

QString TulipSettings::proxyPassword() const {
  return value(TS_ProxyPassword).toString();
}

void TulipSettings::setProxyPassword(const QString &s) {
  setValue(TS_ProxyPassword, s);
}

void TulipSettings::applyProxySettings() {
  QNetworkProxy proxy(QNetworkProxy::NoProxy);

  if (isProxyEnabled()) {
    proxy.setType(proxyType());
    proxy.setHostName(proxyHost());
    proxy.setPort(proxyPort());

    if (isUseProxyAuthentification()) {
      proxy.setUser(proxyUsername());
      proxy.setPassword(proxyPassword());
    }
  }

  QNetworkProxy::setApplicationProxy(proxy);
}

bool TulipSettings::isFirstRun() const {
  return contains(TS_FirstRun) == false;
}

void TulipSettings::setFirstRun(bool f) {
  setValue(TS_FirstRun, f);
}

bool TulipSettings::isFirstTulipMMRun() const {
  return contains(TS_FirstRunMM) == false;
}

void TulipSettings::setFirstTulipMMRun(bool f) {
  setValue(TS_FirstRunMM, f);
}

bool TulipSettings::displayDefaultViews() const {
  return value(TS_AutomaticDisplayDefaultViews, true).toBool();
}

void TulipSettings::setDisplayDefaultViews(bool f) {
  setValue(TS_AutomaticDisplayDefaultViews, f);
}

bool TulipSettings::isAutomaticMapMetric() const {
  return value(TS_AutomaticMapMetric, false).toBool();
}

void TulipSettings::setAutomaticMapMetric(bool f) {
  setValue(TS_AutomaticMapMetric, f);
}

bool TulipSettings::isAutomaticRatio() const {
  return value(TS_AutomaticPerfectAspectRatio, false).toBool();
}

void TulipSettings::setAutomaticRatio(bool f) {
  setValue(TS_AutomaticPerfectAspectRatio, f);
}

bool TulipSettings::isAutomaticCentering() const {
  return value(TS_AutomaticCentering, true).toBool();
}

void TulipSettings::setAutomaticCentering(bool f) {
  setValue(TS_AutomaticCentering, f);
}

bool TulipSettings::isViewOrtho() const {
  return value(TS_ViewOrtho, true).toBool();
}

void TulipSettings::setViewOrtho(bool f) {
  setValue(TS_ViewOrtho, f);
}

void TulipSettings::setFavoriteAlgorithms(const QSet<QString> &lst) {
  setValue(TS_FavoriteAlgorithms, static_cast<QStringList>(lst.toList()));
}

bool TulipSettings::isResultPropertyStored() const {
  return value(TS_ResultPropertyStored, false).toBool();
}

void TulipSettings::setResultPropertyStored(bool f) {
  setValue(TS_ResultPropertyStored, f);
}

// deprecated
bool TulipSettings::isRunningTimeComputed() const {
  return logPluginCall() == LogCallWithExecutionTime;
}

// deprecated
void TulipSettings::setRunningTimeComputed(bool f) {
  if (f)
    setLogPluginCall(LogCallWithExecutionTime);
  else
    setLogPluginCall(NoLog);
}

unsigned int TulipSettings::logPluginCall() const {
  if (contains(TS_RunningTimeComputed)) {
    bool val = value(TS_RunningTimeComputed, false).toBool();

    // ensure compatibility
    if (val)
      const_cast<TulipSettings *>(this)->setValue(TS_LogPluginCall, LogCallWithExecutionTime);

    // no longer used
    const_cast<TulipSettings *>(this)->remove(TS_RunningTimeComputed);
  }

  return value(TS_LogPluginCall, NoLog).toUInt();
}

void TulipSettings::setLogPluginCall(unsigned int val) {
  setValue(TS_LogPluginCall, val);
}

bool TulipSettings::isUseTlpbFileFormat() const {
  return value(TS_UseTlpbFileFormat, false).toBool();
}

void TulipSettings::setUseTlpFileFormat(bool f) {
  setValue(TS_UseTlpbFileFormat, f);
}

unsigned int TulipSettings::seedOfRandomSequence() const {
  return value(TS_SeedForRandomSequence, tlp::getSeedOfRandomSequence()).toUInt();
}

void TulipSettings::setSeedOfRandomSequence(unsigned int seed) {
  setValue(TS_SeedForRandomSequence, seed);
}

void TulipSettings::initSeedOfRandomSequence() {
  tlp::setSeedOfRandomSequence(seedOfRandomSequence());
}

bool TulipSettings::warnUserAboutGraphicsCard() const {
  return value(TS_WarnUserAboutGraphicsCard, true).toBool();
}

void TulipSettings::setWarnUserAboutGraphicsCard(bool f) {
  setValue(TS_WarnUserAboutGraphicsCard, f);
}

bool TulipSettings::showStatusBar() const {
  return value(TS_ShowStatusBar, true).toBool();
}

void TulipSettings::setShowStatusBar(bool f) {
  setValue(TS_ShowStatusBar, f);
}

bool TulipSettings::loggerAnchored() const {
  return value(TS_LoggerAnchored, true).toBool();
}

void TulipSettings::setLoggerAnchored(bool f) {
  setValue(TS_LoggerAnchored, f);
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
