#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
  int retcode = 0;
  int N, T, j, h, t;
  double alpha, pi, *optimal, newprice, candidate, bestone, *shift;

  if(argc != 5){
    printf("use: trade alpha pi N T\n"); retcode = 1; goto BACK;
  }

  alpha = atof(argv[1]);
  pi = atof(argv[2]);
  N = atoi(argv[3]);
  T = atoi(argv[4]);

  printf("alpha = %g pi = %g N = %d T =%d\n", alpha, pi, N, T);

  optimal = (double *)calloc((N + 1)*T, sizeof(double));
  if (!optimal){
	  printf("cannot allocate large matrix\n"); retcode = 2; goto BACK;
  }

 /* shift = (double *)calloc(N + 1, sizeof(double));

  for (j = 0; j <= N; j++)
	  shift[j] = 1 - alpha*pow((double)j, pi);
	  */

  /** do last stage **/
  for (j = 0; j <= N; j++){
	  newprice = 1 - alpha*pow((double)j, pi);
	  optimal[(T - 1)*(N + 1) + j] = newprice*j;
	  // V[k,t] stored at optimal[t*(N+1) + k] 
  }
  for (t= T - 2; t>= 0; t--){
	  for (j = 0; j <= N; j++){

		  bestone = 0;
		  /** enumerate possibilities **/
		  for (h = 0; h <= j; h++){
			  newprice = 1 - alpha*pow((double)h,pi);
			  //newprice = shift[h];
			  candidate = newprice*h + newprice*optimal[(t + 1)*(N + 1) + j - h];

			  if (candidate > bestone)
				  bestone = candidate;
		  }
		  optimal[t*(N + 1) + j] = bestone;
	  }
	  printf("done with stage t = %d\n", t);
  }

  printf("optimal value for trade sequencing = %g\n", optimal[N]);

BACK:
  printf("\nran with code %d\n", retcode);
  return retcode;
}