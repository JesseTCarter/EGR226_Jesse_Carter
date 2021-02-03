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

void print_book(struct Book obj)

{

	printf("\n===================== BOOK INFO =======================\n");
	printf("|Title:  %s\n", obj.title); //Title
	printf("|Author: %s\n", obj.author_name); //Author name
	printf("|ISBN:   %s\n", obj.ISBN); //ISBN
	printf("|Pages:  %d\n", obj.pages); //Number of pages (convert to int)
	printf("|Year:   %d\n", obj.year_published); //Year published (convert to int)
	printf("===================== END OF BOOK INFO ================\n\n");

}

//This function constructs the Book object by itself and then returns the constructed object
struct Book HelperConstructBook(char * title, char * author, char * isbn, char * pages, char * year_published)

{
	//Convert pages and year published to int first.
	int pagecount = 0;
	int year = atoi(year_published);

	if (strcmp(pages, "N/A") == 0)

	{

		pagecount = -99; //Represents that the information is not available.

	}

	else

	{

		pagecount = atoi(pages);

	}

	struct Book to_return = { .pages = pagecount, .year_published = year }; //Assign everything that is not string in this manner.
	//The character/string stuff will have to be 'copied' to one another. We'll use a built in strcpy function to achieve this.
	strcpy(to_return.title, title);
	strcpy(to_return.author_name, author);
	strcpy(to_return.ISBN, isbn);
	//HelperPrint(to_return);

	return to_return;

}

struct Book * parse_file(char* fileName, struct Book * books, int * activeIndex)

{

	int index = 0; //Active index for books array

	FILE* filereader;

	filereader = fopen(fileName, "r"); //Open the file here for reading

	if (filereader)

	{

		char text[999];
		int count = 0;
		while (fgets(text, 999, filereader) != NULL) //This will read the record line by line until end of file is reached.

		{

			char* token;
			//We now have to tokenize this string. We know the format: Title, Author Name, ISBN, Number of pages, Year published
			//printf("%s", text); //Full line
			token = strtok(text, ",");//Title

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

int search_title(struct Book * books, int total_books, char * searchterm)

{

	int i = 0;
	int found = 0;

	while (i < total_books)

	{

	if (strstr(books[i].title, searchterm) != NULL) //Checks if the title contains the search term

		{

			print_book(books[i]);

			found = 1;

		}

        i++;

	}

	return found;

}

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

int main()

{
	struct Book* books;

	books = (struct Book*)malloc(sizeof(struct Book) * 360);

	int total_books = 0;

	parse_file("BookList.csv", books, &total_books);

	//printf("Total books are %d", total_books);

	int choice = 0;

	while (choice != -99)

	{

        printf("What would you like to search by? [0] Title, [1] Author, [2] ISBN. \nOr enter ");
        printf("[-99] to exit the program.\n");

		int result = scanf("%d", &choice);

		if (result > 1)

		{

			printf("Wrong value type entered.....\n");

			return;

		}

		else if (choice != -99)

		{
			if (choice >= 0 && choice <= 2)

			{

				getchar(); //get the extra '\n' if we press enter

				char searchTerm[999];

				printf("\nEnter you search criteria\n");

				gets(searchTerm);

				int retvalue = 0; //return value from the functions

				switch (choice)

				{

                    case 0:
                        retvalue = search_title(books, total_books, searchTerm);
					break;

                    case 1:
                        retvalue = search_author(books, total_books, searchTerm);
					break;

                    case 2:
                        retvalue = search_ISBN(books, total_books, searchTerm);
					break;

				}

				if (retvalue == 0)

				{

					printf("PROMPT: Entered query could not be found!\n");

				}

			}

			else

			{

				printf("Please enter a value between 0 and 2\n\n");
				fflush(stdin);

			}


		}

	}

}
