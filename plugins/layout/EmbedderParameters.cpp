/*******************************************

  Global variables and management of 
  dialog boxes.

*******************************************/

#include <stddef.h>
#include <cmath>

// Parameters used in the program:

// PCA dimensions:
int dim1=0;
int dim2=1;

// number of centers
int num_centers; 
int max_num_centers=50;


// eigen projection related
int tol_coeff=1;
int tol_exp=7;
double tolerance=tol_coeff*pow(10,-tol_exp); 
	


// Style related:
enum Shape{circle, rectangle};
Shape vertex_shape=circle;
int vertex_size=1;
bool label_vertices=false;
bool ThickLines=false;
int ScreenShift=20;
bool drawEdges=true;
bool SHOW_PIVOTS=false;



// When one wants to mark the pivots, we need this:
int * pivots=NULL;
int num_of_pivots=0;
