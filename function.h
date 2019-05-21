#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "structs.h"
#include "smaller_functions.h"

void add_borrower(Borrower **borrower_ptr, int *borrower_counter);
void borrow_book(Book **book_ptr,int book_counter, Borrower **borrower_ptr, int *borrower_counter, Book **new_book, int add_book);

void menu(int *menu_choice) {
	printf("\t      Main Menu\n");
	printf("\t [1] Add Book\n");
	printf("\t [2] Search Book\n");
	printf("\t [3] View Books\n");
	printf("\t [4] Delete Book\n");
	printf("\t [5] Add Borrower\n");
	printf("\t [6] Delete Borrower\n");
	printf("\t [7] Borrow Book\n");
	printf("\t [8] Return Book\n");
	printf("\t [9] View Borrowed Books\n");
	printf("\t [0] Exit Program\n");
	printf("Enter command: ");
	scanf("%i", menu_choice);
}


/***** START OF ADD_BOOK FUNCTION *****/

void add_book(Book **book_ptr, int *book_counter, Borrower **borrower_ptr, int *borrower_counter){
	
	/***** GATHERING CONTENTS FOR THE NEW BOOK *****/
	Book *new_book;
	new_book = (Book *) malloc(sizeof(Book));
	
	printf("\t      Book's Information\n");
	getchar();
	
	int if_unique = 0;
	while (if_unique == 0) {
		printf("Book ID: ");	
		fgets(new_book -> book_id, 256, stdin);
		for(int i = 0; i < 256; i++) if (new_book -> book_id[i] == '\n') new_book -> book_id[i] = new_book -> book_id[i + 1];
		
		if_unique = id_search("book_id", new_book -> book_id , *book_ptr, *book_counter,  *borrower_ptr,  *borrower_counter);
		if (if_unique == 0) printf("There's a Book with exact same ID!\n");
	}

	printf("Book Title: ");
	fgets(new_book -> book_title, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_book -> book_title[i] == '\n') new_book -> book_title[i] = new_book -> book_title[i + 1];

	printf("\t      Book Author's Information\n");
	printf("Author's Last Name: ");
	fgets(new_book -> book_author_lastname, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_book -> book_author_lastname[i] == '\n') new_book -> book_author_lastname[i] = new_book -> book_author_lastname[i + 1];
	printf("Author's First Name: ");
	fgets(new_book -> book_author_firstname, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_book -> book_author_firstname[i] == '\n') new_book -> book_author_firstname[i] = new_book -> book_author_firstname[i + 1];

	printf("\t      Category\n");
	for(int i = 0; i < 5; i ++)	printf("\t [%i] %s\n", i+1, categories[i]);
	printf("Enter Category: ");
	scanf("%i", &new_book -> book_category);

	printf("\t      Availability\n");
	for(int i = 0; i < 2; i ++) printf("\t [%i] %s\n", i+1, status[i]);
	printf("Enter Status: ");
	scanf("%i", &new_book -> book_status);
	
	if (new_book -> book_status == 1) new_book -> borrower = NULL;

	/***** IF BOOK IS BORROWED, GATHER THE INFO FOR THE BORROWER *****/
	else if (new_book -> book_status == 2) {
		borrow_book (book_ptr, (*book_counter), borrower_ptr, borrower_counter, &new_book, 1);
		
		Borrowed_book_list *new_borrowed = (Borrowed_book_list *) malloc(sizeof(Borrowed_book_list));
		
		strcpy(new_borrowed -> borrowed_book_id, new_book -> book_id);
		new_borrowed -> next_borrowed_book = (*borrower_ptr) -> borrowed_books;
		printf("Book Added!\n");

	}
	/*****     *****/
	new_book -> next_book = NULL;

	if ((*book_counter) == 0) (*book_ptr) = new_book;

	/***** END OF GATHERING CONTENTS FOR NEW BOOK *****/

	(*book_counter) ++;
	

	
	/***** INSERT SORTING ALGORITHM HERE *****/
	if ((*book_counter) >= 2)sorting_algorithm("title", book_ptr,&new_book, *book_counter, NULL,0);
	/***** END OF SORTING ALGORITHM *****/

}

/***** END OF ADD BOOK FUNCTION *****/


/***** START OF SEARCH_BOOK FUNCTION *****/

void search_book(Book *book_ptr, int book_counter) {
	
	if (book_counter == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Book!\n");
		printf("===============================================================\n");

	}

	/***** CHOICE ON HOW TO SEARCH *****/

	else {
		int choice;
		printf("\t      Search Book\n");
		printf("\t [1] Search By Author\n");
		printf("\t [2] Search By Title\n");
		printf("Enter Choice: ");
		scanf("%i", &choice);
	
	/*****  END OF HOW TO SEARCH *****/

		if (choice == 1) {
			search_algorithm("author", book_ptr, book_counter);

		}
		
		else if (choice == 2) {
			search_algorithm("title", book_ptr, book_counter);
			
		}
	}
}

/***** END OF SEARCH_BOOK FUNCTION *****/


