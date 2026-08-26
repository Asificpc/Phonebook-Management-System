#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#define MAX_CONTACTS 100

struct Contact
{
    char name[50];   // Name of the contact
    char mobile[12]; // Mobile number of the contact (11 digits + null terminator)
    char email[100]; // Email address of the contact
    int favorite;   // Flag to indicate if the contact is a favorite
};

struct Phonebook {
    struct Contact contacts[MAX_CONTACTS];
    int count;
};

struct Phonebook phonebook = {{}, 0};  // Main phonebook
struct Phonebook backup = {{}, 0};     // Backup for undo functionality


// Function prototypes
void menu();
void addContact();
void searchByName();
void searchByNumber();
void postCaseMenu();
void deleteRecord();
void listContactsAlphabetically();
void modifyContact();
void markAsFavorite();
void listFavorites();
void toLowerCase(const char *source, char *dest);
void saveBackup();
void undoLastAction();
void loadContacts();



int main()
{
    loadContacts(); // Load contacts from the file
    system("color 4f");
    menu();
    return 0;
}

void menu()
{
    system("cls");
    printf("\t\t********** PHONEBOOK MENU **********\n");
    printf("\n1. Add New\t2. List\t\t3. Search\n4. Modify\t5. Delete\t6. Mark as Favorites\n7.Favorites List\t\t8. Undo Last Action\n\t\t9. Exit\n");
    printf("\nSelect an option: ");
    printf("\n");

    switch (getch())
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
        int choice;
        printf("How do you want to search?\n1. By Name\n2. By Number\n");
        if (scanf("%d", &choice) != 1)
        { // Check if input is valid
            printf("Invalid input. Please enter a number.\n");
            break;
        }
        if (choice == 1)
        {
            searchByName();
        }
        else if (choice == 2)
        {
            searchByNumber();
        }
        else
        {
            printf("Invalid choice. Please select 1 or 2.\n");
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
        break;
    default:
        printf("\nInvalid option! Try again.\n");
        getch();
        menu();
    }
}



// Save the current state of the phonebook to the backup
void saveBackup() {
    backup = phonebook;
    printf("Backup saved!\n");
}

// Undo the last action by restoring the backup
void undoLastAction() {
    phonebook = backup;
     // Write the restored state to the file
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL) {
        printf("Error writing to file during undo!\n");
        return;
    }
    for (int i = 0; i < phonebook.count; i++) {
        fprintf(file, "Name: %s, Mobile: %s, Email: %s, Favorite: %d\n",
                phonebook.contacts[i].name, phonebook.contacts[i].mobile,
                phonebook.contacts[i].email, phonebook.contacts[i].favorite);
    }
    fclose(file);
    printf("Last action undone successfully!\n");
}


void toLowerCase(const char *source, char *dest) {
    int i = 0;
    while (source[i] != '\0') {
        dest[i] = tolower(source[i]); // Convert each character to lowercase
        i++;
    }
    dest[i] = '\0'; // Null-terminate the destination string
}

void postCaseMenu()
{
    int choice;
    printf("\nWhat do you want to do now?\n");
    printf("1. Go to Main Menu\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");

    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2))
    {
        fflush(stdin); // Clear invalid input
        printf("Invalid choice. Please enter 1 or 2: ");
    }

    if (choice == 1)
    {
        menu(); // Go back to the main menu
    }
    else
    {
        exit(0); // Exit the program
    }
}

void loadContacts() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("No existing contacts found. Starting fresh.\n");
        return;
    }
    while (fscanf(file, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite: %d\n",
                  phonebook.contacts[phonebook.count].name,
                  phonebook.contacts[phonebook.count].mobile,
                  phonebook.contacts[phonebook.count].email,
                  &phonebook.contacts[phonebook.count].favorite) == 4) {
        phonebook.count++;
    }
    fclose(file);
}


