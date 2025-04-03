#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>

// Create and return a new node with the given value
struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node == NULL) {
        return NULL; // Memory allocation failed
    }
    node->value = value;
    node->next = NULL;
    return node;
}

// Insert a node at the head of the linked list
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *new_node_ptr = new_node(value);
    if (new_node_ptr == NULL) {
        return; // Failed to allocate memory
    }

    if (list->head == NULL) {
        // If the list is empty, the new node is both the head and tail
        list->head = new_node_ptr;
    } else {
        // Insert the new node at the head
        new_node_ptr->next = list->head;
        list->head = new_node_ptr;
    }
}

// Insert a node at the tail of the linked list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *new_node_ptr = new_node(value);
    if (new_node_ptr == NULL) {
        return; // Failed to allocate memory
    }

    if (list->head == NULL) {
        // If the list is empty, the new node is both the head and tail
        list->head = new_node_ptr;
    } else {
        // Traverse to the last node and insert the new node
        struct list_node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node_ptr;
    }
}

// Remove and return the value from the head of the linked list
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0; // Return 0 for empty list (no value to return)
    }

    struct list_node *old_head = list->head;
    size_t value = old_head->value;
    list->head = old_head->next;

    free(old_head);
    return value;
}

// Remove and return the value from the tail of the linked list
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        return 0; // Return 0 for empty list (no value to return)
    }

    if (list->head->next == NULL) {
        // If there's only one node in the list
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }

    // Traverse the list to find the second-to-last node
    struct list_node *current = list->head;
    while (current->next != NULL && current->next->next != NULL) {
        current = current->next;
    }

    // Remove the tail node
    size_t value = current->next->value;
    free(current->next);
    current->next = NULL;

    return value;
}

void free_list(struct linked_list *list) {
    while (list->head != NULL) {
        remove_from_head(list);
    }
}
// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
