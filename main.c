#include "function.h"

void main() {
	int menu_choice;
	struct book_node *head_book_ptr = NULL;
	struct borrower_node *head_borrower_ptr = NULL;
	int book_counter = 0, borrower_counter = 0;
	read_file(&head_book_ptr,&head_borrower_ptr,&book_counter,&borrower_counter);
	
	do {
		menu(&menu_choice);
		system("clear");
		if (menu_choice == 1) add_book(&head_book_ptr, &book_counter, &head_borrower_ptr, &borrower_counter);
		else if (menu_choice == 2) search_book(head_book_ptr, book_counter);
		else if (menu_choice == 3) view_book(head_book_ptr, book_counter);
		else if (menu_choice == 4) delete_book(&head_book_ptr, &book_counter);
		else if (menu_choice == 5) add_borrower(&head_borrower_ptr, &borrower_counter);
		else if (menu_choice == 6) delete_borrower(&head_borrower_ptr, &borrower_counter);
		else if (menu_choice == 7) borrow_book(&head_book_ptr,book_counter,&head_borrower_ptr, &borrower_counter, NULL, 0);
		else if (menu_choice == 8) return_book(&head_book_ptr,book_counter,&head_borrower_ptr, borrower_counter);
		else if (menu_choice == 9) view_borrowed_book(head_book_ptr, book_counter, head_borrower_ptr, borrower_counter);
		else if (menu_choice == 0) save_all(&head_book_ptr, &head_borrower_ptr, book_counter, borrower_counter);
	}
	while (menu_choice != 0);
	printf("Goodbye!\n");
}