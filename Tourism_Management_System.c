/* 
 * @file Tourism Management System.c
 * @brief A command-line based Tourism Management System for managing user accounts and tour bookings.
 *
 * This application enables users to create an account, log in, book a tour package, 
 * cancel bookings, and change their password. It provides a simple CLI interface
 * with menus for user interactions.
 *
 * Key Functionalities:
 *  - User Registration with duplicate-checking.
 *  - User Login with password verification.
 *  - Tour Booking based on predefined packages.
 *  - Booking Cancellation with refund calculations.
 *  - Password Change and Logout operations.
 *
 * Target Users:
 *  - Developers and students working on system-level projects.
 *
 * Code Style:
 *  - Follows general C programming and Doxygen documentation conventions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/** 
 * @enum status
 * @brief Represents the system mode (either in the main menu or after user login).
 */
enum status { menu, loggedIn };

/** Global variable to track current system state. */
enum status currentStatus = menu;

/** 
 * @struct user
 * @brief Represents a system user and their booking details.
 */
typedef struct user {
    char username[100];       ///< Unique username for the user.
    char password[100];       ///< Password for authentication.
    char place[100];          ///< Currently booked tour destination.
    float price;              ///< Price per ticket for the booked tour.
    int numberTicket;         ///< Number of tickets booked.
    struct user *next;        ///< Pointer to the next user in a linked list.
} user;

/* Function prototypes with Doxygen-style comments: */

/**
 * @brief Displays the list of available tour packages.
 *
 * This function clears the screen and prints the tour menu with pricing details.
 */
void showMenu(void);

/**
 * @brief Initializes the user list from a file.
 *
 * Reads the "users.txt" file and creates a linked list of users.
 * @param userptr Pointer to the current user linked list (can be NULL).
 * @return Pointer to the head of the initialized user list.
 */
user* initializeUser(user* userptr);

/**
 * @brief Adds a new user to the system.
 *
 * Prompts for username and password, checks for duplicates, and updates the user list.
 * @param userptr Pointer to the head of the current user list.
 * @return Pointer to the head of the updated user list.
 */
user* addUser(user* userptr);

/**
 * @brief Authenticates a user trying to log in.
 *
 * Validates user credentials and updates system state on successful login.
 * @param userptr Pointer to the user linked list.
 * @return Pointer to the user record on successful login or the list head otherwise.
 */
user* login(user* userptr);

/**
 * @brief Handles a tour booking for a logged-in user.
 *
 * Displays available packages, captures user selection, and stores booking details.
 * @param userptr Pointer to the user linked list.
 */
void booking(user* userptr);

/**
 * @brief Cancels a booked tour for the logged-in user.
 *
 * Searches for the user's booking and resets tour-related fields if found.
 * @param userptr Pointer to the user linked list.
 */
void cancellation(user* userptr);

/**
 * @brief Changes the password for the logged-in user.
 *
 * Prompts for the current password, validates it, then allows a new password to be set.
 * @param userptr Pointer to the user linked list.
 */
void changePassword(user* userptr);

/**
 * @brief Logs out the current user.
 *
 * Resets the current user and system state to the default menu state.
 */
void logout(void);

/**
 * @brief Checks and displays current booking details for the logged-in user.
 *
 * Calculates the total cost based on price and number of tickets.
 * @param userptr Pointer to the user linked list.
 */
void checkTicket(user* userptr);

/**
 * @brief Writes the current user list to the file.
 *
 * Updates the "users.txt" file with the latest user booking and login details.
 * @param userptr Pointer to the head of the user linked list.
 */
void filing(user* userptr);

/**
 * @brief Exits the application after displaying project developer details.
 *
 * Displays project and developer details and pauses before exit.
 */
void exitProgram(void);

/**
 * @brief Displays developer/project information.
 *
 * Prints project title and developer names before clearing the screen after a delay.
 */
void developers(void);

/**
 * @brief Introduces a delay in the program execution.
 *
 * Uses clock cycles to implement a sleep-like functionality.
 * @param t The delay time in seconds.
 */
void delay(float t);

/** 
 * @brief Global variable to store the currently logged-in username.
 */
char currentUser[100];

