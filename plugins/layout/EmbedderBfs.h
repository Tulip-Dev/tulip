#ifndef EMBEDDERBFS_H
#define EMBEDDERBFS_H

#include "EmbedderDefs.h"

class Queue {
 private:	
  int * data;
  int queueSize;
  int end;
  int start;
 public:
  Queue(int size) {data=new int[size]; queueSize=size; start=0; end=0;}
  ~Queue() {delete [] data;}

  void initQueue(int startVertex) {
    data[0]=startVertex;
    start=0; end=1;
  }

  bool dequeue(int & vertex) {		
    if (start>=end)
      return false; // underflow
    vertex=data[start++];
    return true;
  }

  bool enqueue(int vertex) {
    if (end>=queueSize)
      return false; // overflow
    data[end++]=vertex;
    return true;
  }
};


void bfs(int vertex, vtx_data * graph, int n, DistType * dist, Queue & Q);

#endif
