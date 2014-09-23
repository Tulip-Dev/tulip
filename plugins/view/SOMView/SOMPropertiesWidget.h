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

#ifndef SOMPROPERTIESWIDGET_H_
#define SOMPROPERTIESWIDGET_H_

#include <tulip/Observable.h>
#include <tulip/DataSet.h>

#include <QWidget>
#include <QList>

#include "GradientManager.h"

namespace Ui {
class SOMPropertiesWidget;
}

class QButtonGroup;
class QRadioButton;
class QPushButton;
class QAbstractButton;

namespace tlp {

class SOMView;
class ColorScale;
class Graph;
class ColorScalePreview;

class SOMPropertiesWidget: public QWidget, public tlp::Observable {
  Q_OBJECT

  Ui::SOMPropertiesWidget* _ui;

public:

  enum SizeMappingType {
    NoSizeMapping, RealNodeSizeMapping, DistanceMapping
  };
  SOMPropertiesWidget(SOMView* view, QWidget *parent = NULL);
  ~SOMPropertiesWidget();

  unsigned int getGridWidth() const;
  unsigned int getGridHeight() const;
  QString getConnectivityLabel() const;
  bool getOppositeConnected() const;
  double getLearningRateValue() const;
  QString getDiffusionRateMethodLabel() const;
  unsigned int getMaxDistanceValue() const;
  double getDiffusionRateValue() const;
  bool getAutoMapping() const;
  bool getLinkColor() const;

  ColorScale* getDefaultColorScale() const {
    return defaultScale;
  }

  ColorScale* getPropertyColorScale(const std::string& propertyName);
  void clearpropertiesConfigurationWidget();
  void addfilter(Graph *g, std::vector<std::string> &propertyFilterType);
  void graphChanged(Graph *graph);
  std::vector<std::string> getSelectedProperties() const;
  unsigned getIterationNumber() const;
  unsigned getConnectivityIndex() const;
  SizeMappingType getSizeMapping() const;
  QList<QWidget *> configurationWidgets() const;

  void update(std::set<tlp::Observable *>::iterator begin, std::set<tlp::Observable *>::iterator end);

  void observableDestroyed(tlp::Observable *);
  /**
   * @brief Return true if using namiation during switching to preview.
   */
  bool useAnimation() const;
  /**
   * @brief Return the duration of the animation in second.
   */
  unsigned int getAnimationDuration() const;

  /**
   * @brief Store the configuration widget data properties.
   * Function called during getData function to store the state of the configuration widget.
   * @return The dataset containing the configuration state.
   */
  DataSet getData() const;

  /**
   * @brief Restore saved properties from data set.
   * @param data The previously saved dataset.
   */
  void setData(const DataSet& data);

  QButtonGroup* colorScalingButtonGroup;
  QRadioButton* singleColorScale;
  ColorScalePreview *singleColorScaleGradientPreview;
  QRadioButton* multipleColorScale;
  QPushButton* editGradients;

  QButtonGroup* sizeMappingButtonGroup;
  QRadioButton* noNodeSizeMappingRadioButton;
  QRadioButton* realNodeSizeMappingRadioButton;

  bool multiplePropertiesRepresentation;

protected:
  GradientManager gradientManager;
  ColorScale *defaultScale;
  SOMView *view;

protected slots:
  void diffusionMethodChange();
  void scalingMethodChange(QAbstractButton *);
  void animationCheckBoxClicked();
};
}
#endif /* SOMPROPERTIESWIDGET_H_ */
