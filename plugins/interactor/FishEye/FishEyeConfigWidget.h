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

#ifndef FISHEYECONFIGWIDGET_H_
#define FISHEYECONFIGWIDGET_H_

#include <QWidget>

namespace Ui {
class FishEyeConfigWidget;
}

namespace tlp {

class FishEyeConfigWidget : public QWidget {

  Ui::FishEyeConfigWidget* _ui;

public :

  FishEyeConfigWidget(QWidget *parent = NULL);
  ~FishEyeConfigWidget();

  int getFishEyeType() const;

  float getFishEyeRadius() const;
  void setFishEyeRadius(const float radius);

  float getFishEyeHeight() const;
  void setFishEyeHeight(const float height);

  float getFishEyeRadiusIncrementStep() const;
  float getFishEyeHeightIncrementStep() const;

};
}
#endif /* FISHEYECONFIGWIDGET_H_ */
