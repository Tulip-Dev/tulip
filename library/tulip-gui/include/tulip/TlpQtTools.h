/*
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

#ifndef _TLPQTTOOLS_H
#define _TLPQTTOOLS_H

#include <QColor>
#include <QDebug>

#include <tulip/Color.h>
#include <tulip/tulipconf.h>
#include <tulip/PropertyInterface.h>

class QWidget;
class QString;

namespace tlp {

struct PluginLoader;

TLP_QT_SCOPE bool getColorDialog(const QColor &color,QWidget *parent,const QString &title,QColor &result);

inline QColor colorToQColor(const Color& color) {
  return QColor(color.getR(), color.getG(), color.getB(), color.getA());
}
inline Color QColorToColor(const QColor& color) {
  return Color(color.red(), color.green(), color.blue(), color.alpha());
}
/**
  * @brief Convert a string from Tulip to QString.
  **/
inline std::string QStringToTlpString(const QString& toConvert) {
  return std::string(toConvert.toUtf8());
}
/**
  * @brief Convert a QString to tulip string.
  **/
inline QString tlpStringToQString(const std::string& toConvert) {
  return QString::fromUtf8(toConvert.c_str());
}

/**
 * @brief Case insensitive comparison of two QStrings
 **/
inline bool QStringCaseCmp(const QString& s1, const QString& s2) {
  return QString::localeAwareCompare(s1, s2) < 0;
}



/**
    * @brief Convert the property type string to a label to display in the GUI.
    * The property type label is the string to display in the GUI instead of the basic property type string.
    * By example for a property of type "double" the label displayed in the GUI will be "Metric".
    **/
TLP_QT_SCOPE QString propertyTypeToPropertyTypeLabel(const std::string& typeName);

/**
  * @brief Get the string to display as property type for the given property.
  * The property type label is the string to display in the GUI instead of the property type string.
  * By example for a property of type "double" the label displayed in the GUI will be "Metric".
  **/
inline QString propertyInterfaceToPropertyTypeLabel(const tlp::PropertyInterface* const property) {
  return propertyTypeToPropertyTypeLabel(property->getTypename());
}

/**
  * @brief Convert the label of a property type to it's corresponding property type.
  * The property type label is the string to display in the GUI instead of the property type string.
  * By example for a property of type "double" the label displayed in the GUI will be "Metric".
  **/
TLP_QT_SCOPE std::string propertyTypeLabelToPropertyType(const QString& typeNameLabel);

/**
    * @brief Gets the name of the package to retrieve for this version of tulip.
    * The package name uses the Tulip release, platform (windows, unix, ...), architecture (x86, x86_64), and compiler used (GCC, Clang, MSVC) to determine which package this version can use.
    *
    * @param pluginName The name of the plugin for which we want the package name.
    **/
TLP_QT_SCOPE QString getPluginPackageName(const QString& pluginName);

TLP_QT_SCOPE QString getPluginStagingDirectory();

TLP_QT_SCOPE QString getPluginLocalInstallationDir();

TLP_QT_SCOPE QString localPluginsPath();

/**
 @brief Sets up environment when creating an executable using Tulip libraries
 This method performs basic operations when starting a software using Tulip:
 @list
 @li It initializes the tulip library
 @li it checks plugins to be discarded and uninstalls them
 @li It loads plugins from the application path
 @endlist
 */
extern TLP_QT_SCOPE void initTulipSoftware(PluginLoader *loader = NULL, bool removeDiscardedPlugins=false);

/**
 * @brief redirect tlp::debug() to qDebug()
 */
TLP_QT_SCOPE void redirectDebugOutputToQDebug();

/**
 * @brief redirect tlp::warning() to qWarning()
 */
TLP_QT_SCOPE void redirectWarningOutputToQWarning();

/**
 * @brief redirect tlp::error() to qCritical()
 */
TLP_QT_SCOPE void redirectErrorOutputToQCritical();

// Gui test mode
TLP_QT_SCOPE bool inGuiTestingMode();

TLP_QT_SCOPE void setGuiTestingMode(bool);

}

// QDebug extension
inline QDebug operator<<(QDebug dbg, const std::string& s) {
  dbg.nospace() << s.c_str();
  return dbg.space();
}

#endif
