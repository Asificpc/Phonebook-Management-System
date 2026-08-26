#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

#define MAX_CONTACTS 100

struct Contact
{
    char name[50];
    char mobile[12];
    char email[100];
    int favorite;
};

struct Phonebook
{
    struct Contact contacts[MAX_CONTACTS];
    int count;
};

struct Phonebook phonebook = {{{0}}, 0};
struct Phonebook backup = {{{0}}, 0};

/* Function Prototypes */
void menu(void);
void addContact(void);
void searchByName(void);
void searchByNumber(void);
void postCaseMenu(void);
void deleteRecord(void);
void listContactsAlphabetically(void);
void modifyContact(void);
void markAsFavorite(void);
void listFavorites(void);
void toLowerCase(const char *source, char *dest);
void saveBackup(void);
void undoLastAction(void);
void loadContacts(void);
void saveContacts(void);
void clearInputBuffer(void);
int isValidMobile(const char *mobile);
int isValidEmail(const char *email);

/* Clear input buffer */
void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* Clear remaining input */
    }
}

/* Convert string to lowercase */
void toLowerCase(const char *source, char *dest)
{
    int i = 0;

    while (source[i] != '\0')
    {
        dest[i] = (char)tolower((unsigned char)source[i]);
        i++;
    }

    dest[i] = '\0';
}

/* Validate mobile number */
int isValidMobile(const char *mobile)
{
    int i;

    if (strlen(mobile) != 11)
        return 0;

    for (i = 0; i < 11; i++)
    {
        if (!isdigit((unsigned char)mobile[i]))
            return 0;
    }

    return 1;
}

/* Validate email */
int isValidEmail(const char *email)
{
    const char *at;
    const char *dot;

    at = strchr(email, '@');
    dot = strrchr(email, '.');

    if (at == NULL || dot == NULL)
        return 0;

    if (at == email)
        return 0;

    if (dot < at)
        return 0;

    if (*(dot + 1) == '\0')
        return 0;

    if (strstr(email, "@gmail.com") != NULL ||
        strstr(email, "@yahoo.com") != NULL ||
        strstr(email, "@outlook.com") != NULL ||
        strstr(email, "@hotmail.com") != NULL)
    {
        return 1;
    }

    return 0;
}

/* Save contacts to file */
void saveContacts(void)
{
    FILE *file;
    int i;

    file = fopen("contacts.txt", "w");

    if (file == NULL)
    {
        printf("Error opening contacts.txt for writing!\n");
        return;
    }

    for (i = 0; i < phonebook.count; i++)
    {
        fprintf(file,
                "Name: %s, Mobile: %s, Email: %s, Favorite: %d\n",
                phonebook.contacts[i].name,
                phonebook.contacts[i].mobile,
                phonebook.contacts[i].email,
                phonebook.contacts[i].favorite);
    }

    fclose(file);
}

/* Load contacts from file */
void loadContacts(void)
{
    FILE *file;
    struct Contact contact;

    file = fopen("contacts.txt", "r");

    if (file == NULL)
    {
        return;
    }

    phonebook.count = 0;

    while (phonebook.count < MAX_CONTACTS &&
           fscanf(file,
                  "Name: %49[^,], Mobile: %11[^,], Email: %99[^,], Favorite: %d\n",
                  contact.name,
                  contact.mobile,
                  contact.email,
                  &contact.favorite) == 4)
    {
        phonebook.contacts[phonebook.count] = contact;
        phonebook.count++;
    }

    fclose(file);
}

/* Save current state for undo */
void saveBackup(void)
{
    backup = phonebook;
}

/* Undo last action */
void undoLastAction(void)
{
    phonebook = backup;
    saveContacts();

    printf("\nLast action undone successfully!\n");
}

/* Main function */
int main(void)
{
    loadContacts();

    system("color 4F");

    menu();

    return 0;
}

/* Main menu */
void menu(void)
{
    char choice;

    system("cls");

    printf("\n\t\t********** PHONEBOOK MENU **********\n\n");

    printf("1. Add New\t\t2. List\n");
    printf("3. Search\t\t4. Modify\n");
    printf("5. Delete\t\t6. Mark as Favorite\n");
    printf("7. Favorites List\t8. Undo Last Action\n");
    printf("9. Exit\n");

    printf("\nSelect an option: ");

    choice = getch();

    switch (choice)
    {
    case '1':
        addContact();
        postCaseMenu();
        break;

    case '2':
        listContactsAlphabetically();
        postCaseMenu();
        break;

    case '3':
    {
        int searchChoice;

        printf("\nHow do you want to search?\n");
        printf("1. By Name\n");
        printf("2. By Number\n");
        printf("Enter your choice: ");

        if (scanf("%d", &searchChoice) != 1)
        {
            clearInputBuffer();
            printf("Invalid input!\n");
            postCaseMenu();
            break;
        }

        clearInputBuffer();

        if (searchChoice == 1)
        {
            searchByName();
        }
        else if (searchChoice == 2)
        {
            searchByNumber();
        }
        else
        {
            printf("Invalid choice!\n");
        }

        postCaseMenu();
        break;
    }

    case '4':
        modifyContact();
        postCaseMenu();
        break;

    case '5':
        deleteRecord();
        postCaseMenu();
        break;

    case '6':
        markAsFavorite();
        postCaseMenu();
        break;

    case '7':
        listFavorites();
        postCaseMenu();
        break;

    case '8':
        undoLastAction();
        postCaseMenu();
        break;

    case '9':
        exit(0);

    default:
        printf("\nInvalid option! Try again.\n");
        getch();
        menu();
    }
}

