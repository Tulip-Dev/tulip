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

#include "tulip/TulipSettings.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/PropertyTypes.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/GlyphManager.h>
#include <tulip/TulipRelease.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpTools.h>

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

using namespace tlp;
using namespace std;

TulipSettings *TulipSettings::_instance = NULL;

static const QString TS_RemoteLocationsConfigEntry = "app/remote_locations";
static const QString TS_RecentDocumentsConfigEntry = "app/recent_documents";
static const QString TS_PluginsToRemoveConfigEntry = "app/pluginsToRemove";
static const QString TS_DefaultColorConfigEntry = "graph/defaults/color/";
static const QString TS_DefaultLabelColorConfigEntry = "graph/defaults/color/labels";
static const QString TS_DefaultSizeConfigEntry = "graph/defaults/size/";
static const QString TS_DefaultShapeConfigEntry = "graph/defaults/shape/";
static const QString TS_DefaultSelectionColorEntry = "graph/defaults/selectioncolor/";
static const QString TS_FavoriteAlgorithmsEntry = "app/algorithms/favorites";

static const QString TS_FirstRunEntry = QString("app/") + TULIP_MM_VERSION + "/firstRun";

static const QString TS_ProxyEnabledEntry = "app/proxy/enabled";
static const QString TS_ProxyTypeEntry = "app/proxy/type";
static const QString TS_ProxyHostEntry = "app/proxy/host";
static const QString TS_ProxyPortEntry = "app/proxy/port";
static const QString TS_ProxyUseAuthEntry = "app/proxy/user";
static const QString TS_ProxyUsernameEntry = "app/proxy/useAuth";
static const QString TS_ProxyPasswordEntry = "app/proxy/passwd";

static const QString TS_AutomaticDisplayDefaultViews = "graph/auto/defaultViews";
static const QString TS_AutomaticPerfectAspectRatioEntry = "graph/auto/ratio";
static const QString TS_ViewOrthoEntry = "graph/auto/ortho";
static const QString TS_AutomaticMapMetricEntry = "graph/auto/colors";

static const QString TS_ResultPropertyStoredEntry = "graph/auto/result";

static const QString TS_RunningTimeComputedEntry = "graph/auto/time";

static const QString TS_SeedForRandomSequenceEntry = "graph/auto/seed";

static const QString TS_WarnUserAboutGraphicsCardEntry = "app/warn_about_graphics_card";

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}

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
  return value(TS_RecentDocumentsConfigEntry).toStringList();
}