/***** START OF VIEW_BOOK FUNCTION *****/

void view_book(Book *book_ptr, int book_counter) {

	if (book_counter == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Book!\n");
		printf("===============================================================\n");

	}	


	else {
		int choice, book = 0;

		printf("\t      View Book\n");

		for (int i = 0; i < 5; i ++) printf("\t [%i] %s\n", i+1, categories[i]);
		printf("Enter Book Category: ");
		scanf("%i", &choice);

		Book *temp = book_ptr;
		
		printf("===============================================================\n");


		/***** PRINTING THE INFOS OF THE BOOK, IF THERE IS. *****/
		for (int i = 0; i < book_counter; i ++) {
			if (temp -> book_category == choice) {
				printing_books(temp);
				book ++;
				printf("===============================================================\n");
			}
			temp = temp -> next_book;
		}
		if (book == 0) {
			printf("\t There's no Book in the %s Section.\n", categories[choice - 1]);
			printf("===============================================================\n");
		}
	}
	/*****   *****/
}

/***** END OF VIEW_BOOK FUNCTION *****/


/***** START OF DELETE_BOOK FUNCTION *****/

void delete_book(Book **book_ptr, int *book_counter) {
	if ((*book_counter) == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Book!\n");
		printf("===============================================================\n");
	}

	else {
		char title_to_delete[256];
		printf("\t      Delete Book\n");
		printf("Book Title you want to delete: ");
		getchar();
		int book_indexes[(*book_counter)], book_indexed = 0;
		fgets(title_to_delete, 256, stdin);
		for(int i = 0; i < 256; i ++) if (title_to_delete[i] == '\n') title_to_delete[i] = title_to_delete[i + 1];
		Book *temp = (*book_ptr);
		for (int i = 0 ; i < (*book_counter); i ++) {
			char *check;
			check = strstr(temp -> book_title, title_to_delete);
			if (check != NULL) {
				printf("\t [%i] ID: %s, Title: %s, Author: %s %s\n", book_indexed + 1, temp -> book_id, temp -> book_title, temp -> book_author_firstname, temp -> book_author_lastname);
				book_indexes[book_indexed] = i;
				book_indexed ++;
			}
			temp = temp -> next_book;

		}
		if (book_indexed != 0){
			printf("Enter Index No. of Book to Delete: ");
			int index_to_delete;
			scanf("%i", &index_to_delete);
			
			printf("\t      Are you sure you will delete that book?\n");
			printf("\t [1] Yes\n\t [2] No\n");
			int checker;
			printf("Enter Command: ");
			scanf("%i", &checker);

			printf("Your choice\n");
			printf("Deleting...\n");

			while (1) {
				if (book_indexes[index_to_delete - 1] == 0) {
					
					if ( (*book_ptr) -> book_status == 2) {
						printf("Oof! You cannot delete this!\nIt's still borrowed!\n");
						break;
					}

					else (*book_ptr) = (*book_ptr) -> next_book;
				}

				else if (book_indexes[index_to_delete - 1] == 1) {

					if ( (*book_ptr) -> next_book -> book_status == 2) {
						printf("Oof! You cannot delete this!\nIt's still borrowed!\n");
						break;
					}

					else (*book_ptr) -> next_book = (*book_ptr) -> next_book -> next_book;
				}

				else {
					Book *delete_this_book = (*book_ptr);
					
					for(int i = 0; i < book_indexes[index_to_delete - 1] -1 ; i ++) {
						delete_this_book = delete_this_book -> next_book;
					}

					if ( delete_this_book -> next_book -> book_status == 2) {
						printf("Oof! You cannot delete this!\nIt's still borrowed!\n");
						break;
					}

					else {
						delete_this_book -> next_book = delete_this_book -> next_book -> next_book;	
					}

				}
					printf("Deleted!\n");
					(*book_counter) --;
					break;
			}
		}
		else {
			printf("There's No Book with a keyword %s.\n", title_to_delete);
		}
	}
}

/***** END OF DELETE_BOOK FUNCTION *****/


/***** START OF ADD_BORROWER FUNCTION *****/

