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


#include "SOMPropertiesWidget.h"
#include "SOMView.h"
#include "ui_SOMPropertiesWidget.h"
#include "ColorScalePreview.h"

#include <QIntValidator>
#include <QDoubleValidator>
#include <QRadioButton>
#include <QPushButton>

#include <tulip/GraphPropertiesSelectionWidget.h>
#include <tulip/ColorScalesManager.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TlpQtTools.h>


using namespace std;
using namespace tlp;

SOMPropertiesWidget::SOMPropertiesWidget(SOMView* view, QWidget *parent) :
  QWidget(parent), _ui(new Ui::SOMPropertiesWidget), view(view) {
  _ui->setupUi(this);
  dimensionConfigurationWidget = new tlp::ComputeSOMWidget(parent);

  defaultScale = new ColorScale(ColorScalesManager::getLatestColorScale());

  defaultScale->addObserver(this);

  QVBoxLayout *sizeMappingLayout = new QVBoxLayout(_ui->nodeSizeMappingGroupBox);
  sizeMappingLayout->setMargin(0);
  sizeMappingLayout->setSpacing(0);
  sizeMappingLayout->setContentsMargins(0, 5, 0, 0);
  sizeMappingButtonGroup = new QButtonGroup();
  noNodeSizeMappingRadioButton = new QRadioButton("No size mapping");
  sizeMappingButtonGroup->addButton(noNodeSizeMappingRadioButton);
  sizeMappingLayout->addWidget(noNodeSizeMappingRadioButton);
  realNodeSizeMappingRadioButton = new QRadioButton("Map node size on real node size");
  sizeMappingButtonGroup->addButton(realNodeSizeMappingRadioButton);
  sizeMappingLayout->layout()->addWidget(realNodeSizeMappingRadioButton);
  realNodeSizeMappingRadioButton->setChecked(true);

  //Display multiple properties at same time
  multiplePropertiesRepresentation = false;

  dimensionConfigurationWidget->setWindowTitle("Dimensions");
  setWindowTitle("Options");
}

QList<QWidget *> SOMPropertiesWidget::configurationWidgets() const {

  QList<QWidget *> widgets;

  widgets << dimensionConfigurationWidget << const_cast<QWidget *>(static_cast<const QWidget*>(this));

  return widgets;
}

unsigned int SOMPropertiesWidget::getGridWidth() const {
  return _ui->gridWidthSpinBox->value();
}
unsigned int SOMPropertiesWidget::getGridHeight() const {
  return _ui->gridHeightSpinBox->value();
}

QString SOMPropertiesWidget::getConnectivityLabel() const {
  return _ui->nodeConnectivityComboBox->currentText();
}

unsigned SOMPropertiesWidget::getConnectivityIndex() const {
  return _ui->nodeConnectivityComboBox->currentIndex();
}

bool SOMPropertiesWidget::getOppositeConnected() const {
  return _ui->opposedConnectedCheckBox->checkState() == Qt::Checked ? true : false;
}

double SOMPropertiesWidget::getLearningRateValue() const {
  return _ui->baseLearningRateSpinBox->value();
}

QString SOMPropertiesWidget::getDiffusionRateMethodLabel() const {
  return _ui->diffusionRateComputationMethodComboBox->currentText();
}

unsigned int SOMPropertiesWidget::getMaxDistanceValue() const {
  return _ui->maxDistanceSpinBox->value();
}

double SOMPropertiesWidget::getDiffusionRateValue() const {
  return _ui->baseDiffusionRateSpinBox->value();
}

bool SOMPropertiesWidget::getAutoMapping() const {
  return _ui->autoMappingCheckBox->isChecked();
}

bool SOMPropertiesWidget::getLinkColor() const {
  return _ui->colorLinkCheckBox->checkState() == Qt::Checked ? true : false;
}

bool SOMPropertiesWidget::useAnimation() const {
  return _ui->animationCheckBox->isChecked();
}

