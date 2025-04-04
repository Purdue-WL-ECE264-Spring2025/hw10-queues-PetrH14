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
void insert_at_head(struct linked_list* list, size_t value) {
    struct list_node* new_node = new_node(value);
    if (new_node == NULL) {
        return;
    }
    new_node->next = list->head;
    list->head = new_node;
}

// Insert a node at the tail of the list
void insert_at_tail(struct linked_list* list, size_t value) {
    struct list_node* new_node = new_node(value);
    if (new_node == NULL) {
        return;
    }

    if (list->head == NULL) {
        // If the list is empty, the new node becomes the head
        list->head = new_node;
    } else {
        // Otherwise, traverse to the tail and insert
        struct list_node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Remove a node from the head of the list
bool remove_from_head(struct linked_list* list, size_t* value) {
    if (list->head == NULL) {
        // List is empty, return false
        return false;
    }
    struct list_node* temp = list->head;
    *value = temp->value;
    list->head = list->head->next;
    free(temp);
    return true;
}

// Remove a node from the tail of the list
bool remove_from_tail(struct linked_list* list, size_t* value) {
    if (list->head == NULL) {
        // List is empty, return false
        return false;
    }

    // If there's only one node in the list
    if (list->head->next == NULL) {
        *value = list->head->value;
        free(list->head);
        list->head = NULL;
        return true;
    }

    // Otherwise, traverse to the second-to-last node
    struct list_node* current = list->head;
    while (current->next && current->next->next) {
        current = current->next;
    }

    *value = current->next->value;
    free(current->next);
    current->next = NULL;
    return true;
}

// Free the entire list
void free_list(struct linked_list* list) {
    struct list_node* current = list->head;
    while (current != NULL) {
        struct list_node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

// Utility function to dump the list for debugging purposes
void dump_list(FILE *fp, const struct linked_list* list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list->head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
