/*

Name:	        Jesse Carter

Course:	        EGR 226-902

Date:	        01/26/2021

Project:	    LAB01

File:	        mainPart1.c

Professor:	    Trevor Ekin

Description:    This is a looping program and will take a user input resistor value and
                translates that value into it's corresponding color-band value, based
                off of the table shown at the start of the program.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void prompt(void);
void calcResistorColors(int);

int main()

{

    prompt();

    return 0;

}

/*                 | prompt function |

 Brief: The prompt function asks the user to input an
        integer between 1-99M. If it is not an integer
        the program will report back and error, if an
        integer is input, it takes that value to the
        calcResistorColors function to be decoded to
        its color-band values.

 parameters: N/A

 return: N/A

*/

void prompt(void)

{

    start:

    printf("--------------------Resistor Codes-------------------\n");
    printf("| Character |  Color  | Band 1 & 2 |      Band 3     |\n");
    printf("-----------------------------------------------------\n");
    printf("|     K     |  Black  |      0     |          *1     |\n");
    printf("|     N     |  Brown  |      1     |         *10     |\n");
    printf("|     R     |  Red    |      2     |        *100     |\n");
    printf("|     O     |  Orange |      3     |       *1,000    |\n");
    printf("|     Y     |  Yellow |      4     |      *10,000    |\n");
    printf("|     G     |  Green  |      5     |     *100,000    |\n");
    printf("|     B     |  Blue   |      6     |    *1,000,000   |\n");
    printf("|     V     |  Violet |      7     |   *10,000,000   |\n");
    printf("|     E     |  Grey   |      8     |                 |\n");
    printf("|     W     |  White  |      9     |                 |\n");
    printf("|     D     |  Gold   |            |                 |\n");
    printf("|     S     |  Silver |            |                 |\n");
    printf("-----------------------------------------------------\n\n");

    int userinput = 0;
    int closestate = 0;

    while (1)

    {

        printf("Enter any resistance value to have translated into its color-bands\n");
        printf("Choose a value between 1 and 99000000: ");

        if (scanf("%d", &userinput)==1)

        {

            while (userinput < 0 || userinput > 99000000)

            {

                printf("ERROR: Invalid input\n");

                goto start;

            }

            printf("A resistor of %d Ohms would have a color-code of: ", userinput);

            calcResistorColors(userinput);

        }

        else

        {

            printf("\nERROR: Invalid input\n");
        }

        while(getchar()!='\n');    //Clear previous input, to prevent infinite looping.

        printf("\nKey in 0 to exit, or key in any other number to try again.\n");

        if (scanf("%d", &closestate))

        {

            if (closestate == 0)

            {

                printf("Goodbye!\n");

                exit(0);

            }

        }

    }

}

/*              | calcResistorColors function |

 Brief: This function takes the value input in the prompt
        function, and runs it through while loop, to determine
        the color-band value. That value is taken through a case-
        break statement and whatever value matches, it will print out
        that case-statements corresponding color value.

 parameters: int n

 return: N/A

*/

void calcResistorColors(int n)  //Input number from user

{

    int number;    //initializes digits and increment
    int increment;

    number = 0;    //Store reverse of n in number
    increment = 0;

    while (n != 0)  //Finds number of counts in from user input

    {

        number = (number * 10) + (n % 10);
        n /= 10;
        increment++;

    }

    while (number != 0)    //Extract last digit of number and print corresponding color code in words

    {

        switch (number % 10)    //case-break statements for when the value is equal to 1-9,
                                //its associating value will be printed based on bands 1 & 2.
        {

        case 1:
            printf("Brown-");
            break;
        case 2:
            printf("Red-");
            break;
        case 3:
            printf("Orange-");
            break;
        case 4:
            printf("Yellow-");
            break;
        case 5:
            printf("Green-");
            break;
        case 6:
            printf("Blue-");
            break;
        case 7:
            printf("Violet-");
            break;
        case 8:
            printf("Grey-");
            break;
        case 9:
            printf("White-");
            break;

        }

        number /= 10;

    }

    switch (increment)  //Case-break statements for the multiplier of the resistor, based off of band 3 on the table
                        //whatever value is read (2-9) the corresponding color will be printed
    {

    case 2:
        printf("Black");
        break;
    case 3:
        printf("Brown");
        break;
    case 4:
        printf("Red");
        break;
    case 5:
        printf("Orange");
        break;
    case 6:
        printf("Yellow");
        break;
    case 7:
        printf("Green");
        break;
    case 8:
        printf("Blue");
        break;
    case 9:
        printf("Violet");
        break;

    }

}
