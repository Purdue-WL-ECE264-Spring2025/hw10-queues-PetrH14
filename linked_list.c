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

// Insert a node at the head of the list and return the modified list
struct linked_list insert_at_head(struct linked_list list, size_t value) {
    struct list_node* new_node_ptr = new_node(value);  // Renamed to avoid conflict with function name
    if (new_node_ptr == NULL) {
        return list;  // Return unchanged list if memory allocation failed
    }
    new_node_ptr->next = list.head;
    list.head = new_node_ptr;
    return list;  // Return the modified list
}

// Insert a node at the tail of the list and return the modified list
struct linked_list insert_at_tail(struct linked_list list, size_t value) {
    struct list_node* new_node_ptr = new_node(value);  // Renamed to avoid conflict with function name
    if (new_node_ptr == NULL) {
        return list;  // Return unchanged list if memory allocation failed
    }

    if (list.head == NULL) {
        // If the list is empty, the new node becomes the head
        list.head = new_node_ptr;
    } else {
        // Otherwise, traverse to the tail and insert
        struct list_node* current = list.head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node_ptr;
    }
    return list;  // Return the modified list
}

// Remove a node from the head of the list and return the modified list
struct linked_list remove_from_head(struct linked_list list) {
    if (list.head == NULL) {
        // List is empty, return unchanged list
        return list;
    }
    struct list_node* temp = list.head;
    list.head = list.head->next;
    free(temp);
    return list;  // Return the modified list
}

// Remove a node from the tail of the list and return the modified list
struct linked_list remove_from_tail(struct linked_list list) {
    if (list.head == NULL) {
        // List is empty, return unchanged list
        return list;
    }

    // If there's only one node in the list
    if (list.head->next == NULL) {
        free(list.head);
        list.head = NULL;
        return list;  // Return the modified list
    }

    // Otherwise, traverse to the second-to-last node
    struct list_node* current = list.head;
    while (current->next && current->next->next) {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;
    return list;  // Return the modified list
}

// Free the entire list
void free_list(struct linked_list list) {
    struct list_node* current = list.head;
    while (current != NULL) {
        struct list_node* temp = current;
        current = current->next;
        free(temp);
    }
}

// Utility function to dump the list for debugging purposes
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
