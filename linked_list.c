#include "linked_list.h"
#include <stdlib.h>

// Utility function to create a new node
struct list_node* new_node(size_t value) {
    struct list_node* node = (struct list_node*)malloc(sizeof(struct list_node));
    if (node == NULL) {
        // Memory allocation failed, return NULL
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

// Insert a node at the head of the list
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node* new_node_ptr = new_node(value);  // Renamed to avoid conflict with function name
    if (new_node_ptr == NULL) {
        return;
    }
    new_node_ptr->next = list->head;
    list->head = new_node_ptr;
}

// Insert a node at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node* new_node_ptr = new_node(value);  // Renamed to avoid conflict with function name
    if (new_node_ptr == NULL) {
        return;
    }

    if (list->head == NULL) {
        // If the list is empty, the new node becomes the head
        list->head = new_node_ptr;
    } else {
        // Otherwise, traverse to the tail and insert
        struct list_node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node_ptr;
    }
}

// Remove a node from the head of the list
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        // List is empty, return an error code (e.g., SIZE_MAX)
        return (size_t)-1;  // Return an error code or sentinel value
    }
    struct list_node* temp = list->head;
    size_t value = temp->value;
    list->head = list->head->next;
    free(temp);
    return value;
}

// Remove a node from the tail of the list
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        // List is empty, return an error code (e.g., SIZE_MAX)
        return (size_t)-1;  // Return an error code or sentinel value
    }

    // If there's only one node in the list
    if (list->head->next == NULL) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }

    // Otherwise, traverse to the second-to-last node
    struct list_node* current = list->head;
    while (current->next && current->next->next) {
        current = current->next;
    }

    size_t value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

// Free the entire list
int is_visited(struct list_node *node, struct list_node *visited) {
    struct list_node *current = visited;
    while (current != NULL) {
        if (current == node) {
            return 1;  // Node is already visited
        }
        current = current->next;
    }
    return 0;  // Node is not visited
}

// Dump the list while avoiding cycles (infinite loops)
void dump_list(FILE *fp, struct linked_list list) {
    struct list_node *visited = NULL;  // List to track visited nodes
    struct list_node *current = list.head;
    
    fprintf(fp, "[ ");
    while (current != NULL) {
        if (is_visited(current, visited)) {
            fprintf(fp, "Cycle detected! ");
            break;
        }
        fprintf(fp, "%zu ", current->value);
        
        // Mark the current node as visited by adding it to the visited list
        insert_at_tail(&visited, current);
        
        current = current->next;
    }
    fprintf(fp, "]\n");
    
    // Free the visited list to avoid memory leak
    free_list(visited);
}

// Free the entire list while avoiding cycles
void free_list(struct linked_list list) {
    struct list_node *visited = NULL;  // List to track visited nodes
    struct list_node *current = list.head;
    
    while (current != NULL) {
        if (is_visited(current, visited)) {
            break;  // Cycle detected, break out
        }
        
        // Mark the current node as visited by adding it to the visited list
        insert_at_tail(&visited, current);
        
        struct list_node *temp = current;
        current = current->next;
        free(temp);  // Free the current node
    }
    
    // Free the visited list to avoid memory leak
    free_list(visited);
}