// Function to add a new contact
void addContact()
{
    saveBackup(); // Save the current state of the phonebook to the backup
    FILE *file = fopen("contacts.txt", "a"); // Open the file in append mode
    if (!file)
    {
        printf("Error opening file!\n"); // Error handling for file opening
        return;
    }

    struct Contact contact; // Create a struct variable to hold the contact details
    int valid = 0;          // Flag for validation

    // Input and validation for the name
    printf("Enter Name: ");
    fflush(stdin);
    fgets(contact.name, sizeof(contact.name), stdin);
    contact.name[strcspn(contact.name, "\n")] = '\0'; // Remove trailing newline

    phonebook.contacts[phonebook.count++] = contact; // Add to in-memory phonebook

    // Input and validation for the mobile number
    do
    {
        printf("Enter Mobile Number (11 digits): ");
        fflush(stdin);
        fgets(contact.mobile, sizeof(contact.mobile), stdin);
        contact.mobile[strcspn(contact.mobile, "\n")] = '\0'; // Remove trailing newline

        // Check if the mobile number has exactly 11 digits and is numeric
        if (strlen(contact.mobile) == 11 && strspn(contact.mobile, "0123456789") == 11)
        {
            valid = 1; // Mark as valid if the conditions are met
        }
        else
        {
            printf("Invalid mobile number. Please try again.\n");
        }
    } while (!valid); // Repeat until the input is valid

    valid = 0; // Reset the validation flag

    // Input and validation for the email address
    do
    {
        printf("Enter Email Address: ");
        fflush(stdin);
        fgets(contact.email, sizeof(contact.email), stdin);
        contact.email[strcspn(contact.email, "\n")] = '\0'; // Remove trailing newline

        // Check if the email ends with allowed domains
        if (strstr(contact.email, "@gmail.com") ||
            strstr(contact.email, "@yahoo.com") ||
            strstr(contact.email, "@outlook.com")|| strstr(contact.email, "@hotmail.com"))
        {
            valid = 1; // Mark as valid if the conditions are met
        }
        else
        {
            printf("Invalid email address. Must end with @gmail.com, @yahoo.com, or @outlook.com.\n");
        }
    } while (!valid); // Repeat until the input is valid

    contact.favorite = 0; // Set favorite field to 0 (not favorite) by default

    // Save the contact details to the file
    fprintf(file, "Name: %s, Mobile: %s, Email: %s, Favorite: %d\n", contact.name, contact.mobile, contact.email, contact.favorite);
    fclose(file); // Close the file

    printf("Contact saved successfully!\n");
}

// Function to search a contact by name
void searchByName()
{
    FILE *file = fopen("contacts.txt", "r"); // Open the file in read mode
    if (!file)
    {                                    // Check if the file could not be opened
        printf("Error opening file!\n"); // Error message if file opening fails
        return;
    }

    struct Contact contact; // Create a struct variable to hold the contact details
    char searchName[50];    // To store the name the user wants to search
    int found = 0;          // Flag to indicate if the contact was found

    // Prompt the user to enter the name to search
    printf("Enter Name to Search: ");
    fflush(stdin);                                // Clear input buffer
    fgets(searchName, sizeof(searchName), stdin); // Get the name from the user
    searchName[strcspn(searchName, "\n")] = '\0'; // Remove the trailing newline character

    // Loop to read each line from the file and search for the contact
    while (fscanf(file, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite: %d\n",
                  contact.name, contact.mobile, contact.email, &contact.favorite) != EOF)
    {
        if (strcmp(contact.name, searchName) == 0)
        { // Compare the input name with the contact's name
            printf("Found: Name: %s, Mobile: %s, Email: %s, Favorite:%d\n", contact.name, contact.mobile, contact.email, contact.favorite);
            found = 1; // Mark as found
            break;     // Exit the loop once the contact is found
        }
    }

    // If no contact is found, display a message
    if (!found)
    {
        printf("No contact found with the name: %s\n", searchName);
    }

    fclose(file); // Close the file
}

// Function to search a contact by mobile number
void searchByNumber()
{
    FILE *file = fopen("contacts.txt", "r"); // Open the file in read mode
    if (!file)
    {                                    // Check if the file could not be opened
        printf("Error opening file!\n"); // Error message if file opening fails
        return;
    }

    struct Contact contact; // Create a struct variable to hold the contact details
    char searchNumber[12];  // To store the mobile number the user wants to search
    int found = 0;          // Flag to indicate if the contact was found

    // Prompt the user to enter the mobile number to search
    printf("Enter Mobile Number to Search: ");
    fflush(stdin);                                    // Clear input buffer
    fgets(searchNumber, sizeof(searchNumber), stdin); // Get the mobile number from the user
    searchNumber[strcspn(searchNumber, "\n")] = '\0'; // Remove the trailing newline character

    // Loop to read each line from the file and search for the contact
    while (fscanf(file, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite: %d\n",
                  contact.name, contact.mobile, contact.email, &contact.favorite ) != EOF)
    {
        if (strcmp(contact.mobile, searchNumber) == 0)
        { // Compare the input number with the contact's mobile number
            printf("Found: Name: %s, Mobile: %s, Email: %s, Favorite:%d\n", contact.name, contact.mobile, contact.email, contact.favorite);
            found = 1; // Mark as found
            break;     // Exit the loop once the contact is found
        }
    }

    // If no contact is found, display a message
    if (!found)
    {
        printf("No contact found with the mobile number: %s\n", searchNumber);
    }

    fclose(file); // Close the file
}

