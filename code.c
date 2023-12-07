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
void printLinkedList(LinkedList list);
void writeListToFile(LinkedList list, char *filename);
int countElements(LinkedList list);

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


long measureTime(void (*sortFunction)(LinkedList *), LinkedList *list)
{

    // Start measuring time

    struct timespec start, end;

    /* read consecutive nanosecond values */
    clock_gettime(CLOCK_REALTIME, &start);

    // Call the sorting function
    sortFunction(list);

    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    // long long searchTime = (end.tv_nsec - start.tv_nsec);
    
    long sortTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return sortTime;
}

void swap(Game *a, Game *b)
{
    Game t = *a;
    *a = *b;
    *b = t;
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

void parseLine(char *line, struct Game *newGame) {
    char *token;
    int index = 0;

    while ((token = strsep(&line, ",")) != NULL && index < 6) {
        switch (index) {
            case 0:
                newGame->id = atoi(token);
                break;
            case 1:
                strncpy(newGame->name, token, sizeof(newGame->name));
                break;
            case 2:
                newGame->avg_user_rating = atof(token);
                break;
            case 3:
                newGame->user_rating_count = atoi(token);
                break;
            case 4:
                strncpy(newGame->developer, token, sizeof(newGame->developer));
                break;
            case 5:
                newGame->size = atoi(token);
                break;
        }
        index++;
    }
}
LinkedList readAndStoreData(char *filename)
{
    LinkedList gamesLinkedList;
    gamesLinkedList.head = NULL;
    gamesLinkedList.tail = NULL;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // char line[256];
    char line[MAX_LINE_LENGTH];
    struct Game newGame;
    fgets(line, sizeof(line), file);
        while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character at the end, if present
        line[strcspn(line, "\n")] = '\0';

        // Parse the line using strsep
        parseLine(line, &newGame);

        // Now, newGame structure contains the parsed values
        // Do something with the values (e.g., print them)
        // printf("ID: %d, Name: %s, Rating: %.2f, Count: %d, Developer: %s, Size: %d\n",
        //        newGame.id, newGame.name, newGame.avg_user_rating,
        //        newGame.user_rating_count, newGame.developer, newGame.size);



           // Allocate memory for the new game node
        Game *newGameNode = malloc(sizeof(Game));
        if (!newGameNode)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        // Copy the data to the new node
        *newGameNode = newGame;
        newGameNode->next = NULL;
        newGameNode->prev = NULL;

        // Check for duplicates based on name using linear search
        Game *current = gamesLinkedList.head;
        while (current != NULL)
        {
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
                        current->prev->next = current->next;
                        if (current->next)
                        {
                            current->next->prev = current->prev;
                        }
                    }
                    free(current);

                    // Add the new one to the end of the list
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
                else
                {
                    // Ignore the new row
                    free(newGameNode);
                }
                break;
            }
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

    fclose(file);
    return gamesLinkedList;
}


long searchTimeMeasure(void (*searchFunction)(void*, char*), void *data, char *searchName) {
    struct timespec start, end;

    // Start measuring time
    clock_gettime(CLOCK_REALTIME, &start);

    // Call the search function
    searchFunction(data, searchName);

    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and return the search time in nanoseconds
    return (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);
}

void linearSearch(void *data, char *searchName) {
    LinkedList *list = (LinkedList *)data;
    Game *current = list->head;
    int searchCount = 0;

    while (current != NULL) {
        searchCount++;
        if (strcmp(current->name, searchName) == 0) {
            // Game found
            break;
        }
        current = current->next;
    }
}

// Adjust the binarySearch function
long binarySearch(BSTNode *root, char *searchName)
{
    struct timespec start, end;
    int searchCount = 0;

    // Start measuring time
    clock_gettime(CLOCK_REALTIME, &start);

    BSTNode *current = root;

    while (current != NULL)
    {
        // Check if searchName is present at the current node
        int cmp = strcmp(current->game->name, searchName);
        if (cmp == 0)
        {
            // Game found
            searchCount++;
            break;
        }
        else if (cmp < 0)
        {
            // If searchName is greater, move to the right subtree
            current = current->right;
        }
        else
        {
            // If searchName is smaller, move to the left subtree
            current = current->left;
        }
    }

    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    long searchTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    printf("Searching for %s...\n", searchName);
    printf("Single search time: %ld nanoseconds.\n", searchTime);

    return searchTime;
}

