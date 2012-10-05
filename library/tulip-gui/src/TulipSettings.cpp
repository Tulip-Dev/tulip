/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

using namespace tlp;

TulipSettings *TulipSettings::_instance = NULL;

const QString TulipSettings::RemoteLocationsConfigEntry = "app/remote_locations";
const QString TulipSettings::RecentDocumentsConfigEntry = "app/recent_documents";
const QString TulipSettings::PluginsToRemoveConfigEntry = "app/pluginsToRemove";
const QString TulipSettings::DefaultColorConfigEntry = "graph/defaults/color/";
const QString TulipSettings::DefaultSizeConfigEntry = "graph/defaults/size/";
const QString TulipSettings::DefaultShapeConfigEntry = "graph/defaults/shape/";
const QString TulipSettings::DefaultSelectionColorEntry = "graph/defaults/selectioncolor/";
const QString TulipSettings::FavoriteAlgorithmsEntry = "app/algorithms/favorites";
const QString TulipSettings::FirstRunEntry = QString("app/") + TULIP_MM_RELEASE + "/firstRun";

const QString TulipSettings::ProxyEnabledEntry = "app/proxy/enabled";
const QString TulipSettings::ProxyTypeEntry = "app/proxy/type";
const QString TulipSettings::ProxyHostEntry = "app/proxy/host";
const QString TulipSettings::ProxyPortEntry = "app/proxy/port";
const QString TulipSettings::ProxyUseAuthEntry = "app/proxy/user";
const QString TulipSettings::ProxyUsernameEntry = "app/proxy/useAuth";
const QString TulipSettings::ProxyPasswordEntry = "app/proxy/passwd";

const QString TulipSettings::AutomaticPerfectAspectRatioEntry = "graph/auto/ratio";
const QString TulipSettings::AutomaticMapMetricEntry = "graph/auto/colors";

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}

TulipSettings &TulipSettings::instance() {
  if (!_instance)
    _instance = new TulipSettings;

  return *_instance;
}

QStringList TulipSettings::recentDocuments() const {
  return value(RecentDocumentsConfigEntry).toStringList();
}

