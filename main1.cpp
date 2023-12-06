#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <random>
#include <ctime>

using namespace std;

// Define a constant for nanoseconds per second
#define NANOSECONDS_PER_SECOND 1000000000L


// Define a structure to represent a game
struct GAME
{
    int id;
    char *name;
    float avg_user_rating;
    int user_rating_count;
    char *developer;
    int size;
};

// Define a Node class for a linked list
class Node
{
public:
    Node *next, *prev;
    GAME *game;

    // Constructor to initialize a node with a game
    Node(GAME *game)
    {
        next = NULL;
        prev = NULL;
        this->game = game;
    }
};

// Define a LinkedList class
class LinkedList
{
public:
    Node *head, *tail;
    int size;

    // Constructor to initialize a linked list with a game
    LinkedList(GAME *game)
    {
        this->head = game != NULL ? new Node(game) : NULL;
        this->tail = this->head;
        this->size = game != NULL ? 1 : 0;
    }

    // Getter for the size of the linked list
    int getSize()
    {
        return size;
    }

    // Check if the linked list is empty
    bool isEmpty()
    {
        return getSize() <= 0;
    }

    // Insert a node at the head of the linked list
    void insertHead(Node *new_node)
    {
        if (isEmpty())
        {
            head = new_node;
            tail = new_node;
            size = size + 1;
        }
        else
        {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
            size = size + 1;
        }
    };

    // Delete the head node of the linked list
    void deleteHead()
    {
        if (isEmpty())
        {
            cout << "An attempt to delete was made while Linked List is empty";
        }
        if (getSize() == 1)
        {
            delete (head);
            head = NULL;
            tail = NULL;
            size = 0;
        }
        else
        {
            head = head->next;
            head->prev->next = NULL;
            delete (head->prev); // delete operator instead of free to deallocate memory for the deleted nodes
            head->prev = NULL;
            size = size - 1;
        }
    }

    // Insert a node at the tail of the linked list
    void insertTail(Node *new_node)
    {
        if (isEmpty())
        {
            head = new_node;
            tail = new_node;
            size = size + 1;
        }
        else
        {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
            size = size + 1;
        }
    };

    // Delete the tail node of the linked list
    void deleteTail()
    {
        if (isEmpty())
        {
            cout << "An attempt to delete was made while Linked List is empty";
        }
        if (getSize() == 1)
        {
            delete (head);
            head = NULL;
            tail = NULL;
            size = 0;
        }
        else
        {
            tail = tail->prev;
            tail->next->prev = NULL;
            delete (tail->next); // delete operator instead of free to deallocate memory for the deleted nodes
            tail->next = NULL;
            size = size - 1;
        }
    }

    // Find and get game by its index in the linked list
    Node *getGameByIndex(int index)
    {
        if (isEmpty())
        {
            return NULL;
        }
        if (index > size || index < 0)
        {
            return NULL;
        }
        Node *currentNode = this->head;
        int counter = 0;
        while (counter != index)
        {
            currentNode = currentNode->next;
            counter++;
        }
        return currentNode;
    }

    // Linear search
    Node *findNodeByGameName(char *gameName)
    {
        if (isEmpty())
        {
            return NULL;
        }
        Node *currentNode = this->head;
        while (currentNode != NULL)
        {
            if (strcmp(currentNode->game->name, gameName) == 0)
            {
                return currentNode;
            }
            currentNode = currentNode->next;
        }
        return NULL;
    }

    void remove(Node *target_node)
    {
        if (target_node != NULL)
        {
            if (target_node == head)
            {

                deleteHead();
            }
            else if (target_node == tail)
            {

                deleteTail();
            }
            else
            {
                Node *prev = target_node->prev;
                Node *next = target_node->next;
                prev->next = next;
                next->prev = prev;
                delete (target_node);
                size = size - 1;
            }
        }
    }
    // Function to perform insertion sort
    void insertionSort()
    {
        Node *sorted = NULL;
        Node *current = head;

        while (current != NULL)
        {
            Node *next = current->next;
            insertSorted(sorted, current);
            current = next;
        }

        head = sorted;
    }

    // Helper function to insert a node into a sorted list
    void insertSorted(Node *&sorted, Node *new_node)
    {
        if (sorted == NULL || strcmp(sorted->game->name, new_node->game->name) >= 0)
        {
            new_node->next = sorted;
            new_node->prev = NULL;
            if (sorted != NULL)
                sorted->prev = new_node;
            sorted = new_node;
        }
        else
        {
            Node *current = sorted;
            while (current->next != NULL && strcmp(current->next->game->name, new_node->game->name) < 0)
            {
                current = current->next;
            }
            new_node->next = current->next;
            new_node->prev = current;
            if (current->next != NULL)
                current->next->prev = new_node;
            current->next = new_node;
        }
    }

