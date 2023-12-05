#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <random>
#include <ctime>
#define NANOSECONDS_PER_SECOND 1000000000L


using namespace std;

struct GAME
{
    int id;
    char *name;
    float avg_user_rating;
    int user_rating_count;
    char *developer;
    int size;
};

class Node
{
public:
    Node *next, *prev;
    GAME *game;
    Node(GAME *game)
    {
        next = NULL;
        prev = NULL;
        this->game = game;
    }
};

class LinkedList
{
public:
    Node *head, *tail;
    int size;

    LinkedList(GAME *game)
    {
        this->head = game != NULL ? new Node(game) : NULL;
        this->tail = this->head;
        this->size = game != NULL ? 1 : 0;
    }

    int getSize()
    {
        return size;
    }

    bool isEmpty()
    {
        return getSize() <= 0;
    }

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

    void printGameItem(GAME *game)
    {
        cout << "# " << game->id << ", " << game->name << ", " << game->avg_user_rating << ", " << game->user_rating_count << ", " << game->developer << ", " << game->size << endl;
    }

    void printList()
    {
        Node *current = head;
        while (current != NULL)
        {
            printGameItem(current->game);
            current = current->next;
        }
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

long long getTimeForLinearSearch(Node *gameNode, LinkedList *lList)
{
   // Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);


    // Perform the event or task you want to measure
    Node *randomNodeFoundWithName = lList->findNodeByGameName(gameNode->game->name);

   // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    long long sortTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return sortTime;
}

long long getTimeForBinarySearch(Node *gameNode, BinarySearchTree bst)
{
// Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // Perform the event or task you want to measure
    bst.search(gameNode->game->name);

    
    // Stop measuring time
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate and print the search time in nanoseconds
    long long sortTime = (end.tv_sec - start.tv_sec) * NANOSECONDS_PER_SECOND + (end.tv_nsec - start.tv_nsec);

    return sortTime;
}


// long long getTimeForLinearSearch(Node *gameNode, LinkedList *lList)
// {
//     // Record the start time
//     auto start_time = chrono::high_resolution_clock::now();

//     // Perform the event or task you want to measure
//     Node *randomNodeFoundWithName = lList->findNodeByGameName(gameNode->game->name);

//     // Record the end time
//     auto end_time = chrono::high_resolution_clock::now();

//     // Calculate the duration
//     auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

//     // Log the duration
//     // cout << "Event duration: " << duration.count() << " nanoseconds." << endl;
//     return duration.count();
// }

// long long getTimeForBinarySearch(Node *gameNode, BinarySearchTree bst)
// {
//     // Record the start time
//     auto start_time = chrono::high_resolution_clock::now();

//     // Perform the event or task you want to measure
//     bst.search(gameNode->game->name);

//     // Record the end time
//     auto end_time = chrono::high_resolution_clock::now();

//     // Calculate the duration
//     auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

//     // Log the duration
//     // cout << "Event duration: " << duration.count() << " nanoseconds." << endl;
//     return duration.count();
// }

LinkedList *copyLinkedList(LinkedList *originalList)
{
    LinkedList *copiedList = new LinkedList(NULL);

    // Iterate through the original list and copy each node
    Node *current = originalList->head;
    while (current != NULL)
    {
        // Create a new node with a copy of the game data
        GAME *gameCopy = new GAME;
        *gameCopy = *(current->game); // Assuming a shallow copy is sufficient

        Node *newNode = new Node(gameCopy);

        // Insert the new node into the copied list
        copiedList->insertTail(newNode);

        current = current->next;
    }

    return copiedList;
}

GAME *parseLine(char *game_line)
{
    GAME *game = new GAME;

    char *token = strsep(&game_line, ",");
    game->id = atoi(token);

    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        // Handling quoted names
        string name;
        name += token;
        while (name.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            name += ",";
            name += token;
        }
        game->name = new char[name.length() + 1];
        strcpy(game->name, name.substr(1, name.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        game->name = new char[strlen(token) + 1];
        strcpy(game->name, token);
    }

    token = strsep(&game_line, ",");
    game->avg_user_rating = atof(token);

    token = strsep(&game_line, ",");
    game->user_rating_count = atoi(token);

    token = strsep(&game_line, ",");
    if (token[0] == '"')
    {
        // Handling quoted developers
        string developer;
        developer += token;
        while (developer.back() != '"' && game_line != NULL)
        {
            token = strsep(&game_line, ",");
            developer += ",";
            developer += token;
        }
        game->developer = new char[developer.length() + 1];
        strcpy(game->developer, developer.substr(1, developer.length() - 2).c_str()); // Remove leading and trailing quotes
    }
    else
    {
        game->developer = new char[strlen(token) + 1];
        strcpy(game->developer, token);
    }

    token = strsep(&game_line, ",");
    game->size = atoi(token);

    return game;
}

// Declare the random number generator outside the function
std::mt19937 generator(static_cast<unsigned>(std::time(0)));
bool initialized = false;

int generateRandomNumber(int max)
{
    // Seed the random number generator only if not already initialized
    if (!initialized)
    {
        // No need to set the seed here
        initialized = true;
    }

    // Define the distribution for integers between 0 and max
    std::uniform_int_distribution<int> distribution(0, max);

    // Generate a random number
    int random_number = distribution(generator);
    return random_number;
}
void swap(GAME *gameA, GAME *gameB)
{
    GAME temp = *gameA;
    *gameA = *gameB;
    *gameB = temp;
}

int main()
{
    FILE *gameFile = fopen("games.csv", "r");

    if (!gameFile)
    {
        // Print an error message and exit if the file cannot be opened
        printf("Error opening file \n");
        exit(EXIT_FAILURE);
    }

    LinkedList *gamesLinkedList = new LinkedList(NULL);

    char *line = NULL;
    size_t n = 0;
    ssize_t nbytes;

    // Read and skip the header line, which contains column names
    getline(&line, &n, gameFile);

    // Read and process each line of the abilities.csv file
    while ((nbytes = getline(&line, &n, gameFile)) > 0)
    {
        // Set the last character of the line to null terminator
        line[nbytes - 1] = 0;

        char *linep = line;
        // Create a data structure to represent each game.
        GAME *game = parseLine(linep);
        // Store game in linkedList
        Node *newGameNode = new Node(game);
        // If there exist any duplicates based on name, store one with the higher “user rating count” only.
        Node *duplicate = gamesLinkedList->findNodeByGameName(newGameNode->game->name);
        if (duplicate != NULL)
        {
            // printGameItem(duplicate->game);
            // printGameItem(newGameNode->game);
            if (newGameNode->game->user_rating_count > duplicate->game->user_rating_count)
            {
                // remove duplicate
                gamesLinkedList->remove(duplicate);
                // insert newGameNode
                gamesLinkedList->insertTail(newGameNode);
            }
        }
        else
        {
            gamesLinkedList->insertTail(newGameNode);
        }
    }

    // Make a copy to be used for quick sort
    LinkedList *gamesLinkedListCopyForQuickSort = copyLinkedList(gamesLinkedList);
    // Make a copy to be used for linear search test later
    LinkedList *gamesLinkedListCopyForLinearSearch = copyLinkedList(gamesLinkedList);

    
    cout << "Number of elements in LinkedList: " << gamesLinkedList->size << endl << endl;

    cout << "*** Linear Search Test ***" << endl << endl;

    cout << "Before sorting:" << endl << endl;

    for (int i = 0; i < 5; i++)
    {
        GAME *game = new GAME;
        game = gamesLinkedList->getGameByIndex(i)->game;
        cout << "# " << game->id << ", " << game->name << ", " << game->avg_user_rating << ", " << game->user_rating_count << ", " << game->developer << ", " << game->size << endl;
    }

    cout << endl;


    Node *selectedGame1 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame2 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame3 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));
    Node *selectedGame4 = gamesLinkedList->getGameByIndex(generateRandomNumber(gamesLinkedList->size));


    cout << "Search number 1:" << endl;
    cout << "Searching for " << selectedGame1->game->name << "..." << endl;

    int linearSearchTime1 = getTimeForLinearSearch(selectedGame1, gamesLinkedList);
    
    cout << "Single search time: " << linearSearchTime1 << " nanoseconds." << endl;

    int totalLinearSearchTime1 = 0;
    for (int i = 0; i < 10; i++)
    {
        // int timeSpenti = getTimeForLinearSearch(selectedGame1, gamesLinkedList);
        totalLinearSearchTime1 = totalLinearSearchTime1 + getTimeForLinearSearch(selectedGame1, gamesLinkedList);;
    }

    int averageLinearSearchTime1 = (totalLinearSearchTime1 + linearSearchTime1) / 11;
    cout << "Average search time: " << averageLinearSearchTime1 << " nanoseconds." << endl << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << selectedGame2->game->name << "..." << endl;

    int linearSearchTime2 = getTimeForLinearSearch(selectedGame2, gamesLinkedList);

    cout << "Single search time: " << linearSearchTime2 << " nanoseconds." << endl;

    int totalLinearSearchTime2 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalLinearSearchTime2 = totalLinearSearchTime2 + getTimeForLinearSearch(selectedGame2, gamesLinkedList);;
    }

