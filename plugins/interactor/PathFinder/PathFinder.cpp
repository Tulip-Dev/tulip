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

#include "PathFinder.h"

#include <tulip/MouseInteractors.h>
#include <tulip/View.h>
#include <tulip/StringsListSelectionWidget.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/Graph.h>

#include "PathFinderComponent.h"
#include "PathFinderConfigurationWidget.h"
#include "highlighters/EnclosingCircleHighlighter.h"
#include "highlighters/ZoomAndPanHighlighter.h"

#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>

using namespace tlp;
using namespace std;

PLUGIN(PathFinder)

PathFinder::PathFinder(const tlp::PluginContext *) :
  GLInteractorComposite(QIcon(":/pathfinder.png"), "Select the path(s) between two nodes"), weightMetric(NO_METRIC), selectAllPaths(false), edgeOrientation(DEFAULT_ORIENTATION),
  pathsTypes(DEFAULT_PATHS_TYPE), toleranceActivated(DEFAULT_TOLERANCE_ACTIVATION), tolerance(DEFAULT_TOLERANCE), _configurationWidget(NULL), highlightersListWidget(NULL), configureHighlighterBtn(NULL) {

  edgeOrientationLabels[PathAlgorithm::Oriented] = "Consider edges as oriented";
  edgeOrientationLabels[PathAlgorithm::NonOriented] = "Consider edges as non-oriented";
  edgeOrientationLabels[PathAlgorithm::Reversed] = "Consider edges as reversed";
  pathsTypesLabels[PathAlgorithm::AllPaths] = "Select all the paths";
  pathsTypesLabels[PathAlgorithm::AllShortest] = "Select all the shortest paths";
  pathsTypesLabels[PathAlgorithm::OneShortest] = "Select one of the shortest paths";
}

PathFinder::~PathFinder() {
  delete _configurationWidget;
}

bool PathFinder::isCompatible(const std::string &viewName) const {
  return (viewName==NodeLinkDiagramComponent::viewName);
}

void PathFinder::construct() {
  if (view() == NULL)
    return;

  push_back(new MousePanNZoomNavigator);
  PathFinderComponent *component = new PathFinderComponent(this);
  // installing path highlighters on the component
  component->addHighlighter(new EnclosingCircleHighlighter);
  component->addHighlighter(new ZoomAndPanHighlighter);
  push_back(component);

  _configurationWidget = new PathFinderConfigurationWidget();

  Graph *g = view()->graph();
  string s;

  _configurationWidget->addweightComboItem(NO_METRIC);
  forEach (s, g->getProperties()) {
    if (g->getProperty(s)->getTypename().compare("double") == 0)
      _configurationWidget->addweightComboItem(s.c_str());
  }
  _configurationWidget->setCurrentweightComboIndex(_configurationWidget->weightComboFindText(weightMetric.c_str()));

  for (map<PathAlgorithm::EdgeOrientation, string>::iterator it = edgeOrientationLabels.begin(); it != edgeOrientationLabels.end(); ++it)
    _configurationWidget->addedgeOrientationComboItem(it->second.c_str());

  _configurationWidget->setCurrentedgeOrientationComboIndex(_configurationWidget->edgeOrientationComboFindText(edgeOrientationLabels[edgeOrientation].c_str()));

  for (map<PathAlgorithm::PathType, string>::iterator it = pathsTypesLabels.begin(); it != pathsTypesLabels.end(); ++it)
    _configurationWidget->addpathsTypeComboItem(it->second.c_str());

  setPathsType(pathsTypesLabels[pathsTypes].c_str());

  _configurationWidget->toleranceChecked(toleranceActivated);
  _configurationWidget->setToleranceSpinValue(tolerance);

  highlightersListWidget = new StringsListSelectionWidget(_configurationWidget, StringsListSelectionWidget::SIMPLE_LIST, 0);
  vector<string> activeList, inactiveList;
  QSet<PathHighlighter *> highlighters(getPathFinderComponent()->getHighlighters());

  foreach(PathHighlighter *h, highlighters)
  inactiveList.push_back(h->getName());

  highlightersListWidget->setSelectedStringsList(activeList);
  highlightersListWidget->setUnselectedStringsList(inactiveList);

  if (activeList.empty() && inactiveList.empty()) {
    highlightersListWidget->setDisabled(true);
    _configurationWidget->highlightersLabelDisabled(true);
  }

  _configurationWidget->addbottomWidget(highlightersListWidget);
  configureHighlighterBtn = new QPushButton("Configure", _configurationWidget);
  QHBoxLayout *hlLayout = highlightersListWidget->findChild<QHBoxLayout *> ("horizontalLayout_2");

  if (hlLayout)
    hlLayout->addWidget(configureHighlighterBtn);

  connect(configureHighlighterBtn, SIGNAL(clicked(bool)), this, SLOT(configureHighlighterButtonPressed()));
  connect(_configurationWidget, SIGNAL(setWeightMetric(const QString &)), this, SLOT(setWeightMetric(const QString &)));
  connect(_configurationWidget, SIGNAL(setEdgeOrientation(const QString &)), this, SLOT(setEdgeOrientation(const QString &)));
  connect(_configurationWidget, SIGNAL(setPathsType(const QString &)), this, SLOT(setPathsType(const QString &)));
  connect(_configurationWidget, SIGNAL(activateTolerance(bool)), this, SLOT(activateTolerance(bool)));
  connect(_configurationWidget, SIGNAL(setTolerance(int)), this, SLOT(setTolerance(int)));
}