void add_borrower(Borrower **borrower_ptr, int *borrower_counter) {
	Borrower *new_borrower = (Borrower *) malloc(sizeof(Borrower));

	printf("\t      Add Borrower\n");
	
	getchar();
	
	int if_unique = 0;
	while (if_unique == 0) {
		printf("Borrower's ID: ");	
		fgets(new_borrower -> brw_id, 256, stdin);
		for(int i = 0; i < 256; i++) if (new_borrower -> brw_id[i] == '\n') new_borrower -> brw_id[i] = new_borrower -> brw_id[i + 1];
		
		if_unique = id_search("string_to_copy", new_borrower -> brw_id , NULL, 0,  *borrower_ptr,  *borrower_counter);
		if (if_unique == 0) printf("There's a Book with exact same ID!\n");
	}

	printf("Borrower's Last Name: ");
	fgets(new_borrower -> brw_lastname, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_borrower -> brw_lastname[i] == '\n') new_borrower -> brw_lastname[i] = new_borrower -> brw_lastname[i + 1];
	
	printf("Borrower's Frst Name: ");
	fgets(new_borrower -> brw_firstname, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_borrower -> brw_firstname[i] == '\n') new_borrower -> brw_firstname[i] = new_borrower -> brw_firstname[i + 1];
	
	printf("Borrower's Course: ");
	fgets(new_borrower -> brw_course, 256, stdin);
	for(int i = 0; i < 256; i ++) if (new_borrower -> brw_course[i] == '\n') new_borrower -> brw_course[i] = new_borrower -> brw_course[i + 1];

	new_borrower -> borrowed_books = NULL;
	new_borrower -> borrowed_book_counter = 0;
	new_borrower -> next_borrower = NULL;

	new_borrower -> next_borrower = (*borrower_ptr);
	(*borrower_ptr) = new_borrower;
	(*borrower_counter) ++;
	printf("Borrower Added!\n");
}

/***** END OF ADD_BORROWER FUNCTION *****/


/***** START OF DELETE_BORROWER FUNCTION *****/

void delete_borrower(Borrower **borrower_ptr, int *borrower_counter) {
	Borrower *temp = (*borrower_ptr);
	if (temp != NULL) {
		printf("\t      Delete Borrower\n");
		for (int i = 0; i < (*borrower_counter) ; i ++) {
			printf("\t [%i] ID: %s, Name: %s %s, Course: %s\n", i+1, temp -> brw_id, temp -> brw_firstname, temp -> brw_lastname, temp -> brw_course);
		}	

		printf("Enter The Index of the Borrower you want to Delete: ");
		int borrower_to_delete;
		scanf("%i", &borrower_to_delete);

		printf("\t You really wanna delete that?\n");
		printf("\t [1] Yes\n\t [2] No\n");
		printf("Enter Command: ");
		int checker;
		scanf("%i", &checker);

		if (checker == 1) {
			Borrower *delete_next = (*borrower_ptr);
			printf("\t Your Choice.\n");
			if (borrower_to_delete == 1 && (*borrower_counter) == 1) {
				if ((*borrower_ptr) -> borrowed_book_counter == 0) {
					(*borrower_ptr) = NULL;
					printf("Borrower Deleted!\n");
					(*borrower_counter) --;
				}
				else {
					printf("Oof! Borrower still has unreturned Books!\n");
				}
			}
			else {
				for (int i = 0; i < borrower_to_delete - 1; i ++) {
					delete_next = delete_next -> next_borrower;
				}
				if (delete_next -> next_borrower -> borrowed_book_counter != 0) {
					printf("Oof! Borrower still has unreturned Books!\n");
				}
				else {
					delete_next -> next_borrower = delete_next -> next_borrower -> next_borrower;
					printf("Borrower Deleted!\n");
					(*borrower_counter) --;
				}
			}
		}
		else {
			printf("Okay!\n");
		}
	}
	else {
		printf("===============================================================\n");
		printf("\t There's No Available Borrower!\n");
		printf("===============================================================\n");
	}
}

/***** END OF DELETE_BORROWER FUNCTION *****/


/***** START OF BORROW_BOOK FUNCTION *****/