    int averageLinearSearchTime2 = (totalLinearSearchTime2 + linearSearchTime2) / 11;

    cout << "Average search time: " << averageLinearSearchTime2 << " nanoseconds." << endl << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << selectedGame3->game->name << "..." << endl;

    int linearSearchTime3 = getTimeForLinearSearch(selectedGame3, gamesLinkedList);

    cout << "Single search time: " << linearSearchTime3 << " nanoseconds." << endl;

    int totalLinearSearchTime3 = 0;   
    for (int i = 0; i < 10; i++)
    {
        int timeSpenti = getTimeForLinearSearch(selectedGame3, gamesLinkedList);
        totalLinearSearchTime3 = totalLinearSearchTime3 + timeSpenti;
    }
    int averageBeforeSort3 = (totalLinearSearchTime3 + linearSearchTime3) / 11;
    cout << "Average search time: " << averageBeforeSort3 << " nanoseconds." << endl << endl;

    // Record the start time for insertion sort
    struct timespec insertion_sort_start, insertion_sort_end;
    clock_gettime(CLOCK_REALTIME, &insertion_sort_start);

    // Perform insertion sort
    gamesLinkedList->insertionSort();

    // Record the end time for insertion sort
    clock_gettime(CLOCK_REALTIME, &insertion_sort_end);

