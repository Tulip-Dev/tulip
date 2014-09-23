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

#ifndef PATHFINDER_H_
#define PATHFINDER_H_

#include <tulip/GLInteractor.h>

#include "../../utils/StandardInteractorPriority.h"
#include "PathFinding/PathAlgorithm.h"

#define NO_METRIC "None"
#define DEFAULT_ORIENTATION PathAlgorithm::NonOriented
#define DEFAULT_PATHS_TYPE PathAlgorithm::OneShortest
#define DEFAULT_TOLERANCE 100
#define DEFAULT_TOLERANCE_ACTIVATION false

class QPushButton;

namespace tlp {
class PathFinderComponent;
class PathHighlighter;
class PathFinderConfigurationWidget;

class StringsListSelectionWidget;
class BooleanProperty;

/*@{*/
/** \file
*  \brief PathFinder helps you finding paths between nodes in your graph.
*
* This plug-in offers several possibilities to highlight different kinds of paths between two nodes in a graph.
* You can thus display one or several shortest paths between two selected nodes, or you can even extend to non-shortest paths
* with length limitations.
*/
class PathFinder: public tlp::GLInteractorComposite {
  Q_OBJECT
public:

  PLUGININFORMATION("PathFinder", "Tulip Team", "03/24/2010", "Path finding interactor", "1.0", "Information")

  PathFinder(const tlp::PluginContext *);
  virtual ~PathFinder();
  void construct();
  unsigned int priority() const {
    return tlp::StandardInteractorPriority::PathSelection;
  }
  virtual QWidget *configurationWidget() const;

  /**
   * @return The name of the property used to get the weight values over the edges.
   */
  inline std::string getWeightMetricName() const {
    return weightMetric;
  }

  /**
   * @return true if the user chose not to select only one path
   */
  inline bool isSelectAllPaths() const {
    return selectAllPaths;
  }

  /**
   * @return The edge orientation used when computing the path.
   * @see PathAlgorithm::EdgeOrientation
   */
  inline PathAlgorithm::EdgeOrientation getEdgeOrientation() const {
    return edgeOrientation;
  }

  /**
   * @return the type of path the user wants to select.
   * @see PathAlgorithm::PathType
   */
  inline PathAlgorithm::PathType getPathsType() const {
    return pathsTypes;
  }

  /**
   * @return The length tolerance factor when the user doesn't want only the shortest path.
   */
  double getTolerance();

  /**
   * @return The active path highlighters
   */
  std::vector<std::string> getActiveHighlighters();

  /**
   * @return The inactive path highlighters
   */
  std::vector<std::string> getInactiveHighlighters();

  /**
   * @return All the installed path highlighters
   */
  std::vector<std::string> getHighlighters();

  virtual bool isCompatible(const std::string &viewName) const;

public slots:
  void setSelectAllPaths(bool s);
  void setEdgeOrientation(const QString &orientation);
  void setPathsType(const QString &pathType);
  void setWeightMetric(const QString &metric);
  void setTolerance(int i);
  void activateTolerance(bool activated);
  void configureHighlighterButtonPressed();

private:
  PathFinderComponent *getPathFinderComponent();

  std::string weightMetric;
  bool selectAllPaths;
  PathAlgorithm::EdgeOrientation edgeOrientation;
  PathAlgorithm::PathType pathsTypes;
  bool toleranceActivated;
  double tolerance;

  // Used for GUI interaction.
  std::map<PathAlgorithm::EdgeOrientation, std::string> edgeOrientationLabels;
  std::map<PathAlgorithm::PathType, std::string> pathsTypesLabels;

  // GUI elements.
  PathFinderConfigurationWidget *_configurationWidget;
  tlp::StringsListSelectionWidget *highlightersListWidget;
  QPushButton *configureHighlighterBtn;
};

}
#endif /* PATHFINDER_H_ */
