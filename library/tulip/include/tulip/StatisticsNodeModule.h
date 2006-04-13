//-*-c++-*-
/*
 Author: Delorme Maxime
 Email : Maxime.Delorme@gmail.com

 Last modification : 27/07/2005 (fr) 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef STATISTICS_MODULE
#define STATISTICS_MODULE

#include <vector>
#include <tulip/Matrix.h>

class SuperGraph;
class Metric;
class Coord;

/** \brief Structure used to store statistic results
 *
 *  This structure is designed to store statistic results computed by the class StatsNodeModule.
 *  It stores the average, the variance, the standard deviation, the covariance, the bounding box,
 *  the correlationMatrix, the Eigenvectors and the Linear Regression Function.
 */
struct StatisticResults
{
  int nDimensions;

  std::vector<float> averagePoint;
  std::vector<float> variancePoint;
  std::vector<float> standardDeviationPoint;
  std::vector<std::vector<float> > covariancePoints;
  std::vector<float> minPoint;
  std::vector<float> maxPoint;

  tlp::Matrix<float, 3> correlationMatrix;
  tlp::Matrix<float, 3> eigenVectors;

  float linearRegressionFunctionb0;
  float linearRegressionFunctionb1;
};

/** \brief Static class used to compute statistic results
 *
 *  This class is used to compute statistic results from a graph and up to three metrics. \n
 *  ComputeStatisticsResults is way more efficient than calling every other function, because 
 *  it computes every algorithm in only one pass. \n
 *
 *  There is two types of functions for the common statistic results. \n
 *
 *  The first type has 4 parameters which are the following :
 *   \param superGraph The supergraph structure that will be used to retrieve the list of nodes.
 *   \param metrics The metrics that will be treated by the algorithm. There can be up to 3 metrics.
 *   \param nDimensions The number of dimensions that will be treated. This integer ranges from 1 to 3
 *   \param result The vector containing the result of the algorithm.
 *  
 *  That kind of function computes the result for many dimensions (1 to 3)
 *
 *  The second one has only two parameters :
 *   \param superGraph The supergraph structure that will be used to retrieve the list of nodes
 *   \param metric The metric that will be treated by the algorithm
 *
 */

class TLP_SCOPE StatsNodeModule
{
 public:
  /**
   * Computes the Average point.
   */
  static void ComputeAveragePoint(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &result);

  /**
   * Computes the Average on only one metric.
   */
  static float ComputeAverage(SuperGraph *superGraph, Metric *metric);

  /**
   * Computes the Variance point.
   */
  static void ComputeVariancePoint(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &result);

  /**
   * Computes the Variance on only one metric.
   */
  static float ComputeVariance(SuperGraph *superGraph, Metric *metric);

  /**
   * Computes the Standard deviation point
   */
  static void ComputeStandardDeviationPoint(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &result);

  /**
   * Computes the Standard deviation point given a vector of variances
   */
  static void ComputeStandardDeviationPoint(const std::vector<float> &variances, int nDimensions, std::vector<float> &result);

  /**
   * Computes the Standard deviation on only one metric
   */
  static float ComputeStandardDeviation(SuperGraph *superGraph, Metric *metric);

  /**
   * Computes the Standard deviation on only one metric given a variance
   */
  static float ComputeStandardDeviation(float variance);

  /**
   * Computes the Covariance point
   */
  static void ComputeCovariancePoints(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<std::vector<float> > &result);

  /**
   * Computes the Covariance of a set of nodes on two metrics
   */
  static float ComputeCovariance(SuperGraph *superGraph, Metric* metric1, Metric* metric2);
  
  /**
   * Computes the Minimum point
   */
  static void ComputeMinPoint(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &result); 

  /**
   * Computes the Minimum of a metric
   */
  static float ComputeMin(SuperGraph *superGraph, Metric *metric);

  /**
   * Computes the Maximum point
   */
  static void ComputeMaxPoint(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &result);

  /**
   * Computes the Maximum of a metric
   */
  static float ComputeMax(SuperGraph *superGraph, Metric *metric);

  /**
   * Computes the Bounding box of a graph
   */
  static void ComputeMinMaxPoints(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions, std::vector<float> &resMin, std::vector<float> &resMax);

  /**
   * Computes the range of a metric in a graph
   */
  static void ComputeMinMax(SuperGraph *superGraph, Metric *metric, float &min, float &max);

  /**
   * Computes the Linear regression function of a graph on two metrics
   */
  static void ComputeLinearRegressionFunction(SuperGraph *superGraph, Metric *xk, Metric *yk, float &b0, float &b1);

  /**
   * Computes every statistic results and store it in a StatisticResults structure
   */
  static StatisticResults* ComputeStatisticsResults(SuperGraph *superGraph, const std::vector<Metric*> &metrics, int nDimensions);
};

#endif