    // Function to perform quicksort
    void quickSort()
    {
        head = quickSortRecursive(head, tail);
    }

private:
    // Helper function for quicksort (recursive)
    Node *quickSortRecursive(Node *start, Node *end)
    {
        if (start == NULL || start == end)
            return start;

        Node *pivot = partition(start, end);
        if (pivot != NULL && pivot != start)
            pivot->prev = quickSortRecursive(start, pivot->prev);
        if (pivot != NULL && pivot != end)
            pivot->next = quickSortRecursive(pivot->next, end);

        return pivot;
    }

    // Helper function for quicksort (partition)
    Node *partition(Node *start, Node *end)
    {
        char *pivotName = end->game->name;
        Node *i = start->prev;

        for (Node *j = start; j != end; j = j->next)
        {
            if (strcmp(j->game->name, pivotName) < 0)
            {
                i = (i == NULL) ? start : i->next;
                swap(i->game, j->game);
            }
        }

        i = (i == NULL) ? start : i->next;
        swap(i->game, end->game);
        return i;
    }
};

class BSTNode
{
public:
    GAME *game;
    BSTNode *left;
    BSTNode *right;

    BSTNode(GAME *game) : game(game), left(NULL), right(NULL) {}
};

class BinarySearchTree
{
private:
    BSTNode *root;

public:
    BinarySearchTree() : root(NULL) {}

    void insert(GAME *game)
    {
        root = insertRecursive(root, game);
    }

    BSTNode *search(char *target)
    {
        return searchRecursive(root, target);
    }

private:
    BSTNode *insertRecursive(BSTNode *node, GAME *game)
    {
        if (node == NULL)
        {
            return new BSTNode(game);
        }

        if (strcmp(game->name, node->game->name) < 0)
        {
            node->left = insertRecursive(node->left, game);
        }
        else if (strcmp(game->name, node->game->name) > 0)
        {
            node->right = insertRecursive(node->right, game);
        }

        return node;
    }

    BSTNode *searchRecursive(BSTNode *node, char *target)
    {
        if (node == NULL || strcmp(target, node->game->name) == 0)
        {
            return node;
        }

        if (strcmp(target, node->game->name) < 0)
        {
            return searchRecursive(node->left, target);
        }
        else
        {
            return searchRecursive(node->right, target);
        }
    }
};

// Measure the time taken for linear search in a linked list
long long measureLinearSearchTime(Node *gameNode, LinkedList *lList)
{
   // Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);


    // Perform the linear search
    Node *foundNode = lList->findNodeByGameName(gameNode->game->name);

   // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and return the search time in nanoseconds
    long long searchTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return searchTime;
}

// Measure the time taken for binary search in a binary search tree
long long measureBinarySearchTime(Node *gameNode, BinarySearchTree bst)
{
    // Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // Perform the binary search
    bst.search(gameNode->game->name);
    
    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and return the search time in nanoseconds
    long long searchTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return searchTime;
}

// Function to duplicate a linked list with a deep copy of each node's data
LinkedList *duplicateLinkedList(LinkedList *originalList)
{
  
    // Pointer to traverse the original linked list
    Node *originalCurrent = originalList->head;

    // Create a new linked list to store the duplicated nodes
    LinkedList *duplicateList = new LinkedList(NULL);

    // Iterate through the original linked list
    while (originalCurrent != NULL)
    {

        // Create a deep copy of the GAME object in the current node
        GAME *duplicateGame = new GAME;
        *duplicateGame = *(originalCurrent->game); 

        // Create a new node for the duplicated data
        Node *newNode = new Node(duplicateGame);

        // Insert the new node at the tail of the duplicated linked list
        duplicateList->insertTail(newNode);

        // Move to the next node in the original linked list
        originalCurrent = originalCurrent->next;
    }

    // Return the duplicated linked list
    return duplicateList;
}

