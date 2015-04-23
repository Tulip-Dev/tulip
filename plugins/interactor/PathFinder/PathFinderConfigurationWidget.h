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

#ifndef PATHFINDERCONFIGURATIONWIDGET_H_
#define PATHFINDERCONFIGURATIONWIDGET_H_

#include <QWidget>

#include <string>

namespace Ui {
class PathFinderConfigurationData;
}

class QString;

namespace tlp {
class PathFinderConfigurationWidget: public QWidget {
  Q_OBJECT

  Ui::PathFinderConfigurationData* _ui;
public:
  PathFinderConfigurationWidget(QWidget *parent=0);
  ~PathFinderConfigurationWidget();

  void addweightComboItem(const QString &s);
  void setCurrentweightComboIndex(const int i);
  int weightComboFindText(const QString &text) const;
  void addedgeOrientationComboItem(const QString &s);
  void setCurrentedgeOrientationComboIndex(const int i);
  int edgeOrientationComboFindText(const QString &text) const;
  void addpathsTypeComboItem(const QString &s);
  void toleranceChecked(const bool checked);
  void setToleranceSpinValue(const int val);
  void highlightersLabelDisabled(const bool disable);
  void addbottomWidget(QWidget *w);
  void toleranceCheckDisabled(const bool disabled);
  void toleranceSpinDisabled(const bool disabled);
  void toleranceLabelDisabled(const bool disabled);

signals:
  void setWeightMetric(const QString &);
  void setEdgeOrientation(const QString &);
  void setPathsType(const QString &);
  void activateTolerance(bool);
  void setTolerance(int);

};
}
#endif /* PATHFINDERCONFIGURATIONWIDGET_H_ */
