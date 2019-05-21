typedef struct borrowed_date_node {
	int year;
	int month;
	int day;
} Borrowed_Date;

typedef struct borrowed_book_list_node {
	char borrowed_book_id[256];
	struct borrowed_date_node *borrowed_date;
	struct borrowed_book_list_node *next_borrowed_book;
} Borrowed_book_list;


typedef struct borrower_info_node{
	struct borrower_node *borrower_info;
	struct borrowed_date_node *date_of_borrow;
} If_Borrowed;

typedef struct borrower_node {
	char brw_id[256];
	char brw_lastname[256];
	char brw_firstname[256];
	char brw_course[256];
	int borrowed_book_counter;
	struct borrowed_book_list_node *borrowed_books;
	struct borrower_node *next_borrower;
} Borrower;

typedef struct book_node {
	char book_id[256];
	char book_title[256];
	char book_author_lastname[256];
	char book_author_firstname[256];
	int book_category;
	int book_status;
	struct borrower_info_node *borrower;
	struct book_node *next_book;
} Book;