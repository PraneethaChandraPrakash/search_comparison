#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NANOSECONDS_PER_SECOND 1000000000L
#define MAX_LINE_LENGTH 256

// Define the structure for each game
typedef struct Game
{
    int id;
    char name[100];
    float avg_user_rating;
    int user_rating_count;
    char developer[100];
    int size;
    struct Game *next;
    struct Game *prev;
} Game;

// Define the linked list
typedef struct
{
    Game *head;
    Game *tail;
} LinkedList;

typedef struct BSTNode
{
    Game *game;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

void insertionSort(LinkedList *list);
LinkedList readAndStoreData(char *filename);
void writeListToFile(LinkedList list, char *filename);
int countElements(LinkedList list);

void writeListToFile(LinkedList list, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error opening file %s for writing.\n", filename);
        exit(EXIT_FAILURE);
    }

    Game *current = list.head;

    while (current != NULL)
    {
        fprintf(file, "%d,%s,%.2f,%d,%s,%d\n",
                current->id, current->name, current->avg_user_rating, current->user_rating_count,
                current->developer, current->size);

        current = current->next;
    }

    fclose(file);
}
void insertionSort(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
        return;

    Game *sorted = NULL;
    Game *current = list->head;

    while (current != NULL)
    {
        Game *next = current->next;

        if (sorted == NULL || strcmp(sorted->name, current->name) >= 0)
        {
            // Insert at the beginning
            current->next = sorted;
            current->prev = NULL;

            if (sorted != NULL)
                sorted->prev = current;

            sorted = current;
        }
        else
        {
            // Locate the node before the point of insertion
            Game *temp = sorted;
            while (temp->next != NULL && strcmp(temp->next->name, current->name) < 0)
            {
                temp = temp->next;
            }

            // Insert after temp
            current->next = temp->next;
            current->prev = temp;

            if (temp->next != NULL)
                temp->next->prev = current;

            temp->next = current;
        }

        current = next;
    }

    // Update the head and tail of the sorted list
    while (sorted->prev != NULL)
    {
        sorted = sorted->prev;
    }

    list->head = sorted;

    // Update the tail
    Game *tail = list->head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    list->tail = tail;
}

long measureTime(void (*sortFunction)(LinkedList *), LinkedList *list)
{
    // Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // Call the sorting function
    sortFunction(list);

    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    long long sortTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return sortTime;
}

void swap(Game *game_a, Game *game_b)
{
    Game t = *game_a;
    *game_a = *game_b;
    *game_b = t;
}

Game *partition(Game *low, Game *high)
{
    char *pivot = high->name;
    Game *i = low->prev;

    for (Game *j = low; j != high; j = j->next)
    {
        if (strcmp(j->name, pivot) <= 0)
        {
            i = (i == NULL) ? low : i->next;
            swap(i, j);
        }
    }

    i = (i == NULL) ? low : i->next;
    swap(i, high);
    return i;
}

void quicksort(Game *low, Game *high)
{
    if (high != NULL && low != high && low != high->next)
    {
        Game *pivot = partition(low, high);
        quicksort(low, pivot->prev);
        quicksort(pivot->next, high);
    }
}

void quickSortLinkedList(LinkedList *list)
{
    if (list == NULL || list->head == NULL || list->tail == NULL)
    {
        return;
    }

    quicksort(list->head, list->tail);
}
// Function to parse a line and extract game data
void parseLine(char *line, struct Game *newGame)
{
    // Tokenize the input line using ',' as a delimiter
    char *token;

    // Initialize the index to keep track of the field being processed
    int index = 0;

    // Iterate through tokens in the line until the end of the line or up to 6 fields
    while ((token = strsep(&line, ",")) != NULL && index < 6)
    {
        // Switch statement to assign values to corresponding fields in the newGame structure
        switch (index)
        {
        case 0:
            // Convert the token to an integer and assign it to the 'id' field
            newGame->id = atoi(token);
            break;
        case 1:
            // Copy the token to the 'name' field, ensuring it fits within the size of the field
            strncpy(newGame->name, token, sizeof(newGame->name));
            break;
        case 2:
            // Convert the token to a floating-point number and assign it to 'avg_user_rating'
            newGame->avg_user_rating = atof(token);
            break;
        case 3:
            // Convert the token to an integer and assign it to 'user_rating_count'
            newGame->user_rating_count = atoi(token);
            break;
        case 4:
            // Copy the token to the 'developer' field, ensuring it fits within the size of the field
            strncpy(newGame->developer, token, sizeof(newGame->developer));
            break;
        case 5:
            // Convert the token to an integer and assign it to the 'size' field
            newGame->size = atoi(token);
            break;
        }
        // Move to the next field
        index++;
    }
}