    // Calculate and print the time taken for insertion sort
    long long insertion_sort_time = (insertion_sort_end.tv_sec - insertion_sort_start.tv_sec) * NANOSECONDS_PER_SECOND + (insertion_sort_end.tv_nsec - insertion_sort_start.tv_nsec);
    cout << "Insertion Sort Time: " << insertion_sort_time << " nanoseconds" << endl;

    // Record the start time for quicksort
    struct timespec quicksort_start, quicksort_end;
    clock_gettime(CLOCK_REALTIME, &quicksort_start);

    // Perform quicksort on a copy of the list
    gamesLinkedListCopyForQuickSort->quickSort();

    // Record the end time for quicksort
    clock_gettime(CLOCK_REALTIME, &quicksort_end);

    // Calculate and print the time taken for quicksort
    long long quicksort_time = (quicksort_end.tv_sec - quicksort_start.tv_sec) * NANOSECONDS_PER_SECOND + (quicksort_end.tv_nsec - quicksort_start.tv_nsec);
    cout << "Quick Sort Time: " << quicksort_time << " nanoseconds" << endl << endl;
    cout << "After sorting:" << endl <<endl;

    for (int i = 0; i < 5; i++)
    {
        GAME *game = new GAME;

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

    cout << "Search number 1:" << endl;
    cout << "Searching for " << selectedGame1->game->name << "..." << endl;

    int binarySearchTime1 = getTimeForBinarySearch(selectedGame1, bst);

    cout << "Single search time: " << binarySearchTime1 << " nanoseconds." << endl;

    int totalBinarySearchTime1 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime1 = totalBinarySearchTime1 + getTimeForBinarySearch(selectedGame1, bst);
    }

    int averageBinarySearchTime1 = (totalBinarySearchTime1 + binarySearchTime1) / 11;
    cout << "Average search time: " << averageBinarySearchTime1 << " nanoseconds." << endl << endl;

    cout << "Search number 2:" << endl;
    cout << "Searching for " << selectedGame2->game->name << "..." << endl;

    int binarySearchTime2 = getTimeForBinarySearch(selectedGame2, bst);

    cout << "Single search time: " << binarySearchTime2 << " nanoseconds." << endl;
    
    int totalBinarySearchTime2 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime2 = totalBinarySearchTime2 + getTimeForBinarySearch(selectedGame2, bst);
    }

    int averageBinarySearchTime2 = (totalBinarySearchTime2 + binarySearchTime2) / 11;
    cout << "Average search time: " << averageBinarySearchTime2 << " nanoseconds." << endl << endl;

    cout << "Search number 3:" << endl;
    cout << "Searching for " << selectedGame3->game->name << "..." << endl;

    int binarySearchTime3 = getTimeForBinarySearch(selectedGame3, bst);

    cout << "Single search time: " << binarySearchTime2 << " nanoseconds." << endl;

    int totalBinarySearchTime3 = 0;
    for (int i = 0; i < 10; i++)
    {
        totalBinarySearchTime3 = totalBinarySearchTime3 + getTimeForBinarySearch(selectedGame3, bst);
    }
    int averageAfterSort3 = (totalBinarySearchTime3 + binarySearchTime2) / 11;
    cout << "Average search time: " << averageAfterSort3 << " nanoseconds." << endl << endl;

   //Break point

    int linearSearchTime4 = getTimeForLinearSearch(selectedGame4, gamesLinkedList);
    int binarySearchTime4 = getTimeForBinarySearch(selectedGame4, bst);

    cout << "Single linear search time : " << linearSearchTime4 << " nanoseconds." << endl;
    cout << "Single binary search time: " << binarySearchTime4 << " nanoseconds." << endl;

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
