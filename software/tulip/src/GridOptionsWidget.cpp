#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "GridOptionsWidget.h"

#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qvalidator.h>

#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/DrawingTools.h>

using namespace tlp;
using namespace std;

namespace tlp
{
  GridOptionsWidget::GridOptionsWidget(QWidget *parent, const char *name, WFlags fl) : GridOptionsData(parent, name, fl), glGraphWidget(0) {
    connect(CancelBtn, SIGNAL(clicked()), SLOT(close()));
    connect(OkBtn, SIGNAL(clicked()), SLOT(validateGrid()));

    connect(SubdivisionsRB, SIGNAL(clicked()), SLOT(chGridSubdivisions()));
    connect(SizeRB, SIGNAL(clicked()), SLOT(chGridSize()));
    connect(DisplayXCB, SIGNAL(clicked()), SLOT(chDisplayGrid()));
    connect(DisplayYCB, SIGNAL(clicked()), SLOT(chDisplayGrid()));
    connect(DisplayZCB, SIGNAL(clicked()), SLOT(chDisplayGrid()));

    // The validator for int values:
    QIntValidator *intValidator = new QIntValidator(this);
    intValidator->setBottom(0);

    GridSubX->setValidator(intValidator);
    GridSubY->setValidator(intValidator);
    GridSubZ->setValidator(intValidator);

    QDoubleValidator *doubleValidator = new QDoubleValidator( this );
    doubleValidator->setBottom(0); 

    GridSizeX->setValidator(doubleValidator);
    GridSizeY->setValidator(doubleValidator);
    GridSizeZ->setValidator(doubleValidator);
  }
  //==============================================
  GridOptionsWidget::~GridOptionsWidget() {
  }
  //==============================================
  void GridOptionsWidget::setCurrentGraphWidget(GlGraphWidget *graphWidget) {
    this->glGraphWidget = graphWidget;

    if (glGraphWidget != NULL && glGraphWidget != 0)
      grid = (GlADGrid*)glGraphWidget->findGlAugmentedDisplay("Layout Grid");
  }
  //==============================================
  void GridOptionsWidget::validateGrid() {
    if (glGraphWidget != 0) {
      if (ActivatedCB->isChecked()) {
	if (grid != NULL) {
	  glGraphWidget->removeGlAugmentedDisplay(grid);
	  grid = NULL;
	}
	    
	bool display[3] = {DisplayXCB->isChecked(), DisplayYCB->isChecked(), DisplayZCB->isChecked()};
	    
	Coord min, max;
	    
	// We get the min and the max of the Layout to display the grid
	LayoutProperty *layout = glGraphWidget->getGraph()->getProperty<LayoutProperty>("viewLayout");
	SizeProperty *sizes = glGraphWidget->getGraph()->getProperty<SizeProperty>("viewSize");
	DoubleProperty *rotation = glGraphWidget->getGraph()->getProperty<DoubleProperty>("viewRotation");
	pair<Coord, Coord> bboxes = tlp::computeBoundingBox(glGraphWidget->getGraph(), layout, 
							    sizes, rotation);
	max = bboxes.first;	
	min = bboxes.second;


	for(int i=0; i < 3; i++) {
	  min[i] = min[i] - 0.5f;
	  max[i] = max[i] + 0.5f;
	}
	    
	// Building of the cell :
	Size cell;
	if (SubdivisionsRB->isChecked()) {
	  double sub[3] = {GridSubX->text().toDouble(), GridSubY->text().toDouble(), GridSubZ->text().toDouble()};
		
	  if (GridSubX->text() == "")
	    sub[0] = 0;
		
	  if (GridSubY->text() == "")
	    sub[1] = 0;
		
	  if (GridSubZ->text() == "")
	    sub[2] = 0;
		
	  for(int i=0; i < 3; i++)  {
	    if (sub[i] == 0)
	      cell[i] = 0;
	    else
	      cell[i] = fabs(max[i] - min[i]) / sub[i];
	  }
	}
	else {
	  double cellsize[3] = {GridSizeX->text().toDouble(), GridSizeY->text().toDouble(), GridSizeZ->text().toDouble()};
	  for(int i=0; i < 3; i++)
	    cell[i] = cellsize[i];
	}
	grid = new GlADGrid(min, max, cell, Color(0, 0, 0, 255), display);
	glGraphWidget->addGlAugmentedDisplay(grid, "Layout Grid");
	    
      }
      else {
	if (grid != NULL) {
	  glGraphWidget->removeGlAugmentedDisplay(grid);
	  grid = NULL;
	}
      }
      glGraphWidget->draw();   
      close();
    }
  }
  //==============================================
  void GridOptionsWidget::chGridSubdivisions() {
    SubdivisionsRB->setChecked(true);
    SizeRB->setChecked(false);
    
    SubdivisionsFrame->setEnabled(true);
    SizeFrame->setEnabled(false);
    
    GridSubX->setEnabled(true);
    GridSubY->setEnabled(true);
    GridSubZ->setEnabled(true);
  }
  //==============================================
  void GridOptionsWidget::chGridSize() {
    SubdivisionsRB->setChecked(false);
    SizeRB->setChecked(true);
    
    SubdivisionsFrame->setEnabled(false);
    SizeFrame->setEnabled(true);
    
    GridSizeX->setEnabled(true);
    GridSizeY->setEnabled(true);
    GridSizeZ->setEnabled(true);
  }
  //==============================================
  void GridOptionsWidget::chDisplayGrid() {
    if (grid == NULL || !ActivatedCB->isChecked())
      return;
    bool display[3] = {DisplayXCB->isChecked(), DisplayYCB->isChecked(), DisplayZCB->isChecked()};
    grid->setDisplayDim(display);
    glGraphWidget->draw();
  }
  //==============================================
}