QWidget *PathFinder::configurationWidget() const {
  return _configurationWidget;
}

void PathFinder::setWeightMetric(const QString &metric) {
  weightMetric = metric.toUtf8().data();
}

void PathFinder::setEdgeOrientation(const QString &metric) {
  string cmp(metric.toUtf8().data());

  for (map<PathAlgorithm::EdgeOrientation, string>::iterator it = edgeOrientationLabels.begin(); it != edgeOrientationLabels.end(); ++it) {
    if (it->second.compare(cmp) == 0)
      edgeOrientation = it->first;
  }
}

void PathFinder::setSelectAllPaths(bool s) {
  selectAllPaths = s;
}

void PathFinder::setPathsType(const QString &pathType) {
  string cmp(pathType.toStdString());

  for (map<PathAlgorithm::PathType, string>::iterator it = pathsTypesLabels.begin(); it != pathsTypesLabels.end(); ++it) {
    if (it->second.compare(cmp) == 0)
      pathsTypes = it->first;
  }

  bool disabled(pathsTypes != PathAlgorithm::AllPaths);
  _configurationWidget->toleranceCheckDisabled(disabled);
  _configurationWidget->toleranceSpinDisabled(disabled);
  _configurationWidget->toleranceLabelDisabled(disabled);
}

double PathFinder::getTolerance() {
  if (!toleranceActivated)
    return DBL_MAX;

  return tolerance / 100;
}

void PathFinder::setTolerance(int i) {
  tolerance = i;
}

void PathFinder::activateTolerance(bool activated) {
  toleranceActivated = activated;
}

vector<string> PathFinder::getActiveHighlighters() {
  return highlightersListWidget->getSelectedStringsList();
}

vector<string> PathFinder::getInactiveHighlighters() {
  return highlightersListWidget->getUnselectedStringsList();
}

vector<string> PathFinder::getHighlighters() {
  if (highlightersListWidget)
    return highlightersListWidget->getCompleteStringsList();

  return vector<string>();
}

void PathFinder::configureHighlighterButtonPressed() {
  /*
   * Each highlighter has it's own configuration widget.
   * We build a QDialog and integrate this widget into it to display highlighter-specific configuration to the user.
   */
  QListWidget *listWidget = dynamic_cast<QListWidget *> (highlightersListWidget->findChild<QListWidget *> ("listWidget"));

  if (!listWidget)
    return;

  QList<QListWidgetItem *> lst = listWidget->selectedItems();
  string text("");

  for (QList<QListWidgetItem *>::iterator it = lst.begin(); it != lst.end(); ++it)
    text = (*it)->text().toStdString();

  QSet<PathHighlighter *> highlighters(getPathFinderComponent()->getHighlighters());
  PathHighlighter *hler(0);

  foreach(PathHighlighter *h, highlighters) {
    if (h->getName() == text)
      hler = h;
  }

  if (hler && hler->isConfigurable()) {
    QDialog *dialog = new QDialog;
    QVBoxLayout *verticalLayout = new QVBoxLayout(dialog);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    verticalLayout->addLayout(mainLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(dialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);

    verticalLayout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    mainLayout->addWidget(hler->getConfigurationWidget());
    dialog->exec();
    delete dialog;
  }
  else
    QMessageBox::warning(0, "No configuration", "No configuration available for this highlighter");
}

PathFinderComponent *PathFinder::getPathFinderComponent() {
  // Look upon all the installed components and stop as soon as we get a PathFinderComponent * object.
  for (iterator it = begin(); it != end(); ++it) {
    PathFinderComponent *c = dynamic_cast<PathFinderComponent *> (*it);

    if (c)
      return c;
  }

  return 0;
}