void borrow_book(Book **book_ptr,int book_counter, Borrower **borrower_ptr, int *borrower_counter, Book **new_book, int add_book) {
	
	int if_it_is, for_sure, reg;
	If_Borrowed *borrower_info_saved_on_book = (If_Borrowed*) malloc(sizeof(If_Borrowed));
	Borrowed_Date *date_of_borrow;
	Borrowed_book_list *borrowed_book_list = (Borrowed_book_list*) malloc(sizeof(Borrowed_book_list)), *add_borrowed_book_list_here;

	if (add_book == 0 && book_counter == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Book!\n");
		printf("===============================================================\n");
	}


	else {

		printf("\t      Borrow Book\n");
		printf("The Borrower is:\n");
		printf("\t [1] Registered\n");
		printf("\t [2] Not- Registered\n");
		printf("Enter Command: ");
		scanf("%i", &reg);
		

		if (reg == 1) {
			
			if ((*borrower_counter) == 0) {
				printf("There's No Registered Borrower!\n");
			}
			

			else {
				char id[256];
				getchar();
				printf("Enter Borrower's ID: ");
				fgets(id,256,stdin);
				id[strlen(id) - 1] = id[strlen(id)];
				
				int asd = 0;
				Borrower *borrower = (*borrower_ptr);
				for (int i = 0; i < (*borrower_counter) ; i ++) {
					if (strcmp (borrower -> brw_id, id) == 0) {
						asd ++;
						break;
					}
					borrower = borrower -> next_borrower;
				}
				if (asd == 0) {
					printf("There's no Registered Borrower with ID %s!\n", id);
				}
				else {
					printf("Is %s %s the Borrower?\n", borrower -> brw_firstname, borrower -> brw_lastname);
					printf("[1] Yes\n[2] No\n");
					printf("Enter Command:");
					scanf("%i", &for_sure);
				
					if (for_sure == 2) {
						printf("OOF!\nThen you got the wrong id!\n");
					}
				
					else if (for_sure == 1) {
						printf("Okay then!\nProcessing!\n");
				
						if (borrower -> borrowed_book_counter == 5) {
							printf("OOF! %s %s has exceeded the maximum limit. You can borrow books again if you return the books you recently borrowed!\n", borrower -> brw_firstname, borrower -> brw_lastname);
						}
				
						else if (borrower -> borrowed_book_counter == 0) {
										
							if (add_book == 1) {
								
				
								date_of_borrow = put_date();
											
									
								borrower_info_saved_on_book -> borrower_info = borrower;
								borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;
				
									
								(*new_book) -> borrower = borrower_info_saved_on_book;
				
									
								strcpy(borrowed_book_list -> borrowed_book_id, (*new_book) -> book_id);
								borrowed_book_list -> borrowed_date = date_of_borrow;
								borrowed_book_list -> next_borrowed_book = NULL;
													
				
								borrower -> borrowed_books = borrowed_book_list;
								borrower -> borrowed_book_counter = 1;
				
				
								printf("Book Borrowed!\n");
				
							}
				
							else if (add_book == 0) {
								getchar();
								printf("Enter Book ID :");
								char book_id[256];
								fgets(book_id, 256, stdin);
								book_id[strlen(book_id) - 1] = book_id[strlen(book_id)];
				
								Book *book_to_borrow = (*book_ptr);
				
								for (int i = 0; i < book_counter; i ++) {
									if (strcmp(book_to_borrow -> book_id, book_id) == 0) {
										printf("\tThis is the info of the book you want to borrow:\n");
										printing_books(book_to_borrow);
										break;
									}
				
									book_to_borrow = book_to_borrow -> next_book;
								}
				
								printf("[1] Yes\n[2] No\n");
								printf("Enter Command: ");
								scanf("%i", &if_it_is);
				
								if (if_it_is == 2) {
									printf("OOF! Then you got the wrong book id!\n");
									printf("Next!!!\n");
								}
				
								else if (if_it_is == 1) {
				
									printf("Okay!\nProcessing!\n");
									
									if (book_to_borrow -> book_status == 2) {
										printf("THE BOOK IS ALREADY BORROWED!\n");
										
										printf("\t     Current Borrower:\n");
										printf("Name: %s %s\n", book_to_borrow -> borrower -> borrower_info -> brw_firstname, book_to_borrow -> borrower -> borrower_info -> brw_lastname);
										printf("Course: %s\n", book_to_borrow -> borrower -> borrower_info -> brw_course);
										printf("Date of Borrow: %s %i, %i", months[book_to_borrow -> borrower -> date_of_borrow -> month], book_to_borrow -> borrower -> date_of_borrow -> day, book_to_borrow -> borrower -> date_of_borrow -> year);

									}
									else {
										date_of_borrow = put_date();
					
					
										borrower_info_saved_on_book -> borrower_info = borrower;
										borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;
					
					
										strcpy(borrowed_book_list -> borrowed_book_id, book_to_borrow -> book_id);
										borrowed_book_list -> borrowed_date = date_of_borrow;
										borrowed_book_list -> next_borrowed_book = NULL;
					
										book_to_borrow -> borrower = borrower_info_saved_on_book;
										book_to_borrow -> book_status = 2;
					
					
										borrower -> borrowed_book_counter = 1;
										borrower -> borrowed_books = borrowed_book_list;
					
					
										printf("Book Borrowed!\n");
									}
								}
				
					
							}
						}
				
				
						else if (borrower -> borrowed_book_counter >= 1 && borrower -> borrowed_book_counter <= 4) {
										
				
							if (add_book == 1) {
								
				
								date_of_borrow = put_date();
										
									
								borrower_info_saved_on_book -> borrower_info = borrower;
								borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;
				
									
								(*new_book) -> borrower = borrower_info_saved_on_book;
				
									
								strcpy(borrowed_book_list -> borrowed_book_id, (*new_book) -> book_id);
								borrowed_book_list -> borrowed_date = date_of_borrow;
								borrowed_book_list -> next_borrowed_book = NULL;
				
									
									borrower -> borrowed_books = borrowed_book_list;
									borrower -> borrowed_book_counter ++;
				
				
									printf("Book Borrowed!\n");
				
							}
				
							else if (add_book == 0) {
								getchar();
								printf("Enter Book ID :");
								char book_id[256];
								fgets(book_id, 256, stdin);
								book_id[strlen(book_id) - 1] = book_id[strlen(book_id)];
				
								Book *book_to_borrow = (*book_ptr);
				
								for (int i = 0; i < book_counter; i ++) {
									if (strcmp(book_to_borrow -> book_id, book_id) == 0) {
										printf("\tThis is the info of the book you want to borrow:\n");
										printing_books(book_to_borrow);
										break;
									}
				
									book_to_borrow = book_to_borrow -> next_book;
								}
				
								printf("[1] Yes\n[2] No\n");
								printf("Enter Command: ");
								scanf("%i", &if_it_is);
				
								if (if_it_is == 2) {
									printf("OOF! Then you got the wrong book id!\n");
									printf("Next!!!\n");
								}
				
								else if (if_it_is == 1) {
									printf("Okay!\nProcessing!\n");

									if (book_to_borrow -> book_status == 2) {

										printf("THE BOOK IS ALREADY BORROWED!\n");

										printf("\t     Current Borrower:\n");
										printf("Name: %s %s\n", book_to_borrow -> borrower -> borrower_info -> brw_firstname, book_to_borrow -> borrower -> borrower_info -> brw_lastname);
										printf("Course: %s\n", book_to_borrow -> borrower -> borrower_info -> brw_course);
										printf("Date of Borrow: %s %i, %i", months[book_to_borrow -> borrower -> date_of_borrow -> month], book_to_borrow -> borrower -> date_of_borrow -> day, book_to_borrow -> borrower -> date_of_borrow -> year);
									}
								
									else {

										date_of_borrow = put_date();
					
					
										borrower_info_saved_on_book -> borrower_info = borrower;
										borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;
					
					
										strcpy(borrowed_book_list -> borrowed_book_id, book_to_borrow -> book_id);
										borrowed_book_list -> borrowed_date = date_of_borrow;
										borrowed_book_list -> next_borrowed_book = NULL;
					

										book_to_borrow -> borrower = borrower_info_saved_on_book;
										book_to_borrow -> book_status = 2;
					
					
										borrower -> borrowed_book_counter ++;
										borrower -> borrowed_books = borrowed_book_list;
					
					
										printf("Book Borrowed!\n");
									}
								}
								
							}
						}
					}
				}
			}
		}
	

		else if (reg == 2) {

			add_borrower(borrower_ptr, borrower_counter);

			Borrower *borrower = (*borrower_ptr);


			if (borrower -> borrowed_book_counter == 5) {
					printf("OOF! %s %s has exceeded the maximum limit. You can borrow books again if you return the books you recently borrowed!\n", borrower -> brw_firstname, borrower -> brw_lastname);
				}

			else if (borrower -> borrowed_book_counter == 0) {
					
					if (add_book == 1) {
				

						date_of_borrow = put_date();
						
				
						borrower_info_saved_on_book -> borrower_info = borrower;
						borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;

				
						(*new_book) -> borrower = borrower_info_saved_on_book;

				
						strcpy(borrowed_book_list -> borrowed_book_id, (*new_book) -> book_id);
						borrowed_book_list -> borrowed_date = date_of_borrow;
						borrowed_book_list -> next_borrowed_book = NULL;

				
						borrower -> borrowed_books = borrowed_book_list;
						borrower -> borrowed_book_counter = 1;


						printf("Book Borrowed!\n");

					}

					else if (add_book == 0) {
						printf("Enter Book ID :");
						char book_id[256];
						fgets(book_id, 256, stdin);
						book_id[strlen(book_id) - 1] = book_id[strlen(book_id)];

						Book *book_to_borrow = (*book_ptr);

						for (int i = 0; i < book_counter; i ++) {
							if (strcmp(book_to_borrow -> book_id, book_id) == 0) {
								printf("\tThis is the info of the book you want to borrow:\n");
								printing_books(book_to_borrow);
								break;
							}

							book_to_borrow = book_to_borrow -> next_book;
						}

						printf("Is this the book you're looking for?\n");	
						printf("[1] Yes\n[2] No\n");
						printf("Enter Command: ");
						scanf("%i", &if_it_is);

						if (if_it_is == 2) {
							printf("OOF! Then you got the wrong book id!\n");
							printf("Next!!!\n");
						}

						else if (if_it_is == 1) {

							printf("Okay!\nProcessing!\n");


							date_of_borrow = put_date();


							borrower_info_saved_on_book -> borrower_info = borrower;
							borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;


							strcpy(borrowed_book_list -> borrowed_book_id, book_to_borrow -> book_id);
							borrowed_book_list -> borrowed_date = date_of_borrow;
							borrowed_book_list -> next_borrowed_book = NULL;

							book_to_borrow -> borrower = borrower_info_saved_on_book;
							book_to_borrow -> book_status = 2;


							borrower -> borrowed_book_counter = 1;
							borrower -> borrowed_books = borrowed_book_list;


							printf("Book Borrowed!\n");
						}

					}
			}


			else if (borrower -> borrowed_book_counter >= 1 && borrower -> borrowed_book_counter <= 4) {
				if (add_book == 1) {
				

						date_of_borrow = put_date();
						
				
						borrower_info_saved_on_book -> borrower_info = borrower;
						borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;

				
						(*new_book) -> borrower = borrower_info_saved_on_book;

				
						strcpy(borrowed_book_list -> borrowed_book_id, (*new_book) -> book_id);
						borrowed_book_list -> borrowed_date = date_of_borrow;
						borrowed_book_list -> next_borrowed_book = NULL;

				
						borrower -> borrowed_books = borrowed_book_list;
						borrower -> borrowed_book_counter ++;


						printf("Book Borrowed!\n");
				}

				else if (add_book == 0) {
						getchar();
						printf("Enter Book ID :");
						char book_id[256];
						fgets(book_id, 256, stdin);
						book_id[strlen(book_id) - 1] = book_id[strlen(book_id)];

						Book *book_to_borrow = (*book_ptr);

						for (int i = 0; i < book_counter; i ++) {
							if (strcmp(book_to_borrow -> book_id, book_id) == 0) {
								printf("\tThis is the info of the book you want to borrow:\n");
								printing_books(book_to_borrow);
								break;
							}

							book_to_borrow = book_to_borrow -> next_book;
						}

						printf("[1] Yes\n[2] No\n");
						printf("Enter Command: ");
						scanf("%i", &if_it_is);

						if (if_it_is == 2) {
							printf("OOF! Then you got the wrong book id!\n");
							printf("Next!!!\n");
						}

						else if (if_it_is == 1) {

							printf("Okay!\nProcessing!\n");


							date_of_borrow = put_date();


							borrower_info_saved_on_book -> borrower_info = borrower;
							borrower_info_saved_on_book -> date_of_borrow = date_of_borrow;


							strcpy(borrowed_book_list -> borrowed_book_id, book_to_borrow -> book_id);
							borrowed_book_list -> borrowed_date = date_of_borrow;
							borrowed_book_list -> next_borrowed_book = NULL;

							book_to_borrow -> borrower = borrower_info_saved_on_book;
							book_to_borrow -> book_status = 2;


							borrower -> borrowed_book_counter ++;
							borrower -> borrowed_books = borrowed_book_list;


							printf("Book Borrowed!\n");
						}
				}
			}
		}
	}
}