int main() {
    /* Change console color for visibility. */
    system("COLOR FC");
    
    developers();

    unsigned int choice1, choice2;
    
    user *userptr = NULL;
    /* Initialize user list from persistent storage. */
    userptr = initializeUser(userptr);
    
    /* Main loop for menu-driven interface. */
    while (1) {
        if (currentStatus == menu) {
            system("CLS");

            printf("\nWelcome to Muhammad*Muhammad*Muhammad Travels!\n");

            /* Display main menu options. */
            printf("\n1. Add User\n2. Login User\n3. Menu\n4. Exit\n");
            
            printf("\nEnter your selection: ");
            scanf("%u", &choice1);
            
            switch(choice1) {
                case 1:
                    /* Add a new user account. */
                    userptr = addUser(userptr);
                    break;
                case 2:
                    /* Attempt to log in the user. */
                    userptr = login(userptr);
                    break;
                case 3:
                    /* Show available tours. */
                    showMenu();
                    break;
                case 4:
                    /* Display exit information then terminate the program. */
                    exitProgram();
                    exit(0);
                    break;
                default:
                    /* Inform user of invalid entry. */
                    printf("\nInvalid input! Please select a number from the menu.\n");
            }
        }
        else if (currentStatus == loggedIn) {
            system("CLS");
            printf("\nWelcome %s!\n", userptr->username);
                
            /* Display logged in user menu options. */
            printf("\n1. Booking \n2. Check Total \n3. Cancel Booking \n4. Change Password \n5. Logout User \n6. Menu \n7. Exit \n");
            printf("\nEnter your choice: ");
            scanf("%u", &choice2);
            
            switch(choice2) {
                case 1:
                    /* Process a booking request. */
                    booking(userptr);
                    system("PAUSE");
                    system("CLS");
                    break;
                case 2:
                    /* Present the total booking cost. */
                    checkTicket(userptr);
                    system("PAUSE");
                    system("CLS");
                    break;
                case 3:
                    /* Process booking cancellation and notify user about refund. */
                    cancellation(userptr);
                    system("PAUSE");
                    system("CLS");
                    break;
                case 4:
                    /* Allow the user to change password after verifying current credentials. */
                    changePassword(userptr);
                    system("PAUSE");
                    system("CLS");
                    break;
                case 5:
                    /* Log the user out and revert to main menu state. */
                    logout();
                    system("PAUSE");
                    system("CLS");
                    break;
                case 6:
                    /* Display the list of tour packages again. */
                    showMenu();
                    system("CLS");
                    break;
                case 7:
                    /* End the program after a proper exit message. */
                    exitProgram();
                    exit(0);
                    break;
                default:
                    /* Invalid selection message for logged-in menu. */
                    printf("\nInvalid choice! Please try again.\n");
            }
        }
    }
    return 0;
}

user* initializeUser(user *userptr) {
    user *tempptr, *ptr, temp;
    FILE *fp;
    
    fp = fopen("users.txt", "r");
    if (fp == NULL)
        return NULL;
    
    /* If the file is empty, no initialization is needed. */
    if (fgetc(fp) == EOF)
        return NULL;
    
    rewind(fp);
    /* Read user information from file and build linked list of users */
    while (fscanf(fp, "%s %s %s %f %d", temp.username, temp.password, temp.place, &temp.price, &temp.numberTicket) != EOF) {
        ptr = (user*)malloc(sizeof(user));
        
        strcpy(ptr->username, temp.username);
        strcpy(ptr->password, temp.password);
        strcpy(ptr->place, temp.place);
        ptr->price = temp.price;
        ptr->numberTicket = temp.numberTicket;
        ptr->next = NULL;

        if (userptr == NULL)
            userptr = tempptr = ptr;
        else {
            userptr->next = ptr;
            userptr = ptr;
        }
    }
    fclose(fp);
    return tempptr;
}

void filing(user *userptr) {
    FILE *fp;
    fp = fopen("users.txt", "w");
    
    /* Write each user's data into the file to persist state */
    while (userptr != NULL) {
        fprintf(fp, "%s %s %s %f %d\n", 
                userptr->username, userptr->password, userptr->place, userptr->price, userptr->numberTicket);
        userptr = userptr->next;
    }
    fclose(fp);
}

void showMenu(void) {
    system("CLS");
    
    /* Display available tour packages and pricing details */
    printf("\nMENU\n\n");
    printf("1. Paris, France    - Rs 400000\n");
    printf("2. Tokyo, Japan     - Rs 600000\n");
    printf("3. Bangkok, Thailand- Rs 250000\n");
    printf("4. Abu Dhabi, UAE   - Rs 380000\n");
    printf("5. Miami, USA       - Rs 120000\n");
    printf("6. Rome, Italy      - Rs 100000\n");
    printf("7. Munich, Germany  - Rs 300000\n");
    printf("8. Madrid, Spain    - Rs 320000\n");
    printf("9. Istanbul, Turkey - Rs 450000\n");
    printf("10. Gilgit, Pakistan- Rs 75000\n");
    
    system("PAUSE");
}

