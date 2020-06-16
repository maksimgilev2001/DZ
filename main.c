#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <conio.h>
//#include <Windows.h>

struct LibStud{ // структура для хранения книги
    char serial[10]; // номер зачетной книжки
    char surname[25]; // фамилия студента
    char name[15]; // имя студента
    char fathername[25]; // отчество студента
    char fac[5]; // факультет студента
    char spec[30]; // специальность студента
};

struct LibBook{ // структура для хранения книги
    char isbn[20]; // номер ISBN
    char author[30]; // фио автора
    char title[25]; // название
    int all; // всего книг
    int avlbl; // доступно книг
};

struct LibUser{ // структура для хранения пользователей
    char login[15]; // номер зачетной книжки
    char password[15]; // фамилия студента
    int studstate; // доступ к БД со студентами
    int bookstate; // доступ к БД с книгами
};

struct LibStudBook{ // структура для хранения пользователей
    char isbn[20]; // номер ISBN
    char number[10]; // номер зачетной книжки
    int date_d; // дата сдачи
    int date_m; // месяц сдачи
    int date_y; // год сдачи
};

// загрузка из файла
struct LibBook* BookLoad(char* filename, int* num_books);
struct LibStud* StudLoad(char* filename, int* num_stud);
struct LibUser* UserLoad(char* filename, int* num_users);
struct LibStudBook* StudBookLoad(char* filename, int* num_stud_books);


// выгрузка в файл
void BookSave(char* filename, struct LibBook* Books, int num_books);
void StudSave(char* filename, struct LibStud* Students, int num_stud);
void StudBookSave(char* filename, struct LibStudBook* StudBooks, int num_stud_books);

// работа с книгами
void BookPrint(struct LibBook* Books, int num_books);
struct LibBook* BookAdd(struct LibBook* Books, int* num_books);
void BookDelete(struct LibBook* Books, int* num_books, struct LibStudBook* StudBooks, int* num_stud_books);
void BookGet(struct LibBook* Books, int num_books, struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books);
void BookTake(struct LibBook* Books, int num_books, struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books);
void BookInfo(struct LibStud* Students, int num_stud, struct LibBook* Books, int num_books, struct LibStudBook* StudBooks, int num_stud_books);

// работа со студентами
void StudPrint(struct LibStud* Students, int num_stud);
struct LibStud* StudAdd(struct LibStud* Students, int* num_stud);
void StudDelete(struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books);
void StudBackupCreate(struct LibStud* Students, int num_stud) ;
struct LibStud* StudBackupLoad(struct LibStud* students, int* num_stud);
void StudFindSurn(struct LibStud* Students, int num_stud);
void StudInfo(struct LibStud* Students, int num_stud, struct LibBook* Books, int num_books, struct LibStudBook* StudBooks, int num_studbooks);

// логирование
void lprintf(char* login, char* text);

