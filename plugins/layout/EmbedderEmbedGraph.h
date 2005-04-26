#ifndef EMBEDDEREMBEDGRAPH_H
#define EMBEDDEREMBEDGRAPH_H

#include "EmbedderDefs.h"

void embed_graph(vtx_data *graph, int n, int dim, DistType ** (&coords));
void center_coordinate(DistType ** coords, int n, int dim);
void PCA(DistType ** coords, int dim, int n, double ** (&new_coords), int new_dim);
void PCA(DistType ** coords, int dim, int n, double ** (&new_coords), int dim1, int dim2, bool recompute);

#endif











