#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_BOOKS = 100;

class Library
{
    string ISBN, bookTitle, author;
    int numberOfCopies;

public:
    Library() : ISBN(""), bookTitle(""), author(""), numberOfCopies(0) {}

    void getData()
    {
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cin.ignore(); 
        cout << "Enter Book Title: ";
        getline(cin, bookTitle);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Number of Copies: ";
        cin >> numberOfCopies;
    }

    void displayData() const
    {
        cout << "ISBN: " << ISBN << "\nTitle: " << bookTitle << "\nAuthor: " << author << "\nCopies: " << numberOfCopies << endl;
    }

    string getISBN() const
    {
        return ISBN;
    }
    int getNumberOfCopies() const
    {
        return numberOfCopies;
    }
    void updateCopies(int change)
    {
        numberOfCopies += change;
    }

    void saveToFile(ofstream &outFile) const
    {
        outFile << ISBN << "\n"<< bookTitle << "\n"<< author << "\n"<< numberOfCopies << "\n";
    }

    void loadFromFile(ifstream &inFile)
    {
        getline(inFile, ISBN);
        getline(inFile, bookTitle);
        getline(inFile, author);
        inFile >> numberOfCopies;
        inFile.ignore(); 
    }


    static void addBook(Library books[], int &bookCount)
    {
        if (bookCount >= MAX_BOOKS)
        {
            cout << "Library is full! Cannot add more books." << endl;
            return;
        }

        Library newBook;
        newBook.getData();

       
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getISBN() == newBook.getISBN())
            {
                cout << "Book with this ISBN already exists!" << endl;
                return;
            }
        }

        books[bookCount] = newBook;
        bookCount++;
        saveAllToFile(books, bookCount);
        cout << "Book added successfully." << endl;
    }

    static void issueBook(Library books[], int bookCount, const string &isbn)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getISBN() == isbn)
            {
                if (books[i].getNumberOfCopies() > 0)
                {
                    books[i].updateCopies(-1);
                    saveAllToFile(books, bookCount);
                    cout << "Book issued successfully." << endl;
                }
                else
                {
                    cout << "No copies available!" << endl;
                }
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    static void returnBook(Library books[], int bookCount, const string &isbn)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getISBN() == isbn)
            {
                books[i].updateCopies(1);
                saveAllToFile(books, bookCount);
                cout << "Book returned successfully." << endl;
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    static void updateBookDetails(Library books[], int bookCount, const string &isbn)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getISBN() == isbn)
            {
                cout << "Updating book details for ISBN: " << isbn << endl;
                books[i].getData();
                saveAllToFile(books, bookCount);
                cout << "Book details updated successfully." << endl;
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    static void deleteBook(Library books[], int &bookCount, const string &isbn)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getISBN() == isbn)
            {
                if (books[i].getNumberOfCopies() == 0)
                {
                    for (int j = i; j < bookCount - 1; j++)
                    {
                        books[j] = books[j + 1];
                    }
                    bookCount--;
                    saveAllToFile(books, bookCount);
                    cout << "Book deleted successfully." << endl;
                }
                else
                {
                    cout << "Cannot delete a book that still has copies available!" << endl;
                }
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    static void displayAllBooks(const Library books[], int bookCount)
    {
        if (bookCount == 0)
        {
            cout << "No books in the library!" << endl;
            return;
        }

        for (int i = 0; i < bookCount; i++)
        {
            books[i].displayData();
            cout << "-----------------" << endl;
        }
    }

    static void loadAllFromFile(Library books[], int &bookCount)
    {
        ifstream inFile("library_file.txt");
        if (!inFile)
        {
            cout << "No catalog file found. Starting with an empty library." << endl;
            return;
        }

        bookCount = 0;
        while (bookCount < MAX_BOOKS && !inFile.eof())
        {
            books[bookCount].loadFromFile(inFile);
            if (!inFile.eof())
            {
                bookCount++;
            }
        }
        inFile.close();
    }

    static void saveAllToFile(const Library books[], int bookCount)
    {
        ofstream outFile("library_file.txt");
        if (!outFile)
        {
            cout << "Error opening file for writing!" << endl;
            return;
        }

        for (int i = 0; i < bookCount; i++)
        {
            books[i].saveToFile(outFile);
        }
        outFile.close();
    }
};

int main()
{
    Library books[MAX_BOOKS];
    int bookCount = 0;
    Library::loadAllFromFile(books, bookCount);

    int choice;
    string isbn;

    do
    {
        cout << "\nLibrary Management System\n";
        cout << "1. Add New Book\n";
        cout << "2. Issue Book\n";
        cout << "3. Return Book\n";
        cout << "4. Update Book Details\n";
        cout << "5. Delete Book\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            Library::addBook(books, bookCount);
            break;
        case 2:
            cout << "Enter ISBN to issue: ";
            cin >> isbn;
            Library::issueBook(books, bookCount, isbn);
            break;
        case 3:
            cout << "Enter ISBN to return: ";
            cin >> isbn;
            Library::returnBook(books, bookCount, isbn);
            break;
        case 4:
            cout << "Enter ISBN to update: ";
            cin >> isbn;
            Library::updateBookDetails(books, bookCount, isbn);
            break;
        case 5:
            cout << "Enter ISBN to delete: ";
            cin >> isbn;
            Library::deleteBook(books, bookCount, isbn);
            break;
        case 6:
            Library::displayAllBooks(books, bookCount);
            break;
        case 7:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