int main(){
//    setlocale(LC_ALL, "ru");
//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);
    int num_stud = 0, num_books = 0, num_users = 0, num_stud_books = 0; // количество элементов в массиве
    char user_login[15], user_password[15]; // логин и пароль
    struct LibStud* students = StudLoad("students.csv", &num_stud); // массив студентов
    struct LibBook* books = BookLoad("books.csv", &num_books); // массив книг
    struct LibUser* users = UserLoad("users.csv", &num_users); // массив пользователей
    struct LibStudBook* studbooks = StudBookLoad("student_books.csv", &num_stud_books); // массив записей взятых книг
    int access = 0; //уровень доступа к БД (1 - students, 2 - books)
    int admin_access = 0; //уровень доступа к двум БД

    printf("Enter your username: ");
    scanf("%s", user_login); // считываем логин
    printf("Enter the password: ");
    scanf("%s", user_password); // считываем пароль
    printf("\n");
    for (int i = 0; i < num_users; i++) {
        if ((strcmp(users[i].login, user_login) == 0) && (strcmp(users[i].password, user_password) == 0) && (users[i].studstate == 1) && (users[i].bookstate == 0)) { // доступ только к БД со студентами
            access = 1;
            break;
        }
        else if ((strcmp(users[i].login, user_login) == 0) && (strcmp(users[i].password, user_password) == 0) && (users[i].studstate == 0) && (users[i].bookstate == 1)) {
            access = 2;
            break;
        }
        else if ((strcmp(users[i].login, user_login) == 0) && (strcmp(users[i].password, user_password) == 0) && (users[i].studstate == 1) && (users[i].bookstate == 1)) {
            admin_access = 1;
            break;
        }
    }
    if ((access == 0) && (admin_access == 0)) {
        printf("There is no such user\n");
        free(users);
        free(students);
        free(books);
        free(studbooks);
        return 0;
    }

    while (1) {
        int choise = -1;
        if ((admin_access == 1) && (access == 0)) {
            printf("Database access menu\n");
            printf("1. Database of students students\n");
            printf("2. Library database\n");
            printf("Enter the database > ");
            scanf("%d%*c", &choise); // считываем пункт меню

            while ((choise < 1) || (choise > 2)) { // проверка на верность введенного пункта меню
                printf("Invalid menu item, try again > ");
                scanf("%d%*c", &choise);
            }
            switch (choise) {
                case 1: access = 1;
                    lprintf(user_login, "gotostmenu");
                    break;
                case 2: access = 2;
                    lprintf(user_login, "gotobmenu");
                    break;
            }
            printf("\n");
        }
        if (access == 1) {
            int choise = -1;
            while (choise != 0) {
                printf("Menu of the list of students\n");
                printf("1. Output a list of students\n");
                printf("2. Add a new student\n");
                printf("3. To remove a student\n");
                printf("4. Database backup\n");
                printf("5. Restoring a database from a backup\n");
                printf("6. Search by student's last name\n");
                printf("7. Information about the student's books\n");
                printf("8. Save and exit\n");
                if (admin_access == 1) {
                    printf("0.Go to the DB selection menu\n");
                }
                printf("Enter a menu item > ");
                scanf("%d%*c", &choise); // считываем пункт меню

                while (((choise < 1) || (choise > 8)) && (choise != 0)) { // проверка на верность введенного пункта меню
                    printf("Invalid menu item, try again > ");
                    scanf("%d%*c", &choise);
                }

                switch (choise) {
                    case 1:
                        StudPrint(students, num_stud);
                        lprintf(user_login, "printtablest");
                        break;
                    case 2:
                        students = StudAdd(students, &num_stud);
                        lprintf(user_login, "addstudent");
                        break;
                    case 3:
                        StudDelete(students, &num_stud, studbooks, &num_stud_books);
                        lprintf(user_login, "delstudent");
                        break;
                    case 4:
                        StudBackupCreate(students, num_stud);
                        lprintf(user_login, "createbackupst");
                        break;
                    case 5:
                        StudBackupLoad(students, &num_stud);
                        lprintf(user_login, "loadbackupst");
                        break;
                    case 6:
                        StudFindSurn(students, num_stud);
                        lprintf(user_login, "findstudent");
                        break;
                    case 7:
                        StudInfo(students, num_stud, books, num_books, studbooks, num_stud_books);
                        lprintf(user_login, "studentinfo");
                        break;
                    case 8:
                        StudSave("students.csv", students, num_stud); // сохраняем студентов в файл
                        StudBookSave("student_books.csv", studbooks, num_stud_books);
                        lprintf(user_login, "savetablest");
                        free(users);
                        free(students);
                        free(books);
                        free(studbooks); // освобождаем динамическую память
                        return 0;
                }
                printf("\n");
            } // повторяем, пока не решим выйти
            lprintf(user_login, "backtomainmenu");
            access = 0;
        }
        if (access == 2) {
            int choise = -1;
            while (choise != 0) {
                printf("Welcome to the library!\n");
                printf("1. Output of the library\n");
                printf("2. Add a new book to the library\n");
                printf("3. Delete a book from the library\n");
                printf("4. Getting a book\n");
                printf("5. Return a book\n");
                printf("6. Information about the owners of the books\n");
                printf("7. Save and exit\n");
                if (admin_access == 1) {
                    printf("0. Go to the DB selection menu\n");
                }
                printf("Enter a menu item > ");
                scanf("%d%*c", &choise); // считываем пункт меню

                while (((choise < 1) || (choise > 7)) && (choise != 0)) { // проверка на верность введенного пункта меню
                    printf("Invalid menu item, try again: ");
                    scanf("%d%*c", &choise);
                }

                switch (choise) {
                    case 1:
                        BookPrint(books, num_books);
                        lprintf(user_login, "booker");
                        break;
                    case 2:
                        books = BookAdd(books, &num_books);
                        lprintf(user_login, "newbook");
                        break;
                    case 3:
                        BookDelete(books, &num_books, studbooks, &num_stud_books);
                        lprintf(user_login, "deletbooklib");
                        break;
                    case 4:
                        BookGet(books, num_books, students, &num_stud, studbooks, &num_stud_books);
                        lprintf(user_login, "getbook");
                        break;
                    case 5:
                        BookTake(books, num_books, students, &num_stud, studbooks, &num_stud_books);
                        lprintf(user_login, "returnbook");
                        break;
                    case 6:
                        BookInfo(students, num_stud, books, num_books, studbooks, num_stud_books);
                        lprintf(user_login, "bookinfo");
                        break;
                    case 7:
                        BookSave("books.csv", books, num_books); // сохраняем книги в файл
                        StudBookSave("student_books.csv", studbooks, num_stud_books);
                        lprintf(user_login, "savebook");
                        free(users);
                        free(students);
                        free(books);
                        free(studbooks); // освобождаем динамическую память
                        return 0;
                }
                printf("\n");
            }  // повторяем, пока не решим выйти
            lprintf(user_login, "backtomainmenu");
            access = 0;
        }
    }
}

