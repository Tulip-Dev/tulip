/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef _Tulip_PLUGINSHELP_H_
#define _Tulip_PLUGINSHELP_H_

#include <QtCore/QString>

#include <tulip/tulipconf.h>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE PluginsHelp {

  public :

    static QString minimumHtmlFile(const QString &title);
    static void appendFiles(const QString &baseDirectory,const QString &pluginType,const QStringList &filesList,QByteArray &outArray);
    static QString getPluginType(const QByteArray &inArray);
    static bool partFiles(const QByteArray &inArray,const QString &baseDirectory);
    static void addInFile(const QString &fileName,const QString &textToAdd, const QString &textAfter);
    static void removeInFile(const QString &fileName,const QString &textToRemove);
    static void checkViewHelp();

    static void installHelpDoc(const QString &fileName);
    static void removeHelpDoc(const QString &fileName);

  };

}

#endif
