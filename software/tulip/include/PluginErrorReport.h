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

#ifndef PLUGINERRORREPORT_H
#define PLUGINERRORREPORT_H

#include <QLabel>

/**
  @brief This widget provides with maintenance actions related to a plugin load error.
  When a plugin crash at load time, the user shall have the following choices:
  @list
  @li Uninstall the plugin (removes the file permanantly)
  @li Reinstall it from a distant repository
  @list
  */
class PluginErrorReport : public QLabel {
  Q_OBJECT
public:
  explicit PluginErrorReport(const QString &fileName, const QString &errorMsg,
                             QWidget *parent = nullptr);

protected:
  void focusInEvent(QFocusEvent *) override;
  void focusOutEvent(QFocusEvent *) override;
};
#endif // PLUGINERRORREPORT_H