//
// загрузка из файла
//
struct LibBook* BookLoad(char* filename, int* num_books) {
    FILE* file = fopen(filename, "r");

    if (file == 0) {
        printf("Failed to open the file '%s'\n", filename);
        *num_books = 0;
        return NULL;
    }

    struct LibBook* books = (struct LibBook*)malloc(sizeof(struct LibBook));
    *num_books = 0;

    while (feof(file) == 0) {
        fscanf(file, "%[^;]%*c", books[*num_books].isbn);
        fscanf(file, "%[^;]%*c", books[*num_books].author);
        fscanf(file, "%[^;]%*c;", books[*num_books].title);
        fscanf(file, "%d%*c", &books[*num_books].all);
        fscanf(file, "%d%*c", &books[*num_books].avlbl);
        *num_books = *num_books + 1;
        books = (struct LibBook*)realloc(books, (*num_books + 1) * sizeof(struct LibBook));
    }

    fclose(file);
    return books;
}

struct LibStud* StudLoad(char* filename, int* num_stud) {
    FILE* file = fopen(filename, "r");

    if (file == 0) { // если не удалось открыть файл
        printf("Failed to open the file '%s'\n", filename);
        *num_stud = 0;
        return NULL;
    }

    struct LibStud* students = (struct LibStud*)malloc(sizeof(struct LibStud)); // выделяем память под один элемент
    *num_stud = 0; // изначально элементов нет

    while (feof(file) == 0) {
        fscanf(file, "%[^;]%*c", students[*num_stud].serial);
        fscanf(file, "%[^;]%*c", students[*num_stud].surname);
        fscanf(file, "%[^;]%*c", students[*num_stud].name);
        fscanf(file, "%[^;]%*c", students[*num_stud].fathername);
        fscanf(file, "%[^;]%*c", students[*num_stud].fac);
        fscanf(file, "%[^\n]%*c", students[*num_stud].spec);
        *num_stud = *num_stud + 1; // увеличиваем размер
        students = (struct LibStud*)realloc(students, (*num_stud + 1) * sizeof(struct LibStud)); // перераспределяем память
    }

