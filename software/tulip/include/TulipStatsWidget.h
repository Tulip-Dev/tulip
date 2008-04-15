//-*-c++-*-
/*
 Author: Delorme Maxime
 Email : Maxime.Delorme@gmail.com

 Last modification : 07/06/2005 (fr) 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef _STATS_WIDGET_H_
#define _STATS_WIDGET_H_

#include "TulipStatsData.h"
#include <QtGui/qdialog.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Graph.h>
#include <tulip/StatisticsNodeModule.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Plane.h>


#define N_STATS_GLAD 7

#define GLAD_AVERAGE           0
#define GLAD_STANDARDDEVIATION 1
#define GLAD_AABB              2
#define GLAD_EIGENVECTORS      3
#define GLAD_LINEARREGRESSION  4
#define GLAD_CLUSTERINGPLANE   5
#define GLAD_GRID              6


class GlMainWidget;
class SGHierarchyWidget;

namespace tlp {
class DoubleProperty;

/** \brief Panel widget using tulip data to compute statistical results.
 *
 * This class is a widget which takes places in the main interface of tulip, in the tabwidget of properties.
 * This panel has mainly 3 uses :
 * 
 *  -Firstly : It changes the graph placement and order it under the form of an histogram or a scatter plot according to a defined number of metrics (up to 3).
 *  -Secondly : It computes and display some basic statistics results on the graph : average, variance, standard deviation, bounding box, linear regression and eigenvectors.
 *  -Thirdly : It clusterize the graph by a plane which is based upon statistic results calculated above.
 */
  class TulipStats
    : public QDialog, public Ui::TulipStatsData, public GraphObserver
{
  Q_OBJECT

 protected:
  GlMainWidget *glMainWidget; /**< A reference to the graphwidget we are currently editing */
  Graph *graph; /**< A reference to the graph we are considering */
  tlp::StatisticResults *statsResults; /**< A structure to store the results of the calculus */
  SGHierarchyWidget *clusterTreeWidget; /**< A reference to the clusterTree widget of the properties panel, to make an up to date */

  //tlp::GlADAxisPoint *averagePoint; /**< An augmented display for the average point */

  //tlp::GlADBox *stdDeviationBox; /**< An augmented display for the standard deviation */
  //tlp::GlADBox *AABB; /**< An augmented display for the bounding box */

  //tlp::GlADLine *eigenVectors[3]; /**< An augmented display for the eigenvectors */
  //tlp::GlADLine *linearRegression; /**< An augmented display for the linear regression */

  //tlp::GlADQuad *clusteringPlane; /**< An augmented display for the clustering plane */

  // GRID_REGION
  //  GlADGrid *grid; /**< An augmented display for the gris \attention This will be removed soon */

  int histoMax; /**< The value of the biggest column in the case of an histogram. Used to fit correctly the augmented displays */

  std::vector<DoubleProperty*> metrics; /**< The metrics used for the computation */
  int nMetrics; /**< The number of metrics used. \f$ 1 \leq nMetrics \leq \f$ 3 */

  float discretizationStep[3]; /**< The discretization step used to regroup informations */

 private:

  /**
   * Function used to reset every information of the panel.
   */
  void reset();

  /**
   * Function used to reset the Display tab.
   * 
   * It resets every computed result.
   */
  void resetDisplayTab();

  /**
   * Function used to reset the Algorithm tab.
   *
   * It resets mainly the combobox of the tab.
   */
  void resetClusteringTab();

  /**
   * Function used to clear the composite.
   */
  void resetComposite(bool resetGlComposite);

  /**
   * Function used to init the clustering tab.
   *
   * It mainly inits the combobox with proper value (in function of what has been computed).
   */
  void initClusteringTab();

  /**
   * Function used to update the list of available metrics.
   */
  void updateMetrics();

  /**
   * Function used to update the displayed Algorithm plane when the user changes the coordinates.
   */
  void updateClusteringPlane();

  /**
   * Utility function used to convert a std::vector of float in a string.
   */
  std::string vectorfToString(std::vector<float> vec, int nElem);

  /**
   * Utility function used to convert a tlp::Vector of float in a string.
   */
  std::string vectorfToString(tlp::Vector<float, 3> vec);

 public:
  
  /**
   * Base constructor.
   */
  TulipStats(QWidget *parent = 0);
  
  /**
   * Destructor.
   */
  ~TulipStats();

  /**
   * Function used by viewGl to set the reference to the clusterTree.
   */
  void setSGHierarchyWidgetWidget(SGHierarchyWidget *);

  /**
   * Accessor in reading to the graph.
   */
  Graph* getGraph();

  /**
   * Accessor in reading to the glMainWidget.
   */
  GlMainWidget* getGlMainWidget();

  /**
   * Accessor in writing to the glMainWidget.
   */
  void setGlMainWidget(GlMainWidget *);

  /**
   * Overriden function used to notify when the graph is going to be destroyed :
   */
  void destroy(Graph *);

 private slots:

  /**
   * Reset the panel or update the available metrics in function of the state of the glMainWidget.
   */
  void refreshMetricsSlot();

  /**
   * Adds a metric in the "used metrics" list.
   */
  void addMetricSlot();

  /**
   * Deletes the selected metric in the "used metrics" list.
   */
  void delMetricSlot();

  /**
   * Deletes the metric of metricID index in the "used metrics" list.
   */
  void delMetricSlot(int metricID);

  /**
   * Function computing the statistics results depending on the metrics selected.
   */
  void computeResultsSlot();

  /**
   * Function changing the layout of the graph to reorder it under the form of an histogram or a scatter plot.
   */
  void changeLayoutSlot();

  /**
   * Enables or disables the edit box for the discretization steps.
   */
  void updateDiscretizationSlot();

  /**
   * Verify whether the value in the discretization edit boxes are valid.
   */
  void checkStepValueSlot();

  /**
   * Enables or Disables the display on the graph of the average point.
   */
  void chDisplayAverageSlot();

  /**
   * Enables or Disables the display on the graph of the standard deviation box.
   */
  void chDisplayStdDeviationSlot();

  /**
   * Enables or Disables the display on the graph of the bounding box.
   */
  void chDisplayBoundingBoxSlot();

  /**
   * Enables or Disables the display of the linear regression line.
   */
  void chDisplayLinearRegressionSlot();

  /**
   * Enables or Disables the display of the eigenvectors.
   */
  void chDisplayEigenVectorsSlot();

  /**
   * Enables or Disables the display of the clustering plane.
   */
  void chDisplayClusteringPlaneSlot();

  /**
   * Changes the coordinates of the clustering plane according to the selection in the combobox of the "clustering" tab.
   */
  void chClusteringModelSlot(const QString &string);

  /**
   * Updates the position of the clustering plane according to the values of the coordinates edit boxes.
   */
  void updateClusteringPlaneSlot();

  /**
   * Clusterizes the graph.
   */
  void clusterizeSlot();

  /**
   * Enables the usage of the "Add Metric" button.
   */
  void enableAddMetricBtn();

  /**
   * Enables the usage of the "Remove Metric" button.
   */
  void enableRemoveMetricBtn();
};
}
#endif