void TulipSettings::checkRecentDocuments() {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocumentsConfigEntry).toList();

  foreach(QVariant doc, recentDocumentsValue) {
    if (!QFileInfo(doc.toString()).exists())
      recentDocumentsValue.removeAll(doc);
  }

  setValue(TS_RecentDocumentsConfigEntry,recentDocumentsValue);
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocumentsConfigEntry).toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  setValue(TS_RecentDocumentsConfigEntry,recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocationsConfigEntry).toStringList();

  if(!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue(TS_RemoteLocationsConfigEntry, remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocationsConfigEntry).toStringList();

  if(remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue(TS_RemoteLocationsConfigEntry, remoteLocations);
}

const QStringList TulipSettings::remoteLocations() const {
  return value(TS_RemoteLocationsConfigEntry).toStringList();
}

const QStringList TulipSettings::pluginsToRemove() const {
  return value(TS_PluginsToRemoveConfigEntry).toStringList();
}

void TulipSettings::markPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemoveConfigEntry).toStringList();

  if(!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue(TS_PluginsToRemoveConfigEntry, markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemoveConfigEntry).toStringList();

  if(markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue(TS_PluginsToRemoveConfigEntry, markedPlugins);
}


QString TulipSettings::elementKey(const QString& configEntry, tlp::ElementType elem) {
  return configEntry + (elem == tlp::NODE ? "node" : "edge");
}

tlp::Color TulipSettings::defaultColor(tlp::ElementType elem) {
  QString val = value(elementKey(TS_DefaultColorConfigEntry,elem),(elem == tlp::NODE ? "(255, 95, 95)" : "(180,180,180)")).toString();
  Color result;
  ColorType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultColor(tlp::ElementType elem, const tlp::Color& color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(elementKey(TS_DefaultColorConfigEntry,elem),value);
  TulipViewSettings::instance().setDefaultColor(elem, color);
}

Color TulipSettings::defaultLabelColor() {
  QString val = value(TS_DefaultLabelColorConfigEntry,"(0, 0, 0)").toString();
  Color result;
  ColorType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultLabelColor(const Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultLabelColorConfigEntry,value);
  TulipViewSettings::instance().setDefaultLabelColor(color);
}

tlp::Size TulipSettings::defaultSize(tlp::ElementType elem) {
  QString val = value(elementKey(TS_DefaultSizeConfigEntry,elem),(elem == tlp::NODE ? "(1,1,1)" : "(0.125,0.125,0.5)")).toString();
  Size result;
  SizeType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultSize(tlp::ElementType elem, const tlp::Size& size) {
  QString value = tlp::SizeType::toString(size).c_str();
  setValue(elementKey(TS_DefaultSizeConfigEntry,elem),value);
  TulipViewSettings::instance().setDefaultSize(elem, size);
}

int TulipSettings::defaultShape(tlp::ElementType elem) {
  return value(elementKey(TS_DefaultShapeConfigEntry,elem),(elem == tlp::NODE ? int(NodeShape::Circle) : int(EdgeShape::Polyline))).toInt();
}

void TulipSettings::setDefaultShape(tlp::ElementType elem, int shape) {
  setValue(elementKey(TS_DefaultShapeConfigEntry,elem),shape);
  TulipViewSettings::instance().setDefaultShape(elem, shape);
}

tlp::Color TulipSettings::defaultSelectionColor() {
  QString val = value(TS_DefaultSelectionColorEntry,"(23, 81, 228)").toString();
  Color result;
  ColorType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultSelectionColor(const tlp::Color& color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultSelectionColorEntry,value);
}

QSet<QString> TulipSettings::favoriteAlgorithms() const {
  return value(TS_FavoriteAlgorithmsEntry,QStringList()).toStringList().toSet();
}

void TulipSettings::addFavoriteAlgorithm(const QString& name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.insert(name);
  setFavoriteAlgorithms(favAlgs);
}

void TulipSettings::removeFavoriteAlgorithm(const QString& name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.remove(name);
  setFavoriteAlgorithms(favAlgs);
}

bool TulipSettings::isProxyEnabled() const {
  return value(TS_ProxyEnabledEntry).toBool();
}

void TulipSettings::setProxyEnabled(bool f) {
  setValue(TS_ProxyEnabledEntry,f);
}

QNetworkProxy::ProxyType TulipSettings::proxyType() const {
  return (QNetworkProxy::ProxyType)(value(TS_ProxyTypeEntry).toInt());
}

void TulipSettings::setProxyType(QNetworkProxy::ProxyType t) {
  setValue(TS_ProxyTypeEntry,(int)t);
}

QString TulipSettings::proxyHost() const {
  return value(TS_ProxyHostEntry).toString();
}

void TulipSettings::setProxyHost(const QString &h) {
  setValue(TS_ProxyHostEntry,h);
}

unsigned int TulipSettings::proxyPort() const {
  return value(TS_ProxyPortEntry).toUInt();
}

void TulipSettings::setProxyPort(unsigned int p) {
  setValue(TS_ProxyPortEntry,p);
}

bool TulipSettings::isUseProxyAuthentification() const {
  return value(TS_ProxyUseAuthEntry).toBool();
}

void TulipSettings::setUseProxyAuthentification(bool f) {
  setValue(TS_ProxyUseAuthEntry,f);
}

QString TulipSettings::proxyUsername() const {
  return value(TS_ProxyUsernameEntry).toString();
}

void TulipSettings::setProxyUsername(const QString &s) {
  setValue(TS_ProxyUsernameEntry,s);
}

QString TulipSettings::proxyPassword() const {
  return value(TS_ProxyPasswordEntry).toString();
}

void TulipSettings::setProxyPassword(const QString& s) {
  setValue(TS_ProxyPasswordEntry,s);
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
  return contains(TS_FirstRunEntry) == false;
}

void TulipSettings::setFirstRun(bool f) {
  setValue(TS_FirstRunEntry,f);
}

bool TulipSettings::displayDefaultViews() const {
  return value(TS_AutomaticDisplayDefaultViews, true).toBool();
}

void TulipSettings::setDisplayDefaultViews(bool f) {
  setValue(TS_AutomaticDisplayDefaultViews,f);
}

bool TulipSettings::isAutomaticMapMetric() const {
  return value(TS_AutomaticMapMetricEntry,false).toBool();
}

void TulipSettings::setAutomaticMapMetric(bool f) {
  setValue(TS_AutomaticMapMetricEntry,f);
}

bool TulipSettings::isAutomaticRatio() const {
  return value(TS_AutomaticPerfectAspectRatioEntry,false).toBool();
}

void TulipSettings::setAutomaticRatio(bool f) {
  setValue(TS_AutomaticPerfectAspectRatioEntry,f);
}

bool TulipSettings::isViewOrtho() const {
  return value(TS_ViewOrthoEntry,true).toBool();
}

void TulipSettings::setViewOrtho(bool f) {
  setValue(TS_ViewOrthoEntry,f);
}

void TulipSettings::setFavoriteAlgorithms(const QSet<QString>& lst) {
  setValue(TS_FavoriteAlgorithmsEntry,(QStringList)(lst.toList()));
}

bool TulipSettings::isResultPropertyStored() const {
  return value(TS_ResultPropertyStoredEntry,false).toBool();
}

void TulipSettings::setResultPropertyStored(bool f) {
  setValue(TS_ResultPropertyStoredEntry,f);
}

bool TulipSettings::isRunningTimeComputed() const {
  return value(TS_RunningTimeComputedEntry,false).toBool();
}

void TulipSettings::setRunningTimeComputed(bool f) {
  setValue(TS_RunningTimeComputedEntry,f);
}

unsigned int TulipSettings::seedOfRandomSequence() const {
  return value(TS_SeedForRandomSequenceEntry,
               tlp::getSeedOfRandomSequence()).toUInt();
}

void TulipSettings::setSeedOfRandomSequence(unsigned int seed) {
  setValue(TS_SeedForRandomSequenceEntry, seed);
}

void TulipSettings::initSeedOfRandomSequence() {
  tlp::setSeedOfRandomSequence(seedOfRandomSequence());
}

bool TulipSettings::warnUserAboutGraphicsCard() const {
  return value(TS_WarnUserAboutGraphicsCardEntry,true).toBool();
}

void TulipSettings::setWarnUserAboutGraphicsCard(bool f) {
  setValue(TS_WarnUserAboutGraphicsCardEntry, f);
}

void TulipSettings::treatEvent(const Event &message) {
  const ViewSettingsEvent *sev = dynamic_cast<const ViewSettingsEvent *>(&message);

  if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_COLOR_MODIFIED) {
    setDefaultColor(sev->getElementType(), sev->getColor());
  }
  else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SIZE_MODIFIED) {
    setDefaultSize(sev->getElementType(), sev->getSize());
  }
  else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SHAPE_MODIFIED) {
    setDefaultShape(sev->getElementType(), sev->getShape());
  }
  else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_LABEL_COLOR_MODIFIED) {
    setDefaultLabelColor(sev->getColor());
  }
}

