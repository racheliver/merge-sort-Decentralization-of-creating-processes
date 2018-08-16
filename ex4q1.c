// CPP Program to implement merge sort using
// multi-threading

#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>


void merge_sort(int low, int high,int level,int array[]);
void* merge_sortTheard(void* arg);



int *array;int *array1;//use for the indexs in merge_sortTheard
int *a;//array for numbers
int MAX;// number of elements in array
int THREAD_MAX;// number of threads
int level;

/*
 *
 *
 *
 * In this Program we will implement the MarSort algorithm in a distributed way (parallel calculation).
 * Distortion will be performed between different processes when communication
 * between them will be done using a threads.
 *
 *
 * */


int main(int argc ,char* argv[]) {



    //checking the input correct
    if (argc<2)
    {
        printf("please use: ./ex4q1 <filename>\n");
        exit(0);
    }
    /* Code to test the MergeSort function. */
        array= (int*)malloc(sizeof(int)*2);
	array1= (int*)malloc(sizeof(int)*2);
        FILE *f = fopen(argv[1],"r");
	if(f==NULL)
	{
	puts("error opening file: No such file or directory");
	exit(0);
	}
    fscanf(f,"%d",&MAX);
    fscanf(f,"%d",&THREAD_MAX);
	
    a= (int*)malloc(sizeof(int)*MAX);


    int level=(int)log2(THREAD_MAX);

    for (int i=0;i<MAX;i++)
    {
        a[i]=0;
        fscanf(f,"%d,",&a[i]);
    }
    fclose(f);//close the file
    printf("Amount of numbers that sort: %d\n",MAX);
    printf("Degree of parallelism: %d\n",THREAD_MAX);
    printf("before sort: ");
    for (int i=0;i<MAX;i++)
    {
        if(i!=MAX-1)
            printf("%d,",a[i]);
        else
            printf("%d\n",a[i]);
    }

 	
    // t1 and t2 for calculating time for
    // merge sort
    clock_t t1, t2;

    t1 = clock();


   
    merge_sort(0,MAX,level,array);



    t2 = clock();

    // displaying sorted array
     printf("After sort: ");
    for (int i=1;i<MAX+1;i++)
    {
        if(i!=MAX)
            printf("%d,",a[i]);
        else
            printf("%d\n",a[i]);
    }
	printf("\n");
    	// time taken by merge sort in seconds
    	printf("Time taken: %lf", ( (t2 - t1) /(double)CLOCKS_PER_SEC ));
	free(a);
	free(array1);
	free(array);

    return 0;
}

// merge function for merging two parts
void merge(int low, int mid, int high)
{
    int* left = (int*)malloc(sizeof(int)*(mid - low + 1));
    int* right = (int*)malloc(sizeof(int)*(high-mid));

    // n1 is size of left part and n2 is size
    // of right part
    int n1 = mid - low + 1, n2 = high - mid, i, j;

    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;
    i = j = 0;

    // merge let and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1) {
        a[k++] = left[i++];
    }

    // insert remaining values from right
    while (j < n2) {
        a[k++] = right[j++];
    }
free(left);
free(right);
}

// merge sort function
void merge_sort(int low, int high,int level,int array[])
{
    pthread_t threadl;pthread_t threadr;
   
    // calculating mid point of array
    int mid = low + (high - low) / 2;
    if (low < high) {
        if(level!=1) {
            
            level--;
            // calling first half
            merge_sort(low, mid,level,array);

            // calling second half
            merge_sort(mid + 1, high,level,array);

            // merging the two halves
            merge(low, mid, high);
       
        }
        else
        {


            array[0]=low;array[1]=(mid);
            pthread_create(&threadl, NULL,merge_sortTheard, (void*)array);

           


            array1[0]=mid+1;array1[1]=(high);
            pthread_create(&threadr, NULL,merge_sortTheard, (void*)array1);
   	
  	    pthread_join(threadl, NULL);//waiting to the prosses to ending
            pthread_join(threadr, NULL);
            merge(low, mid, high);
        }


    }

}



// thread function for multi-threading
void* merge_sortTheard(void* arg)
{     
    printf("Create a Theard : %u\n",(unsigned int)pthread_self());
    int *val_p=(int*)arg;
   

    int low = val_p[0];
    int high = val_p[1];

    if (low < high) {
        merge_sort(low, high,-1,val_p);
    }
}

// Driver Code