// Function to insert a game into the BST
BSTNode *insertBSTNode(BSTNode *root, Game *game)
{
    if (root == NULL)
    {
        // Create a new node for the game
        BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
        newNode->game = game;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    // Compare the game names
    int cmp = strcmp(root->game->name, game->name);

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
    BSTNode *root = NULL;
    Game *current = list->head;

    while (current != NULL)
    {
        root = insertBSTNode(root, current);
        current = current->next;
    }

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

void printLinkedList(LinkedList list)
{
    Game *current = list.head;

    while (current != NULL)
    {
        printf("ID: %d, Name: %s, Avg User Rating: %.2f, User Rating Count: %d, Developer: %s,Size: %d\n", current->id, current->name, current->avg_user_rating, current->user_rating_count, current->developer, current->size);

        current = current->next;
    }
}

// void compareLinearVsBinaryVsQuicksort(LinkedList sortedList, BSTNode *bstRoot, char searchNames[][100], int numSearches, int m)
// {
//     long totalLinearSearchTime = 0;
//     long totalBinarySearchTime = 0;

//     // Perform m linear searches and measure the total time
//     for (int i = 0; i < m; i++)
//     {
//         long searchTime = linearSearch(sortedList, searchNames[i % numSearches]);
//         totalLinearSearchTime += searchTime;
//     }

//     printf("Total time for %d linear searches: %ld nanoseconds.\n", m, totalLinearSearchTime);

//     // Measure time for binary search
//     for (int i = 0; i < m; i++)
//     {
//         long searchTime = binarySearch(bstRoot, searchNames[i % numSearches]);
//         totalBinarySearchTime += searchTime;
//     }

//     printf("Total time for %d binary searches: %ld nanoseconds.\n", m, totalBinarySearchTime);

//     // Measure time for quicksort
//     LinkedList copyList = sortedList;
//     long quickSortTime = measureTime(quickSortLinkedList, &copyList);
//     printf("Time for quicksort: %ld nanoseconds.\n", quickSortTime);

//     // Compare the times and print the result
//     if (totalLinearSearchTime < totalBinarySearchTime && totalLinearSearchTime < quickSortTime)
//     {
//         printf("Linear search is faster than binary search and quicksort for %d searches.\n", m);
//     }
//     else
//     {
//         printf("Linear search is not faster than binary search and quicksort for %d searches.\n", m);
//     }
// }
int main()
{
    char filename[] = "games.csv";
    LinkedList gamesLinkedList = readAndStoreData(filename);

    int numElements = countElements(gamesLinkedList);
    printf("Number of elements in LinkedList: %d\n", numElements);
    writeListToFile(gamesLinkedList, "output.txt");

    // Linear Search Test
    printf("*** Linear Search Test ***\n\nBefore sorting:\n\n");

    // Search for specific games and measure search time
    // char searchNames[][100] = {"Sudoku", "Reversi", "Morocco", "Sudoku (Free)", "Space Control"};
    char searchNames[][100] = {"Transport Empire","European War", "Zen Wars", "Pebbles","Jisgaw +0.5K","Bitcoin Billionaire"};
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
searchTimeMeasure(linearSearch, &gamesLinkedList, searchNames[i]);

        // long searchTime = linearSearch(gamesLinkedList, searchNames[i]);
                totalSearchTime += searchTime;
                
                        printf("Average search time: %ld nanoseconds.\n\n",        // long searchTime = linearSearch(gamesLinkedList, searchNames[i]);
        totalSearchTime += searchTime;

        printf("Average search time: %ld nanoseconds.\n\n", totalSearchTime / (i + 1));
    }

    // Sort the linked list by name using insertion sort
    insertionSort(&sortedGamesLinkedList);

    quickSortLinkedList(&sortedGamesLinkedList);
    Game *currentSorted = sortedGamesLinkedList.head;

    printf("\nAfter sorting: \n\n");

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
    printf("Time for insertion sort: %ld nanoseconds.\n", insertionSortTime);

    // Measure time for quicksort
    long quickSortTime = measureTime(quickSortLinkedList, &sortedGamesLinkedList);
    printf("Time for quicksort: %ld nanoseconds.\n", quickSortTime);

    printf("*** Binary Search Test ***\n\n");
    BSTNode *bstRoot = buildBSTFromList(&sortedGamesLinkedList);

    // Search for specific games and measure search time
    long totalBinarySearchTime = 0;

    for (int i = 0; i < numSearches; i++)
    {
        long searchTime = binarySearch(bstRoot, searchNames[i]);
        totalBinarySearchTime += searchTime;

        printf("Average binary search time: %ld nanoseconds.\n\n", totalBinarySearchTime / (i + 1));
    }

     int m = 1000;  // You can adjust the value of m

    // compareLinearVsBinaryVsQuicksort(sortedGamesLinkedList, bstRoot, searchNames, numSearches, m);
    return 0;
}