    fclose(file);
    return students;
}

struct LibUser* UserLoad(char* filename, int* num_users) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file '%s'\n", filename);
        *num_users = 0;
        return NULL;
    }

    struct LibUser* users = (struct LibUser*)malloc(sizeof(struct LibUser)); // выделяем память под один элемент
    *num_users = 0; // изначально элементов нет

    while (feof(file) == 0) {
        fscanf(file, "%[^;]%*c", users[*num_users].login);
        fscanf(file, "%[^;]%*c;", users[*num_users].password);
        fscanf(file, "%d%*c", &users[*num_users].studstate);
        fscanf(file, "%d%*c", &users[*num_users].bookstate);
        *num_users = *num_users + 1; // увеличиваем размер
        users = (struct LibUser*)realloc(users, (*num_users + 1) * sizeof(struct LibUser)); // перераспределяем память
    }

    fclose(file);
    return users;
}

struct LibStudBook* StudBookLoad(char* filename, int* num_stud_books) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file '%s'\n", filename);
        *num_stud_books = 0;
        return NULL;
    }

    struct LibStudBook* stbs = (struct LibStudBook*)malloc(sizeof(struct LibStudBook)); // выделяем память под один элемент
    *num_stud_books = 0; // изначально элементов нет

    while (feof(file) == 0) {
        fscanf(file, "%[^;]%*c", stbs[*num_stud_books].isbn);
        fscanf(file, "%[^;]%*c;", stbs[*num_stud_books].number);
        fscanf(file, "%d%*c", &stbs[*num_stud_books].date_d);
        fscanf(file, "%d%*c", &stbs[*num_stud_books].date_m);
        fscanf(file, "%d%*c", &stbs[*num_stud_books].date_y);
        *num_stud_books = *num_stud_books + 1; // увеличиваем размер
        stbs = (struct LibStudBook*)realloc(stbs, (*num_stud_books + 1) * sizeof(struct LibStudBook)); // перераспределяем память
    }

    fclose(file);
    return stbs;
}

//
// выгрузка в файл
//
void BookSave(char* filename, struct LibBook* Books, int num_books) {
    if (num_books == 0) {
        return ;
    }

    FILE* file = fopen(filename, "w");

    for (int i = 0; i < num_books; i++) {
        fprintf(file, "%s;", Books[i].isbn);
        fprintf(file, "%s;", Books[i].author);
        fprintf(file, "%s;", Books[i].title);
        fprintf(file, "%d;", Books[i].all);
        fprintf(file, "%d", Books[i].avlbl);

        if (i < num_books - 1)
            fprintf(file, "\n");
    }

    fclose(file);
}

void StudSave(char* filename, struct LibStud* Students, int num_stud) {
    if (num_stud == 0) {
        return ;
    }

    FILE* file = fopen(filename, "w");

    for (int i = 0; i < num_stud; i++) {
        fprintf(file, "%s;", Students[i].serial);
        fprintf(file, "%s;", Students[i].surname);
        fprintf(file, "%s;", Students[i].name);
        fprintf(file, "%s;", Students[i].fathername);
        fprintf(file, "%s;", Students[i].fac);
        fprintf(file, "%s", Students[i].spec);

        if (i != num_stud - 1)
            fprintf(file, "\n");
    }

    fclose(file);
}

void StudBookSave(char* filename, struct LibStudBook* StudBooks, int num_stud_books) {
    FILE* file = fopen(filename, "w");

    for (int i = 0; i < num_stud_books; i++) {
        fprintf(file, "%s;", StudBooks[i].isbn);
        fprintf(file, "%s;", StudBooks[i].number);
        fprintf(file, "%d.", StudBooks[i].date_d);
        fprintf(file, "%d.", StudBooks[i].date_m);
        fprintf(file, "%d", StudBooks[i].date_y);

        if (i != num_stud_books - 1)
            fprintf(file, "\n");
    }

    fclose(file);
}

