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
#ifndef _ConvolutionClustering_H
#define _ConvolutionClustering_H

#include <vector>

#include <tulip/TulipPlugin.h>

class ConvolutionClustering:public tlp::Algorithm
{ 
public:

  ConvolutionClustering(tlp::AlgorithmContext);
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





