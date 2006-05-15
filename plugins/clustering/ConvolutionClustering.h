//-*-c++-*-
#ifndef _ConvolutionClustering_H
#define _ConvolutionClustering_H

#include <vector>

#include <tulip/TulipPlugin.h>

class ConvolutionClustering:public Clustering
{ 
public:

  ConvolutionClustering(ClusterContext);
  ~ConvolutionClustering();
  bool run();
  bool check(std::string &);
  void reset();
  std::vector<double> *getHistogram();
  void setParameters(int histosize,int threshold,int width);
  void getParameters(int &histosize,int &threshold,int &width);
  void autoSetParameter();  
  std::list<int> getLocalMinimum();
private:
  void buildSubGraphs(const std::vector<int>& ranges);
  std::vector<double> smoothHistogram;
  std::map<int,int> histogramOfValues;
  int histosize,threshold,width;
  tlp::DoubleProperty *metric;
};

#endif





