/******************************************

	Breadth First Search
	Computes single-source distances for
	unweighted graphs

******************************************/



#include <stdlib.h>
#include "EmbedderDefs.h"
#include "EmbedderBfs.h"


void bfs(int vertex, vtx_data * graph, int n, DistType * dist, Queue & Q)  {
  int i;

  // initial distances with edge weights:
  for (i=0; i<n; i++) 
    dist[i]=-1;
  dist[vertex]=0;
		
  Q.initQueue(vertex);
	
  int closestVertex, neighbor;
  DistType closestDist;
  while (Q.dequeue(closestVertex)) {
    closestDist=dist[closestVertex];
    for (i=1; i<graph[closestVertex].nedges; i++) {
      neighbor=graph[closestVertex].edges[i];
      if (dist[neighbor]<-0.5) {  // first time to reach neighbor
	dist[neighbor]=closestDist+(DistType)graph[closestVertex].ewgts[i];
	Q.enqueue(neighbor);
      }
    }
  }
  
  // For dealing with disconnected graphs:
  for (i=0; i<n; i++)
    if (dist[i]<-0.5) // 'i' is not connected to 'vertex'
      dist[i]=closestDist+10;
}
