/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPSETTINGS_H
#define TULIPSETTINGS_H

#include <QSettings>
#include <QNetworkProxy>
#include <QString>

#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @brief This class provides convenience functions to access the Tulip settings file (using
 * QSettings)
 * TulipSettings is a wrapper for QSettings providing quick access to common keys provided in the
 * tulip configuration file.
 * This object does not mask any method from the QSettings class, which mean that the user can
 * still access custom keys by invoking the QSettings::value method.
 */
class TLP_QT_SCOPE TulipSettings : public QSettings, GlDefaultSelectionColorManager, Observable {
  Q_OBJECT
  Q_ENUMS(DisplayProperty)

public:
  static TulipSettings &instance();

  static QString elementKey(const QString &configEntry, tlp::ElementType elem);

  static void synchronizeSettings();

  static void synchronizeViewSettings();

  /**
    @brief Retrieves the list of documents recently opened with tulip.
    */
  static QStringList recentDocuments();

  /**
    @brief Registers a file in the list of recently opened documents.
    If the list already contains 5 elements, the last one (in chronological order) will be removed.
    */
  static void addToRecentDocuments(const QString &);

  /**
    @brief Check recently opened documents list and remove non-existing ones
    */
  static void checkRecentDocuments();

  /**
   * @brief Adds a remote location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  static void addRemoteLocation(const QString &remoteLocation);

  /**
   * @brief Removes a location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  static void removeRemoteLocation(const QString &remoteLocation);

  /**
   * @brief Retrieves the list of locations from which to list plugins.
   *
   * @return const QStringList
   **/
  static const QStringList remoteLocations();
  static const QStringList pluginsToRemove();

  static void markPluginForRemoval(const QString &pluginLibrary);
  static void unmarkPluginForRemoval(const QString &pluginLibrary);

  static tlp::Color defaultColor(tlp::ElementType elem, bool tulipDefault = false);
  static void setDefaultColor(tlp::ElementType elem, const tlp::Color &color);

  static tlp::Color defaultLabelColor(bool tulipDefault = false);
  static void setDefaultLabelColor(const tlp::Color &color);

  static tlp::Size defaultSize(tlp::ElementType elem, bool tulipDefault = false);
  static void setDefaultSize(tlp::ElementType elem, const tlp::Size &size);

  static int defaultShape(tlp::ElementType elem, bool tulipDefault = false);
  static void setDefaultShape(tlp::ElementType elem, int shape);

  // in GlDefaultSelectionColorManager interface
  tlp::Color defaultSelectionColor(bool tulipDefault = false) override;

  static tlp::Color getDefaultSelectionColor(bool tulipDefault = false);
  static void setDefaultSelectionColor(const tlp::Color &color);

  static QSet<QString> favoriteAlgorithms();
  static void addFavoriteAlgorithm(const QString &name);
  static void removeFavoriteAlgorithm(const QString &name);

  // Proxy settings
  static bool isProxyEnabled();
  static void setProxyEnabled(bool);

  static QNetworkProxy::ProxyType proxyType();
  static void setProxyType(QNetworkProxy::ProxyType);

  static QString proxyHost();
  static void setProxyHost(const QString &);

  static unsigned int proxyPort();
  static void setProxyPort(unsigned int);

  static bool isUseProxyAuthentification();
  static void setUseProxyAuthentification(bool);

  static QString proxyUsername();
  static void setProxyUsername(const QString &);

  static QString proxyPassword();
  static void setProxyPassword(const QString &);

  static void applyProxySettings();

  static bool isFirstRun();
  static void setFirstRun(bool);

  static bool isFirstTulipMMRun();
  static bool userHasLaunchedTulipMM(const std::string &mmVersion);
  static void setFirstTulipMMRun(bool);

  static bool displayDefaultViews();
  static void setDisplayDefaultViews(bool);

  static bool isAutomaticMapMetric();
  static void setAutomaticMapMetric(bool);

  static bool isAutomaticCentering();
  static void setAutomaticCentering(bool);

  static bool isAutomaticRatio();
  static void setAutomaticRatio(bool);

  static bool isViewOrtho();
  static void setViewOrtho(bool);

  static bool isResultPropertyStored();
  static void setResultPropertyStored(bool);

  enum LogPluginCall { NoLog = 0, LogCall = 1, LogCallWithExecutionTime = 2 };
  static unsigned int logPluginCall();
  static void setLogPluginCall(unsigned int);

  static bool isUseTlpbFileFormat();
  static void setUseTlpbFileFormat(bool);

  static unsigned int seedOfRandomSequence();
  static void setSeedOfRandomSequence(unsigned int);
  static void initSeedOfRandomSequence();

  static bool warnUserAboutGraphicsCard();
  static void setWarnUserAboutGraphicsCard(bool);

  static bool isDisplayInDarkMode();
  static void setDisplayInDarkMode(bool);

  // no longer used
  _DEPRECATED static bool showStatusBar();
  _DEPRECATED static void setShowStatusBar(bool);

  static bool loggerAnchored();
  static void setLoggerAnchored(bool);

  static QStringList recentPerspectiveFiles();
  static void setRecentPerspectiveFiles(QStringList &doc);

  void treatEvent(const Event &message) override;

private:
  TulipSettings();
  static TulipSettings *_instance;

  void setFavoriteAlgorithms(const QSet<QString> &lst);
};
} // namespace tlp
#endif // TULIPSETTINGS_H
///@endcond