/***** END OF BORROW_BOOK FUNCTION *****/

/***** START OF RETURN_BOOK FUNCTION *****/

void return_book(Book **book_ptr,int book_counter, Borrower **borrower_ptr,int borrower_counter) {

	int penalty, paid = 0;


	if (borrower_counter == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Borrower!\n");
		printf("===============================================================\n");
	}

	else if (book_counter == 0) {
		printf("===============================================================\n");
		printf("\t There's No Available Book!\n");
		printf("===============================================================\n");
	}

	else {

		getchar();
		printf("\t      Return Book\n");
		
		printf("Enter Borrower ID: ");
		char id[256];
		fgets(id,256,stdin);
		id[strlen(id) - 1] = id[strlen(id)];

		int asd = 0;
		Borrower *borrower_that_return = (*borrower_ptr);
		for (int i = 0; i < borrower_counter; i ++){
			if (strcmp(id, borrower_that_return -> brw_id) == 0) {
				asd ++;
				break;
			}
			borrower_that_return = borrower_that_return -> next_borrower;
		}

		if (asd == 0) {
			printf("There's no Registered Borrower with ID %s.\n", id);
		}

		else {
			printf("Hello, %s %s!\n", borrower_that_return -> brw_firstname, borrower_that_return -> brw_lastname);
				printf("Enter Book ID that you will return: ");
				char book_id_return[256];
				fgets(book_id_return,256,stdin);
				book_id_return[strlen(book_id_return) - 1] = book_id_return[strlen(book_id_return)];
				int qwe = 0;
				Borrowed_book_list *book_return = borrower_that_return -> borrowed_books;
				for (int i = 0; i < borrower_that_return -> borrowed_book_counter; i ++) {
					if (strcmp(book_id_return,book_return -> borrowed_book_id) == 0) {
						qwe ++;
						break;
					}
					book_return = book_return -> next_borrowed_book;
				}
		
				if (qwe == 0) {
					printf("Oof! There's no Book with ID %s that %s %s borrowed!\n", id, borrower_that_return -> brw_firstname, borrower_that_return -> brw_lastname);
				}
		
				else {
					
					printf("\t   Date of Return\n");
					
					Borrowed_Date *date_of_return = put_date();
					Borrowed_Date *date_of_borrow = book_return -> borrowed_date;
					printf("Month : %i, Day : %i, Year : %i\n",date_of_borrow -> month, date_of_borrow -> day, date_of_borrow -> year);
					printf("Month : %i, Day : %i, Year : %i\n",date_of_return -> month, date_of_return -> day, date_of_return -> year);

		
					if (date_of_return -> month == date_of_borrow -> month && date_of_return -> year == date_of_borrow -> year) {
						
						if (date_of_borrow -> day > date_of_return -> day) {
							printf("Oof! You can't go to past to return a book you didn't borrow at that time!\n");
						}
						
						else {
							penalty = date_of_return -> day - date_of_borrow -> day;
		
							if (penalty > 7) {
								printf("You exceeded the maximum allowed time to borrow a book! You must pay %i.00 Php for the penalty.\n", penalty - 7);
								printf("====================\n");
								printf("Penalty Fee: %i.00 Php\n", penalty - 7 );
								printf("====================\n");
								printf("Pay The Penalty Fee in the Reception Area!\n");
							}
						
							else if (penalty <= 7) {
								printf("Okay! You didn't reached the maximum allowed time to borrow a book. You don't have to pay anything.\n");
								printf("Thank You!\n");
							}
							
							paid ++;
		
		
		
						}
					}
		
					else if (date_of_borrow -> year == date_of_return -> year && date_of_borrow -> month != date_of_return -> month) {
		
						if (date_of_borrow -> month > date_of_return -> month) {
							printf("Oof! You can't go to past to return a book you didn't borrow at that time!\n");
						}
		
						else if (date_of_borrow -> month < date_of_return -> month) {
							while (date_of_return -> month != date_of_borrow -> month) {
								if (date_of_borrow -> month % 2 != 0 || date_of_borrow -> month == 8) {
									date_of_return -> day = date_of_return -> day + 31;
								}
								else if (date_of_borrow -> month % 2 == 0 && date_of_borrow -> month != 2) {
									date_of_return -> day = date_of_return -> day + 30;
								}	
		
								else if (date_of_borrow -> month == 2) {
									if (date_of_borrow -> year % 4 != 0) {
										date_of_return -> day = date_of_return -> day + 28;
									}
		
									else if (date_of_borrow -> year % 4 == 0) {
										date_of_return -> day = date_of_return -> day + 29;
									}
								}
								date_of_return -> month --;
							}
		
							penalty = date_of_return -> day - date_of_borrow -> day;
		
							if (penalty > 7) {
								printf("You exceeded the maximum allowed time to borrow a book! You must pay %i.00 Php for the penalty.\n", penalty - 7);
								printf("====================\n");
								printf("Penalty Fee: %i.00 Php\n", penalty - 7 );
								printf("====================\n");
								printf("Pay The Penalty Fee in the Reception Area!\n");
							}
						
							else if (penalty <= 7) {
								printf("Okay! You didn't reached the maximum allowed time to borrow a book. You don't have to pay anything.\n");
								printf("Thank You!\n");
							}
		
							paid ++;
		
						}
					} 
		
					else if (date_of_return -> month != date_of_borrow -> month && date_of_borrow -> year != date_of_return -> year) {
						
						if (date_of_return -> year < date_of_borrow -> year) {
							printf("Oof! You can't go to past to return a book you didn't borrow at that time!\n");
						}
		
						else if (date_of_return -> year > date_of_borrow -> year) {
							
							while (date_of_borrow -> month != date_of_return -> month) {
								if (date_of_borrow -> month % 2 != 0 || date_of_borrow -> month == 8) {
									date_of_borrow -> day = date_of_borrow -> day + 31;
								}
								else if (date_of_borrow -> month % 2 == 0 && date_of_borrow -> month != 2) {
									date_of_borrow -> day = date_of_borrow -> day + 30;
								}	
		
								else if (date_of_borrow -> month == 2) {
									if (date_of_borrow -> year % 4 != 0) {
										date_of_borrow -> day = date_of_borrow -> day + 28;
									}
		
									else if (date_of_borrow -> year % 4 == 0) {
										date_of_borrow -> day = date_of_borrow -> day + 29;
									}
								}
								date_of_borrow -> month --;
							}
							
							if (date_of_return -> year % 4 == 0) {
								date_of_return -> day = date_of_return -> day + 366;
							}
							
							else if (date_of_return -> year % 4 != 0) {
								date_of_return -> day = date_of_return -> day + 365;
							}
		
							penalty = date_of_return -> day - date_of_borrow -> day;
		
							if (penalty > 7) {
								printf("You exceeded the maximum allowed time to borrow a book! You must pay %i.00 Php for the penalty.\n", penalty - 7);
								printf("====================\n");
								printf("Penalty Fee: %i.00 Php\n", penalty - 7 );
								printf("====================\n");
								printf("Pay The Penalty Fee in the Reception Area!\n");
							}
						
							else if (penalty <= 7) {
								printf("Okay! You didn't reached the maximum allowed time to borrow a book. You don't have to pay anything.\n");
								printf("Thank You!\n");
							}
							paid++;
						}
					}
				
					if (paid != 0) {
						printf("Okay!\n");
						printf("Processing!\n");
		
						Book *book_that_return = (*book_ptr);
						for (int i = 0; i < book_counter; i ++) {
							if (strcmp(book_that_return -> book_id, book_id_return) == 0) break;
							book_that_return = book_that_return -> next_book;
						} 	
		
						book_that_return -> book_status = 1;
						book_that_return -> borrower = NULL;
		
						struct borrowed_book_list_node *delete_that_book = borrower_that_return -> borrowed_books;
						if (borrower_that_return -> borrowed_book_counter > 1) {
							for (int i = 0; i < borrower_that_return -> borrowed_book_counter; i ++) {
								if (strcmp(delete_that_book -> next_borrowed_book -> borrowed_book_id,book_id_return)  == 0) break;
								delete_that_book = delete_that_book -> next_borrowed_book;
							}
							delete_that_book -> next_borrowed_book = delete_that_book -> next_borrowed_book -> next_borrowed_book;
						}

						else {
							borrower_that_return -> borrowed_books = NULL;
						}
						borrower_that_return -> borrowed_book_counter --;
					}	
		
				}
		}

	}


}
/***** END OF RETURN_BOOK FUNCTION *****/


