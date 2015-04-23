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
#ifndef CONVOLUTIONCLUSTERINGSETUP_H
#define CONVOLUTIONCLUSTERINGSETUP_H

#include <QDialog>

namespace Ui {
class ConvolutionClusteringSetupData;
}

class QWidget;

namespace tlp {

class ConvolutionClustering;

class ConvolutionClusteringSetup : public QDialog {
  Q_OBJECT

  Ui::ConvolutionClusteringSetupData* _ui;
public:
  ConvolutionClusteringSetup(ConvolutionClustering *convolPlugin,QWidget* parent = NULL);
  ~ConvolutionClusteringSetup();

  inline ConvolutionClustering *getPlugin() const {
    return convolPlugin;
  }
  inline bool getLogarithmicScale() const {
    return useLogarithmicScale;
  }
  void abort() {
    reject();
  }

public slots:
  void update();
  void setlog(bool);
private:
  QWidget *histogramWidget;
  ConvolutionClustering *convolPlugin;
  bool useLogarithmicScale;
};
}
#endif // CONVOLUTIONCLUSTERINGSETUP_H
