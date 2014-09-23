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

#ifndef PIXELORIENTEDOPTIONSWIDGET_H_
#define PIXELORIENTEDOPTIONSWIDGET_H_

#include <QWidget>

#include <tulip/Color.h>

namespace Ui {
class PixelOrientedOptionsWidgetData;
}

namespace tlp {

class PixelOrientedOptionsWidget : public QWidget {

  Q_OBJECT

  Ui::PixelOrientedOptionsWidgetData* _ui;
public :

  PixelOrientedOptionsWidget(QWidget *parent = 0);
  ~PixelOrientedOptionsWidget();

  Color getBackgroundColor() const;
  void setBackgroundColor(const Color &color);

  std::string getLayoutType() const;
  void setLayoutType(const std::string& layoutType);

  bool configurationChanged();

private slots :

  void pressBackgroundColorButton();

public slots :

  void applySettings() {
    emit(applySettingsSignal());
  }

signals :

  void applySettingsSignal();

private :

  bool oldValuesInitialized;
  Color oldBackgroundColor;
  std::string oldLayoutType;

};
}
#endif /* PIXELORIENTEDOPTIONSWIDGET_H_ */
