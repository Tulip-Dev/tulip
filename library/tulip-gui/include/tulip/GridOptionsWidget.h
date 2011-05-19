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
#ifndef _GRID_WIDGET_H_
#define _GRID_WIDGET_H_

#include "ui_GridOptions.h"

#include <tulip/GlMainWidget.h>
#include <tulip/RenderingParametersDialog.h>
#include <tulip/GlGrid.h>

namespace tlp {
/** \brief Widget for displaying a grid on the graph.
 *
 * This class is a widget accessible from the interface of Tulip Software.
 * It is used to display a grid with some options. 
 *
 * You can, first, choose which type of cell you'll have :
 *  - Subdivisions cells are computed by taking the whole length of each axis of the graph, andby dividing them.
 *  - Sized cells are directly computed with the size you indicate.
 * 
 * Other options are available :
 * 
 *  - Display Dimensions : Allows the user to choose on which dimensions the grid will be displayed.
 *
 */
  class TLP_QT_SCOPE GridOptionsWidget : public QDialog, public Ui::GridOptionsData {

  Q_OBJECT

 protected:
  GlMainWidget *glMainWidget; /**< The considered GlMainWidget */
  RenderingParametersDialog *paramDialog; /**< The considered RenderingParametersDialog */
  tlp::GlGrid *grid; /**< The considered GlADGrid */

 public:
  /**
   * Constructor.
   */
  GridOptionsWidget(QWidget *parent = 0);

  /**
   * Destructor.
   */
  ~GridOptionsWidget();

  /**
   * Function used to define on which GlMainWidget we are working
   */
  void setCurrentMainWidget(GlMainWidget *graphWidget);

  /**
   * Function used to define on which RenderingParametersDialog we are working
   */
  void setCurrentRenderingParametersDialog(RenderingParametersDialog *paramDialog);						

 public slots:
 
  /**
   * Function used to compute the grid and close the widget (Button OK).
   */
  void validateGrid();

  /**
   * Function used to enable the panel "Subdivision"
   */
  void chGridSubdivisions();

  /**
   * Function used to enable the panel "Size"
   */
  void chGridSize();

  /**
   * Function used to change on which axis will be displayed the grid.
   */
  void chDisplayGrid();
};
}
#endif
