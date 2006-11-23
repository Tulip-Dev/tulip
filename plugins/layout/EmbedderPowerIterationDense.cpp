/***************************************

  Power iteration algorithm.

  Implementation for dense matrices.
  Needed for the finding top eigenvectors 
  of the covariance matrix.

*****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void      cpvec(double      *copy,int beg, int end, double *vec);
double    dot(double        *vec1,int beg, int end, double *vec2); 
void      scadd(double      *vec1,int beg, int end, double fac,   double *vec2);
void      vecscale(double   *vec1,int beg, int end, double alpha, double *vec2);
double    norm(double       *vec, int beg, int end);

int p_iteration_terations=1;
double p_iteration_threshold=1e-3;

void mat_mult_vec(double ** mat, int dim1, int dim2, double * vec, double *result) {
  // computes mat*vec, where mat is a dim1*dim2 matrix
  int i,j;
  double sum;
	
  for (i=0; i<dim1; i++) {
    sum=0;
    for (j=0; j<dim2; j++) {
      sum += mat[i][j]*vec[j];
    }
    result[i]=sum;
  }
}

void power_iteration(double ** square_mat, int n, int neigs, double ** eigs, double * evals) {
  int i,j;
  double *tmp_vec = new double[n]; 
  double *last_vec = new double[n];
  double * curr_vector;
  double len;
  double angle;
  double alpha;
  int iteration;

  double tol=1-p_iteration_threshold;
  //  printf("\t tolerance is: %lf\n", tol);

  if (neigs>=n) {
    neigs=n;
  }

  for (i=0; i<neigs; i++) {
    curr_vector = eigs[i];
    // guess the i-th eigen vector
  choose:
    for (j=0; j<n; j++)
      curr_vector[j] = rand()%100;
    // orthogonalize against higher eigenvectors
    for (j=0; j<i; j++) {
      alpha = -dot(eigs[j], 0, n-1, curr_vector);
      scadd(curr_vector, 0, n-1, alpha, eigs[j]);
    }
    len = norm(curr_vector, 0, n-1);
    if (len<1e-10) {
      // We have chosen a vector colinear with prvious ones
      goto choose;
    }
    vecscale(curr_vector, 0, n-1, 1.0 / len, curr_vector);	
    iteration=0;
    do {
      iteration++;
      cpvec(last_vec,0,n-1,curr_vector);
			
      mat_mult_vec(square_mat,n,n,curr_vector,tmp_vec);
      cpvec(curr_vector,0,n-1,tmp_vec);
						
      // orthogonalize against higher eigenvectors
      for (j=0; j<i; j++) {
	alpha = -dot(eigs[j], 0, n-1, curr_vector);
	scadd(curr_vector, 0, n-1, alpha, eigs[j]);
      }
      len = norm(curr_vector, 0, n-1);
      if (len<1e-10) {
	// We have reached the null space (e.vec. associated with e.val. 0)
	goto exit;
      }

      vecscale(curr_vector, 0, n-1, 1.0 / len, curr_vector);
      angle = dot(curr_vector, 0, n-1, last_vec);
    } while (angle<tol);
    //    printf("Finish computing eigenvector no. %d\n", i);
    evals[i]=angle*len; // this is the Rayleigh quotient (up to errors due to orthogonalization):
    // u*(A*u)/||A*u||)*||A*u||, where u=last_vec, and ||u||=1
  }
 exit:
  for (; i<neigs; i++) {
    // compute the smallest eigenvector, which are 
    // probably associated with eigenvalue 0 and for
    // which power-iteration is dangerous
    curr_vector = eigs[i];
    // guess the i-th eigen vector
    for (j=0; j<n; j++)
      curr_vector[j] = rand()%100;
    // orthogonalize against higher eigenvectors
    for (j=0; j<i; j++) {
      alpha = -dot(eigs[j], 0, n-1, curr_vector);
      scadd(curr_vector, 0, n-1, alpha, eigs[j]);
    }
    len = norm(curr_vector, 0, n-1);
    vecscale(curr_vector, 0, n-1, 1.0 / len, curr_vector);
    evals[i]=0;		
  }

  // sort vectors by their evals, for overcoming possible mis-convergence:
  int largest_index;
  double largest_eval;
  for (i=0; i<neigs-1; i++) {
    largest_index=i;
    largest_eval=evals[largest_index];
    for (j=i+1; j<neigs; j++) {
      if (largest_eval<evals[j]) {
	largest_index=j;
	largest_eval=evals[largest_index];
      }
    }
    if (largest_index!=i) { // exchange eigenvectors:
      cpvec(tmp_vec,0,n-1,eigs[i]);
      cpvec(eigs[i],0,n-1,eigs[largest_index]);
      cpvec(eigs[largest_index],0,n-1,tmp_vec);

      evals[largest_index]=evals[i];
      evals[i]=largest_eval;
    }
  }
  delete [] tmp_vec; delete [] last_vec;
}

/* Copy a range of a double vector to a double vector */
void      cpvec(double   *copy,int  beg, int end, double   *vec) {
  int       i;
  copy = copy + beg;
  vec  = vec  + beg;
  for (i = end - beg + 1; i; i--) {
    *copy++ = *vec++;
  }
}

/* Returns scalar product of two double n-vectors. */
double    dot(double   *vec1, int beg, int end, double *vec2) {
  int       i;
  double    sum;

  sum = 0.0;
  vec1 = vec1 + beg;
  vec2 = vec2 + beg;
  for (i = end - beg + 1; i; i--) {
    sum += (*vec1++) * (*vec2++);
  }
  return (sum);
}


/* Scaled add - fills double vec1 with vec1 + alpha*vec2 over range*/
void      scadd(double   *vec1,int beg, int end, double  fac, double *vec2) {
  int       i;

  vec1 = vec1 + beg;
  vec2 = vec2 + beg;
  for (i = end - beg + 1; i; i--) {
    (*vec1++) += fac * (*vec2++);
  }
}

/* Scale - fills vec1 with alpha*vec2 over range, double version */
void      vecscale(double   *vec1,int beg, int end, double alpha, double   *vec2) {
  int       i;

  vec1 += beg;
  vec2 += beg;
  for (i = end - beg + 1; i; i--) {
    (*vec1++) = alpha * (*vec2++);
  }
}

/* Returns 2-norm of a double n-vector over range. */
double    norm(double   *vec, int beg, int end) {
  return (sqrt(dot(vec, beg, end, vec)));
}