//
// работа с книгами
//
void BookPrint(struct LibBook* Books, int num_books){
    if (num_books == 0){
        printf("no books have been added yet\n");
        return;
    }

    printf("ISBN   Authors  Book title  Total   Have\n");
    for (int i = 0; i < num_books; i++){
        printf("%s", Books[i].isbn);
        printf("%s", Books[i].author);
        printf("%s", Books[i].title);
        printf("%d", Books[i].all);
        printf("%d", Books[i].avlbl);
        printf("|\n");
    }

}

struct LibBook* BookAdd(struct LibBook* Books, int* num_books){
    printf("Enter information about the new book\n");
    struct LibBook abook;
    printf("Enter ISBN: ");
    scanf("%[^\n]%*c", abook.isbn);
    
    printf("Enter the author: ");
    scanf("%[^\n]%*c", abook.author);
    
    printf("Enter name book: ");
    scanf("%[^\n]%*c", abook.title);
    
    printf("Enter number of books: ");
    scanf("%d%*c", &abook.all);
    
    printf("Enter number of avaliable books:");
    scanf("%d%*c", &abook.avlbl);

    for (int i = 0; i < *num_books; i++){
        if (strcmp(Books[i].isbn, abook.isbn) == 0) {
            printf("ISBN '%s' already used\n", abook.isbn);
            return Books;
        }
    }
    (*num_books)++;
    
    Books = (struct LibBook*) realloc(Books, (*num_books) * sizeof(struct LibBook));
    Books[*num_books-1] = abook;
    

    return Books;
}

void BookDelete(struct LibBook* Books, int* num_books, struct LibStudBook* StudBooks, int* num_stud_books){
    if (num_books == 0) {
        printf("There is no books to delete\n");
        return;
    }

    char isbn[20];
    printf("Enter the ISBN: ");
    scanf("%s", isbn);

    for (int i = 0; i < *num_stud_books; i++) {
        if (strcmp(StudBooks[i].isbn, isbn) == 0) {
            printf("This book is on hands\n");
            return;
        }
    }

    int flag = 1; // флаг для поиска совпадений
    int j = 0; // новый индекс для элементов после удаления

    for (int i = 0; i < *num_books; i++){
        if (strcmp(Books[i].isbn, isbn) != 0) {
            Books[j++] = Books[i];
        } else {
            flag = 0;
        }
    }

    if (flag) {
        printf("book with ISBN '%s' not exist\n", isbn);
    }

    *num_books = j;
}

void BookGet(struct LibBook* Books, int num_books, struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books) {
    int exist = 0; //проверка на существование студента
    int dmin = 31, mmin = 12, ymin = 2080;
    int stbssum, min;
    char isbn[20], number[10];
    printf("Enter the ISBN of the book you want to get: ");
    scanf("%s", isbn);
    
    printf("Enter the student ID of the student who received the number: ");
    scanf("%s", number);

    for (int i = 0; i < num_books; i++) {
        if (strcmp(Students[i].serial, number) == 0) {
            exist++;
        }
    }
    if (exist == 0) {
        printf("There is no student with this number\n");
        return ;
    }

    for (int i = 0; i < num_books; i++) {
        if (strcmp(Books[i].isbn, isbn) == 0) { // если нашли книгу
            if (Books[i].avlbl == 0) {
                for (int j = 0; j < *num_stud_books; j++) { // поиск ближайшей даты сдачи книги
                    if (strcmp(StudBooks[j].isbn, isbn) == 0) {
                        stbssum = StudBooks[j].date_y * 10000 + StudBooks[j].date_m * 100 + StudBooks[j].date_d;
                        min = ymin * 10000 + mmin * 100 + dmin;
                        if (stbssum < min) {
                            dmin = StudBooks[j].date_d;
                            mmin = StudBooks[j].date_m;
                            ymin = StudBooks[j].date_y;
                        }
                    }
                }
                printf("This book is not available. The nearest book will be delivered %d.%d.%d\n", dmin, mmin, ymin);
            } else {
                Books[i].avlbl--;
                StudBooks = (struct LibStudBook*)realloc(StudBooks, (*num_stud_books + 1) * sizeof(struct LibStudBook));
                strcpy(StudBooks[*num_stud_books].isbn, isbn);
                strcpy(StudBooks[*num_stud_books].number, number);

                time_t TimeStartUnix = time(NULL);
                struct tm *Time = localtime(&TimeStartUnix);
                
                StudBooks[*num_stud_books].date_d = Time->tm_mday;
                StudBooks[*num_stud_books].date_m = Time->tm_mon + 1;
                StudBooks[*num_stud_books].date_y = Time->tm_year + 1900;
                
                (*num_stud_books)++;
                printf("The book was received successfully\n");
            }
            return ;
        }
    }

    printf("Books with ISBN '%s' do not exist\n", isbn);
}

