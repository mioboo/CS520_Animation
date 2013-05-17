/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

*/

#include "jello.h"
#include "perlinNoise.h"
#include "physics.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <stdlib.h>
#include <time.h>

/* Computes acceleration to every control point of the jello cube, 
   which is in state given by 'jello'.
   Returns result in array 'a'. */
void computeAcceleration(struct world * jello)
{

	/*
	printf("V start\n");
	for (int i=0; i<particleNum; i++) {
		printf("a[%d].x = %f   a.y = %f   a.z = %f\n",i,jello->v[i][0][0].x,jello->v[i][0][0].y, jello->v[i][0][0].z);
	}	
	printf("\n");
	*/
	
	int ArraySize = 8 + (particleNum - 2) * 3;
	int row = 0;
	if (CRING) {
		row = particleNum+2;
	}else {
		row = particleNum+1;
	}
	int column = ArraySize - row;
	double *a_data = new double [ArraySize * ArraySize];
	double *b_data = new double [ArraySize];
	
	//Mass
	for (int i=0; i<column; i++) {
		for (int j=0; j<column; j++) {
			if (i == j) {
				a_data[i*ArraySize + j] = particleMass;
			}else {
				a_data[i*ArraySize + j] = 0;
			}
		}
	}
	
	//Empty Matrix
	for (int i=0; i<row; i++) {
		for (int j=0; j<row; j++) {
			a_data[ArraySize*column + column + (i*ArraySize + j)] = 0;
		}
	}
	
	//DEL_C
	for (int i=0; i<row; i++) {
		for (int j=0; j<column; j++) {
			if(i==0){
				if (j==0) {
					a_data[ArraySize*column] = 1;
				}else {
					a_data[ArraySize*column + j] = 0;
				}
			}else if (i==1) {
				if (j==1) {
					a_data[ArraySize*column + (i*ArraySize + j)] = 1;
				}else {
					a_data[ArraySize*column + (i*ArraySize + j)] = 0;
				}
			}else if (i == row-1 && CRING == 1) {
				if (j == column-2) {
					a_data[ArraySize*column + (i*ArraySize + j)] = 2 * (jello->p[i-2].x);
				}else if (j == column-1) {
					a_data[ArraySize*column + (i*ArraySize + j)] = 2 * (jello->p[i-2].y);	
				}else {
					a_data[ArraySize*column + (i*ArraySize + j)] = 0;
				}
			}else {
				if (j == (i-2)*2) {
					a_data[ArraySize*column + (i*ArraySize + j)] = (-2) * (jello->p[i-1].x - jello->p[i-2].x);
				}else if (j == (i-2)*2 + 1) {
					a_data[ArraySize*column + (i*ArraySize + j)] = (-2) * (jello->p[i-1].y - jello->p[i-2].y);
				}else if (j == (i-2)*2 + 2) {
					a_data[ArraySize*column + (i*ArraySize + j)] = 2 * (jello->p[i-1].x - jello->p[i-2].x);
				}else if (j == (i-2)*2 + 3) {
					a_data[ArraySize*column + (i*ArraySize + j)] = 2 * (jello->p[i-1].y - jello->p[i-2].y);
				}else {
					a_data[ArraySize*column + (i*ArraySize + j)] = 0;
				}
			}
		}
	}
	
	//Transpose of DEL_C
	for (int i=0; i<column; i++) {
		for (int j=0; j<row; j++) {
			a_data[column + ArraySize*i + j] = a_data[ArraySize * (row-j-1+column) + i];
		}
	}
	
	//set F value(Gravity)
	for (int i=0; i<column; i++) {
		if (i%2) {
			b_data[i] = jello->Force.y - (alpha * jello->v[(i-1)/2].y);
		}else {
			b_data[i] = jello->Force.x - (alpha * jello->v[i/2].x);
		}
	}
	
	gsl_matrix *dcdq = gsl_matrix_alloc(row, column);
	gsl_matrix *dcdqdot = gsl_matrix_alloc(row, column);
	gsl_matrix *velocity = gsl_matrix_alloc(column, 1);
	gsl_vector *C = gsl_vector_alloc(row);
	gsl_matrix *temp1 = gsl_matrix_alloc(row, 1);
	gsl_matrix *temp2 = gsl_matrix_alloc(row, 1);
	
	//set dzdqdot
	for (int i=0; i<row; i++) {
		for (int j=0; j<column; j++) {
			if(i==0){
				gsl_matrix_set(dcdqdot, i, j, 0);
			}else if (i==1) {
				gsl_matrix_set(dcdqdot, i, j, 0);
			}else if (i == row-1 && CRING == 1) {
				if (j == column-2) {
					gsl_matrix_set(dcdqdot, i, j, 2 * (jello->v[i-2].x));
				}else if (j == column-1) {
					gsl_matrix_set(dcdqdot, i, j, 2 * (jello->v[i-2].y));
				}else {
					gsl_matrix_set(dcdqdot, i, j, 0);
				}
			}else {
				if (j == (i-2)*2) {
					gsl_matrix_set(dcdqdot, i, j, (-2) * (jello->v[i-1].x - jello->v[i-2].x));
				}else if (j == (i-2)*2 + 1) {
					gsl_matrix_set(dcdqdot, i, j, (-2) * (jello->v[i-1].y - jello->v[i-2].y));
				}else if (j == (i-2)*2 + 2) {
					gsl_matrix_set(dcdqdot, i, j, 2 * (jello->v[i-1].x - jello->v[i-2].x));
				}else if (j == (i-2)*2 + 3) {
					gsl_matrix_set(dcdqdot, i, j, 2 * (jello->v[i-1].y - jello->v[i-2].y));
				}else {
					gsl_matrix_set(dcdqdot, i, j, 0);
				}
			}			
		}
	}	
	
	//set dcdq
	for (int i=0; i<row; i++) {
		for (int j=0; j<column; j++) {
			if(i==0){
				if (j==0) {
					gsl_matrix_set(dcdq, i, j, 1);
				}else {
					gsl_matrix_set(dcdq, i, j, 0);
				}
			}else if (i==1) {
				if (j==1) {
					gsl_matrix_set(dcdq, i, j, 1);
				}else {
					gsl_matrix_set(dcdq, i, j, 0);
				}
			}else if (i == row-1 && CRING == 1) {
				if (j == column-2) {
					gsl_matrix_set(dcdq, i, j, 2 * (jello->p[i-2].x));
				}else if (j == column-1) {
					gsl_matrix_set(dcdq, i, j, 2 * (jello->p[i-2].y));
				}else {
					gsl_matrix_set(dcdq, i, j, 0);
				}
			}else {
				if (j == (i-2)*2) {
					gsl_matrix_set(dcdq, i, j, (-2) * (jello->p[i-1].x - jello->p[i-2].x));
				}else if (j == (i-2)*2 + 1) {
					gsl_matrix_set(dcdq, i, j, (-2) * (jello->p[i-1].y - jello->p[i-2].y));
				}else if (j == (i-2)*2 + 2) {
					gsl_matrix_set(dcdq, i, j, 2 * (jello->p[i-1].x - jello->p[i-2].x));
				}else if (j == (i-2)*2 + 3) {
					gsl_matrix_set(dcdq, i, j, 2 * (jello->p[i-1].y - jello->p[i-2].y));
				}else {
					gsl_matrix_set(dcdq, i, j, 0);
				}
			}			
		}
	}
	
	//set velocity
	for (int i=0; i<column; i++) {
		if (i == 0 || i == 1) {
			gsl_matrix_set(velocity, i, 0, 0);
		}else if (i%2) {
			gsl_matrix_set(velocity, i-1, 0, jello->v[(i-1)/2].x);
			gsl_matrix_set(velocity, i, 0, jello->v[(i-1)/2].y);
		}
	}
	
	//set C
	for (int i=0; i<row; i++) {
		if (i==0) {
			gsl_vector_set(C, i, jello->p[0].x);
		}else if (i==1) {
			gsl_vector_set(C, i, jello->p[0].y - 0.5);
		}else if (i==row-1 && CRING == 1) {
			gsl_vector_set(C, i, pow(jello->p[i-2].x, 2) + pow(jello->p[i-2].y, 2) - 0.25);
		}else {
			gsl_vector_set(C, i, pow(jello->p[i-1].x - jello->p[i-2].x ,2) + pow(jello->p[i-1].y - jello->p[i-2].y, 2) - pow(particleLength, 2));
		}
	}
	
	//get the error of cpnstraints and print it
	constraintsError = 0;
	for (int i =0; i<row; i++) {
		constraintsError += gsl_vector_get(C, i);
	}
	//printf("%g\n",constraintsError);
	
	//set temp
	for (int i=0; i<row; i++) {
		gsl_matrix_set(temp1, i, 0, 0.0);
		gsl_matrix_set(temp2, i, 0, 0.0);
	}
	
	//get temp1
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, dcdqdot, velocity, 0.0, temp1);
	
	//get temp2
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, dcdq, velocity, 0.0, temp2);
	
	
	//st b_data
	for (int i=0; i<row; i++) {
		b_data[column+i] = (-1 * gsl_matrix_get(temp1, i, 0)) - (2* beta * gsl_matrix_get(temp2, i, 0)) - (pow(beta, 2) * gsl_vector_get(C, i)) ;
	}
	/*
	printf("dcdqdot start\n");
	for (int i=0; i<row; i++) {
		for (int j=0; j<column; j++) {
			printf("%f ",gsl_matrix_get(dcdqdot, i, j));
		}
		printf("\n");
	}
	printf("velocity start\n");
	for (int j=0; j<column; j++) {
		printf("%f ",gsl_matrix_get(velocity, j,0));
	}
	printf("\n");	
	printf("temp1 start\n");
	for (int j=0; j<row; j++) {
		printf("%f ",gsl_matrix_get(temp1, j,0));
	}
	printf("\n");
	
	
	//print a_data value 
	printf("a_data start\n");
	for (int i=0; i<ArraySize; i++) {
		for (int j=0; j<ArraySize; j++) {
			printf("%f ",a_data[i*ArraySize+j]);
		}
		printf("\n");
	}
	//print b_data value
	printf("b_data start\n");
	for (int i=0; i<ArraySize; i++) {
		printf("%f ",b_data[i]);
	}
	printf("\n");
	*/
	
	gsl_matrix *A = gsl_matrix_alloc(ArraySize, ArraySize);
	gsl_matrix *V = gsl_matrix_alloc(ArraySize, ArraySize);
	gsl_vector *S = gsl_vector_alloc(ArraySize);
	gsl_vector *work = gsl_vector_alloc(ArraySize);
	gsl_vector *B = gsl_vector_alloc(ArraySize);
	gsl_vector *X = gsl_vector_alloc (ArraySize);
	
	//set A matrix
	for (int i=0; i<ArraySize; i++) {
		for (int j=0; j<ArraySize; j++) {
			gsl_matrix_set(A, i, j, a_data[i*ArraySize + j]);
		}
	}
	//set B vector
	for (int i=0; i<ArraySize; i++) {
		gsl_vector_set(B, i, b_data[i]);
	}
	
	gsl_linalg_SV_decomp(A, V, S, work);
	
		
	//Check the singular value
	double eps = 1e-6;
	double MaxSValue = gsl_vector_get(S, 0);
	double CheckValue = MaxSValue * eps;
	
	for (int i=0; i<ArraySize; i++) {
		if (gsl_vector_get(S, i) < CheckValue) {
			gsl_vector_set(S, i, 0.0);
		}
	}
	
	gsl_linalg_SV_solve(A, V, S, B, X);
	
	/*
	//Check Value
	printf("A start\n");
	for (int i=0; i<ArraySize; i++) {
		for (int j=0; j<ArraySize; j++) {
			printf("%f ", gsl_matrix_get (A, i, j));
		}
		printf("\n");
	}
	printf("S start\n");
	for (int i=0; i<ArraySize; i++) {
			printf("%f ", gsl_vector_get (S, i));
	}
	printf("\n");
	printf("X start\n");
	for (int i=0; i<ArraySize; i++) {
		printf("%f ", gsl_vector_get (X, i));
	}
	printf("\n");
	*/

	for (int i=0; i<particleNum; i++) {
		jello->a[i].x = gsl_vector_get(X, i*2);
		jello->a[i].y = gsl_vector_get(X, i*2 +1);
		jello->a[i].z = 0;		
	}
	
	/*
	printf("A start\n");
	for (int i=0; i<particleNum; i++) {
		printf("a[%d].x = %f   a.y = %f   a.z = %f\n",i,jello->a[i][0][0].x,jello->a[i][0][0].y, jello->a[i][0][0].z);
	}	
	printf("\n");
	*/
	//printf("Fx: %f   Fy: %f\n", jello->Force.x, jello->Force.y);
	
	//release data
	delete [] a_data;
	delete [] b_data;
	gsl_matrix_free(A);
	gsl_matrix_free(V);
	gsl_vector_free(S);
	gsl_vector_free(work);
	gsl_vector_free(B);
	gsl_vector_free(X);

}
/* performs one step of Euler Integration */
/* as a result, updates the jello structure */
void Euler(struct world * jello)
{
  computeAcceleration(jello);
  
	/*printf("A start\n");
	for (int i=0; i<particleNum; i++) {
		printf("a[%d].x = %f   a.y = %f   a.z = %f\n",i,jello->a[i][0][0].x,jello->a[i][0][0].y, jello->a[i][0][0].z);
	}	
	printf("\n");*/
	
	for (int i=1; i<particleNum; i++)
      {
		  jello->v[i].x += jello->dt * jello->a[i].x;
		  jello->v[i].y += jello->dt * jello->a[i].y;
		  jello->p[i].x += jello->dt * jello->v[i].x;
		  jello->p[i].y += jello->dt * jello->v[i].y;

      }
}


