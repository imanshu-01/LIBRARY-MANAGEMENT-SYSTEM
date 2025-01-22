#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

const int MAX_STUDENTS = 20;
const int MAX_BOOKS = 15;
const int MAX_NAME_LENGTH = 50;

struct Student {
    int roll;
    char name[MAX_NAME_LENGTH];
    double balance;
};

struct Book {
    char title[MAX_NAME_LENGTH];
    char author[MAX_NAME_LENGTH];
    int isbn;
    bool available;
};

Student students[MAX_STUDENTS];
Book books[MAX_BOOKS];
int student_count = 0;
int book_count = 0;

void create_account();
void display(int roll);
void deposit_amount(int roll, double amount);
void issue_item(int roll);
void display_sorted();
int find_student(int roll);
int find_book(int isbn);
void add_book();
void edit_book();
void view_books();

int main() {
    for (int i = 0; i < MAX_BOOKS; i++) {
        strcpy(books[i].title, "Title");
        strcpy(books[i].author, "Author");
        books[i].isbn = i + 1000;
        books[i].available = true;
    }
    book_count = MAX_BOOKS;

    while (true) {
        int option;
        cout << "Enter 1 for Admin, 2 for Student, 0 to Exit: ";
        cin >> option;
        if (option == 0) break;

        string password;
        cout << "Enter password: ";
        cin >> password;

        if (password == "password") {
            if (option == 1) {
                int admin_option;
                cout << "1. Add Book\n2. Edit Book\n3. View Books\n4. Display Sorted\n5. Exit\n";
                cin >> admin_option;
                switch (admin_option) {
                    case 1: add_book(); break;
                    case 2: edit_book(); break;
                    case 3: view_books(); break;
                    case 4: display_sorted(); break;
                    case 5: break;
                }
            } else {
                int roll;
                cout << "Enter roll number: ";
                cin >> roll;
                int index = find_student(roll);
                if (index == -1) {
                    create_account();
                } else {
                    int student_option;
                    cout << "1. Display\n2. Deposit\n3. Issue Book\n";
                    cin >> student_option;
                    switch (student_option) {
                        case 1: display(roll); break;
                        case 2: {
                            double amount;
                            cout << "Enter amount to deposit: ";
                            cin >> amount;
                            deposit_amount(roll, amount);
                            break;
                        }
                        case 3: issue_item(roll); break;
                    }
                }
            }
        } else {
            cout << "Invalid password!\n";
        }
    }
    return 0;
}

void create_account() {
    if (student_count >= MAX_STUDENTS) {
        cout << "Student limit reached.\n";
        return;
    }
    int roll;
    cout << "Enter roll number: ";
    cin >> roll;
    if (find_student(roll) != -1) {
        cout << "Account already exists.\n";
        return;
    }
    students[student_count].roll = roll;
    cout << "Enter student name: ";
    cin.ignore();
    cin.getline(students[student_count].name, MAX_NAME_LENGTH);
    double initial_deposit;
    cout << "Enter initial deposit: ";
    cin >> initial_deposit;
    if (initial_deposit < 50) {
        cout << "Initial deposit must be at least $50.\n";
        return;
    }
    students[student_count].balance = initial_deposit - 50; // Deducting account opening fee
    student_count++;
}

void display(int roll) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Roll No: " << students[index].roll << "\nName: " << students[index].name
         << "\nBalance: $" << fixed << setprecision(2) << students[index].balance << endl;
}

void deposit_amount(int roll, double amount) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }
    students[index].balance += amount;
    cout << "New balance: $" << fixed << setprecision(2) << students[index].balance << endl;
}

void issue_item(int roll) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Available books:\n";
    for (int i = 0; i < book_count; i++) {
        if (books[i].available) {
            cout << i + 1 << ". " << books[i].title << " by " << books[i].author << " (ISBN: " << books[i].isbn << ")\n";
        }
    }
    int choice;
    cout << "Select book number to issue (0 to cancel): ";
    cin >> choice;
    if (choice == 0) return;
    if (books[choice - 1].available && students[index].balance >= 2) {
        books[choice - 1].available = false;
        students[index].balance -= 2;
        cout << "Book issued successfully. New balance: $" << fixed << setprecision(2) << students[index].balance << endl;
    } else {
        cout << "Cannot issue the book. Insufficient balance or book is unavailable.\n";
    }
}

void display_sorted() {
    for (int i = 0; i < student_count; i++) {
        for (int j = i + 1; j < student_count; j++) {
            if (students[i].roll > students[j].roll) {
                swap(students[i], students[j]);
            }
        }
    }
    for (int i = 0; i < student_count; i++) {
        cout << students[i].roll << " - " << students[i].name << " - Balance: $" << fixed << setprecision(2) << students[i].balance << endl;
    }
}

int find_student(int roll) {
    for (int i = 0; i < student_count; i++) {
        if (students[i].roll == roll) {
            return i;
        }
    }
    return -1;
}

int find_book(int isbn) {
    for (int i = 0; i < book_count; i++) {
        if (books[i].isbn == isbn) {
            return i;
        }
    }
    return -1;
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        cout << "Book limit reached.\n";
        return;
    }
    cout << "Enter book title: ";
    cin.ignore();
    cin.getline(books[book_count].title, MAX_NAME_LENGTH);
    cout << "Enter book author: ";
    cin.getline(books[book_count].author, MAX_NAME_LENGTH);
    cout << "Enter book ISBN: ";
    cin >> books[book_count].isbn;
    books[book_count].available = true;
    book_count++;
}

void edit_book() {
    int isbn;
    cout << "Enter ISBN of the book to edit: ";
    cin >> isbn;
    int index = find_book(isbn);
    if (index == -1) {
        cout << "Book not found.\n";
        return;
    }
    cout << "Current book title: " << books[index].title << endl;
    cout << "Enter new book title: ";
    cin.ignore();
    cin.getline(books[index].title, MAX_NAME_LENGTH);
    cout << "Current book author: " << books[index].author << endl;
    cout << "Enter new book author: ";
    cin.getline(books[index].author, MAX_NAME_LENGTH);
    cout << "Book details updated.\n";
}

void view_books() {
    for (int i = 0; i < book_count; i++) {
        cout << "Title: " << books[i].title << "\nAuthor: " << books[i].author
             << "\nISBN: " << books[i].isbn << "\nAvailable: " << (books[i].available ? "Yes" : "No") << "\n\n";
    }
}