void BookTake(struct LibBook* Books, int num_books, struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books) {
    int exist_stud = 0, exist_book = 0;
    char isbn[20], number[10];
    printf("Enter the ISBN of the book you want to return: ");
    scanf("%[^\n]%*c", isbn);
    printf("Enter the student ID of the student who received the number: ");
    scanf("%[^\n]%*c", number);

    for (int i = 0; i<num_books; i++) {
        if (strcmp(Students[i].serial, number) == 0) {
            exist_stud++;
        }
    }
    if (exist_stud == 0) {
        printf("There is no student with this number\n");
        return ;
    }
    for (int i = 0; i < *num_stud_books; i++) {
        if ((strcmp(StudBooks[i].number, number) == 0) && (strcmp(StudBooks[i].isbn, isbn) == 0)) {
            exist_book++;
        }
    }
    if (exist_book == 0) {
        printf("A student with this number does not have such a book available\n");
        return ;
    }


    for (int i = 0; i < num_books; i++) {
        if (strcmp(Books[i].isbn, isbn) == 0) {
            if (Books[i].all == Books[i].avlbl) {
                printf("It is not possible to return this book - all copies are availa\n");
            }
            else {
                Books[i].avlbl++;
                int j = 0;
                for (int i = 0; i < *num_stud_books; i++) {
                    if ((strcmp(StudBooks[i].isbn, isbn) != 0) || (strcmp(StudBooks[i].number, number) != 0)) {
                        StudBooks[j++] = StudBooks[i];
                    }
                }
                *num_stud_books = j;
                printf("The book was returned successfully\n");
            }
            return ;
        }
    }

    printf("There is no book with ISBN '%s'\n", isbn);
}

void BookInfo(struct LibStud* Students, int num_stud, struct LibBook* Books, int num_books, struct LibStudBook* StudBooks, int num_stud_books) {
    char isbn[20];
    int flag = 0; // флаг на нахождение книги

    scanf("%s", isbn);

    printf("Number   Surname   Name   Middle name   Faculty   Specialty   Return date  \n");
    for (int i = 0; i < num_stud; i++) {

        if (strcmp(Books[i].isbn,isbn) == 0) { // если нашли книгу

            for (int j = 0; j < num_stud_books; j++) {

                if (strcmp(StudBooks[j].isbn, isbn) == 0) { // если нашли студента с книгой

                    for (int r = 0; r < num_books; r++) {

                        if (strcmp(Students[r].serial, StudBooks[j].number) == 0) { // поиск студента в таблице
                            printf("%s ", Students[r].serial);
                            printf("%s ", Students[r].surname);
                            printf("%s ", Students[r].name);
                            printf("%s ", Students[r].fathername);
                            printf("%s ", Students[r].fac);
                            printf("%s ", Students[r].spec);
                            printf("%d.%d.%d ", StudBooks[j].date_d, StudBooks[j].date_m, StudBooks[j].date_y);
                            printf("\n");
                        }
                    }
                }
            }
            flag++;
        }
    }
    if (flag == 0) {
        printf("Books with ISBN '%s' do not exist\n", isbn);
    }
}

