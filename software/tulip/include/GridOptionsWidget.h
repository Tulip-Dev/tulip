//-*-c++-*-
/*
 Author: Delorme Maxime
 Email : Maxime.Delorme@gmail.com

 Last modification : 04/08/2005 (fr) 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef _GRID_WIDGET_H_
#define _GRID_WIDGET_H_

#include "GridOptionsData.h"

#include <tulip/GlMainWidget.h>
#include <tulip/LayerManagerWidget.h>
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
class GridOptionsWidget : public GridOptionsData {

  Q_OBJECT

 protected:
  GlMainWidget *glMainWidget; /**< The considered GlMainWidget */
  LayerManagerWidget *layerWidget; /**< The considered LayerManagerWidget */
  tlp::GlGrid *grid; /**< The considered GlADGrid */

 public:
  /**
   * Constructor.
   */
  GridOptionsWidget(QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0);

  /**
   * Destructor.
   */
  ~GridOptionsWidget();

  /**
   * Function used to define on which GlMainWidget we are working
   */
  void setCurrentMainWidget(GlMainWidget *graphWidget);

  /**
   * Function used to define on which LayerManagerWidget we are working
   */
  void setCurrentLayerManagerWidget(LayerManagerWidget *layerWidget);						

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
