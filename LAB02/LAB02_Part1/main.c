/*

Name:	        Jesse Carter

Course:	        EGR 226-902

Date:	        02/03/2021

Project:	    LAB02

File:	        mainPart1.c

Professor:	    Trevor Ekin

Description:    This is a looping program and will take a user input resistor value and
                translate that value into its corresponding color-band value, or take
                a color-band value and translate that back into a resistor value; based
                off of the table shown at the start of the program.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

void prompt(void);
void getColorBands(char*, char*, char*, char*);
void calcResistance(char, char, char, char);
void calcResistorColors(int);


int main()

{

    prompt();

    return 0;

}

/*                 | prompt function |

 Brief: The prompt function asks the user if they would like
        to calculate resistance from color-code values, or
        calculate color-code values from resistance. The
        input has to be an integer between 1-99M. If it
        is not an integer the program will report back an
        error, if an integer is input, it takes that value to the
        calcResistorColors function to be decoded to
        its color-band values; and vice-versa for the color-band
        values, they're taken to calcResistance and outputs the
        resistance value and its tolerance.

 parameters: N/A

 return: N/A

*/

void prompt(void)

{

    start:
    resume:

    printf("--------------------Resistor Codes---------------------------------\n");
    printf("| Character |  Color  | Band 1 & 2 |      Band 3     |    Band 4   |\n");
    printf("-------------------------------------------------------------------\n");
    printf("|     K     |  Black  |      0     |          *1     |   +/- 1%%   |\n");
    printf("|     N     |  Brown  |      1     |         *10     |   +/- 2%%   |\n");
    printf("|     R     |  Red    |      2     |        *100     |            |\n");
    printf("|     O     |  Orange |      3     |       *1,000    |            |\n");
    printf("|     Y     |  Yellow |      4     |      *10,000    |            |\n");
    printf("|     G     |  Green  |      5     |     *100,000    |  +/- 0.5%%  |\n");
    printf("|     B     |  Blue   |      6     |    *1,000,000   |  +/- 0.25%% |\n");
    printf("|     V     |  Violet |      7     |   *10,000,000   |  +/- 0.1%%  |\n");
    printf("|     E     |  Grey   |      8     |                 |  +/- 0.05%% |\n");
    printf("|     W     |  White  |      9     |                 |            |\n");
    printf("|     D     |  Gold   |            |                 |   +/- 5%%   |\n");
    printf("|     S     |  Silver |            |                 |   +/- 10%%  |\n");
    printf("-------------------------------------------------------------------\n\n");

    int userinput = 0;
    int closestate = 0;
    int userprompt = 0;
    char color1, color2, color3, color4;

    printf("Choose which resistor function you would like to perform:\n");
    printf("    (1): Determine color-code for given resistance value\n");
    printf("    (2): Determine resistance value from given color-code\n");
    printf("Enter a value between 1 and 2: ");

    scanf("%d", &userprompt);

    while (userprompt != 1 && userprompt !=2)

        {
            printf("ERROR: Invalid input\n");
            fflush(stdin);
            scanf("%d", &userprompt);
        }

    if(userprompt == 2)

    {
        getColorBands(&color1, &color2, &color3, &color4);
        printf("\nKey in 0 to exit program, or key in any other number to try another resistor.\n");



        if (scanf("%d", &closestate))

        {

            if (closestate == 0)

            {

                printf("Goodbye\n");
                exit(0);

            }

            else
                goto resume;

        }

    }

    else if(userprompt == 1)

    {



    while (1)

    {

        printf("Choose a value between 1 and 99000000: ");

        if (scanf("%d", &userinput)==1)

        {

            while (userinput < 0 || userinput > 99000000)

            {

                printf("ERROR: Invalid input\n");

                goto start;

            }

            printf("A resistor of [%d] Ohms would have a color-code of: ", userinput);

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

            else
                goto resume;

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

/*                 | getColorBands function |

 Brief: This function takes the user input and error checks it against
        certain cases to make sure the inputs can be calculated when they're
        called back to the calcResistance function.

 parameters: char* code1, char* code2, char* code3, char* code4

 return: N/A

*/

void getColorBands(char* code1, char* code2, char* code3, char* code4)
{
    int i;
    i = 0;

    colors:
    fflush(stdin);
    printf("Key in the values shown in the ""Character"" column.\n");

    printf("Enter the color-code for band-1: ");

    scanf("%c", code1);
    //printf("%c", code1);

    if (*code1 == 'K' || *code1 == 'k' || *code1 == 'N' || *code1 == 'n' || *code1 == 'R' || *code1 == 'r' || *code1 == 'O' || *code1 == 'o' || *code1 == 'Y' ||
        *code1 == 'y' || *code1 == 'G' || *code1 == 'g' || *code1 == 'B' || *code1 == 'b' || *code1 == 'V' || *code1 == 'v' || *code1 == 'E' || *code1 == 'e' ||
        *code1 == 'W' || *code1 == 'w')
    {
        //printf("");
        i = 0;
    }
    else
    {
        i = 1;
        printf("ERROR: Invalid input\n");
    }

    fflush(stdin);
    printf("Enter the color-code for band-2: ");
    scanf("%c", code2);

    if (*code2 == 'K' || *code2 == 'k' || *code2 == 'N' || *code2 == 'n' || *code2 == 'R' || *code2 == 'r' || *code2 == 'O' || *code2 == 'o' || *code2 == 'Y' ||
        *code2 == 'y' || *code2 == 'G' || *code2 == 'g' || *code2 == 'B' || *code2 == 'b' || *code2 == 'V' || *code2 == 'v' || *code2 == 'E' || *code2 == 'e' ||
        *code2 == 'W' || *code2 == 'w')
    {
        //printf("");
        i = 0;
    }
    else
    {
        printf("ERROR: Invalid input\n");
        i = 1;
    }

    fflush(stdin);
    printf("Enter the color-code for band-3: ");
    scanf("%c", code3);

    if (*code3 == 'K' || *code3 == 'k' || *code3 == 'N' || *code3 == 'n' || *code3 == 'R' || *code3 == 'r' || *code3 == 'O' ||
        *code3 == 'o' || *code3 == 'Y' || *code3 == 'y' || *code3 == 'G' || *code3 == 'g' || *code3 == 'B' || *code3 == 'b' ||
        *code3 == 'V' || *code3 == 'v')
    {
        //printf("");
        i = 0;
    }
    else
    {
        printf("ERROR: Invalid input\n");
        i = 1;
    }

    fflush(stdin);
    printf("Enter the color-code for band-4: ");
    scanf("%c", code4);

    if (*code4 == 'K' || *code4 == 'k' || *code4 == 'N' || *code4 == 'n' || *code4 == 'G' || *code4 == 'g' || *code4 == 'B' || *code4 == 'b' ||
        *code4 == 'V' || *code4 == 'v' || *code4 == 'E' || *code4 == 'e' || *code4 == 'D' || *code4 == 'd' || *code4 == 'S' || *code4 == 's')
    {
        //printf("");
        i = 0;
    }
    else
    {
        printf("ERROR: Invalid input\n");
        i = 1;
    }
    if(i == 1)
    {
        printf("Please enter valid letters.\n");
        goto colors;
    }

    printf("The resistor is [", &code1, &code2, &code3, &code4);

    calcResistance(*code1, *code2, *code3, *code4);

}

/*                 | calcResistance function |

 Brief:     This function takes the values from getColorBands
            and matches them up with the correct cases and prints
            the corresponding assigned values.

 parameters: char code1, char code2, char code3, char code4

 return: N/A

*/

void calcResistance(char code1, char code2, char code3, char code4)

{

    switch (code1)

    {

    case 'K':
        printf("0");
        break;
    case 'N':
        printf("1");
        break;
    case 'R':
        printf("2");
        break;
    case 'O':
        printf("3");
        break;
    case 'Y':
        printf("4");
        break;
    case 'G':
        printf("5");
        break;
    case 'B':
        printf("6");
        break;
    case 'V':
        printf("7");
        break;
    case 'E':
        printf("8");
        break;
    case 'W':
        printf("9");
        break;
    case 'D':
        break;
    case 'S':
        break;
    case 'k':
        printf("0");
        break;
    case 'n':
        printf("1");
        break;
    case 'r':
        printf("2");
        break;
    case 'o':
        printf("3");
        break;
    case 'y':
        printf("4");
        break;
    case 'g':
        printf("5");
        break;
    case 'b':
        printf("6");
        break;
    case 'v':
        printf("7");
        break;
    case 'e':
        printf("8");
        break;
    case 'w':
        printf("9");
        break;
    case 'd':
        break;
    case 's':
        break;

    }

    switch (code2)

    {

    case 'K':
        printf("0");
        break;
    case 'N':
        printf("1");
        break;
    case 'R':
        printf("2");
        break;
    case 'O':
        printf("3");
        break;
    case 'Y':
        printf("4");
        break;
    case 'G':
        printf("5");
        break;
    case 'B':
        printf("6");
        break;
    case 'V':
        printf("7");
        break;
    case 'E':
        printf("8");
        break;
    case 'W':
        printf("9");
        break;
    case 'D':
        break;
    case 'S':
        break;
    case 'k':
        printf("0");
        break;
    case 'n':
        printf("1");
        break;
    case 'r':
        printf("2");
        break;
    case 'o':
        printf("3");
        break;
    case 'y':
        printf("4");
        break;
    case 'g':
        printf("5");
        break;
    case 'b':
        printf("6");
        break;
    case 'v':
        printf("7");
        break;
    case 'e':
        printf("8");
        break;
    case 'w':
        printf("9");
        break;
    case 'd':
        break;
    case 's':
        break;

    }

    switch (code3)

    {

    case 'K':
        printf("]Ohms ");
        break;
    case 'N':
        printf("0]Ohms ");
        break;
    case 'R':
        printf("00]Ohms ");
        break;
    case 'O':
        printf("000]Ohms ");
        break;
    case 'Y':
        printf("0000]Ohms ");
        break;
    case 'G':
        printf("00000]Ohms ");
        break;
    case 'B':
        printf("000000]Ohms ");
        break;
    case 'V':
        printf("0000000]Ohms ");
        break;
    case 'E':
        break;
    case 'W':
        break;
    case 'D':
        printf("*0.1]Ohms ");
        break;
    case 'S':
        printf("*0.01]Ohms ");
        break;
    case 'k':
        printf("]Ohms ");
        break;
    case 'n':
        printf("0]Ohms ");
        break;
    case 'r':
        printf("00]Ohms ");
        break;
    case 'o':
        printf("000]Ohms ");
        break;
    case 'y':
        printf("0000]Ohms ");
        break;
    case 'g':
        printf("00000]Ohms ");
        break;
    case 'b':
        printf("000000]Ohms ");
        break;
    case 'v':
        printf("0000000]Ohms ");
        break;
    case 'e':
        break;
    case 'w':
        break;
    case 'd':
        printf("*0.1]Ohms ");
        break;
    case 's':
        printf("*0.01]Ohms ");
        break;

    }

    switch (code4)

    {

    case 'K':
        printf("with a +/-1%% tolerance\n");
        break;
    case 'N':
        printf("with a +/-2%% tolerance\n");
        break;
    case 'R':
        break;
    case 'O':
        break;
    case 'Y':
        break;
    case 'G':
        printf("with a +/- 0.5%% tolerance\n");
        break;
    case 'B':
        printf("with a +/- 0.25%% tolerance\n");
        break;
    case 'V':
        printf("with a +/- 0.1%% tolerance\n");
        break;
    case 'E':
        printf("with a +/- 0.05%% tolerance\n");
        break;
    case 'W':
        break;
    case 'D':
        printf("with a +/-5%% tolerance\n");
        break;
    case 'S':
        printf("with a +/-10%% tolerance\n");
        break;
    case 'k':
        printf("with a +/-1%% tolerance\n");
        break;
    case 'n':
        printf("with a +/-2%% tolerance\n");
        break;
    case 'r':
        break;
    case 'o':
        break;
    case 'y':
        break;
    case 'g':
        printf("with a +/- 0.5%% tolerance\n");
        break;
    case 'b':
        printf("with a +/- 0.25%% tolerance\n");
        break;
    case 'v':
        printf("with a +/- 0.1%% tolerance\n");
        break;
    case 'e':
        printf("with a +/- 0.05%% tolerance\n");
        break;
    case 'w':
        break;
    case 'd':
        printf("with a +/-5%% tolerance\n");
        break;
    case 's':
        printf("with a +/-10%% tolerance\n");
        break;

    }

}