//
// работа со студентами
//
void StudPrint(struct LibStud* Students, int num_stud) {
    if (num_stud == 0) {
        return;
    }

    printf("Number   Surname   Name   Middle name   Faculty   Specialty\n");
    for (int i = 0; i < num_stud; i++){
        printf("%s ", Students[i].serial);
        printf("%s ", Students[i].surname);
        printf("%s ", Students[i].name);
        printf("%s ", Students[i].fathername);
        printf("%s ", Students[i].fac);
        printf("%s\n", Students[i].spec);
    }

}

struct LibStud* StudAdd(struct LibStud* Students, int* num_stud) {
    printf("Enter information about the new student\n");
    struct LibStud astudent;
    
    printf("Enter your credit card number: ");
    scanf("%[^\n]%*c", astudent.serial);
    
    printf("Enter the student's last name: ");
    scanf("%[^\n]%*c", astudent.surname);
    
    printf("Enter the student's name: ");
    scanf("%[^\n]%*c", astudent.name);
    
    printf("Enter the student's middle name: ");
    scanf("%[^\n]%*c", astudent.fathername);
    
    printf("Enter the student's Department: ");
    scanf("%[^\n]%*c", astudent.fac);
    
    printf("Enter the student's specialty: ");
    scanf("%[^\n]%*c", astudent.spec);


    for (int i = 0; i < *num_stud; i++) {
        if (!strcmp(Students[i].serial, astudent.serial)) {
            printf("This number is already used\n");
            return Students;
        }
    }
    
    (*num_stud)++;
    Students = (struct LibStud*)realloc(Students, (*num_stud) * sizeof(struct LibStud));
    Students[*num_stud-1] = astudent;

    return Students;
}

void StudDelete(struct LibStud* Students, int* num_stud, struct LibStudBook* StudBooks, int* num_stud_books){
    if (num_stud == 0) {
        printf("There is no students\n");
        return;
    }

    char number[10];
    printf("Enter number of the student: ");
    scanf("%s", number);

    for (int i = 0; i < *num_stud_books; i++) {
        if (strcmp(StudBooks[i].number, number) == 0) {
            printf("This student have a book\n");
            return;
        }
    }

    int flag = 0; // флаг для поиска совпадений
    int j = 0; // новый индекс для элементов после удаления

    for (int i = 0; i < *num_stud; i++) {
        if (strcmp(Students[i].serial, number) != 0) {
            Students[j++] = Students[i];
        }
        else {
            flag = 1;
        }
    }

    if (flag == 0) {
        printf("There is no student with the number '%s'\n", number);
    }

    *num_stud = j;
}

void StudFindSurn(struct LibStud* Students, int num_stud) {
    char surname[25];
    int flagst = 0; // флаг 0 если нет такого студента
    printf("Enter surname: ");
    scanf("%s", surname);

    printf("Number   Surname   Name   Middle name   Faculty   Specialty\n");

    for (int i = 0; i < num_stud; i++) {
        if (strcmp(Students[i].surname, surname) == 0) { // если нашли студента
            printf("%s ", Students[i].serial);
            printf("%s ", Students[i].surname);
            printf("%s ", Students[i].name);
            printf("%s ", Students[i].fathername);
            printf("%s ", Students[i].fac);
            printf("%s\n", Students[i].spec);
            flagst++;
        }
    }
    if (flagst == 0) {
        printf("There is no student with surname '%s'\n", surname);
    }
}

