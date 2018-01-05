#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <iostream>

using namespace std;

typedef struct model_output
{
	double optimal;
	int* plan;

	double alpha, pi1, pi2, rho, p1, p2;
	int N, T;
}model_output;



void do_computation();

std::vector < model_output * > OutputSets;
std::mutex m;
FILE *file = fopen("data.txt", "r");

int main(int *argc, char **argv){
	int num_thread = atoi(argv[1]);
	std::vector<std::thread> threads;

	//Compute optimal values
	for (int i = 0; i < num_thread; i++){
		threads.push_back(std::thread(do_computation));
	}

	for (auto& thread : threads){
		thread.join();
	}

	//Start printing outputs
	//std::cout << "Main Thread" << "\t" << std::this_thread::get_id() << std::endl;
	printf("Computation finished. Now start outputting results.\n");
	int count = OutputSets.size();
	for (int i = 0; i < count; i++){
		printf("alpha = %g pi1 = %g pi2 = %g rho = %g p1 = %g p2 = %g N = %d T =%d\n", OutputSets[i]->alpha, OutputSets[i]->pi1, OutputSets[i]->pi2, OutputSets[i]->rho, OutputSets[i]->p1, OutputSets[i]->p2, OutputSets[i]->N, OutputSets[i]->T);
		printf("optimal value for trade sequencing = %g\n", OutputSets[i]->optimal);
		printf("The selling plan is: \n");
		for (int t = 0; t < OutputSets[i]->T; t++){
			printf("Stage %d: %d\n", t, OutputSets[i]->plan[t]);
		}
	}

	system("pause");
	return 0;
}

void do_computation(){

	int N, T, j, h, t, temp_tag, temp_plan, *tag, *plan;
	double alpha, pi1, rho, pi2, p1, p2, *optimal, temp0, temp1, temp2, candidate, bestone;
	model_output * thisOutput;
	while (1){
		//read file, get parameters
		m.lock();
		if (fscanf_s(file, "%lf %lf %lf %lf %lf %lf %d %d", &alpha, &pi1, &pi2, &rho, &p1, &p2, &N, &T) != EOF)
		{
			thisOutput = (model_output *)calloc(1, sizeof(model_output));
			OutputSets.push_back(thisOutput);
			printf("N = %d\n", N);
			m.unlock();
		}
		else
		{
			printf("over.\n");
			m.unlock();
			return;
		}
		//std::cout << "Hello from sub thread " << std::this_thread::get_id() << std::endl;

		//printf("alpha = %g pi1 = %g pi2 = %g rho = %g p1 = %g p2 = %g N = %d T =%d\n", alpha, pi1, pi2, rho, p1, p2, N, T);
		thisOutput->alpha = alpha; thisOutput->pi1 = pi1; thisOutput->pi2 = pi2; thisOutput->rho = rho;
		thisOutput->p1 = p1; thisOutput->p2 = p2; thisOutput->N = N; thisOutput->T = T;

		optimal = (double *)calloc((N + 1)*T, sizeof(double));
		tag = (int *)calloc((N + 1)*T, sizeof(int));
		plan = (int *)calloc(T, sizeof(int));

		/** do last stage **/
		for (j = 0; j <= N; j++){
			bestone = 0;
			temp_tag = 0;
			for (h = 0; h <= j; h++){
				temp1 = (1 - alpha*pow((double)h, pi1))*p1;
				temp2 = (1 - alpha*pow((double)h, pi2))*p2;
				candidate = (temp1 + temp2)*h*rho;
				if (candidate > bestone){
					bestone = candidate;
					temp_tag = h;
				}
			}
			optimal[(T - 1)*(N + 1) + j] = bestone;
			tag[(T - 1)*(N + 1) + j] = temp_tag;
			// V[k,t] stored at optimal[t*(N+1) + k]  
		}
		//printf("done with stage t = %d\n", T - 1);

		for (t = T - 2; t >= 0; t--){
			for (j = 0; j <= N; j++){

				bestone = 0;
				temp_tag = 0;
				/** enumerate possibilities **/
				for (h = 0; h <= j; h++){
					temp0 = (1 - rho)*optimal[(t + 1)*(N + 1) + j] + rho*(h + optimal[(t + 1)*(N + 1) + j - h]);
					temp1 = (1 - alpha*pow((double)h, pi1))*p1;
					temp2 = (1 - alpha*pow((double)h, pi2))*p2;
					candidate = (temp1 + temp2)*temp0;

					if (candidate > bestone){
						bestone = candidate;
						temp_tag = h;
					}

				}
				optimal[t*(N + 1) + j] = bestone;
				tag[t*(N + 1) + j] = temp_tag;
			}
			//printf("done with stage t = %d\n", t);
		}

		//printf("optimal value for trade sequencing = %g\n", optimal[N]);

		plan[0] = tag[N];
		//printf("%d  ", plan[0]);
		temp_plan = 0;
		//printf("The selling plan is: \n");
		//printf("Stage 0: %d\n", plan[0]);
		for (t = 1; t <= T - 1; t++){
			temp_plan += plan[t - 1];
			plan[t] = tag[(N + 1)*t + N - temp_plan];
			//printf("Stage %d: %d\n", t, plan[t]);
		}

		thisOutput->optimal = optimal[N];
		thisOutput->plan = plan;
	}
}