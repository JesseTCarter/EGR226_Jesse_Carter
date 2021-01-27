/*

Name:	        Jesse Carter

Course:	        EGR 226-902

Date:	        01/26/2021

Project:	    LAB01

File:	        mainPart2.c

Professor:	    Trevor Ekin

Description:    This program will open and read a .txt file called data and read through the document. It will take
                the information and reallocate memory dynamically-as a new element gets added, incrementally increasing
                the size of the array. Then, it'll take the reallocated data and run it through the functions that are
                called at the end of the InterfaceHandler function. These functions are stored in a library (stats_lib.h),
                and are defined in another file (stats_lib.c).

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stats_lib.h"

void InterfaceHandler(void);

int main()

{

    InterfaceHandler();

    return(0);

}

/*                 | InterfaceHandler function |

 Brief: This function does all the work, it opens and reads a file called
 data.txt. It takes the information stored in the file and reallocates it into memory.

 parameters: N/A

 return: N/A

*/

void InterfaceHandler(void)    //This function basically handles everything, it reads the file and then outputs the data accordingly.

{
    float* data = malloc(sizeof(float) * (0));

    int size = 0;

    FILE* readFile;

    readFile = fopen("data.txt", "r");

    if (!readFile)

    {

        printf("The requested file could not be opened!\n");

    }

    else

    {

        while (1)

        {

            float temp;

            while (fscanf(readFile, "%f", &temp) != EOF)

            {

                data = realloc(data, sizeof(float) * (size + 1));   //Reallocate memory dynamically-as new element gets added, increase the size of the array.

                if (data != NULL)   //Check if reallocation was successful.

                {

                    data[size] = temp;
                    size++;
                }


            }

            fclose(readFile);

            break;

        }

    }


    //Call everything in the stated format from stats_lib.c.


    printf("File has been successfully read.... generating results..... %d elements found.\n", size);

    printf("Maximum: %.0lf\n", maximum(data, size));
    printf("Minimum: %.0lf\n", minimum(data, size));
    printf("Mean: %.2lf\n", mean(data, size));
    printf("Median: %.0lf\n", median(data, size));
    printf("Variance: %.2lf\n", variance(data, size));
    printf("Standard Deviation: %.2lf", standard_deviation(data, size));

}
