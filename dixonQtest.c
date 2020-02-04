/* Author: George Violettas, georgevio@uom.edu.gr
 * 2020-02-01
 
 * Implementing a Dixon Q-test Function inspired from
 * Python implementation: https://sebastianraschka.com/Articles/2014_dixon_test.html
 
    Keyword arguments:
        data = A ordered or unordered list of data points (int or float).
        left = Q-test of minimum value in the ordered list if True.
        right = Q-test of maximum value in the ordered list if True.
        q_dict = A dictionary of Q-values for a given confidence level,
            where the dict. keys are sample sizes N, and the associated values
            are the corresponding critical Q values. E.g.,
            {3: 0.97, 4: 0.829, 5: 0.71, 6: 0.625, ...}

 *   In python, returns a list of 2 values for the outliers, or None.
 *   E.g.,
 *      for [1,1,1] -> [None, None]
 *      for [5,1,1] -> [None, 5]
 *      for [5,1,5] -> [1, None]
 *
 * Read about Dixon Q test here: 
 * https://www.statisticshowto.datasciencecentral.com/dixons-q-test/
 */

#include<stdio.h> 
#include "dixonQtest.h"   

/* minimum is 3 NOT zero, i.e., 0.941 is for 3 incoming values */    
static float q90[] = 
		{0.941, 0.765, 0.642, 0.560, 0.507, 0.468, 0.437,
       0.412, 0.392, 0.376, 0.361, 0.349, 0.338, 0.329,
       0.320, 0.313, 0.306, 0.300, 0.295, 0.290, 0.285, 
       0.281, 0.277, 0.273, 0.269, 0.266, 0.263, 0.260
       };

static float q95[] = 
		{0.970, 0.829, 0.710, 0.625, 0.568, 0.526, 0.493, 0.466,
       0.444, 0.426, 0.410, 0.396, 0.384, 0.374, 0.365, 0.356,
       0.349, 0.342, 0.337, 0.331, 0.326, 0.321, 0.317, 0.312,
       0.308, 0.305, 0.301, 0.290
      };

static float q99[] = 
		{0.994, 0.926, 0.821, 0.740, 0.680, 0.634, 0.598, 0.568,
       0.542, 0.522, 0.503, 0.488, 0.475, 0.463, 0.452, 0.442,
       0.433, 0.425, 0.418, 0.411, 0.404, 0.399, 0.393, 0.388,
       0.384, 0.380, 0.376, 0.372
       };
       
static int queue_index = 0; /* no queue in standard C? */
static float incoming_values[dixon_n_vals];
static float sort[dixon_n_vals];
double critical_value = 0; //confidence level to compare with

/**************************************************************/
/* return 0 if there is no outlier in the current data
 * return 1 if the new value is a small outlier,
 * return 2 if the incoming value is a big outlier
 */
static int add_dixon_val(float value){
	int answer = 0;
	float q_small;
	float q_big;
	int all_values_exist = 0; /* array is not populated yet */
	
	incoming_values[queue_index] = value;
	printf("q_ incom[%d]=%ld\n",queue_index, 
			incoming_values[queue_index]);
	
	queue_index++;
	if (queue_index == dixon_n_vals){
		queue_index = 0;	
		all_values_exist = 1; /* array has now all cells populated */
	}	
	
	if(all_values_exist == 1){ /* start calculating possible outliers */
		float sort[dixon_n_vals];
		int i;
		for( i=0;i<dixon_n_vals;i++){
			sort[i] = incoming_values[i];
		}
		quicksort(sort, sort[0], sort[dixon_n_vals]); /* sort a NEW array */

		/* Denominator = (x[n]-x[1] */
		float denom = sort[dixon_n_vals-1] - sort[0];	
		printf("\nQ_last=%ld - Q_0=%ld --> denominator=%ld\n",
				sort[dixon_n_vals-1], sort[0], denom);

		if( denom != 0){ /* avoid division by zero */		
			/* Lower Outlier: Q = (x[2]-x[1])/(x[n]-x[1]) */		
			q_small = (sort[1] - sort[0]) / denom;
			printf("\nQ_1=%ld - Q_0=%ld / denom=%ld --> q_small: %ld\n",
					sort[1],sort[0],denom,q_small);
									
			/* Higher Outlier: Q = (x[n]-x[n-1])/(x[n]-x[1]) */
			q_big = (sort[dixon_n_vals-1] - sort[dixon_n_vals -2]) / denom;
			printf("\nsort[last]=%ld - sort[last-1]=%ld / denom=%f --> q_big: %f\n",
					sort[dixon_n_vals-1], sort[dixon_n_vals-2], denom, q_big);
		}
		if (confidence_level == 0){
			critical_value = q90[dixon_n_vals-3];			
		}
		else if (confidence_level == 1){
			critical_value = q95[dixon_n_vals-3];		
		}
		else if (confidence_level == 1){
			critical_value = q99[dixon_n_vals-3];				
		}
		printf("Critical value to compare qxx: %ld\n", critical_value);
			
		/* first confidence for n==3 */
		if(q_small > critical_value){ 
			printf("\nQ_small is an outlier\n");
			answer = 1;
		}
		if(q_big > critical_value){
			printf("\nQ_big is an outlier\n");
			answer = 2;
		}
		
		/* Print only for testing */
		if(q_small > q95[dixon_n_vals-3]) 
			printf("q_small is an outlier in q95\n");
		if(q_small > q99[dixon_n_vals-3]) 
			printf("q_small is an outlier in q99\n");			
	}
	return answer;
}

void sortDouble(double grade[], int n){
	int i, j, swapped;
	double temp;
	for (i = 0; i < n; ++i){
		 for (j = i + 1; j < n; ++j)
		 {
		     if (grade[i] < grade[j])
		     {
		         temp = grade[i];
		         grade[i] = grade[j];
		         grade[j] = temp;
		     }//end if
		 }//end inner for
	}//end outer for

	for (i = 0; i < n; ++i){
		 printf("q_after: %f\n", grade[i]);
	}//end for
}//end sortDouble

void quicksort(float number[dixon_n_vals],float first,float last){
	int i, j, pivot, temp;
	if(first<last){
		pivot=first;
		i=first;
		j=last;
		while(i<j){
			while(number[i]<=number[pivot]&&i<last)
				i++;
			while(number[j]>number[pivot])
				j--;
			if(i<j){
				temp=number[i];
				number[i]=number[j];
				number[j]=temp;
			}
		}
		temp=number[pivot];
		number[pivot]=number[j];
		number[j]=temp;
		quicksort(number,first,j-1);
		quicksort(number,j+1,last);
	}
}