// Function to delete a contact

void deleteRecord()
{
    saveBackup(); // Save the current state of the phonebook to the backup
    struct Contact contact[MAX_CONTACTS];
    int found = 0, count = 0;
    char searchItem[50];

    FILE *fileread = fopen("contacts.txt", "r");
    if (fileread == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fileread, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite:%d\n",
                  contact[count].name, contact[count].mobile, contact[count].email, &contact[count].favorite) == 4)
    {
        count++;
    }
    fclose(fileread);

    printf("Enter the name of the contact you want to delete: ");
    fflush(stdin);
    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    FILE *filewrite = fopen("contacts.txt", "w");
    if (filewrite == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(contact[i].name, searchItem) == 0 || strcmp(contact[i].mobile, searchItem) == 0)
        {
            printf("Contact '%s' deleted successfully!\n", contact[i].name);
            found = 1;
            continue;
        }
        fprintf(filewrite, "Name: %s, Mobile: %s, Email: %s, Favorite:%d\n",
                contact[i].name, contact[i].mobile, contact[i].email, contact[i].favorite);
    }

    fclose(filewrite);

    if (!found)
    {
        printf("No contact found with the name or mobile number: %s\n", searchItem);
    }
}

// Function to list contacts alphabetically

void listContactsAlphabetically()
{
    struct Contact contact[MAX_CONTACTS];
    int count = 0;

    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite:%d\n",
                  contact[count].name, contact[count].mobile, contact[count].email, &contact[count].favorite) == 4)
    {
        count++;
    }
    fclose(file);

    if (count == 0)
    {
        printf("No contacts found in the phonebook.\n");
        return;
    }

    struct Contact temp;
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (strcmp(contact[i].name, contact[j].name) > 0)
            {
                temp = contact[i];
                contact[i] = contact[j];
                contact[j] = temp;
            }
        }
    }

    printf("Contacts in the Phonebook:\n");
    printf("----------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("Name: %s, Mobile: %s, Email: %s, Favorite:%d\n", contact[i].name, contact[i].mobile, contact[i].email, contact[i].favorite);
    }
}

// Function to modify a contact

void modifyContact()
{
    saveBackup(); // Save the current state of the phonebook to the backup
    struct Contact contacts[MAX_CONTACTS];
    int count = 0, found = 0;
    char searchItem[50];

    FILE *fileread = fopen("contacts.txt", "r");
    if (fileread == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fileread, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite:%d\n",
                  contacts[count].name, contacts[count].mobile, contacts[count].email, &contacts[count].favorite) == 4)
    {
        count++;
    }

    fclose(fileread);

    printf("Enter name or the mobile number of the contact you want to modify: ");
    fflush(stdin);
    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].name, searchItem) == 0 || strcmp(contacts[i].mobile, searchItem) == 0)
        {
            found = 1;

            // Display the current contact details
            printf("\nContact Found:\n");
            printf("1. Name: %s\n", contacts[i].name);
            printf("2. Mobile: %s\n", contacts[i].mobile);
            printf("3. Email: %s\n", contacts[i].email);

            printf("\nEnter the field you want to modify (1-4):\n");
            printf("1. Name\n2. Mobile\n3. Email\n4.All\n");
            printf("Enter your choice: ");

            int choice;

            while (scanf("%d", &choice) != 1 || (choice < 1 || choice > 4))
            {
                fflush(stdin); // Clear invalid input
                printf("Invalid choice. Please enter a number between 1 and 4: ");
            }

            fflush(stdin); // Clear input buffer

            switch (choice)
            {
            case 1:
                printf("Enter the new name: ");
                fgets(contacts[i].name, sizeof(contacts[i].name), stdin);
                contacts[i].name[strcspn(contacts[i].name, "\n")] = '\0';
                break;

            case 2:
                printf("Enter the new mobile number: ");
                fgets(contacts[i].mobile, sizeof(contacts[i].mobile), stdin);
                contacts[i].mobile[strcspn(contacts[i].mobile, "\n")] = '\0';
                break;

            case 3:
                printf("Enter the new email: ");
                fgets(contacts[i].email, sizeof(contacts[i].email), stdin);
                contacts[i].email[strcspn(contacts[i].email, "\n")] = '\0';
                break;

            case 4:
                printf("Enter the new name: ");
                fgets(contacts[i].name, sizeof(contacts[i].name), stdin);
                contacts[i].name[strcspn(contacts[i].name, "\n")] = '\0';

                printf("Enter the new mobile number: ");
                fgets(contacts[i].mobile, sizeof(contacts[i].mobile), stdin);
                contacts[i].mobile[strcspn(contacts[i].mobile, "\n")] = '\0';

                printf("Enter the new email: ");
                fgets(contacts[i].email, sizeof(contacts[i].email), stdin);
                contacts[i].email[strcspn(contacts[i].email, "\n")] = '\0';
                break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
            }

            printf("Contact modified successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("No contact found with the name or mobile number: %s\n", searchItem);
    }

    //Save the modified contacts to the file
    FILE *filewrite = fopen("contacts.txt", "w");
    if (filewrite == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(filewrite, "Name: %s, Mobile: %s, Email: %s\n, Favorite:%d\n",
                contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].favorite);
    }

    fclose(filewrite);
}

