#include <tulip/StatisticsNodeModule.h>
#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Coord.h>
#include <math.h>

using namespace std;

namespace tlp
{
  void StatsNodeModule::ComputeAveragePoint(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &result)
  {
    Iterator<node> *itN = graph->getNodes();
    vector<float> average(nDimensions);

    for(int i=0; i < nDimensions; i++)
      average[i] = 0;

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  average[i] += metrics[i]->getNodeValue(n);
      }

    for(int i=0; i < nDimensions; i++)
      average[i] /= graph->numberOfNodes();

    delete itN;

    result = average;
  }

  float StatsNodeModule::ComputeAverage(Graph *graph, DoubleProperty *metric)
  {
    Iterator<node> *itN = graph->getNodes();
  
    float average = 0.0f;

    while (itN->hasNext())
      {
	node n = itN->next();

	average += metric->getNodeValue(n);
      }

    delete itN;

    average /= graph->numberOfNodes();

    return average;
  }

  void StatsNodeModule::ComputeVariancePoint(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &result)
  {
    Iterator<node> *itN = graph->getNodes();

    vector<float> average(nDimensions);
    vector<float> variance(nDimensions);
    float nodeVal;

    for(int i=0; i < nDimensions; i++)
      variance[i] = 0;

    StatsNodeModule::ComputeAveragePoint(graph, metrics, nDimensions, average);

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    nodeVal = metrics[i]->getNodeValue(n);

	    variance[i] += (nodeVal - average[i]) * (nodeVal - average[i]);
	  }
      }
    delete itN;

    int nNodes = graph->numberOfNodes();

    for(int i=0; i < nDimensions; i++)
      variance[i] /= nNodes;

    result = variance;
  }

  float StatsNodeModule::ComputeVariance(Graph *graph, DoubleProperty *metric)
  {
    Iterator<node> *itN = graph->getNodes();

    float average;
    float variance = 0.0f;
    float nodeVal;

    average = StatsNodeModule::ComputeAverage(graph, metric);

    while (itN->hasNext())
      {
	node n = itN->next();

	nodeVal = metric->getNodeValue(n);

	variance += (nodeVal - average) * (nodeVal - average);
      }

    delete itN;

    variance /= graph->numberOfNodes();

    return variance;
  }

  void StatsNodeModule::ComputeStandardDeviationPoint(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &result)
  {
    ComputeVariancePoint(graph, metrics, nDimensions, result);

    for(int i=0; i < nDimensions; i++)
      result[i] = sqrt(result[i]);
  }

  void StatsNodeModule::ComputeStandardDeviationPoint(const std::vector<float> &variances, int nDimensions, std::vector<float> &result)
  {
    result.resize(nDimensions);

    for(int i=0; i < nDimensions; i++)
      result[i] = sqrt(variances[i]);
  }

  float StatsNodeModule::ComputeStandardDeviation(Graph *graph, DoubleProperty *metric)
  {
    float variance = ComputeVariance(graph, metric);

    return sqrt(variance);
  }

  float StatsNodeModule::ComputeStandardDeviation(float variance)
  {
    return sqrt(variance);
  }


  float StatsNodeModule::ComputeCovariance(Graph *graph, DoubleProperty* metric1, DoubleProperty* metric2)
  {
    Iterator<node> *itN = graph->getNodes();

    float ave1, ave2, ave3;
    float sum1, sum2, sum3;

    ave1 = ave2 = ave3 = 0;
    sum1 = sum2 = sum3 = 0;

    while(itN->hasNext())
      {
	node n = itN->next();

	float v1 = metric1->getNodeValue(n);
	float v2 = metric2->getNodeValue(n);

	sum1 += v1;
	sum2 += v2;
	sum3 += (v1 * v2);
      }

    delete itN;

    ave1 = sum1 / graph->numberOfNodes();
    ave2 = sum2 / graph->numberOfNodes();
    ave3 = sum3 / graph->numberOfNodes();

    return (ave3) - (ave1 * ave2);
  }

  void StatsNodeModule::ComputeCovariancePoints(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<std::vector<float> > &result)
  {
    Iterator<node> *itN = graph->getNodes();

    std::vector<float> ave(nDimensions);
    std::vector<float> sum(nDimensions);
    std::vector<float> bigsum(nDimensions * nDimensions);
    std::vector<float> val(nDimensions);

    result.resize(nDimensions);

    for(int i=0; i < nDimensions; i++)
      {
	sum[i] = 0;

	for(int j=0; j < nDimensions; j++)
	  bigsum[i * nDimensions + j] = 0;

	result[i].resize(nDimensions);
      }

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    val[i] = metrics[i]->getNodeValue(n);
	    sum[i] += val[i];
	  }

	for(int i=0; i < nDimensions; i++)
	  for(int j=0; j < nDimensions; j++)
	    bigsum[i*nDimensions + j] += (val[i] * val[j]);
      }

    float nNode = graph->numberOfNodes();

    for(int i=0; i < nDimensions; i++)
      ave[i] = sum[i] / nNode;


    for(int i=0; i < nDimensions; i++)
      for(int j=0; j < nDimensions; j++)
	result[i][j] = (bigsum[i * nDimensions + j] / nNode) - (ave[i] * ave[j]);
  }

  void StatsNodeModule::ComputeMinPoint(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &result)
  {
    Iterator<node> *itN = graph->getNodes();
    vector<float> min(nDimensions);
    float nodeVal;

    for(int i=0; i < nDimensions; i++)
      min[i] = INT_MAX;

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    nodeVal = metrics[i]->getNodeValue(n);

	    if (nodeVal < min[i])
	      min[i] = nodeVal;
	  }
      }

    delete itN;

    result = min;
  }

  float StatsNodeModule::ComputeMin(Graph *graph, DoubleProperty *metric)
  {
    Iterator<node> *itN = graph->getNodes();

    float min = INT_MAX;

    while (itN->hasNext())
      {
	node n = itN->next();

	float nodeVal = metric->getNodeValue(n);

	if (nodeVal < min)
	  min = nodeVal;
      }

    delete itN;

    return min;
  }

  void StatsNodeModule::ComputeMaxPoint(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &result)
  {
    Iterator<node> *itN = graph->getNodes();
    vector<float> max(nDimensions);
    float nodeVal;

    for(int i=0; i < nDimensions; i++)
      max[i] = INT_MIN;

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    nodeVal = metrics[i]->getNodeValue(n);

	    if (nodeVal > max[i])
	      max[i] = nodeVal;
	  }
      }

    delete itN;

    result = max;
  }


  float StatsNodeModule::ComputeMax(Graph *graph, DoubleProperty *metric)
  {
    Iterator<node> *itN = graph->getNodes();

    float max = INT_MIN;

    while (itN->hasNext())
      {
	node n = itN->next();

	float nodeVal = metric->getNodeValue(n);

	if (nodeVal > max)
	  max = nodeVal;
      }

    delete itN;

    return max;
  }

  void StatsNodeModule::ComputeMinMaxPoints(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions, std::vector<float> &resMin, std::vector<float> &resMax)
  {
    Iterator<node> *itN = graph->getNodes();

    vector<float> min(nDimensions);
    vector<float> max(nDimensions);

    for(int i=0; i < nDimensions; i++)
      {
	min[i] = INT_MAX;
	max[i] = INT_MIN;
      }

    float nodeVal;

    while (itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    nodeVal = metrics[i]->getNodeValue(n);

	    if (nodeVal > max[i])
	      max[i] = nodeVal;

	    if (nodeVal < min[i])
	      min[i] = nodeVal;
	  }
      }

    delete itN;

    resMin = min;
    resMax = max;
  }

  void StatsNodeModule::ComputeMinMax(Graph *graph, DoubleProperty *metric, float &resMin, float &resMax)
  {
    Iterator<node> *itN = graph->getNodes();

    float min = INT_MAX;
    float max = INT_MIN;

    float nodeVal;
 
    while (itN->hasNext())
      {
	node n = itN->next();

	nodeVal = metric->getNodeValue(n);
      
	if (nodeVal > max)
	  max = nodeVal;

	if (nodeVal < min)
	  min = nodeVal;
      }

    delete itN;

    resMin = min;
    resMax = max;
  }

  void StatsNodeModule::ComputeLinearRegressionFunction(Graph *graph, DoubleProperty *xk, DoubleProperty *yk, float &b0, float &b1)
  {
    Iterator<node> *itN = graph->getNodes();

    float nodeValx, nodeValy;
    float sxk, syk, sxkxk, sxkyk;

    sxk   = 0;
    syk   = 0;
    sxkxk = 0;
    sxkyk = 0;

    while(itN->hasNext())
      {
	node n = itN->next();

	nodeValx = xk->getNodeValue(n);
	nodeValy = yk->getNodeValue(n);

	// We compute the sum of xk, yk, xk² and xkyk for the whole set of nodes
	sxk   += nodeValx;
	sxkxk += (nodeValx * nodeValx);
	syk   += nodeValy;
	sxkyk += (nodeValx * nodeValy);
      }

    delete itN;

    int n = graph->numberOfNodes();

    // Then we compute b0 and b1 :
    // The equation used is equation #6 on : http://www.unilim.fr/pages_perso/jean.debord/math/reglin/reglin.htm
    b0 = (sxkxk * syk - sxk * sxkyk) / (n * sxkxk - sxk * sxk);
    b1 = (n * sxkyk - sxk * syk)     / (n * sxkxk - sxk * sxk);
  }

  StatisticResults* StatsNodeModule::ComputeStatisticsResults(Graph *graph, const std::vector<DoubleProperty*> &metrics, int nDimensions)
  {
    StatisticResults *res = new StatisticResults;

    res->nDimensions = nDimensions;

    res->averagePoint.resize(nDimensions);
    res->variancePoint.resize(nDimensions);
    res->standardDeviationPoint.resize(nDimensions);
    res->minPoint.resize(nDimensions);
    res->maxPoint.resize(nDimensions);
    res->covariancePoints.resize(nDimensions);

    float sxk, syk, sxkxk, sxkyk;
    sxk = syk = sxkxk = sxkyk = 0;

    for(int i=0; i < nDimensions; i++)
      {
	res->covariancePoints[i].resize(nDimensions);

	res->minPoint[i] = INT_MAX;
	res->maxPoint[i] = INT_MIN;
      }

    Iterator<node> *itN = graph->getNodes();
  
    vector<float> bigsum(nDimensions * nDimensions);
    vector<float> vals(nDimensions);

    while(itN->hasNext())
      {
	node n = itN->next();

	for(int i=0; i < nDimensions; i++)
	  {
	    vals[i] = metrics[i]->getNodeValue(n);

	    res->averagePoint[i] += vals[i];

	    if (vals[i] < res->minPoint[i])
	      res->minPoint[i] = vals[i];
	    if (vals[i] > res->maxPoint[i])
	      res->maxPoint[i] = vals[i];
	  }

	for(int i=0; i < nDimensions; i++)
	  for(int j=0; j < nDimensions; j++)
	    bigsum[i * nDimensions + j] += (vals[i] * vals[j]);

	if (nDimensions == 2)
	  {
	    sxk   += vals[0];
	    syk   += vals[1];
	    sxkxk += (vals[0] * vals[0]);
	    sxkyk += (vals[0] * vals[1]);
	  }
      }

    delete itN;

    float nNodes = graph->numberOfNodes();

    for(int i=0; i < nDimensions; i++)
      res->averagePoint[i] /= nNodes;


    for(int i=0; i < nDimensions; i++)
      {
	for(int j=0; j < nDimensions; j++)
	  res->covariancePoints[i][j] = (bigsum[i * nDimensions + j] / nNodes) - (res->averagePoint[i] * res->averagePoint[j]);

	res->variancePoint[i] = res->covariancePoints[i][i];
	res->standardDeviationPoint[i] = sqrt(res->variancePoint[i]);
      }
	
    if (nDimensions == 2)
      {
	res->linearRegressionFunctionb0 = (sxkxk * syk - sxk * sxkyk)  / (nNodes * sxkxk - sxk * sxk);
	res->linearRegressionFunctionb1 = (nNodes * sxkyk - sxk * syk) / (nNodes * sxkxk - sxk * sxk);
      }

    if (nDimensions == 3)
      {
	res->correlationMatrix = tlp::Matrix<float, 3>(res->covariancePoints);
	if (!res->correlationMatrix.computeEigenVectors(res->eigenVectors))
	  {
	    cerr << "Couldn't Compute Eigen Vectors :(\n";
	    return NULL;
	  }
      }

    return res;
  }

}