void checkTicket(user *userptr) {
    /* Traverse the linked list to locate details for the current user */
    while (userptr != NULL) {
        if (!strcmp(userptr->username, currentUser))
            break;
        userptr = userptr->next;
    }
    
    /* If no booking exists, inform the user */
    if (!strcmp(userptr->place, "\0") || userptr->price == 0.0 || userptr->numberTicket == 0) {
        printf("\nNo ticket booked!\n");
        return;
    }
    
    float total = userptr->price * userptr->numberTicket;
    
    printf("\n%d ticket(s) booked for a total of Rs %.0f for destination %s.\n", 
           userptr->numberTicket, total, userptr->place);
}

user* addUser(user* userptr) {
    user *tempptr = userptr;
    user *newptr = (user*)malloc(sizeof(user));
    
    fflush(stdin);
    printf("\nEnter new username: ");
    scanf("%s", newptr->username);
    
    /* Check for existing username to prevent duplicates */
    while (userptr != NULL) {
        if (!strcmp(userptr->username, newptr->username)) {
            printf("\nError: Username already exists!\n");
            delay(2.0);
            return tempptr;
        }
        userptr = userptr->next;
    }
    
    userptr = tempptr;  // Reset pointer for further use.
    
    fflush(stdin);
    printf("\nEnter new password: ");
    scanf(" %[^\n]s", &newptr->password);
    
    printf("\nUser account created successfully!\n");
    system("PAUSE");
    
    newptr->next = NULL;
    strcpy(newptr->place, "N/A");   // No tour booked initially.
    newptr->price = 0.0;
    newptr->numberTicket = 0;

    if (userptr == NULL)
        userptr = tempptr = newptr;
    else {
        while (userptr->next != NULL)
            userptr = userptr->next;
        userptr->next = newptr;
    }
    filing(tempptr);
    
    return tempptr;
}

user* login(user* userptr) {
    char username[100];
    char password[100];
    
    fflush(stdin);
    printf("\nEnter Username: ");
    scanf(" %s", username);
    fflush(stdin);
    printf("\nEnter Password: ");
    scanf(" %[^\n]s", password);
    
    while (userptr != NULL) {
        /* Validate username and password successively */
        if ((!strcmp(userptr->username, username)) && (!strcmp(userptr->password, password))) {
            currentStatus = loggedIn;
            strcpy(currentUser, username);
            printf("\nLogin successful!\n");
            system("PAUSE");
            return userptr;
        } else if ((!strcmp(userptr->username, username)) && (strcmp(userptr->password, password))) {
            printf("\nWrong Password! Access denied.\n");
            return userptr;
        }
        userptr = userptr->next;
    }
    
    printf("\nUser not found! Please register first.\n");
    delay(2.0);
    return NULL;
}

void booking(user *userptr) {
    user *tempptr = userptr;
    char place[100];
    char placeList[10][100] = {"Paris, France", "Tokyo, Japan", "Bangkok, Thailand", "Abu Dhabi, UAE", 
                                "Miami, USA", "Rome, Italy", "Munich, Germany", "Madrid, Spain", 
                                "Istanbul, Turkey", "Gilgit, Pakistan"};
    
    /* Locate the logged-in user in the linked list */
    while (userptr != NULL) {
        if (!strcmp(userptr->username, currentUser))
            break;
        userptr = userptr->next;
    }
    
    if (userptr == NULL)
        return;
    
    /* Prevent multiple bookings for same user without cancellation */
    if (userptr->price != 0.0) {
        printf("\nYou already have an active booking. Please cancel your previous ticket before booking a new one!\n");
        return;
    }
    
    showMenu();
    
    /* Price list corresponding to tour packages */
    float priceList[] = {400000.0, 600000.0, 250000.0, 380000.0, 120000.0,
                         100000.0, 300000.0, 320000.0, 450000.0, 75000.0};
    
    fflush(stdin);
    printf("\nEnter the tour code number: ");
    scanf(" %[^\n]s", place);
    
    char choice;
    
    fflush(stdin);
    printf("\nConfirm booking?\n1. Yes\n2. No\n");
    printf("\nEnter your choice: ");
    scanf(" %c", &choice);
    
    float price = 0.0;
    
    if (choice != '1')
        return;
    
    /* Map the code number to the corresponding package from the lists */
    if (strcmp(place, "1") == 0) {
        price = priceList[0];
        strcpy(place, placeList[0]);
    } else if (strcmp(place, "2") == 0) {
        price = priceList[1];
        strcpy(place, placeList[1]);
    } else if (strcmp(place, "3") == 0) {
        price = priceList[2];
        strcpy(place, placeList[2]);
    } else if (strcmp(place, "4") == 0) {
        price = priceList[3];
        strcpy(place, placeList[3]);
    } else if (strcmp(place, "5") == 0) {
        price = priceList[4];
        strcpy(place, placeList[4]);
    } else if (strcmp(place, "6") == 0) {
        price = priceList[5];
        strcpy(place, placeList[5]);
    } else if (strcmp(place, "7") == 0) {
        price = priceList[6];
        strcpy(place, placeList[6]);
    } else if (strcmp(place, "8") == 0) {
        price = priceList[7];
        strcpy(place, placeList[7]);
    } else if (strcmp(place, "9") == 0) {
        price = priceList[8];
        strcpy(place, placeList[8]);
    } else if (strcmp(place, "10") == 0) {
        price = priceList[9];
        strcpy(place, placeList[9]);
    } else {
        printf("\nInvalid tour code number entered!\n");
        return;
    }
    
    printf("\nEnter the number of tickets for booking: ");
    scanf("%d", &userptr->numberTicket);
    
    /* If number of tickets is zero, abort booking */
    if (userptr->numberTicket == 0)
        return;
    
    strcpy(userptr->place, place);
    userptr->price = price;
    
    filing(tempptr);
    printf("\nBooking completed successfully!\n");
}

