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
#ifndef _ConvolutionClustering_H
#define _ConvolutionClustering_H

#include <vector>

#include <tulip/TulipPluginHeaders.h>

/** This plugin allow the discretization and the filtering of the distribution of
 * a node metric using convolution.
 *
 * A detailed usage of this procedure is detailed in:
 *
 * D. Auber, M. Delest and Y. Chiricota \n
 * "Strahler based graph clustering using convolution",\n
 * Published by the IEEE Computer Society, \n
 * 2004.
 *
 */

namespace tlp {

class ConvolutionClustering : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Convolution", "David Auber", "14/08/2001",
      "Discretization and filtering of the distribution of a node metric using a convolution "
      "following: <br>"
      "Strahler based graph clustering using convolution, "
      "D. Auber, M. Delest and Y. Chiricota, "
      "Proceedings of the Eighth International Conference on Information Visualisation, 2004. IV "
      "2004, "
      "doi: <a href=\"https://doi.org/10.1109/IV.2004.1320123\">10.1109/IV.2004.1320123</a>",
      "2.1", "Clustering")
  ConvolutionClustering(tlp::PluginContext *context);
  bool run() override;
  bool check(std::string &) override;
  std::vector<double> *getHistogram();
  void setParameters(int histosize, int threshold, int width);
  void getParameters(int &histosize, int &threshold, int &width);
  void autoSetParameter();
  std::list<int> getLocalMinimum();

private:
  void getClusters(const std::vector<int> &ranges);
  std::vector<double> smoothHistogram;
  std::map<int, int> histogramOfValues;
  int histosize, threshold, width;
  tlp::NumericProperty *metric;
};
} // namespace tlp

#endif
