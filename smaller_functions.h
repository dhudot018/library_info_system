char categories[5][18] = {"General Reference", "Journal", "Magazine", "Newspaper", "Literary Works"};
char status[2][10] = {"Available", "Borrowed"};
char months[13][20] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void printing_books(Book *temp) {
	printf("Book ID: %s \n", temp -> book_id);
	printf("Book Title: %s \n", temp -> book_title);
	printf("Book Author: %s %s \n", temp -> book_author_firstname, temp -> book_author_lastname);
	printf("Book Category: %s \n", categories[temp -> book_category - 1]);
	printf("Book Status: %s \n", status[temp -> book_status - 1]);
}


Borrowed_Date *put_date() {
	Borrowed_Date *date = (Borrowed_Date *) malloc(sizeof(Borrowed_Date));
	printf("\t      DATE\n");
	printf("Year: ");
	scanf("%i", &date -> year);
	for (int i = 0; i < 12; i ++) printf("\t [%i] %s\n", i+1, months[i]);
	printf("Month: ");
	scanf("%i", &date -> month);
	printf("Day: ");
	scanf("%i", &date -> day);
	return date;
}


void borrower_info_in_book (Book *book_ptr, Borrower *borrower) {
	book_ptr -> borrower -> borrower_info = borrower;
}



int id_search(char *id_type, char *to_compare ,Book *book_ptr, int book_counter, Borrower *borrower_ptr, int borrower_counter) {
	
	if (strcmp(id_type, "book_id") == 0) {
		
		if (book_counter == 0) return 1;

		Book *temp = book_ptr;
		
		for (int i = 0; i < book_counter; i ++) {
			if (strcmp(temp -> book_id, to_compare) == 0) return 0;
			temp = temp -> next_book; 
		}

	}

	else if (strcmp(id_type, "borrower_id") == 0) {

		if(borrower_counter == 0) return 1;

		Borrower *temp = borrower_ptr;
	
		for (int i = 0; i < borrower_counter ; i ++) {
			if(strcmp(temp -> brw_id, to_compare) == 0) return 0;
			temp = temp -> next_borrower;
		}
	}
	
	return 1;
}

int string_compare(char *string1, char *string2) {
	int min;
	if (strlen(string1) > strlen(string2)) min = strlen(string1);
	else min = strlen(string2);
	for (int str_index = 0; str_index < min; str_index ++) {
		int string1_index = toupper(string1[str_index] - '\0'), string2_index = toupper(string2[str_index] - '\0');
		if (string1_index > string2_index) return 0;
		else if (string1_index < string2_index) return 1;
	}
	return 0;
}

void sorting_algorithm(char *to_sort, Book **book_ptr, Book **book_to_sort, int book_counter, Borrower **borrower_ptr,int borrower_counter) {
	if (strcmp(to_sort, "title") == 0) {
		if (book_counter == 2) {
			int strcmp = string_compare((*book_ptr) -> book_title, (*book_to_sort) -> book_title);
			if (strcmp == 1) (*book_ptr) -> next_book = (*book_to_sort);
			else {
				(*book_to_sort) -> next_book = (*book_ptr);
				(*book_ptr) = (*book_to_sort);
			}
		}

		else if (book_counter > 2) {
			Book *prev = (*book_ptr), *current = (*book_ptr) -> next_book;
			do  {
				int strcmp1 = string_compare(prev -> book_title, (*book_to_sort) -> book_title);
				int strcmp2 = string_compare(current -> book_title, (*book_to_sort) -> book_title);
				if (strcmp1 == 0 && strcmp2 == 0) {
					(*book_to_sort) -> next_book = prev;
					(*book_ptr) = (*book_to_sort);
					break;
				}
				if (strcmp1 == 1 && strcmp2 == 0) {
					
					prev -> next_book = (*book_to_sort);
					(*book_to_sort) -> next_book = current;
					break;
				}
				if (strcmp1 == 1 && strcmp2 == 1) {
					(*book_to_sort) -> next_book = current -> next_book;
					current -> next_book = (*book_to_sort);
					prev -> next_book = current;
					prev = current;
					current = prev -> next_book -> next_book;
				}
			}
			while (current != NULL);

		}


	}

	// else if (strcmp(to_sort,"author") == 0) {

	// }
}


void search_algorithm(char *search_by, Book *book_ptr, int book_counter){
	char search_this[256];
	Book *temp = book_ptr;
	getchar();
	int counter = 0;
	
	if (strcmp(search_by, "title") == 0) {		
		printf("Enter Book's Name: ");
		fgets(search_this, 256, stdin);
		for(int i = 0; i < 256; i ++) if (search_this[i] == '\n') search_this[i] = search_this[i + 1];
		printf("===============================================================\n");

		for (int i = 0; i < book_counter; i ++) {
			char *check;
			check = strstr(temp -> book_title, search_this);
			if (check != NULL) {
				printing_books(temp);
				printf("\n===============================================================\n");
				
				counter ++;
			}
			temp = temp -> next_book;
		}

		if (counter == 0) {
			printf("There's No Book With a keyword %s\n", search_this);
			printf("===============================================================\n");
		}
	}

	else if (strcmp(search_by, "author") == 0) {
	
		printf("\t Author's Name can be incomplete, just use keywords!\n");
		printf("Enter Author's Name: ");
		fgets(search_this, 256, stdin);
		for(int i = 0; i < 256; i ++) if (search_this[i] == '\n') search_this[i] = search_this[i + 1];
		printf("===============================================================\n");

		for (int i = 0; i < book_counter; i ++) {
			char *check_firstname, *check_lastname;
			check_firstname = strstr(temp -> book_author_firstname, search_this);
			check_lastname = strstr(temp -> book_author_lastname, search_this);
			if (check_firstname != NULL || check_lastname != NULL) {
				printing_books(temp);
				printf("===============================================================\n");
				
				counter ++;
			}
			temp = temp -> next_book;
		}

		if (counter == 0) {
			printf("There's No Book Written By %s\n", search_this);
			printf("===============================================================\n");
		}
	}
}