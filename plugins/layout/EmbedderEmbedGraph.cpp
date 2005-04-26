/************************************************

	Functions for computing the high-dimensional
	embedding and the PCA projection.

************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "EmbedderDefs.h"
#include "EmbedderDijkstra.h"
#include "EmbedderBfs.h"
#include "EmbedderParameters.h"

//#define WEIGHTED_GRAPH

typedef DistType * dist_ptr;
typedef double * double_ptr;

unsigned int high_dim_time, center_time, pca_matrix_time, power_iteration_time, projection_time; 

void embed_graph(vtx_data * graph, int n, int dim, DistType ** (&coords)) {
		
  high_dim_time = clock();
  int i,j;

  if (coords!=NULL) {
    delete [] coords[0];
    delete [] coords;
  }

  DistType * storage = new DistType[n*dim];

  coords = new dist_ptr[dim];  // this matrix stores the distance between each node and each "pivot"
  for (i=0; i<dim; i++)
    coords[i] = storage+i*n;

  DistType * dist = new DistType[n]; // this vector stores  the distances of each nodes to the selected "pivots"

  if (SHOW_PIVOTS) {
    delete [] pivots;
    pivots = new int[dim];
    num_of_pivots=dim;
  }
  else 
    num_of_pivots = 0;

	
  // select the first pivot
  int node = rand()%n;

  if (SHOW_PIVOTS) {
    pivots[0] = node;
  }


#ifdef WEIGHTED_GRAPH
  dijkstra(node, graph, n, coords[0]);
#else
  Queue Q(n);
  bfs(node, graph, n, coords[0], Q);
#endif
	
  DistType max_dist=0;
  for (i=0; i<n; i++) {
    dist[i]=coords[0][i];
    if (dist[i]>max_dist) {
      node=i;
      max_dist=dist[i];
    }
  }
	
  // select other dim-1 nodes as pivots
  for (i=1; i<dim; i++) {
    if (SHOW_PIVOTS) {
      pivots[i] = node;
    }		
#ifdef WEIGHTED_GRAPH
    dijkstra(node, graph, n, coords[i]);
#else
    bfs(node, graph, n, coords[i],Q);
#endif
    max_dist=0;
    for (j=0; j<n; j++) {
      dist[j]=min(dist[j],coords[i][j]);
      if (dist[j]>max_dist) {
	node=j;
	max_dist=dist[j];
      }
    }
	
  }

  delete [] dist;

  high_dim_time=clock()-high_dim_time;
}

// Make each axis centered around 0
void center_coordinate(DistType ** coords, int n, int dim) {
  center_time=clock();
  int i,j;
  double sum,avg;
  for (i=0; i<dim; i++) {
    sum=0;
    for (j=0; j<n; j++) {
      sum += coords[i][j];
    }
    avg=sum/n;
    for (j=0; j<n; j++) {
      coords[i][j]-=(DistType)avg;
    }
  }
  center_time=clock()-center_time;
}

void power_iteration(double ** square_mat, int n, int neigs, double ** eigs, double * evals);

void PCA(DistType ** coords, int dim, int n, double ** (&new_coords), int new_dim) {
  double ** DD; // dim*dim matrix: coords*coords^T
  double sum;
  int i,j,k;
  double ** eigs;
  double * evals;
	
  pca_matrix_time=clock();
  //  printf("Starting computation of PCA matrix...\n");
  DD = new double_ptr[dim];  // this matrix stores the distance between each node and each "pivot"
  for (i=0; i<dim; i++)
    DD[i] = new double[dim];

  for (i=0; i<dim; i++) {
    for (j=0; j<=i; j++) {
      // compute coords[i]*coords[j]
      sum=0;
      for (k=0; k<n; k++) {
	sum += coords[i][k]*coords[j][k];
      }
      DD[i][j] = DD[j][i] = sum;
    }
  }
  pca_matrix_time=clock()-pca_matrix_time;
	
  power_iteration_time=clock();
	
  eigs = new double_ptr[new_dim];
  for (i=0; i<new_dim; i++)
    eigs[i] = new double[dim];
  evals = new double[new_dim];

  //  printf("Starting power iteration...\n");
  power_iteration(DD, dim, new_dim, eigs, evals);
  power_iteration_time=clock()-power_iteration_time;

	
  projection_time=clock();	
  //  printf("Projecting to lower dimension...\n");
  for (i=0; i<n; i++) {
    for (j=0; j<new_dim; j++) {
      sum = 0;
      for (k=0; k<dim; k++) {
	sum += coords[k][i]*eigs[j][k];
      }
      new_coords[j][i]=sum;
    }
  }
  projection_time=clock()-projection_time;
	
  for (i=0; i<dim; i++)
    delete [] DD[i];
  delete [] DD;
	
  for (i=0; i<new_dim; i++)
    delete [] eigs[i];
  delete [] eigs;
  delete [] evals;
}



void PCA(DistType ** coords, int dim, int n, double ** (&new_coords), int dim1, int dim2, bool recompute) {
  static double ** DD=NULL; // dim*dim matrix: coords*coords^T
  double sum;
  int i,j,k;
  static double ** eigs=NULL;
  static double * evals=NULL;
  const int max_dim=6;
  static int dimX=-1;
  static int dimY=-1;
  static double * storage;
  double * storage_ptr;
	
  if (recompute) {
    if (eigs!=NULL) {
      for (i=0; i<max_dim; i++)
	delete [] eigs[i];
      delete [] eigs;
      delete [] evals;
    }	
    eigs = new double_ptr[max_dim];
    for (i=0; i<max_dim; i++)
      eigs[i] = new double[dim];
    evals = new double[max_dim];

    if (DD!=NULL) {
      delete [] storage;
      delete [] DD;
    }	

    DD = new double_ptr[dim];  // this matrix stores the distance between each node and each "pivot"
    storage_ptr = storage = new double[dim*dim];
    for (i=0; i<dim; i++) {
      DD[i] = storage_ptr;
      storage_ptr += dim;
    }
	
    pca_matrix_time=clock();
    //    printf("Starting computation of PCA matrix...\n");
		
    for (i=0; i<dim; i++) {
      for (j=0; j<=i; j++) {
	// compute coords[i]*coords[j]
	sum=0;
	for (k=0; k<n; k++) {
	  sum += coords[i][k]*coords[j][k];
	}
	DD[i][j] = DD[j][i] = sum;
      }
    }
    pca_matrix_time=clock()-pca_matrix_time;
		
    power_iteration_time=clock();
		
    //    printf("Starting power iteration...\n");
    power_iteration(DD, dim, max_dim, eigs, evals);
    power_iteration_time=clock()-power_iteration_time;
  }
	
  if (recompute || dim1!=dimX || dim2!=dimY) {
    projection_time=clock();
    // re-project the X-axis:
    dimX=dim1;
    for (i=0; i<n; i++) {
      sum = 0;
      for (k=0; k<dim; k++) {
	sum += coords[k][i]*eigs[dim1][k];
      }
      new_coords[dim1][i]=sum;
    }
    // re-project the Y-axis:
    dimY=dim2;
    for (i=0; i<n; i++) {
      sum = 0;
      for (k=0; k<dim; k++) {
	sum += coords[k][i]*eigs[dim2][k];
      }
      new_coords[dim2][i]=sum;
    }
    projection_time=clock()-projection_time;
  }
}














