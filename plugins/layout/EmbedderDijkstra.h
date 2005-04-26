#ifndef EMBEDDERDIJKSTRA_H
#define EMBEDDERDIJKSTRA_H

#include "EmbedderDefs.h"

void dijkstra(int vertex, vtx_data * graph, int n, DistType * dist);
void bfs(int vertex, vtx_data * graph, int n, DistType * dist) ;

#endif