// Function to parse a comma-separated string and create a GAME object
GAME *parseLine(char *game_line)
{
    // Create a new GAME object to store the parsed data
    GAME *game = new GAME;

    // Tokenize the input string using ',' as a delimiter and extract the ID
    char *token = strsep(&game_line, ",");
    game->id = atoi(token);

    // Tokenize and extract the name field, handling quoted names if present
    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        string name;
        name += token;
        while (name.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            name += ",";
            name += token;
        }

        // Allocate memory for the name and copy the parsed value
        game->name = new char[name.length() + 1];
        strcpy(game->name, name.substr(1, name.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        // Allocate memory for the name and copy the parsed value
        game->name = new char[strlen(token) + 1];
        strcpy(game->name, token);
    }

    // Tokenize and extract the average user rating
    token = strsep(&game_line, ",");
    game->avg_user_rating = atof(token);

    // Tokenize and extract the user rating count
    token = strsep(&game_line, ",");
    game->user_rating_count = atoi(token);

    // Tokenize and extract the developer field, handling quoted names if present
    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        string developer;
        developer += token;
        while (developer.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            developer += ",";
            developer += token;
        }

        // Allocate memory for the developer and copy the parsed value
        game->developer = new char[developer.length() + 1];
        strcpy(game->developer, developer.substr(1, developer.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        // Allocate memory for the developer and copy the parsed value
        game->developer = new char[strlen(token) + 1];
        strcpy(game->developer, token);
    }

    // Tokenize and extract the size field
    token = strsep(&game_line, ",");
    game->size = atoi(token);

    // Return the populated GAME object
    return game;
}

// Function to generate a random integer between 0 and 'max' (inclusive)
int generateRandomNumber(int max)
{
    // Initialize a random device to obtain a seed for the random number generator
    std::random_device rd;

    // Create a default random number generator engine and seed it with the random device
    std::default_random_engine generator(rd());

    // Define a uniform distribution for integers in the range [0, max]
    std::uniform_int_distribution<int> distribution(0, max);

    // Generate a random number using the distribution and return it
    int random_number = distribution(generator);
    return random_number;

}

// Function to swap the contents of two GAME objects
void swap(GAME *gameA, GAME *gameB)
{
    GAME temp = *gameA;
    *gameA = *gameB;
    *gameB = temp;
}


int main()
{
    // open the file "games.csv"
    FILE *gameFile = fopen("games.csv", "r");

    // Check if the file opening operation was successful
    if (!gameFile)
    {
        // Print an error message and exit if the file cannot be opened
        printf("Error opening games.csv \n");
        exit(EXIT_FAILURE);
    }

    LinkedList *gamesLinkedList = new LinkedList(NULL);

    char *line = NULL;
    size_t n = 0;
    ssize_t nbytes;

    // Read and skip the header line, which contains column names
    getline(&line, &n, gameFile);

    // Read and process each line of the games.csv file
    while ((nbytes = getline(&line, &n, gameFile)) > 0)
    {
        // Set the last character of the line to null terminator
        line[nbytes - 1] = 0;

        // Create a pointer to the current line
        char *linep = line;

        // Create a data structure to represent each game by parsing the current line
        GAME *game = parseLine(linep);

        // Create a new node for the game and insert it into the linked list
        Node *newGameNode = new Node(game);
        
        // Check for duplicates based on game name in the linked list
        Node *duplicate = gamesLinkedList->findNodeByGameName(newGameNode->game->name);

       // If a duplicate exists, compare user rating counts and update the linked list accordingly
        if (duplicate != NULL)
        {

            if (newGameNode->game->user_rating_count > duplicate->game->user_rating_count)
            {
                // Remove the existing duplicate node
                gamesLinkedList->remove(duplicate);

                // Insert the newGameNode with a higher user rating count
                gamesLinkedList->insertTail(newGameNode);
            }
        }
        else
        {
            // If no duplicate, simply insert the newGameNode at the tail of the linked list
            gamesLinkedList->insertTail(newGameNode);
        }
    }

    // Create a duplicate of the gamesLinkedList for quicksort
    LinkedList *quickSortList = duplicateLinkedList(gamesLinkedList);
  
    // Create another duplicate of the gamesLinkedList for linear search in order to check the break point
    LinkedList *linearSearchList = duplicateLinkedList(gamesLinkedList);

    cout << "Number of elements in LinkedList: " << gamesLinkedList->size << endl << endl;

    cout << "*** Linear Search Test ***" << endl << endl;

    cout << "Before sorting:" << endl << endl;

    // Print only first 5 elements from linked list
    for (int i = 0; i < 5; i++)
    {

        // Create a new GAME object
        GAME *game = new GAME;

        // Retrieve the i-th game node from gamesLinkedList and access the game object
        game = gamesLinkedList->getGameByIndex(i)->game;

        cout << "# " << game->id << ", " << game->name << ", " << game->avg_user_rating << ", " << game->user_rating_count << ", " << game->developer << ", " << game->size << endl;
    }

    cout << endl;

    // Select random game nodes from the gamesLinkedList using generateRandomNumber
    Node *selectedGame1 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame2 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame3 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame4 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    
    // Search number 1 using linear search
    cout << "Search number 1:" << endl;

    // Display the item being searched for
    cout << "Searching for " << selectedGame1->game->name << "..." << endl;

    // Measure the time spent on a single linear search for the selected item
    int linearSearchTime1 = measureLinearSearchTime(selectedGame1, gamesLinkedList);
    cout << "Single search time: " << linearSearchTime1 << " nanoseconds." << endl;

    // Measure the total time spent on 10 linear searches for the selected item to test the speed of the algorithm
    int totalLinearSearchTime1 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalLinearSearchTime1 = totalLinearSearchTime1 + measureLinearSearchTime(selectedGame1, gamesLinkedList);
    }

    // Calculate the average search time for the selected item 
    int averageLinearSearchTime1 = (totalLinearSearchTime1 + linearSearchTime1) / 11;
    cout << "Average search time: " << averageLinearSearchTime1 << " nanoseconds." << endl << endl;

    // Search number 2 using linear search
    cout << "Search number 2:" << endl;

    // Display the item being searched for
    cout << "Searching for " << selectedGame2->game->name << "..." << endl;

    // Measure the time spent on a single linear search for the selected item 
    int linearSearchTime2 = measureLinearSearchTime(selectedGame2, gamesLinkedList);
    cout << "Single search time: " << linearSearchTime2 << " nanoseconds." << endl;

    // Measure the total time spent on 10 linear searches for the selected item to test the speed of the algorithm
    int totalLinearSearchTime2 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalLinearSearchTime2 = totalLinearSearchTime2 + measureLinearSearchTime(selectedGame2, gamesLinkedList);
    }

    // Calculate the average search time for the selected item
    int averageLinearSearchTime2 = (totalLinearSearchTime2 + linearSearchTime2) / 11;
    cout << "Average search time: " << averageLinearSearchTime2 << " nanoseconds." << endl << endl;

    // Search number 3 using linear search
    cout << "Search number 3:" << endl;

    // Display the item being searched for
    cout << "Searching for " << selectedGame3->game->name << "..." << endl;

    // Measure the time spent on a single linear search for the selected item 
    int linearSearchTime3 = measureLinearSearchTime(selectedGame3, gamesLinkedList);
    cout << "Single search time: " << linearSearchTime3 << " nanoseconds." << endl;

    // Measure the total time spent on 10 linear searches for the selected item to test the speed of the algorithm
    int totalLinearSearchTime3 = 0;   
    for (int i = 0; i < 10; i++)
    {
        totalLinearSearchTime3 = totalLinearSearchTime3 + measureLinearSearchTime(selectedGame3, gamesLinkedList);
    }

    // Calculate the average search time for the selected item
    int averageBeforeSort3 = (totalLinearSearchTime3 + linearSearchTime3) / 11;
    cout << "Average search time: " << averageBeforeSort3 << " nanoseconds." << endl << endl;


    // Start measuring time for insertion sort
    struct timespec insertion_sort_start, insertion_sort_end;
    clock_gettime(CLOCK_REALTIME, &insertion_sort_start);

    // Perform insertion sort
    gamesLinkedList->insertionSort();

    // Stop measuring time for insertion sort
    clock_gettime(CLOCK_REALTIME, &insertion_sort_end);

    // Calculate and print the time taken for insertion sort
    long long insertion_sort_time = (insertion_sort_end.tv_sec - insertion_sort_start.tv_sec) * NANOSECONDS_PER_SECOND + (insertion_sort_end.tv_nsec - insertion_sort_start.tv_nsec);
    cout << "Insertion Sort Time: " << insertion_sort_time << " nanoseconds" << endl;

    // Start measuring time for quicksort
    struct timespec quicksort_start, quicksort_end;
    clock_gettime(CLOCK_REALTIME, &quicksort_start);

    // Perform quicksort
    quickSortList->quickSort();

    // Stop measuring time for quicksort
    clock_gettime(CLOCK_REALTIME, &quicksort_end);

    // Calculate and print the time taken for quicksort
    long long quicksort_time = (quicksort_end.tv_sec - quicksort_start.tv_sec) * NANOSECONDS_PER_SECOND + (quicksort_end.tv_nsec - quicksort_start.tv_nsec);
    cout << "Quick Sort Time: " << quicksort_time << " nanoseconds" << endl << endl;
    cout << "After sorting:" << endl <<endl;

    // Print only first 5 elements from sorted array 
    for (int i = 0; i < 5; i++)
    {
        // Create a new GAME object
        GAME *game = new GAME;

        // Retrieve the i-th game node from gamesLinkedList and access the game object
        game = gamesLinkedList->getGameByIndex(i)->game;

        cout << "# " << game->id << ", " << game->name << ", " << game->avg_user_rating << ", " << game->user_rating_count << ", " << game->developer << ", " << game->size << endl;
    }

    cout << endl;

    cout << "*** Binary Search Test ***" << endl << endl;

    BinarySearchTree bst;

    // Iterate through the LinkedList and insert each game into the BST
    Node *current = gamesLinkedList->head;
    while (current != NULL)
    {
        bst.insert(current->game);
        current = current->next;
    }

    // Search number 1 using binary search 
    cout << "Search number 1:" << endl;
    
    // Display the item being searched for
    cout << "Searching for " << selectedGame1->game->name << "..." << endl;

    // Measure the time spent on a single binary search for the selected item 
    int binarySearchTime1 = measureBinarySearchTime(selectedGame1, bst);
    cout << "Single search time: " << binarySearchTime1 << " nanoseconds." << endl;

    // Measure the total time spent on 10 binary searches for the selected item to test the speed of the algorithm
    int totalBinarySearchTime1 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime1 = totalBinarySearchTime1 + measureBinarySearchTime(selectedGame1, bst);
    }

    // Calculate the average search time for the selected item
    int averageBinarySearchTime1 = (totalBinarySearchTime1 + binarySearchTime1) / 11;
    cout << "Average search time: " << averageBinarySearchTime1 << " nanoseconds." << endl << endl;

    // Search number 2 using binary search 
    cout << "Search number 2:" << endl;

    // Display the item being searched for
    cout << "Searching for " << selectedGame2->game->name << "..." << endl;

    // Measure the time spent on a single binary search for the selected item 
    int binarySearchTime2 = measureBinarySearchTime(selectedGame2, bst);

    cout << "Single search time: " << binarySearchTime2 << " nanoseconds." << endl;
    
    // Measure the total time spent on 10 binary searches for the selected item to test the speed of the algorithm
    int totalBinarySearchTime2 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime2 = totalBinarySearchTime2 + measureBinarySearchTime(selectedGame2, bst);
    }

    // Calculate the average search time for the selected item
    int averageBinarySearchTime2 = (totalBinarySearchTime2 + binarySearchTime2) / 11;
    cout << "Average search time: " << averageBinarySearchTime2 << " nanoseconds." << endl << endl;

    // Search number 2 using binary search 
    cout << "Search number 3:" << endl;

    // Display the item being searched for
    cout << "Searching for " << selectedGame3->game->name << "..." << endl;

    // Measure the time spent on a single binary search for the selected item 
    int binarySearchTime3 = measureBinarySearchTime(selectedGame3, bst);
    cout << "Single search time: " << binarySearchTime2 << " nanoseconds." << endl;

    // Measure the total time spent on 10 binary searches for the selected item to test the speed of the algorithm
    int totalBinarySearchTime3 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime3 = totalBinarySearchTime3 + measureBinarySearchTime(selectedGame3, bst);
    }

    // Calculate the average search time for the selected item
    int averageAfterSort3 = (totalBinarySearchTime3 + binarySearchTime2) / 11;
    cout << "Average search time: " << averageAfterSort3 << " nanoseconds." << endl << endl;

   //Break point

    int linearSearchTime4 = measureLinearSearchTime(selectedGame4, linearSearchList);
    int binarySearchTime4 = measureBinarySearchTime(selectedGame4, bst);

    int totalBinarySearchTime4 = quicksort_time + binarySearchTime4;
    int m = 0;
    int totalLinearSearchTime4 = linearSearchTime4;
    while(totalLinearSearchTime4 < totalBinarySearchTime4){
        totalLinearSearchTime4+=linearSearchTime4;
        totalBinarySearchTime4+=binarySearchTime4;
        m++;
    }

    cout <<  "Break point m = " << m <<" between repeated linear searches and sort-once & multiple binary searches."<<endl; 
   

    fclose(gameFile);
    return 0;
}
