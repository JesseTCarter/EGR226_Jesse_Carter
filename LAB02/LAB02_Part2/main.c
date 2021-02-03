/*

Name:	        Jesse Carter

Course:	        EGR 226-902

Date:	        02/03/2021

Project:	    LAB02

File:	        mainPart2.c

Professor:	    Trevor Ekin

Description:    This is a looping program that will take a user input search criteria
                and match it against a file called "BookList.csv" that was read into
                a memory array index, whatever the user inputs per the search criteria
                the program will build the book using a structure called book and
                tokenized strings. The books are then prints in the console window
                for the user to view.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    //Defintion of struct book.

struct Book

{

	char title[255];
	char author_name[50];
	char ISBN[10];
	int pages;
	int year_published;

};

/*                 | print_book function |

 Brief:     This function prints the book information grabbed
            and stored from the other functions outside of main,
            main will call the corresponding functions per the user
            input, store them, and call them.

 parameters: struct Book obj

 return: N/A

*/

void print_book(struct Book obj)

{

	printf("\n===================== BOOK INFO =======================\n");
	printf("|Title:  %s\n", obj.title);    //Title
	printf("|Author: %s\n", obj.author_name);   //Author name
	printf("|ISBN:   %s\n", obj.ISBN);    //ISBN
	printf("|Pages:  %d\n", obj.pages);    //Number of pages (convert to int)
	printf("|Year:   %d\n", obj.year_published);    //Year published (convert to int)
	printf("===================== END OF BOOK INFO ================\n");

}


/*                 | HelperConstructBook function |

 Brief:     HelperConstructBook will build the book object from
            the stored information and return the corresponding
            completed structure object.

 parameters: char * title, char * author, char * isbn, char * pages, char * year_published

 return: struct

*/


struct Book HelperConstructBook(char * title, char * author, char * isbn, char * pages, char * year_published)  //This function constructs the Book object by itself and then returns the constructed object

{
	//Convert pages and year published to int first.

	int pagecount = 0;
	int year = atoi(year_published);

	if (strcmp(pages, "N/A") == 0)

	{

		pagecount = -99;    //Represents that the information is not available.

	}

	else

	{

		pagecount = atoi(pages);

	}

	struct Book to_return = { .pages = pagecount, .year_published = year };    //Assign everything that is not string in this manner.

	//The character/string stuff will have to be 'copied' to one another. We'll use a built in strcpy function to achieve this.

	strcpy(to_return.title, title);
	strcpy(to_return.author_name, author);
	strcpy(to_return.ISBN, isbn);

	//HelperPrint(to_return);

	return to_return;

}

/*                 | parse_file function |

 Brief:     parse_file will search for and open the BookList.csv
            if the file is found, it will open and read the separated
            values until the end of the file, Then it will take the
            values and tokenize the strings so that they can be copied
            in other functions. If the file is not found, the program will
            close.

 parameters: char* fileName, struct Book * books, int * activeIndex

 return: struct

*/

struct Book * parse_file(char* fileName, struct Book * books, int * activeIndex)

{

	int index = 0;     //Active index for books array

	FILE* filereader;

	filereader = fopen(fileName, "r");     //Open the file here for reading

	if (filereader)

	{

		char text[999];
		int count = 0;
		while (fgets(text, 999, filereader) != NULL)    //This will read the record line by line until end of file is reached.

		{

			char* token;

			//We now have to tokenize this string. We know the format: Title, Author Name, ISBN, Number of pages, Year published

			//printf("%s", text); //Full line

			token = strtok(text, ",");    //Title

			char* author = strtok(NULL, ",");
			char* isbn = strtok(NULL, ",");
			char* pages = strtok(NULL, ",");
			char* year_published = strtok(NULL, ",");

			books[index] = HelperConstructBook(token, author, isbn, pages, year_published);

			index++;

		}

	}

	else

	{

		printf("Closed");

	}

	*activeIndex = index;

	return books;

}

/*                 | search_title function |

 Brief:     search_title will check the search term input
            by the user for a title query against the stored strings and will
            return any of the strings that are found within the
            searched term.

 parameters: struct Book * books, int total_books, char * searchterm

 return: int

*/

int search_title(struct Book * books, int total_books, char * searchterm)

{

	int i = 0;
	int found = 0;

	while (i < total_books)

	{

	if (strstr(books[i].title, searchterm) != NULL)    //Checks if the title contains the search term

		{

			print_book(books[i]);

			found = 1;

		}

        i++;

	}

	return found;

}

/*                 | search_author function |

 Brief:     search_author will check the search term input
            by the user for an author query against the stored strings and will
            return any of the strings that are found within the
            searched term.

 parameters: struct Book* books, int total_books, char* searchterm

 return: int

*/

int search_author(struct Book* books, int total_books, char* searchterm)

{

	int i = 0;
	int found = 0;

	while (i < total_books)

	{

		if (strstr(books[i].author_name, searchterm) != NULL)

		{

			print_book(books[i]);

			found = 1;

		}

		i++;

	}

	return found;

}

/*                 | search_ISBN function |

 Brief:     search_ISBN will check the search term input
            by the user for an ISBN query against the stored strings and will
            return any of the strings that are found within the
            searched term.

 parameters: struct Book* books, int total_books, char* searchterm

 return: int

*/

int search_ISBN(struct Book* books, int total_books, char* searchterm)

{

	int i = 0;
	int found = 0;

	while (i < total_books)

	{

		if (strstr(books[i].ISBN, searchterm) != NULL)

		{

			print_book(books[i]);
			found = 1;

		}

		i++;

	}

	return found;

}

/*                 | main function |

 Brief:     Main will store all of the values from the BookList
            into allocated memory and keep them there. It then
            prompts the user to search by 1 of 3 search terms.
            If an improper search term is input, an error statement
            will print. Otherwise, it will call by the corresponding
            search criteria, that specific search function will look for
            strings that match the search term, build the book object using
            the book structure and return that information through
            the print_book function. The program will continue this ask and search
            loop, until the user chooses to force quit the program, or quit
            by entering the integer value "-99".

 parameters: N/A

 return: int

*/

int main()

{
	struct Book* books;

	books = (struct Book*)malloc(sizeof(struct Book) * 360);

	int total_books = 0;

	parse_file("BookList.csv", books, &total_books);

	//printf("Total books are %d", total_books);

	int choice = 4;

	while (choice != -99)

	{

        printf("\nWhat would you like to search by? [0] Title, [1] Author, [2] ISBN. \nOr enter ");
        printf("[-99] to exit the program.\n");

		int result = scanf("%d", &choice);

		if (result > 1)

		{

			printf("ERROR: Invalid Input\n");

			return;

		}

		else if (choice != -99)

		{
			if (choice >= 0 && choice <= 2)

			{

				getchar();     //get the extra '\n' if we press enter

				char searchTerm[999];

				printf("\nEnter you search criteria:\n");

				gets(searchTerm);

				int retvalue = 0;    //return value from the functions

				switch (choice)

				{

                    case 0:
                        retvalue = search_title(books, total_books, searchTerm);
                        choice = 4;    //for error checking
					break;

                    case 1:
                        retvalue = search_author(books, total_books, searchTerm);
                        choice = 4;    //for error checking
					break;

                    case 2:
                        retvalue = search_ISBN(books, total_books, searchTerm);
                        choice = 4;    //for error checking
					break;

				}

				if (retvalue == 0)

				{

					printf("\nEntered query could not be found!\n");

				}

			}

			else

			{

				printf("\n<Please enter a value between 0 and 2>\n\n");
				fflush(stdin);

			}


		}

	}

}
