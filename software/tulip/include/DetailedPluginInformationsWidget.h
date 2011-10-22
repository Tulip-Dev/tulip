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
#ifndef DETAILEDPLUGININFORMATIONSWIDGET_H
#define DETAILEDPLUGININFORMATIONSWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
class DetailedPluginInformationsWidgetData;
}

namespace tlp {
class PluginInformations;
}

class DetailedPluginInformationsWidget: public QWidget {
  Q_OBJECT
public:
  explicit DetailedPluginInformationsWidget(tlp::PluginInformations *infos,QWidget *parent = 0);

  tlp::PluginInformations *pluginInformations() {
    return _pluginInformations;
  }

public slots:
  void hideNavigationBar();

signals:
  void goBack();
  void fetch();
  void fetch(tlp::PluginInformations*);
  void remove();
  void remove(tlp::PluginInformations*);

protected slots:
  void fetchButtonClicked();
  void removeButtonClicked();

private:
  Ui::DetailedPluginInformationsWidgetData *_ui;
  tlp::PluginInformations *_pluginInformations;

};

#endif // DETAILEDPLUGININFORMATIONSWIDGET_H
