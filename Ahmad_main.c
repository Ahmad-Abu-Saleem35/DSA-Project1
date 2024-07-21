//ahmad abu saleem
//1201315
//section:2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRING_LENGTH 30
//(is digit function)
// Defining the structure for a doubly linked list node
typedef struct Node {
    char data[MAX_STRING_LENGTH + 1];  // Holds the string data
    struct Node* next;  // Points to the next node in the list
    struct Node* prev;  // Points to the previous node in the list
} Node;
// Creating a shortcut name 'List' for the Node pointer.
typedef Node* List;
// Function to create a new node with the given string value
List create_node(char* value) {
    List new_node = (List)malloc(sizeof(Node));
    strcpy(new_node->data, value);
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

// Function to insert a new node at the end of the list
void insert_node(List* head, char* value) {
    List new_node = create_node(value);
    if (*head == NULL) {
        // If the list is empty, the new node becomes the first node
        *head = new_node;
    } else {
        // Moving through the list to locate the last node and adding the new node.
        List last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
        new_node->prev = last;
    }
}

// Function to print the strings in the list
void print_book(List book) {
    if (book == NULL) {
        printf("the List is empty.\n");
    } else {
        // Going through the list and printing each string.
        while (book != NULL) {
            printf("%s\n", book->data);
            book = book->next;
        }
    }
}

// Function to delete the entire list and free the memory
void delete_list(List* head) {
    while (*head != NULL) {
        List temp = *head;
        *head = (*head)->next;

        if (*head != NULL) {
            // If there's another node, update its previous pointer
            (*head)->prev = NULL;
        }

        free(temp);
    }
}

// Function to get the maximum string length in the list
int get_max_string_length(List head) {
    int max = 0;
    List current = head;
    while (current != NULL) {
        // Calculate the length of each string and find the maximum
        int len = strlen(current->data);
        if (len > max) {
            max = len;
        }
        current = current->next;
    }
    return max;
}

// Function to perform counting sort on the list based on a specific digit (exp)
void count_sort(List* head, int exp) {
    const int RADIX = 128;  // Representing ASCII characters
    // Memory allocation for an array to hold sorted elements
    List* output = (List*)malloc(RADIX * sizeof(List));
    if (output == NULL) {
        printf("Memory allocation failed for output array.\n");
        exit(EXIT_FAILURE);
    }

    // Initializing the output array
    for (int i = 0; i < RADIX; i++) {
        output[i] = NULL;
    }

   // Sorting strings by distributing them into buckets according to a specific digit.
    while (*head != NULL) {
        int index = (int)(*head)->data[exp];

        List temp = *head;
        *head = (*head)->next;

        temp->next = NULL;
        temp->prev = NULL;

        if (output[index] == NULL) {
            output[index] = temp;
        } else {
            List last = output[index];
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = temp;
            temp->prev = last;
        }
    }

// Putting the sorted strings back together into the main list.
    *head = NULL;
    for (int i = 0; i < RADIX; i++) {
        if (output[i] != NULL) {
            List last = output[i];
            while (last->next != NULL) {
                last = last->next;
            }

            if (*head == NULL) {
                *head = output[i];
            } else {
                List temp = *head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = output[i];
                output[i]->prev = temp;
            }

            last->next = NULL;
        }
    }

    // Freeing the memory allocated for the output array
    free(output);
}

// Function to perform radix sort on the list of strings
void radix_sort(List* head) {
    int max_len = get_max_string_length(*head);

    // Performing counting sort for each digit (character position) in reverse order
    for (int exp = max_len - 1; exp >= 0; exp--) {
        count_sort(head, exp);
    }
}

// Function to save the list to a file
void save_to_file(List head, const char* filename) {
    FILE* output = fopen(filename, "w");
    if (output == NULL) {
        printf("Error! Failed to open the output file.\n");
        return;
    }

    // Writing each element of the list to the file
    List current = head;
    while (current != NULL) {
        fprintf(output, "%s\n", current->data);
        current = current->next;
    }

    fclose(output);
}

int main() {
    List book = NULL;

    // the main loop for the program
    while (1) {
        printf("\nMenu:\n");
        printf("1. Load the strings\n");
        printf("2. Print the strings before sorting\n");
        printf("3. Sort the strings\n");
        printf("4. Print the sorted strings\n");
        printf("5. Add a new word to the list of sorted strings (and sort it)\n");
        printf("6. Delete a word from the sorted strings\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char filename[50];
                printf(" can you Enter the file please : ");
                scanf("%s", filename);
                FILE* input = fopen(filename, "r");
                if (input == NULL) {
                    printf("cant open the file.\n");
                    break;
                }

                char word[MAX_STRING_LENGTH + 1];
                while (fscanf(input, "%s", word) != EOF) {
                    // Checking if the word starts with a number
                    if (isdigit(word[0])) {
                        printf("Not allowed: The word '%s' starts with a number.\n", word);
                        continue;
                    }
                    // Inserting each word into the list
                    insert_node(&book, word);
                }
                fclose(input);
                printf("\n Words loaded.\n");
                break;
            }

            case 2:
                if (book == NULL) {
                    printf("List is empty so Load words first.\n");
                } else {
                    printf("Words before sorting:\n");
                    print_book(book);
                }
                break;

            case 3:
                if (book == NULL) {
                    printf("List is emptyso please Load words first.\n");
                } else {
                    // Sorting the words using radix sort
                    radix_sort(&book);
                    printf("Words sorted \n");
                }
                break;

            case 4:
                if (book == NULL) {
                    printf("List is empty so please Load words first.\n");
                } else {
                    printf("Sorted words:\n");
                    print_book(book);
                }
                break;

            case 5: {
                char new_word[MAX_STRING_LENGTH + 1];
                printf("Enter the new word to add: ");
                scanf("%s", new_word);
                // Checking if the new word starts with a number
                if (isdigit(new_word[0])) {
                    printf("Not allowed: The word '%s' starts with a number.\n", new_word);
                } else {
                    // Adding the new word to the list and resort the list
                    insert_node(&book, new_word);
                    radix_sort(&book);
                    printf("Word inserted, and the list has been re-sorted.\n");
                }
                break;
            }

            case 6: {
                char word_to_delete[MAX_STRING_LENGTH + 1];
                printf("Enter the word you want to delete it : ");
                scanf("%s", word_to_delete);

                // Finding and deleting word
                List current = book;
                while (current != NULL && strcmp(current->data, word_to_delete) != 0) {
                    current = current->next;
                }

                if (current == NULL) {
                    printf("Word not found.\n");
                } else {
                    if (current->prev == NULL) {
                        // If the word is at the beginning of the list
                        book = current->next;
                        if (current->next != NULL) {
                            current->next->prev = NULL;
                        }
                    } else {
                        // If the word is in the middle or at the end of the list
                        current->prev->next = current->next;
                        if (current->next != NULL) {
                            current->next->prev = current->prev;
                        }
                    }

                    // Freeing the memory occupied by the deleted node and re-sort the list
                    free(current);
                    radix_sort(&book);
                    printf("\n Word deleted.\n");
                }
                break;
            }

            case 7: {
                char output_filename[50];
                printf("Enter the output file please : ");
                scanf("%s", output_filename);

                // Saving the sorted list to the output file
                save_to_file(book, output_filename);
                printf("saved the list in the output file complete.\n");
                break;
            }

            case 8:
                // Deleting the entire list and exiting from the program
                delete_list(&book);
                printf("Exiting from the program.\n");
                return 0;

            default:
                printf("Invalid choice so can you Please enter a valid option ?.\n");
        }
    }

    return 0;
}