/* Add a new contact */
void addContact(void)
{
    struct Contact contact;
    int i;

    if (phonebook.count >= MAX_CONTACTS)
    {
        printf("\nPhonebook is full! Maximum %d contacts allowed.\n",
               MAX_CONTACTS);
        return;
    }

    saveBackup();

    printf("\nEnter Name: ");

    fgets(contact.name, sizeof(contact.name), stdin);
    contact.name[strcspn(contact.name, "\n")] = '\0';

    if (strlen(contact.name) == 0)
    {
        printf("Name cannot be empty!\n");
        return;
    }

    /* Mobile number */
    do
    {
        printf("Enter Mobile Number (11 digits): ");

        fgets(contact.mobile, sizeof(contact.mobile), stdin);
        contact.mobile[strcspn(contact.mobile, "\n")] = '\0';

        if (!isValidMobile(contact.mobile))
        {
            printf("Invalid mobile number. Please enter exactly 11 digits.\n");
        }

    } while (!isValidMobile(contact.mobile));

    /* Check duplicate mobile */
    for (i = 0; i < phonebook.count; i++)
    {
        if (strcmp(phonebook.contacts[i].mobile, contact.mobile) == 0)
        {
            printf("This mobile number already exists!\n");
            return;
        }
    }

    /* Email */
    do
    {
        printf("Enter Email Address: ");

        fgets(contact.email, sizeof(contact.email), stdin);
        contact.email[strcspn(contact.email, "\n")] = '\0';

        if (!isValidEmail(contact.email))
        {
            printf("Invalid email address!\n");
            printf("Allowed domains: gmail.com, yahoo.com, outlook.com, hotmail.com\n");
        }

    } while (!isValidEmail(contact.email));

    contact.favorite = 0;

    phonebook.contacts[phonebook.count] = contact;
    phonebook.count++;

    saveContacts();

    printf("\nContact saved successfully!\n");
}