/* performs one step of RK4 Integration */
/* as a result, updates the jello structure */

void RK4(struct world * jello)
{
  point F1p[999], F1v[999], 
        F2p[999], F2v[999],
        F3p[999], F3v[999],
        F4p[999], F4v[999];


  struct world buffer;

  int i,j,k;

  buffer = *jello; // make a copy of jello

  computeAcceleration(jello);

  for (i=0; i<particleNum; i++){
	pMULTIPLY(jello->v[i],jello->dt,F1p[i]);
	pMULTIPLY(jello->a[i],jello->dt,F1v[i]);
	pMULTIPLY(F1p[i],0.5,buffer.p[i]);
	pMULTIPLY(F1v[i],0.5,buffer.v[i]);
	pSUM(jello->p[i],buffer.p[i],buffer.p[i]);
	pSUM(jello->v[i],buffer.v[i],buffer.v[i]);
  }

  computeAcceleration(&buffer);

  for (i=0; i<particleNum; i++){
         // F2p = dt * buffer.v;
         pMULTIPLY(buffer.v[i],jello->dt,F2p[i]);
         // F2v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(buffer.a[i],jello->dt,F2v[i]);
         pMULTIPLY(F2p[i],0.5,buffer.p[i]);
         pMULTIPLY(F2v[i],0.5,buffer.v[i]);
         pSUM(jello->p[i],buffer.p[i],buffer.p[i]);
         pSUM(jello->v[i],buffer.v[i],buffer.v[i]);
      }

  computeAcceleration(&buffer);

  for (i=0; i<particleNum; i++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i],jello->dt,F3p[i]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(buffer.a[i],jello->dt,F3v[i]);
         pMULTIPLY(F3p[i],0.5,buffer.p[i]);
         pMULTIPLY(F3v[i],0.5,buffer.v[i]);
         pSUM(jello->p[i],buffer.p[i],buffer.p[i]);
         pSUM(jello->v[i],buffer.v[i],buffer.v[i]);
      }
         
  computeAcceleration(&buffer);


  for (i=0; i<particleNum; i++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i],jello->dt,F4p[i]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(jello->a[i],jello->dt,F4v[i]);

         pMULTIPLY(F2p[i],2,buffer.p[i]);
         pMULTIPLY(F3p[i],2,buffer.v[i]);
         pSUM(buffer.p[i],buffer.v[i],buffer.p[i]);
         pSUM(buffer.p[i],F1p[i],buffer.p[i]);
         pSUM(buffer.p[i],F4p[i],buffer.p[i]);
         pMULTIPLY(buffer.p[i],1.0 / 6,buffer.p[i]);
         pSUM(buffer.p[i],jello->p[i],jello->p[i]);

         pMULTIPLY(F2v[i],2,buffer.p[i]);
         pMULTIPLY(F3v[i],2,buffer.v[i]);
         pSUM(buffer.p[i],buffer.v[i],buffer.p[i]);
         pSUM(buffer.p[i],F1v[i],buffer.p[i]);
         pSUM(buffer.p[i],F4v[i],buffer.p[i]);
         pMULTIPLY(buffer.p[i],1.0 / 6,buffer.p[i]);
         pSUM(buffer.p[i],jello->v[i],jello->v[i]);
      }

  return;  
}