void StudBackupCreate(struct LibStud* Students, int num_stud) {
    if (num_stud == 0) {
        printf("There is no students\n");
        return ;
    }
    
    time_t TimeStartUnix = time(NULL);
    struct tm *Time = localtime(&TimeStartUnix);
    char backupname[100] = {0};
    sprintf(backupname, "students_%02d.%02d.%04d_%02d.%02d.%02d.csv", Time->tm_mday,Time->tm_mon+1, Time->tm_year + 1900, Time->tm_hour,Time->tm_min, Time->tm_sec);
    FILE* file = fopen(backupname, "w");
    
    if (file == NULL) { // если не удалось открыть файл
        printf("Failed to create a file '%s'\n", backupname);
        return ; // возвращаем пустой указатель
    }

    for (int i = 0; i < num_stud; i++) {
        fprintf(file, "%s;", Students[i].serial);
        fprintf(file, "%s;", Students[i].surname);
        fprintf(file, "%s;", Students[i].name);
        fprintf(file, "%s;", Students[i].fathername);
        fprintf(file, "%s;", Students[i].fac);
        fprintf(file, "%s", Students[i].spec);

        if (i != num_stud - 1)
            fprintf(file, "\n");
    }
    printf("Backup created\n");
    fclose(file);
}

struct LibStud* StudBackupLoad(struct LibStud* students, int* num_stud){
    char backupname[20] = {0};
    printf("Enter the backup file name: ");
    scanf("%s", backupname);

    FILE* file = fopen(backupname, "r");

    if (file == NULL) { // если не удалось открыть файл
        printf("Failed to open the file '%s'\n", backupname);
        *num_stud = 0;
        return NULL; // возвращаем пустой указатель
    }

    struct LibStud* Students = (struct LibStud*) malloc(sizeof(struct LibStud)); // выделяем память под один элемент
    *num_stud = 0; // изначально элементов нет

    while (feof(file) == 0) {
        fscanf(file, "%[^;]%*c", Students[*num_stud].serial);
        fscanf(file, "%[^;]%*c", Students[*num_stud].surname);
        fscanf(file, "%[^;]%*c;", Students[*num_stud].name);
        fscanf(file, "%[^;]%*c", Students[*num_stud].fathername);
        fscanf(file, "%[^;]%*c", Students[*num_stud].fac);
        fscanf(file, "%[^\n]%*c;", Students[*num_stud].spec);
        *num_stud = *num_stud + 1; // увеличиваем размер
        Students = (struct LibStud*)realloc(Students, (*num_stud + 1) * sizeof(struct LibStud)); // перераспределяем память
    }

    fclose(file);
    printf("file open\n");
    return Students; // возвращаем считанный массив
}

void StudInfo(struct LibStud* Students, int num_stud, struct LibBook* Books, int num_books, struct LibStudBook* StudBooks, int num_studbooks) {
    char number[10];
    int flag = 0;
    printf("Enter number of the student: ");
    scanf("%s", number);

    printf("ISBN    Authors    Book title    Return date  \n");

    for (int i = 0; i < num_stud; i++) {
        if (strcmp(Students[i].serial, number) == 0) { // если нашли студента
            for (int j = 0; j < num_studbooks; j++) {
                if (strcmp(StudBooks[j].number, number) == 0) { // если нашли книгу у студента
                    for (int r = 0; r < num_books; r++) {
                        if (strcmp(Books[r].isbn, StudBooks[j].isbn) == 0) { // поиск книги в библиотеке
                            printf("%s ", Books[r].isbn);
                            printf("%s ", Books[r].author);
                            printf("%s ", Books[r].title);
                            printf("%d.%d.%d\n", StudBooks[j].date_d, StudBooks[j].date_m, StudBooks[j].date_y);
                        }
                    }
                }
            }
            flag++;
        }
    }
    if (flag == 0) {
        printf("There is no such student with number '%s'\n", number);
    }
}

//
// логирование
//
void lprintf(char* login, char* text) {
    FILE* file = fopen("library.log", "a+");
    time_t s_time;
    struct tm* m_time;
    char timer[100];
    s_time = time(NULL);
    m_time = localtime(&s_time);
    strftime(timer, 128, "%d.%m.%y %H:%M:%S", m_time);
    fprintf(file, "%s;%s;%s", timer, login, text);
    fprintf(file, "\n");
    fclose(file);
}