void cancellation(user *userptr) {
    user *tempptr = userptr;
    
    /* Locate the current user in the linked list */
    while (userptr != NULL) {
        if (!strcmp(userptr->username, currentUser))
            break;
        userptr = userptr->next;
    }
    
    int flag = -1;
    
    if (userptr == NULL) {
        printf("\nUser not found in the system!\n");
        return;
    }
    
    /* Check whether a valid tour is booked by comparing destination names */
    if (strcmp(userptr->place, "Paris, France") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Tokyo, Japan") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Bangkok, Thailand") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Abu Dhabi, UAE") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Miami, USA") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Rome, Italy") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Munich, Germany") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Madrid, Spain") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Istanbul, Turkey") == 0)
        flag++; 
    else if (strcmp(userptr->place, "Gilgit, Pakistan") == 0)
        flag++; 
    else {
        printf("\nNo tour has been booked to cancel!\n");
        return;
    }
    
    /* If a valid booking exists, reset tour details and inform user about the refund. */
    if (flag == 0) {
        printf("\nYour booking for %s (%d ticket(s)) has been cancelled. A refund of Rs %.0f will be processed.\n", 
               userptr->place, userptr->numberTicket, userptr->price * userptr->numberTicket);
        strcpy(userptr->place, "N/A");
        userptr->price = 0.0;
        userptr->numberTicket = 0;
        filing(tempptr);
    }
}

void changePassword(user *userptr) {
    user *tempptr = userptr;
    char passCurrent[100];
    
    fflush(stdin);
    printf("\nEnter your current password to continue: ");
    scanf(" %[^\n]s", passCurrent);
    
    /* Verify that the entered current password matches stored password. */
    while (userptr != NULL) {
        if (!strcmp(userptr->username, currentUser))
            break;
        printf("\nInvalid credentials! Please try again.\n");
        userptr = userptr->next;
    }
    
    if (userptr == NULL)
        return;
    
    if (!strcmp(passCurrent, userptr->password)) {
        printf("\nEnter your new password: ");
        scanf(" %[^\n]s", userptr->password);
        printf("\nPassword updated successfully!\n");
    } else {
        printf("\nIncorrect password provided. Password was not changed.\n");
    }
    
    filing(tempptr);
}

void logout(void) {
    /* Ensure that a user is logged in before attempting to log out. */
    if (currentStatus == menu || strcmp(currentUser, "\0") == 0) {
        printf("\nError: No user is currently logged in. Please log in first.\n");
        return;
    }
    
    strcpy(currentUser, "\0");
    currentStatus = menu;
    printf("\nYou have been successfully logged out.\n");
}

void exitProgram(void) {
    /* Display project and developer information on exit */
    printf("\nProgramming Fundamentals Laboratory Project BS(CS)-1E\n");
    printf("\nDevelopers:\n");
    printf("Muhammad Talha     --> 21K-3349\n");
    printf("Muhammad Hamza     --> 21K-4579\n");
    printf("Muhammad Hasan     --> 21K-4885\n");
    system("PAUSE");
}

void developers(void) {
    /* Provide project introduction and developer credits before main menu */
    printf("\nProject: Tourism Management System\n");
    printf("\nDevelopers: Talha, Hamza, and Hasan\n");
    
    delay(3.5);
    system("CLS");
}

void delay(float t) {
    clock_t start = clock();
    /* Delay execution for the specified time in seconds */
    while ((clock() - start) < (t * 1000));
    return;
}