#include "stats_lib.h"
#include <math.h>

/*                 | maximum function |

 Brief: This function initializes to a very small number,
        that way, it can sort through all possible data
        and stores it in an array, after it's done sorting,
        the maximum value will be stored in the variable
        maximum.

 parameters: nums, array, and int n.

 return: Float value.

*/

float maximum(float nums[], int n)

{

	float maximum = -999999;    //Initialize to a very small value

    int i;

    for (i = 0; i < n; i++)    //Repeat this till the end of the array. When this function exited, the maximum value is stored in the variable maximum.

	{
		if (nums[i] > maximum)

		{

			maximum = nums[i];

		}

	}

	return maximum;

}

/*                 | minimum function |

 Brief: This function initializes to a very large number,
        that way, it can sort through all possible data
        and stores it in an array, after it's done sorting,
        the minimum value will be stored in the variable
        minimum.

 parameters: nums, array, and int n.

 return: Float value.

*/

float minimum(float nums[], int n)

{

	float minimum = 999999;    //Opposite of maximum. Initialize to a high value

    int i;

	for (i = 0; i < n; i++)

	{
		if (nums[i] < minimum)

		{

			minimum = nums[i];

		}

	}

	return minimum;

}

/*                 | mean function |

 Brief: This function takes the sum of all the numbers
        in the array and divides it by the total amount
        of numbers that are in the array.

 parameters: nums, array, and int n.

 return: Float value.

*/

float mean(float nums[], int n)

{

	float sum = 0.0f;

    int i;

	for (i = 0; i < n; i++)

	{

		sum = sum + nums[i];

	}

	return (sum / n);

}

/*                 | median function |

 Brief: This function sorts the data in ascending order,
        it will always look for the next smallest number
        and place that number in the 0th index, we get the
        total number of index's in the array and divide by 2
        then the returned value is that value that was stored
        in that array, which will be the median.

 parameters: nums, array, and int n.

 return: Float value.

*/

float median(float nums[], int n)

{

	//First we will sort the data
	//Using selection sort and sorting it in the ascending order.

	int index = 0, i = 0;           //Index of the minimum value found in the respective iteration
                                    //Selection Sort will repeatedly find the minimum element first and would then sort it accordingly.
	for (i = 0; i < n; i++)

	{

		index = i;
		int j;

		for (j = i + 1; j < n; j++)

		{

			if (nums[j] < nums[index])      //The reason here is that the sorting will always be done on the 0th element of the char-string array

			{

				index = j;

			}

		}
                                            //Now we swap the element such that it always goes to the 0th index.
		float temp = nums[i];
		nums[i] = nums[index];
		nums[index] = temp;

	}

	int median_val = n / 2;

	return nums[median_val];

}

/*                 | variance function |

 Brief: This function finds the degree of spread in the data set,
        or the variance. It takes the value of each nums array
        and subtracts it from the mean, and squares it, the result
        then takes that total and divides it by int n minus one
        to get the variance.


 parameters: nums, array, and int n.

 return: Float value.

*/

float variance(float nums[], int n)

{

	float u = mean(nums, n);    //mean

	float sum = 0;


    //Since we have a summation notation for this equation. We will take the x - mean value for each value in the nums array. We will then compute
    //the numerator.


	int i;

	for (i = 0; i < n; i++)

	{

		float temp = (nums[i] - u) * (nums[i] - u);
		sum = temp + sum;

	}

	float variance = sum / (n - 1);

	return variance;

}

/*                 | standard_deviation function |

 Brief: This function finds the standard deviation of the data read.
        standard deviation is just square root of the variance, so
        it calls back the variance value and using the math.h lib
        the variance is square rooted to return the standard deviation
        value.

 parameters: nums, array, and int n.

 return: Float value.

*/

float standard_deviation(float nums[], int n)

{

	float var = variance(nums, n);

	return sqrt(var);

}