// Function to mark a contact as favorite

void markAsFavorite()
{
    saveBackup(); // Save the current state of the phonebook to the backup  
    struct Contact contacts[MAX_CONTACTS];
    int count = 0, found = 0;
    char searchItem[50];

    FILE *fileread = fopen("contacts.txt", "r");
    if (fileread == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fileread, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite:%d\n",
                  contacts[count].name, contacts[count].mobile, contacts[count].email, &contacts[count].favorite) == 4)
    {
        count++;
    }

    fclose(fileread);

    printf("Enter name or the mobile number of the contact you want to mark as favorite: ");
    fflush(stdin);
    fgets(searchItem, sizeof(searchItem), stdin);
    searchItem[strcspn(searchItem, "\n")] = '\0';

    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].name, searchItem) == 0 || strcmp(contacts[i].mobile, searchItem) == 0)
        {
            found = 1;
            contacts[i].favorite = 1;
            printf("Contact '%s' marked as favorite successfully!\n", contacts[i].name);
            break;
        }
    }

    if (!found)
    {
        printf("No contact found with the name or mobile number: %s\n", searchItem);
    }

    // Save the modified contacts to the file
    FILE *filewrite = fopen("contacts.txt", "w");
    if (filewrite == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(filewrite, "Name: %s, Mobile: %s, Email: %s, Favorite:%d\n",
                contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].favorite);
    }

    fclose(filewrite);
}


// Function to list favorite contacts

void listFavorites() {
    struct Contact contacts[MAX_CONTACTS];
    int count = 0;

    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Read contacts from the file
    while (fscanf(file, "Name: %[^,], Mobile: %[^,], Email: %[^,], Favorite:%d\n",
                  contacts[count].name, contacts[count].mobile, contacts[count].email, &contacts[count].favorite) == 4) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No contacts found in the phonebook.\n");
        return;
    }

    printf("Favorite Contacts in the Phonebook:\n");
    printf("------------------------------------\n");
    int favoriteCount = 0;

    // Display favorite contacts
    for (int i = 0; i < count; i++) {
        if (contacts[i].favorite == 1) {
            printf("Name: %s, Mobile: %s, Email: %s\n",
                   contacts[i].name, contacts[i].mobile, contacts[i].email);
            favoriteCount++;
        }
    }

    if (favoriteCount == 0) {
        printf("No favorite contacts found.\n");
        return;
    }

    // Ask user if they want to unmark any favorite contact
    char choice[10];
    printf("\nDo you want to unmark any favorite contact? (yes/no): ");
    while (getchar() != '\n'); // Clear input buffer
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = '\0'; // Remove newline

    if (strcmp(choice, "yes") == 0) {
        char searchItem[50], searchItemLower[50];
        int found = 0;

        printf("Enter the name or mobile number of the contact you want to unmark as favorite: ");
        fgets(searchItem, sizeof(searchItem), stdin);
        searchItem[strcspn(searchItem, "\n")] = '\0';
        toLowerCase(searchItem, searchItemLower);

        // Update contacts
        for (int i = 0; i < count; i++) {
            char nameLower[50], mobileLower[20];
            toLowerCase(contacts[i].name, nameLower);
            toLowerCase(contacts[i].mobile, mobileLower);

            if (strstr(nameLower, searchItemLower) != NULL || strstr(mobileLower, searchItemLower) != NULL) {
                found = 1;
                contacts[i].favorite = 0;
                printf("Contact %s unmarked as favorite successfully!\n", contacts[i].name);
            }
        }

        if (!found) {
            printf("No contact found matching: %s\n", searchItem);
            return;
        }

        // Write updated contacts to the file
        FILE *filewrite = fopen("contacts.txt", "w");
        if (filewrite == NULL) {
            printf("Error opening file!\n");
            return;
        }

        for (int i = 0; i < count; i++) {
            fprintf(filewrite, "Name: %s, Mobile: %s, Email: %s, Favorite:%d\n",
                    contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].favorite);
        }
        fclose(filewrite);
        printf("Contacts file updated successfully.\n");
    }
}



