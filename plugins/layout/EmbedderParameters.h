#ifndef EMBEDDERPARAMETERS_H
#define EMBEDDERPARAMETERS_H

// Parameters used in the program:

// Power iteration tolerance:
extern double p_iteration_threshold;

// PCA dimensions:
extern int dim1;
extern int dim2;

// number of centers
extern int num_centers;
extern int max_num_centers; 

extern double tolerance;

// Style related:
enum Shape{circle, rectangle};
extern Shape vertex_shape;
extern int vertex_size;
extern bool label_vertices;
extern bool ThickLines;
extern int ScreenShift;
extern bool drawEdges;
extern bool SHOW_PIVOTS;

extern int * pivots;
extern int num_of_pivots;

#endif
