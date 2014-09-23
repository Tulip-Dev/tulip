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

#ifndef PLUGINERRORREPORT_H
#define PLUGINERRORREPORT_H

#include <QWidget>

namespace Ui {
class PluginErrorReportData;
}

/**
  @brief This widget provides with maintenance actions related to a plugin load errror.
  When a plugin crash at load time, the user shall have the following choices:
  @list
  @li Uninstall the plugin (removes the file permanantly)
  @li Reinstall it from a distant repository
  @list
  */
class PluginErrorReport: public QWidget {
  Q_OBJECT
public:
  explicit PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent=NULL);

protected:
  virtual void focusInEvent(QFocusEvent *);
  virtual void focusOutEvent(QFocusEvent *);

private:
  Ui::PluginErrorReportData *_ui;
};
#endif // PLUGINERRORREPORT_H