unsigned int SOMPropertiesWidget::getAnimationDuration() const {
  return _ui->animationStepsSpinBox->value();
}

unsigned SOMPropertiesWidget::getIterationNumber() const {
  return dimensionConfigurationWidget->number();
}

void SOMPropertiesWidget::clearpropertiesConfigurationWidget() {
  dimensionConfigurationWidget->clearLists();
}

void SOMPropertiesWidget::addfilter(Graph *g, vector<string> &propertyFilterType) {
  dimensionConfigurationWidget->setWidgetParameters(g, propertyFilterType);
}

vector<string> SOMPropertiesWidget::getSelectedProperties() const {
  return dimensionConfigurationWidget->getSelectedProperties();
}

SOMPropertiesWidget::~SOMPropertiesWidget() {
  delete defaultScale;
  delete _ui;
}

void SOMPropertiesWidget::diffusionMethodChange() {
}

void SOMPropertiesWidget::scalingMethodChange(QAbstractButton * button) {
  if (button == singleColorScale) {
    editGradients->setEnabled(false);
  }
  else {
    editGradients->setEnabled(true);
  }
}

void SOMPropertiesWidget::graphChanged(tlp::Graph *graph) {

  vector<string> types;
  types.push_back("double");
  tlp::GraphPropertiesSelectionWidget s;
  s.setWidgetParameters(graph, types);
  //Init the set of colors with all the possible properties.
  gradientManager.init(s.getCompleteStringsList());

}

tlp::ColorScale* SOMPropertiesWidget::getPropertyColorScale(const std::string&) {
  return defaultScale;
}

SOMPropertiesWidget::SizeMappingType SOMPropertiesWidget::getSizeMapping() const {
  if (noNodeSizeMappingRadioButton->isChecked())
    return NoSizeMapping;
  else
    return RealNodeSizeMapping;

}

void SOMPropertiesWidget::update(std::set<tlp::Observable *>::iterator, std::set<tlp::Observable *>::iterator) {
  view->updateDefaultColorProperty();
}

void SOMPropertiesWidget::observableDestroyed(tlp::Observable*) {

}

void SOMPropertiesWidget::animationCheckBoxClicked() {
  _ui->animationStepsSpinBox->setEnabled(_ui->animationCheckBox->isChecked());
}

DataSet SOMPropertiesWidget::getData() const {
  DataSet data;

  //Save grid state.
  data.set("gridWidth", getGridWidth());
  data.set("gridHeight", getGridHeight());
  data.set("oppositeConnected", getOppositeConnected());
  data.set("connectivity", _ui->nodeConnectivityComboBox->currentIndex());
  //Learning rate properties.
  data.set("learningRate", getLearningRateValue());

  //Diffusion rate properties.
  data.set("diffusionMethod", _ui->diffusionRateComputationMethodComboBox->currentIndex());
  data.set("maxDistance", getMaxDistanceValue());
  data.set("diffusionRate", getDiffusionRateValue());

  //Representation properties
  data.set("performMapping", getAutoMapping());
  data.set("linkColors", getLinkColor());

  //SizeMapping
  data.set("useSizeMapping", getSizeMapping() == SOMPropertiesWidget::RealNodeSizeMapping);

  //Animation
  data.set("withAnimation", useAnimation());
  data.set("animationDuration", getAnimationDuration());

  //Save current properties.
  vector<string> properties = dimensionConfigurationWidget->getSelectedProperties();

  if (!properties.empty()) {
    //Use QStringList class to store a list in a string
    QStringList stringlist;

    for (vector<string>::iterator it = properties.begin(); it != properties.end(); ++it) {
      stringlist.push_back(tlpStringToQString(*it));
    }

    data.set("properties", QStringToTlpString(stringlist.join(";")));
  }

  //Save iteration number
  data.set("iterationNumber", dimensionConfigurationWidget->number());

  //Save color scale
  DataSet colorScaleDataSet;
  map<float, Color> colorScaleMap = defaultScale->getColorMap();
  QStringList colorsList;

  for (map<float, Color>::iterator it = colorScaleMap.begin(); it != colorScaleMap.end(); ++it) {
    colorsList.push_back(tlpStringToQString(ColorType::toString(it->second)));
  }

  colorScaleDataSet.set("colorList", QStringToTlpString(colorsList.join(";")));
  colorScaleDataSet.set("gradient", defaultScale->isGradient());
  data.set("defaultScale", colorScaleDataSet);

  return data;
}

