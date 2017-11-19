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

#ifndef HISTOGRAMVIEWNAVIGATOR_H_
#define HISTOGRAMVIEWNAVIGATOR_H_

#include <tulip/GLInteractor.h>

namespace tlp {

class Histogram;
class HistogramView;

class HistogramViewNavigator : public GLInteractorComponent {

public:
  HistogramViewNavigator();
  ~HistogramViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *) override {
    return false;
  }
  bool compute(GlMainWidget *) override {
    return false;
  }
  void viewChanged(View *view) override;

private:
  Histogram *getOverviewUnderPointer(const Coord &sceneCoord) const;

  HistogramView *histoView;
  Histogram *selectedHistoOverview;
};
}

#endif /* HISTOGRAMVIEWNAVIGATOR_H_ */