void TulipSettings::checkRecentDocuments() {
  QList<QVariant> recentDocumentsValue = value(RecentDocumentsConfigEntry).toList();
  foreach(QVariant doc, recentDocumentsValue) {
    if (!QFileInfo(doc.toString()).exists())
      recentDocumentsValue.removeAll(doc);
  }
  setValue(RecentDocumentsConfigEntry,recentDocumentsValue);
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value(RecentDocumentsConfigEntry).toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  setValue(RecentDocumentsConfigEntry,recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TulipSettings::RemoteLocationsConfigEntry).toStringList();

  if(!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue(TulipSettings::RemoteLocationsConfigEntry, remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TulipSettings::RemoteLocationsConfigEntry).toStringList();

  if(remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue(TulipSettings::RemoteLocationsConfigEntry, remoteLocations);
}

const QStringList TulipSettings::remoteLocations() const {
  return value(TulipSettings::RemoteLocationsConfigEntry).toStringList();
}

const QStringList TulipSettings::pluginsToRemove() const {
  return value(PluginsToRemoveConfigEntry).toStringList();
}

void TulipSettings::markPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(PluginsToRemoveConfigEntry).toStringList();

  if(!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue(PluginsToRemoveConfigEntry, markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(PluginsToRemoveConfigEntry).toStringList();

  if(markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue(PluginsToRemoveConfigEntry, markedPlugins);
}


QString TulipSettings::elementKey(const QString& configEntry, tlp::ElementType elem) {
  return configEntry + (elem == tlp::NODE ? "node" : "edge");
}

tlp::Color TulipSettings::defaultColor(tlp::ElementType elem) {
  QString val = value(elementKey(DefaultColorConfigEntry,elem),(elem == tlp::NODE ? "(255, 95, 95)" : "(180,180,180)")).toString();
  Color result;
  ColorType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultColor(tlp::ElementType elem, const tlp::Color& color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(elementKey(DefaultColorConfigEntry,elem),value);
}

tlp::Size TulipSettings::defaultSize(tlp::ElementType elem) {
  QString val = value(elementKey(DefaultSizeConfigEntry,elem),(elem == tlp::NODE ? "(1,1,1)" : "(0.125,0.125,0.5)")).toString();
  Size result;
  SizeType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultSize(tlp::ElementType elem, const tlp::Size& size) {
  QString value = tlp::SizeType::toString(size).c_str();
  setValue(elementKey(DefaultSizeConfigEntry,elem),value);
}

int TulipSettings::defaultShape(tlp::ElementType elem) {
  const int CIRCLE_GLYPH_ID = tlp::GlyphManager::getInst().glyphId("2D - Circle");
  return value(elementKey(DefaultShapeConfigEntry,elem),(elem == tlp::NODE ? CIRCLE_GLYPH_ID : SPLINESHAPE)).toInt();
}

void TulipSettings::setDefaultShape(tlp::ElementType elem, int shape) {
  setValue(elementKey(DefaultShapeConfigEntry,elem),shape);
}

tlp::Color TulipSettings::defaultSelectionColor() {
  QString val = value(DefaultSelectionColorEntry,"(23, 81, 228)").toString();
  Color result;
  ColorType::fromString(result,val.toStdString());
  return result;
}

void TulipSettings::setDefaultSelectionColor(const tlp::Color& color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(DefaultSelectionColorEntry,value);
}

QSet<QString> TulipSettings::favoriteAlgorithms() const {
  return value(FavoriteAlgorithmsEntry,QStringList()).toStringList().toSet();
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
  return value(ProxyEnabledEntry).toBool();
}

void TulipSettings::setProxyEnabled(bool f) {
  setValue(ProxyEnabledEntry,f);
}

QNetworkProxy::ProxyType TulipSettings::proxyType() const {
  return (QNetworkProxy::ProxyType)(value(ProxyTypeEntry).toInt());
}

void TulipSettings::setProxyType(QNetworkProxy::ProxyType t) {
  setValue(ProxyTypeEntry,(int)t);
}

QString TulipSettings::proxyHost() const {
  return value(ProxyHostEntry).toString();
}

void TulipSettings::setProxyHost(const QString &h) {
  setValue(ProxyHostEntry,h);
}

unsigned int TulipSettings::proxyPort() const {
  return value(ProxyHostEntry).toUInt();
}

void TulipSettings::setProxyPort(unsigned int p) {
  setValue(ProxyHostEntry,p);
}

bool TulipSettings::isUseProxyAuthentification() const {
  return value(ProxyUseAuthEntry).toBool();
}

void TulipSettings::setUseProxyAuthentification(bool f) {
  setValue(ProxyUseAuthEntry,f);
}

QString TulipSettings::proxyUsername() const {
  return value(ProxyUsernameEntry).toString();
}

void TulipSettings::setProxyUsername(const QString &s) {
  setValue(ProxyUsernameEntry,s);
}

QString TulipSettings::proxyPassword() const {
  return value(ProxyPasswordEntry).toString();
}

void TulipSettings::setProxyPassword(const QString& s) {
  setValue(ProxyPasswordEntry,s);
}

void TulipSettings::applyProxySettings() {
  QNetworkProxy proxy;
  proxy.setType(QNetworkProxy::NoProxy);

  if (isUseProxyAuthentification()) {
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
  return value(FirstRunEntry).toBool();
}

void TulipSettings::setFirstRun(bool f) {
  setValue(FirstRunEntry,f);
}

bool TulipSettings::isAutomaticMapMetric() const {
  return value(AutomaticMapMetricEntry,false).toBool();
}

void TulipSettings::setAutomaticMapMetric(bool f) {
  setValue(AutomaticMapMetricEntry,f);
}

bool TulipSettings::isAutomaticRatio() const {
  return value(AutomaticPerfectAspectRatioEntry,false).toBool();
}

void TulipSettings::setAutomaticRatio(bool f) {
  setValue(AutomaticPerfectAspectRatioEntry,f);
}

void TulipSettings::setFavoriteAlgorithms(const QSet<QString>& lst) {
  setValue(FavoriteAlgorithmsEntry,(QStringList)(lst.toList()));
}
