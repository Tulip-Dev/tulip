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

#ifndef SOMPROPERTIESWIDGET_H_
#define SOMPROPERTIESWIDGET_H_

#include "ui_SOMPropertiesWidget.h"

#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <QtGui/QDialog>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>

#include "GradientManager.h"
#include "ColorScalePreview.h"
class SOMView;

class SOMPropertiesWidget: public QWidget, public Ui::SOMPropertiesWidget, public tlp::Observable {
Q_OBJECT
public:

  enum SizeMappingType {
    NoSizeMapping, RealNodeSizeMapping, DistanceMapping
  };
  SOMPropertiesWidget(SOMView* view, QWidget *parent = NULL);
  virtual ~SOMPropertiesWidget();

  unsigned int getGridWidth() const {
    return gridWidthSpinBox->value();
  }
  unsigned int getGridHeight() const {
    return gridHeightSpinBox->value();
  }

  QString getConnectivityLabel() const {
    return nodeConnectivityComboBox->currentText();
  }

  bool getOppositeConnected() const {
    return opposedConnectedCheckBox->checkState() == Qt::Checked ? true : false;
  }

  double getLearningRateValue() const {
    return baseLearningRateSpinBox->value();
  }

  QString getDiffusionRateMethodLabel() const {
    return diffusionRateComputationMethodComboBox->currentText();
  }

  unsigned int getMaxDistanceValue() const {
    return maxDistanceSpinBox->value();
  }

  double getDiffusionRateValue() const {
    return baseDiffusionRateSpinBox->value();
  }

  bool getAutoMapping() const {
    return autoMappingCheckBox->isChecked();
  }

  bool getLinkColor() const {
    return colorLinkCheckBox->checkState() == Qt::Checked ? true : false;
  }

  tlp::ColorScale* getDefaultColorScale() const {
    return defaultScale;
  }

  tlp::ColorScale* getPropertyColorScale(const std::string& propertyName);

  void graphChanged(tlp::Graph *graph);

  SizeMappingType getSizeMapping() const;

  void update(std::set<tlp::Observable *>::iterator begin, std::set<tlp::Observable *>::iterator end);

  void observableDestroyed(tlp::Observable *);
  /**
   * @brief Return true if using namiation during switching to preview.
   */
  bool useAnimation() const {
    return animationCheckBox->isChecked();
  }
  /**
   * @brief Return the duration of the animation in second.
   */
  unsigned int getAnimationDuration() const {
    return animationStepsSpinBox->value();
  }

  /**
   * @brief Store the configuration widget data properties.
   * Function called during getData function to store the state of the configuration widget.
   * @return The dataset containing the configuration state.
   */
  tlp::DataSet getData() const;

  /**
   * @brief Restore saved properties from data set.
   * @param data The previously saved dataset.
   */
  void setData(const tlp::DataSet& data);

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
  tlp::ColorScale *defaultScale;
  SOMView *view;

protected slots:
  void diffusionMethodChange();
  void scalingMethodChange(QAbstractButton *);
  void animationCheckBoxClicked();
};

#endif /* SOMPROPERTIESWIDGET_H_ */
