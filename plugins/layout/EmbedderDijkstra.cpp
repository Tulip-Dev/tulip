/******************************************

	Dijkstra algorithm
	Computes single-source distances for
	weighted graphs

******************************************/


#include <limits.h>
#include <stdlib.h>
#include "EmbedderDefs.h"



typedef DistType Word;

#define LOOP while(true)

/* This heap class is suited to the Dijkstra alg.
   data[i]=vertexNum <==> index[vertexNum]=i
*/

class heap {
private:	
  int * data;
  int heapSize;
  int left(int i) {return 2*i;}
  int right(int i) {return 2*i+1;}
  int parent(int i) {return i/2;}
  bool insideHeap(int i) {return i<heapSize;}
  bool greaterPriority(int i, int j, Word dist[]) {
    return dist[data[i]]<dist[data[j]];
  }
  void exchange(int i, int j, int index[]) {
    int temp;
    temp=data[i];
    data[i]=data[j];
    data[j]=temp;
		
    index[data[i]]=i;
    index[data[j]]=j;
  }
  void assign(int i, int j, int index[]) {
    data[i]=data[j];
		
    index[data[i]]=i;
  }
  void heapify(int i, int index[], Word dist[]) {
    int l,r,largest;
    LOOP {
      l=left(i); r=right(i);
      if (insideHeap(l) && greaterPriority(l,i,dist))
	largest=l;
      else
	largest=i;
      if (insideHeap(r) && greaterPriority(r,largest,dist))
	largest=r;
			
      if (largest==i) 
	break;

      exchange(largest,i,index);
      i=largest;
    }
  }
			

public:
  heap(int size) {data=new int[size]; heapSize=0;}
  heap() {data=NULL; heapSize=0;}
  ~heap() {delete [] data;}
  void initHeap(int startVertex,int index[], Word dist[], int n) {
    int i,count;
    data=(int*) realloc (data, (n-1)*sizeof(int));
    heapSize=n-1;
		
    for (count=0,i=0; i<n; i++) 
      if (i!=startVertex) {
	data[count]=i;
	index[i]=count;
	count++;
      }
		
    int j; // We cannot use an unsigned value in this loop
    for (j=(n-1)/2; j>=0; j--)
      heapify(j,index,dist);
  }

  bool extractMax(int & max, int index[], Word dist[]) {
		
    if (heapSize==0)
      return false;

    max=data[0];
    data[0]=data[heapSize-1];
    index[data[0]]=0;
    heapSize--;
    heapify(0,index,dist);

    return true;

  }

  void increaseKey(int increasedVertex, Word newDist, int index[], Word dist[]) {
    if (dist[increasedVertex]<=newDist)
      return;

    int placeInHeap=index[increasedVertex];
		
    dist[increasedVertex]=newDist;

    int i=placeInHeap;
    while (i>0 && dist[data[parent(i)]]>newDist) { // can write here: greaterPriority(i,parent(i),dist)
      assign(i,parent(i),index);
      i=parent(i);
    }
    data[i]=increasedVertex;
    index[increasedVertex]=i;
  }
};



void dijkstra(int vertex, vtx_data * graph, int n, DistType * dist) {
  int i;

  const double MAX_DIST=INT_MAX;

  heap H(n);
  static int  * index = new int [n];

  // initial distances with edge weights:
  for (i=0; i<n; i++) 
    dist[i]=(DistType)MAX_DIST;
  dist[vertex]=0;
  for (i=1; i<graph[vertex].nedges; i++)
    dist[graph[vertex].edges[i]] = (DistType) graph[vertex].ewgts[i];
	
  H.initHeap(vertex,index,dist,n);
	
  int closestVertex, neighbor;
  DistType closestDist;
  while (H.extractMax(closestVertex, index, dist)) {
    closestDist=dist[closestVertex];
    if (closestDist==MAX_DIST)
      break;
    for (i=1; i<graph[closestVertex].nedges; i++) {
      neighbor=graph[closestVertex].edges[i];
      H.increaseKey(neighbor, closestDist+(DistType)graph[closestVertex].ewgts[i], index, dist);
    }
  }
}	