/***** START OF VIEW_BORROWED_BOOKS FUNCTION *****/

void view_borrowed_book(Book *head_book_ptr, int book_counter, Borrower *head_borrower_ptr, int borrower_counter){
	Book *temp = head_book_ptr;
	int counter = 0;
	printf("===============================================================\n");
	for(int i = 0 ; i < book_counter ; i++){
		if (temp -> book_status == 2) {
			printf("\t      Book Info\n");
			printing_books(temp);
			printf("\n\t      Borrower Info:\n");
			printf("Borrower's ID: %s\n", temp -> borrower -> borrower_info -> brw_id);
			printf("Borrower's Name: %s %s\n",temp -> borrower -> borrower_info -> brw_firstname, temp -> borrower -> borrower_info -> brw_lastname);
			printf("Borrower's Course: %s\n", temp -> borrower -> borrower_info -> brw_course);
			printf("===============================================================\n");
			counter ++;
		}
		temp = temp -> next_book;
	}
	if (counter == 0) {
		printf("\t No Books are Borrowed at this moment!\n");
		printf("===============================================================\n");
	}

}

/***** END OF VIEW_BORROWED_BOOKS FUNCTION *****/


/***** START OF SAVE_ALL FUNCTION *****/

void save_all(Book **book_ptr, Borrower **borrower_ptr, int book_counter, int borrower_counter){


	FILE *fh;
	fh = fopen("books.txt","w");
	Book *temporary = (*book_ptr);
	int book_counter2 = book_counter;
	fprintf(fh,"%i\n", book_counter);
	for (int i = 0 ; i < book_counter2 ; i++) {
		fprintf(fh, "%s\n", temporary -> book_id);
		fprintf(fh, "%s\n", temporary -> book_title);
		fprintf(fh, "%s\n", temporary -> book_author_lastname);
		fprintf(fh, "%s\n", temporary -> book_author_firstname);
		fprintf(fh, "%i\n", temporary -> book_category);
		if  (temporary -> book_status == 2) {
			fprintf(fh, "%i\n", temporary -> book_status);
			char *borrower_id;
			strcpy(borrower_id,temporary -> borrower -> borrower_info -> brw_id);
			fprintf(fh, "%s\n", borrower_id);
			fprintf(fh, "%i,%i,%i\n", temporary -> borrower -> date_of_borrow -> month, temporary -> borrower -> date_of_borrow -> day, temporary -> borrower -> date_of_borrow -> year);
		}
		else {
			fprintf(fh, "%i\n", temporary -> book_status);
			
		}
	
		temporary = temporary -> next_book;
	}
	if (book_counter2 != 0) free((*book_ptr));
	fclose(fh);


	FILE *file_borrower;
	file_borrower = fopen("borrowers.txt","w");
	Borrower *temp = (*borrower_ptr);
	int borrower_counter2 = borrower_counter;
	fprintf(file_borrower, "%i\n", borrower_counter);

	for (int i = 0; i < borrower_counter2; i ++) {
		fprintf(file_borrower, "%s\n", temp -> brw_id);
		fprintf(file_borrower, "%s\n", temp -> brw_lastname);
		fprintf(file_borrower, "%s\n", temp -> brw_firstname);
		fprintf(file_borrower, "%s\n", temp -> brw_course);
		fprintf(file_borrower, "%i\n", temp -> borrowed_book_counter);

		temp = temp -> next_borrower;
		}

	fclose(file_borrower);



	if (borrower_counter2 != 0) free((*borrower_ptr));

}

/***** END OF SAVE_ALL FUNCTION *****/

/***** START OF read_file FUNCTION *****/

void read_file(struct book_node **head_book_ptr,struct borrower_node **head_borrower_ptr, int *book_counter,int *borrower_counter){


}


/***** ENCOUNTERED BUGS *****/

// SEARCH BOOK FUNCTION : CREATE AN ALGO OR A LINKED LIST THAT IS ALPHABETICALIZED BY AUTHOR NOT BY TITLE, THINK ABOUT HOW TO SOLVE THE DILEMMA IN SEARCHING BY AUTHOR
// ADD BOOK FUNCTION : NEED SORTING ALGO AND ADDING BORROWER IF NEEDED, OR CHECKING IF BORROWER IS REGISTERED
// DELETE BOOK FUNCTION : FEELS LIKE ITS WORKING AIGHT NO BUGS DETECTED
// DELETE BORROWER NOT YET FUNCTIONAL
// SORTING ALGO IS GOOD FOR THE FIRST 2 ELEMENTS