void SOMPropertiesWidget::setData(const DataSet& data) {

  double doubleValue = 0;
  unsigned int uintValue = 0;
  bool boolValue = false;
  int intValue = 0;
  //Restore grid state.
  data.get("gridWidth", uintValue);
  _ui->gridWidthSpinBox->setValue(uintValue);
  data.get("gridHeight", uintValue);
  _ui->gridHeightSpinBox->setValue(uintValue);
  data.get("connectivity", intValue);
  _ui->nodeConnectivityComboBox->setCurrentIndex(intValue);
  data.get("oppositeConnected", boolValue);
  _ui->opposedConnectedCheckBox->setChecked(boolValue);

  //Learning rate properties.
  data.get("learningRate", doubleValue);
  _ui->baseLearningRateSpinBox->setValue(doubleValue);

  //Diffusion rate properties.
  data.get("diffusionMethod", intValue);
  _ui->diffusionRateComputationMethodComboBox->setCurrentIndex(intValue);

  data.get("maxDistance", uintValue);
  _ui->maxDistanceSpinBox->setValue(uintValue);
  data.get("diffusionRate", doubleValue);
  _ui->baseDiffusionRateSpinBox->setValue(doubleValue);

  //Representaion properties
  data.get("performMapping", boolValue);
  _ui->autoMappingCheckBox->setChecked(boolValue);
  data.get("linkColors", boolValue);
  _ui->colorLinkCheckBox->setChecked(boolValue);

  //SizeMapping
  data.get("useSizeMapping", boolValue);

  if (boolValue) {
    realNodeSizeMappingRadioButton->setChecked(true);
  }
  else {
    noNodeSizeMappingRadioButton->setChecked(true);
  }

  //Animation
  data.get("withAnimation", boolValue);
  _ui->animationCheckBox->setChecked(boolValue);
  data.get("animationDuration", uintValue);
  _ui->animationStepsSpinBox->setValue(uintValue);

  //If there is saved properties reload them.
  if (data.exist("properties")) {
    string propertiesString;
    data.get("properties", propertiesString);
    //Use QString split function to parse string.
    QStringList list = tlpStringToQString(propertiesString).split(";", QString::SkipEmptyParts);
    vector<string> properties;

    foreach(const QString &s, list) {
      properties.push_back(QStringToTlpString(s));
    }

    dimensionConfigurationWidget->setOutputPropertiesList(properties);
  }

  data.get("iterationNumber", uintValue);
  dimensionConfigurationWidget->setNumber(uintValue);

  //Reload color scale

  DataSet colorScaleDataSet;
  data.get("defaultScale", colorScaleDataSet);
  string colorsList;
  colorScaleDataSet.get("colorList", colorsList);
  QString colorListQString = tlpStringToQString(colorsList);
  QStringList colorStringList = colorListQString.split(";");
  vector<Color> colors;

  for (QStringList::iterator it = colorStringList.begin(); it != colorStringList.end(); ++it) {
    Color c;

    if (ColorType::fromString(c, QStringToTlpString(*it))) {
      colors.push_back(c);
    }
  }

  colorScaleDataSet.get("gradient", boolValue);
  //Avoid calling update while data initialization causing segfault
  defaultScale->removeObserver(this);
  defaultScale->setColorScale(colors, boolValue);
  defaultScale->addObserver(this);
}