// Function to read data from a file and store it in a doubly linked list
LinkedList readAndStoreData(char *filename)
{
    // Initialize a linked list to store game data
    LinkedList gamesLinkedList;
    gamesLinkedList.head = NULL;
    gamesLinkedList.tail = NULL;

    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        // Print an error message and exit if the file cannot be opened
        printf("Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    
    // Define the maximum length of a line in the file
    // char line[MAX_LINE_LENGTH];

    // Create a structure to store game data
    struct Game newGame;

    // Ignore the first line containing the header columns
    fgets(line, sizeof(line), file);
    // Loop through the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Remove newline character at the end, if present
        line[strcspn(line, "\n")] = '\0';

         // Parse the line using strsep
        parseLine(line, &newGame);
        // Allocate memory for a new node to store the game data
        Game *newGameNode = malloc(sizeof(Game));
        if (!newGameNode)
        {
             // Print an error message and exit if memory allocation fails
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        // Copy the data to the new node
        *newGameNode = newGame;
        newGameNode->next = NULL;
        newGameNode->prev = NULL;

        // Check for duplicates based on name
        Game *current = gamesLinkedList.head;
        while (current != NULL)
        {
            // Check if the current game's name matches the new game's name
            if (strcmp(current->name, newGameNode->name) == 0)
            {
                // Duplicate found, check user rating count
                if (newGameNode->user_rating_count > current->user_rating_count)
                {
                    // Remove the old one
                    if (current->prev == NULL)
                    {
                        gamesLinkedList.head = current->next;
                        if (current->next)
                        {
                            current->next->prev = NULL;
                        }
                    }
                    else
                    {
                         // Update the next and previous pointers to remove the current node
                        current->prev->next = current->next;
                        if (current->next)
                        {
                            current->next->prev = current->prev;
                        }
                    }
                    // Free the memory of the current node
                    free(current);

                    // Add the new one to the end of the list
                    if (gamesLinkedList.head == NULL)
                    {
                        // Update the head and tail of the linked list
                        gamesLinkedList.head = newGameNode;
                        gamesLinkedList.tail = newGameNode;
                    }
                    else
                    {
                        // Update the next and previous pointers of the tail node to add the new node
                        gamesLinkedList.tail->next = newGameNode;
                        newGameNode->prev = gamesLinkedList.tail;
                        gamesLinkedList.tail = newGameNode;
                    }
                }
                else
                {
                    // Free the memory of the new node
                    free(newGameNode);
                }
                // Exit the loop after processing the duplicate
                break;
            }
            // Move to the next node in the linked list
            current = current->next;
        }

        // If no duplicate found, add to the linked list
        if (current == NULL)
        {
            if (gamesLinkedList.head == NULL)
            {
                gamesLinkedList.head = newGameNode;
                gamesLinkedList.tail = newGameNode;
            }
            else
            {
                gamesLinkedList.tail->next = newGameNode;
                newGameNode->prev = gamesLinkedList.tail;
                gamesLinkedList.tail = newGameNode;
            }
        }
    }

    // Close the file after processing all lines
    fclose(file);

    // Return the populated linked list
    return gamesLinkedList;
}

// Function to measure the execution time of a search function
long searchTimeMeasure(void (*searchFunction)(void *, char *), void *data, char *searchName)
{
    // Variables to store the start and end times
    struct timespec start, end;

    // Start measuring time using CLOCK_REALTIME
    clock_gettime(CLOCK_REALTIME, &start);

    // Call the search function
    searchFunction(data, searchName);

    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    // Convert seconds to nanoseconds and add the remaining nanoseconds
    long searchTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    // Return the measured search time in nanoseconds
    return searchTime;
}

// Function to perform linear search in a linked list based on game name
void linearSearch(void *data, char *searchName)
{
    LinkedList *list = (LinkedList *)data;
    Game *current = list->head;
    int searchCount = 0;

    // Iterate through the linked list until the end or until the game is found
    while (current != NULL)
    {
        // Increment the search count for each iteration
        searchCount++;

        // Compare the name of the current game with the target search name
        if (strcmp(current->name, searchName) == 0)
        {
            // Exit loop if game is found
            break;
        }

        // Move to the next node in the linked list
        current = current->next;
    }
}

// Function to perform binary search using binary search tree 
void binarySearch(void *data, char *searchName)
{
    BSTNode *root = (BSTNode *)data;
    BSTNode *current = root;
    int searchCount = 0;

    // Iterate through the BST until the end or until the game is found
    while (current != NULL)
    {
        // Compare the name of the current game with the target search name
        int cmp = strcmp(current->game->name, searchName);
        if (cmp == 0)
        {
            // Exit loop if game is found
            searchCount++;
            break;
        }
        else if (cmp < 0)
        {
            // Move to the right subtree
            current = current->right;
        }
        else
        {
            // Move to the left subtree
            current = current->left;
        }
    }
}

// Function to insert a game into the BST
BSTNode *insertBSTNode(BSTNode *root, Game *game)
{
    if (root == NULL)
    {
    // If the current subtree is empty, create a new node for the game
        BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
        newNode->game = game;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    // Compare the game names to determine the insertion direction
    int cmp = strcmp(root->game->name, game->name);

    // Insert into the appropriate subtree based on the comparison result
    if (cmp < 0)
    {
        // Insert into the right subtree
        root->right = insertBSTNode(root->right, game);
    }
    else if (cmp > 0)
    {
        // Insert into the left subtree
        root->left = insertBSTNode(root->left, game);
    }

    return root;
}

// Function to build a BST from a linked list
BSTNode *buildBSTFromList(LinkedList *list)
{
    // Initialize the root of the BST to NULL
    BSTNode *root = NULL;

    // Start with the head of the linked list
    Game *current = list->head;

    // Iterate through the linked list and insert each game into the BST
    while (current != NULL)
    {
        root = insertBSTNode(root, current);
        current = current->next;
    }

    // Return the root of the built BST
    return root;
}


int countElements(LinkedList list)
{
    int count = 0;
    Game *current = list.head;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

void getRandomGames(LinkedList *gamesLinkedList, char searchNames[][100], int numGames)
{
    // Get the total number of elements in the linked list
    int numElements = countElements(*gamesLinkedList);

    // Check if the linked list is not empty and numGames is less than or equal to the total elements
    if (numElements > 0 && numGames <= numElements)
    {
        for (int i = 0; i < numGames; i++)
        {
            // Generate a random index within the range of the linked list size
            int randomIndex = rand() % numElements;

            // Traverse the linked list to the randomly selected index
            Game *current = gamesLinkedList->head;
            for (int j = 0; j < randomIndex && current != NULL; j++)
            {
                current = current->next;
            }

            // Check if a valid game is found at the random index
            if (current != NULL)
            {
                // Copy the game name to the searchNames array
                strcpy(searchNames[i], current->name);
            }
            else
            {
                printf("Error: Unable to retrieve a random game.\n");
                break; // Exit loop if an error occurs
            }
        }
    }
    else
    {
        printf("Error: The linked list is empty or numGames is invalid.\n");
    }
}

int main()
{
    char filename[] = "games.csv";
    LinkedList gamesLinkedList = readAndStoreData(filename);

    int numElements = countElements(gamesLinkedList);
    printf("Number of elements in LinkedList: %d\n\n", numElements);
    writeListToFile(gamesLinkedList, "output.txt");

    // Linear Search Test
    printf("*** Linear Search Test ***\n\nBefore sorting:\n\n");

    // Search for specific games and measure search time
    // char searchNames[][100] = {"Sudoku", "Reversi", "Morocco", "Sudoku (Free)", "Space Control"};
    // Seed the random number generator
    srand(time(NULL));
    char searchNames[3][100];
    getRandomGames(&gamesLinkedList, searchNames, 3);

    int numSearches = sizeof(searchNames) / sizeof(searchNames[0]);

    long totalSearchTime = 0;

    // Print first 5 elements from linked list before sorting
    Game *currentBeforeSorting = gamesLinkedList.head;
    for (int i = 0; i < 5 && currentBeforeSorting != NULL; i++)
    {
        printf("%d,%s,%.2f,%d,%s,%d\n",
               currentBeforeSorting->id, currentBeforeSorting->name, currentBeforeSorting->avg_user_rating,
               currentBeforeSorting->user_rating_count, currentBeforeSorting->developer, currentBeforeSorting->size);
        currentBeforeSorting = currentBeforeSorting->next;
    }

    // Create a new linked list for the sorted list
    LinkedList sortedGamesLinkedList = readAndStoreData(filename);
    // Perform linear search and print single search time and average search tim
    for (int i = 0; i < numSearches; i++)
    {
        long searchTime = searchTimeMeasure(linearSearch, &gamesLinkedList, searchNames[i]);
        printf("\n\nSearch number %d:", i+1);
        printf("\nSearching for %s...", searchNames[i]);
        printf("\nSingle search time: %ld nanoseconds.\n", searchTime);
        totalSearchTime += searchTime;

        printf("Average search time: %ld nanoseconds.\n", totalSearchTime / (i + 1));
    }

    // Sort the linked list by name using insertion sort
    insertionSort(&sortedGamesLinkedList);

    quickSortLinkedList(&sortedGamesLinkedList);
    Game *currentSorted = sortedGamesLinkedList.head;

    printf("\n\nAfter sorting: \n\n");

    // Print first 5 elements from linked list after sorting
    for (int i = 0; i < 5 && currentSorted != NULL; i++)
    {
        printf("%d,%s,%.2f,%d,%s,%d\n",
               currentSorted->id, currentSorted->name, currentSorted->avg_user_rating,
               currentSorted->user_rating_count, currentSorted->developer, currentSorted->size);
        currentSorted = currentSorted->next;
    }

    // Measure time for insertion sort
    long insertionSortTime = measureTime(insertionSort, &sortedGamesLinkedList);
    printf("\n\nTime for insertion sort: %ld nanoseconds.\n", insertionSortTime);

    // Measure time for quicksort
    long quickSortTime = measureTime(quickSortLinkedList, &sortedGamesLinkedList);
    printf("Time for quicksort: %ld nanoseconds.\n\n\n", quickSortTime);

    printf("*** Binary Search Test ***\n\n");
    BSTNode *bstRoot = buildBSTFromList(&sortedGamesLinkedList);

    // Search for specific games and measure search time
    long totalBinarySearchTime = 0;

    for (int i = 0; i < numSearches; i++)
    {
        // long searchTime = binarySearch(bstRoot, searchNames[i]);
        long searchTime = searchTimeMeasure(binarySearch, bstRoot, searchNames[i]);
        printf("Search number %d:\n", i+1);
        printf("Searching for %s...\n", searchNames[i]);
        printf("Single search time: %ld nanoseconds.\n", searchTime);
        totalBinarySearchTime += searchTime;

        printf("Average binary search time: %ld nanoseconds.\n\n", totalBinarySearchTime / (i + 1));
    }

    int m = 1000; // You can adjust the value of m

    // Iterate through different values of m
    for (int m = 1; m <= 1000; m += 50)
    {
        // Perform linear searches and measure total search time
        long totalLinearSearchTime = 0;
        for (int i = 0; i < m; i++)
        {
            long searchTime = searchTimeMeasure(linearSearch, &gamesLinkedList, searchNames[i % numSearches]);
            totalLinearSearchTime += searchTime;
        }

        // Sort the linked list by name using insertion sort for sorting once
        LinkedList sortedGamesLinkedList = readAndStoreData(filename);
        insertionSort(&sortedGamesLinkedList);

        // Measure time for sorting once using insertion sort
        long insertionSortTime = measureTime(insertionSort, &sortedGamesLinkedList);

        // Measure time for binary searches
        long totalBinarySearchTime = 0;
        BSTNode *bstRoot = buildBSTFromList(&sortedGamesLinkedList);
        for (int i = 0; i < m; i++)
        {
            long searchTime = searchTimeMeasure(binarySearch, bstRoot, searchNames[i % numSearches]);
            totalBinarySearchTime += searchTime;
        }

        // Print the results for the current value of m
        printf("m = %d: Linear Search Time = %ld, Insertion Sort Time = %ld, Binary Search Time = %ld\n",
               m, totalLinearSearchTime, insertionSortTime, totalBinarySearchTime);

        // Check if linear searches are more efficient than sorting once and binary searches
        if (totalLinearSearchTime < insertionSortTime + totalBinarySearchTime)
        {
            printf("Break point m = %d between repeated linear searches and sort-once & multiple binary searches.\n",m);
            break;
        }
    }

    return 0;
}
