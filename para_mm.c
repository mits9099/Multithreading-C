#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>
#include<pthread.h>

#define matrix_length     216    //size of matrix

static struct timeval start,end;

// to calculate time take by program to execute
void start_timer()
{
	gettimeofday(&start,(struct timezone*)NULL);
}

float stop_timer()
{
	gettimeofday(&end,(struct timezone*)NULL);
	float elapsed_time = (float)(end.tv_sec - start.tv_sec)+(end.tv_usec - start.tv_usec)/1000000.0;

return elapsed_time;
}

void start_timer();
float stop_timer();
pthread_barrier_t hold;
int a[matrix_length][matrix_length], b[matrix_length][matrix_length], ab[matrix_length][matrix_length]; //creating 2d array for matrix a,b and ab
int value;

void mm(int *val)  //matrix multiplication process
{ 
	int i,j,k,temp;
	int start= (*val * matrix_length)/value;
	int end= ((*val+1) * matrix_length)/value;   
	printf("%d\t%d\n",start,end);
        for(i=start;i<end;i++)
        {
            for(j=0;j<matrix_length;j++)
            {
		temp=0;
                for(k=0;k<matrix_length;k++)
                {
                    temp=temp+(a[i][k]*b[k][j]);
                }
                ab[i][j]=temp;
            }
        }
pthread_barrier_wait(&hold);
}

int main(int argc,char *argv[])
{
	int i,j,k;
	float e_time;
	value = atoi(argv[1]);  //take argument 
	pthread_t child[value];
	pthread_barrier_init(&hold,NULL,value+1);    //init pthread barrier
	for(i=0;i<matrix_length;i++){    //assigning value as 1 to every position in matrix 
	for(j=0;j<matrix_length;j++)
		{
			a[i][j]=1;
			b[i][j]=1;
		}
	}
	int range[16];
	start_timer(); // Timer Started
	
	for(i=0;i<value;i++)
		{
			range[i]=i;
			if(pthread_create(&child[value],NULL,(void *)mm,&range[i])!=0)
			{
				printf("Error Creating Thread");
			}
		}

	pthread_barrier_wait(&hold);    // pthread barrier
	e_time = stop_timer();
	printf("The Elapsed time = %f \n", e_time);

  return 0;
}