/* Search contact by name */
void searchByName(void)
{
    char searchName[50];
    char searchLower[50];
    char nameLower[50];
    int found = 0;
    int i;

    printf("\nEnter Name to Search: ");

    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    toLowerCase(searchName, searchLower);

    for (i = 0; i < phonebook.count; i++)
    {
        toLowerCase(phonebook.contacts[i].name, nameLower);

        if (strcmp(nameLower, searchLower) == 0)
        {
            printf("\nContact Found!\n");
            printf("----------------------------\n");
            printf("Name: %s\n", phonebook.contacts[i].name);
            printf("Mobile: %s\n", phonebook.contacts[i].mobile);
            printf("Email: %s\n", phonebook.contacts[i].email);
            printf("Favorite: %s\n",
                   phonebook.contacts[i].favorite ? "Yes" : "No");

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo contact found with the name: %s\n", searchName);
    }
}

/* Search contact by mobile number */
void searchByNumber(void)
{
    char searchNumber[20];
    int found = 0;
    int i;

    printf("\nEnter Mobile Number to Search: ");

    fgets(searchNumber, sizeof(searchNumber), stdin);
    searchNumber[strcspn(searchNumber, "\n")] = '\0';

    for (i = 0; i < phonebook.count; i++)
    {
        if (strcmp(phonebook.contacts[i].mobile, searchNumber) == 0)
        {
            printf("\nContact Found!\n");
            printf("----------------------------\n");
            printf("Name: %s\n", phonebook.contacts[i].name);
            printf("Mobile: %s\n", phonebook.contacts[i].mobile);
            printf("Email: %s\n", phonebook.contacts[i].email);
            printf("Favorite: %s\n",
                   phonebook.contacts[i].favorite ? "Yes" : "No");

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo contact found with the mobile number: %s\n",
               searchNumber);
    }
}

/* Delete contact */
void deleteRecord(void)
{
    char searchItem[100];
    char searchLower[100];
    char nameLower[50];
    int found = 0;
    int i;
    int j;

    printf("\nEnter the name or mobile number of the contact to delete: ");

    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    toLowerCase(searchItem, searchLower);

    for (i = 0; i < phonebook.count; i++)
    {
        toLowerCase(phonebook.contacts[i].name, nameLower);

        if (strcmp(nameLower, searchLower) == 0 ||
            strcmp(phonebook.contacts[i].mobile, searchItem) == 0)
        {
            saveBackup();

            printf("\nContact '%s' deleted successfully!\n",
                   phonebook.contacts[i].name);

            for (j = i; j < phonebook.count - 1; j++)
            {
                phonebook.contacts[j] = phonebook.contacts[j + 1];
            }

            phonebook.count--;

            saveContacts();

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo contact found with: %s\n", searchItem);
    }
}

/* List contacts alphabetically */
void listContactsAlphabetically(void)
{
    struct Contact sortedContacts[MAX_CONTACTS];
    struct Contact temp;
    int i;
    int j;

    if (phonebook.count == 0)
    {
        printf("\nNo contacts found in the phonebook.\n");
        return;
    }

    for (i = 0; i < phonebook.count; i++)
    {
        sortedContacts[i] = phonebook.contacts[i];
    }

    for (i = 0; i < phonebook.count - 1; i++)
    {
        for (j = i + 1; j < phonebook.count; j++)
        {
            if (strcmp(sortedContacts[i].name,
                       sortedContacts[j].name) > 0)
            {
                temp = sortedContacts[i];
                sortedContacts[i] = sortedContacts[j];
                sortedContacts[j] = temp;
            }
        }
    }

    printf("\n\t\tCONTACTS IN PHONEBOOK\n");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < phonebook.count; i++)
    {
        printf("Name: %s\n", sortedContacts[i].name);
        printf("Mobile: %s\n", sortedContacts[i].mobile);
        printf("Email: %s\n", sortedContacts[i].email);
        printf("Favorite: %s\n",
               sortedContacts[i].favorite ? "Yes" : "No");
        printf("------------------------------------------------------------\n");
    }
}

/* Modify contact */
void modifyContact(void)
{
    char searchItem[100];
    char searchLower[100];
    char nameLower[50];
    int found = 0;
    int choice;
    int i;

    printf("\nEnter name or mobile number of the contact to modify: ");

    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    toLowerCase(searchItem, searchLower);

    for (i = 0; i < phonebook.count; i++)
    {
        toLowerCase(phonebook.contacts[i].name, nameLower);

        if (strcmp(nameLower, searchLower) == 0 ||
            strcmp(phonebook.contacts[i].mobile, searchItem) == 0)
        {
            found = 1;

            printf("\nContact Found!\n");
            printf("----------------------------\n");
            printf("1. Name: %s\n", phonebook.contacts[i].name);
            printf("2. Mobile: %s\n", phonebook.contacts[i].mobile);
            printf("3. Email: %s\n", phonebook.contacts[i].email);

            printf("\nWhat do you want to modify?\n");
            printf("1. Name\n");
            printf("2. Mobile\n");
            printf("3. Email\n");
            printf("4. All\n");
            printf("Enter your choice: ");

            if (scanf("%d", &choice) != 1)
            {
                clearInputBuffer();
                printf("Invalid input!\n");
                return;
            }

            clearInputBuffer();

            saveBackup();

            switch (choice)
            {
            case 1:
                printf("Enter new name: ");

                fgets(phonebook.contacts[i].name,
                      sizeof(phonebook.contacts[i].name),
                      stdin);

                phonebook.contacts[i].name[
                    strcspn(phonebook.contacts[i].name, "\n")] = '\0';

                break;

            case 2:
                do
                {
                    printf("Enter new mobile number: ");

                    fgets(phonebook.contacts[i].mobile,
                          sizeof(phonebook.contacts[i].mobile),
                          stdin);

                    phonebook.contacts[i].mobile[
                        strcspn(phonebook.contacts[i].mobile, "\n")] = '\0';

                    if (!isValidMobile(phonebook.contacts[i].mobile))
                    {
                        printf("Invalid mobile number!\n");
                    }

                } while (!isValidMobile(phonebook.contacts[i].mobile));

                break;

            case 3:
                do
                {
                    printf("Enter new email: ");

                    fgets(phonebook.contacts[i].email,
                          sizeof(phonebook.contacts[i].email),
                          stdin);

                    phonebook.contacts[i].email[
                        strcspn(phonebook.contacts[i].email, "\n")] = '\0';

                    if (!isValidEmail(phonebook.contacts[i].email))
                    {
                        printf("Invalid email address!\n");
                    }

                } while (!isValidEmail(phonebook.contacts[i].email));

                break;

            case 4:
                printf("Enter new name: ");

                fgets(phonebook.contacts[i].name,
                      sizeof(phonebook.contacts[i].name),
                      stdin);

                phonebook.contacts[i].name[
                    strcspn(phonebook.contacts[i].name, "\n")] = '\0';

                do
                {
                    printf("Enter new mobile number: ");

                    fgets(phonebook.contacts[i].mobile,
                          sizeof(phonebook.contacts[i].mobile),
                          stdin);

                    phonebook.contacts[i].mobile[
                        strcspn(phonebook.contacts[i].mobile, "\n")] = '\0';

                    if (!isValidMobile(phonebook.contacts[i].mobile))
                    {
                        printf("Invalid mobile number!\n");
                    }

                } while (!isValidMobile(phonebook.contacts[i].mobile));

                do
                {
                    printf("Enter new email: ");

                    fgets(phonebook.contacts[i].email,
                          sizeof(phonebook.contacts[i].email),
                          stdin);

                    phonebook.contacts[i].email[
                        strcspn(phonebook.contacts[i].email, "\n")] = '\0';

                    if (!isValidEmail(phonebook.contacts[i].email))
                    {
                        printf("Invalid email address!\n");
                    }

                } while (!isValidEmail(phonebook.contacts[i].email));

                break;

            default:
                printf("Invalid choice!\n");
                return;
            }

            saveContacts();

            printf("\nContact modified successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("\nNo contact found with: %s\n", searchItem);
    }
}

/* Mark contact as favorite */
void markAsFavorite(void)
{
    char searchItem[100];
    char searchLower[100];
    char nameLower[50];
    int found = 0;
    int i;

    printf("\nEnter name or mobile number of the contact: ");

    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    toLowerCase(searchItem, searchLower);

    for (i = 0; i < phonebook.count; i++)
    {
        toLowerCase(phonebook.contacts[i].name, nameLower);

        if (strcmp(nameLower, searchLower) == 0 ||
            strcmp(phonebook.contacts[i].mobile, searchItem) == 0)
        {
            saveBackup();

            phonebook.contacts[i].favorite = 1;

            saveContacts();

            printf("\nContact '%s' marked as favorite successfully!\n",
                   phonebook.contacts[i].name);

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo contact found with: %s\n", searchItem);
    }
}

/* List favorite contacts */
void listFavorites(void)
{
    char choice[10];
    char searchItem[100];
    char searchLower[100];
    char nameLower[50];
    int favoriteCount = 0;
    int found = 0;
    int i;

    if (phonebook.count == 0)
    {
        printf("\nNo contacts found in the phonebook.\n");
        return;
    }

    printf("\n\t\tFAVORITE CONTACTS\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < phonebook.count; i++)
    {
        if (phonebook.contacts[i].favorite == 1)
        {
            printf("Name: %s\n", phonebook.contacts[i].name);
            printf("Mobile: %s\n", phonebook.contacts[i].mobile);
            printf("Email: %s\n", phonebook.contacts[i].email);
            printf("--------------------------------------------\n");

            favoriteCount++;
        }
    }

    if (favoriteCount == 0)
    {
        printf("No favorite contacts found.\n");
        return;
    }

    printf("\nDo you want to unmark any favorite contact? (yes/no): ");

    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = '\0';

    if (strcmp(choice, "yes") == 0 ||
        strcmp(choice, "Yes") == 0 ||
        strcmp(choice, "YES") == 0)
    {
        printf("\nEnter name or mobile number to unmark: ");

        fgets(searchItem, sizeof(searchItem), stdin);
        searchItem[strcspn(searchItem, "\n")] = '\0';

        toLowerCase(searchItem, searchLower);

        for (i = 0; i < phonebook.count; i++)
        {
            toLowerCase(phonebook.contacts[i].name, nameLower);

            if (phonebook.contacts[i].favorite == 1 &&
                (strcmp(nameLower, searchLower) == 0 ||
                 strcmp(phonebook.contacts[i].mobile, searchItem) == 0))
            {
                saveBackup();

                phonebook.contacts[i].favorite = 0;

                saveContacts();

                printf("\nContact '%s' unmarked as favorite successfully!\n",
                       phonebook.contacts[i].name);

                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("\nNo favorite contact found with: %s\n",
                   searchItem);
        }
    }
}

/* Post-action menu */
void postCaseMenu(void)
{
    int choice;

    printf("\nWhat do you want to do now?\n");
    printf("1. Go to Main Menu\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");

    while (scanf("%d", &choice) != 1 ||
           (choice != 1 && choice != 2))
    {
        clearInputBuffer();
        printf("Invalid choice. Please enter 1 or 2: ");
    }

    clearInputBuffer();

    if (choice == 1)
    {
        menu();
    }
    else
    {
        exit(0);
    }
}
