#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

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
void insert_at_head(struct linked_list* list, size_t value) {
    struct list_node* new_node_ptr = new_node(value);  // Renamed to avoid conflict with function name
    if (new_node_ptr == NULL) {
        return;
    }
    new_node_ptr->next = list->head;
    list->head = new_node_ptr;
}

// Insert a node at the tail of the list
void insert_at_tail(struct linked_list* list, size_t value) {
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
size_t remove_from_head(struct linked_list* list) {
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
size_t remove_from_tail(struct linked_list* list) {
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
void free_list(struct linked_list* list) {
    struct list_node* current = list->head;
    while (current != NULL) {
        struct list_node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;  // Set the head to NULL after freeing all nodes
}

// Check if a value exists in the list (i.e., if the state was visited)
int is_visited(struct linked_list* list, size_t value) {
    struct list_node* current = list->head;
    while (current != NULL) {
        if (current->value == value) {
            return 1;  // State already visited
        }
        current = current->next;
    }
    return 0;  // State not visited
}

// Utility function to dump the list for debugging purposes
void dump_list(FILE *fp, struct linked_list* list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list->head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
