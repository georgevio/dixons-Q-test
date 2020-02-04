#ifndef dixon_n_vals
#define dixon_n_vals 8 /* Change this within [3,21] */ 
#endif

#ifndef confidence_level
#define confidence_level 1 // 0 = q90, 1 = q95, 2 = q99
#endif

/**
@input an array to sort. quicksort needs two starting points, usualy the
fisrt and the last element.
*/
void quicksort(float number[dixon_n_vals],float first,float last);

/* return 0 if there is no outlier in the current data
 * return 1 if the new value is a small outlier,
 * return 2 if the incoming value is a big outlier
 */
static int add_dixon_val(float value);
