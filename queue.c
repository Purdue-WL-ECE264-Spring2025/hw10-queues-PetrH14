#include "queue.h"
#include "tile_game.h"  // Including tile_game.h for game-related functions
#include <stdlib.h>      // Needed for malloc() and free()

// Enqueue a new state into the queue
void enqueue(struct queue *q, struct game_state state) {
    // Serialize the game state to store it in the linked list
    uint64_t serialized_state = serialize(state);

    // Create a new node for the linked list
    struct linked_list_node *new_node = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));
    if (new_node == NULL) {
        // Handle memory allocation failure (optional)
        return;
    }
    
    new_node->data = serialized_state;  // Store the serialized state
    new_node->next = NULL;

    // If the queue is empty, the new node will be the first element
    if (q->data.head == NULL) {
        q->data.head = new_node;
    } else {
        // Otherwise, append to the end of the list
        struct linked_list_node *temp = q->data.head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Dequeue a state from the queue
struct game_state dequeue(struct queue *q) {
    // Check if the queue is empty
    if (q->data.head == NULL) {
        struct game_state invalid_state = {{0}}; // Return an invalid state if the queue is empty
        return invalid_state;
    }

    // Remove the first node from the linked list
    struct linked_list_node *node_to_remove = q->data.head;
    q->data.head = node_to_remove->next;

    // Deserialize the serialized state from the node
    struct game_state state = deserialize(node_to_remove->data);
    
    // Free the removed node
    free(node_to_remove);

    return state;
}

// Number of moves function (example implementation)
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    q.data.head = NULL;  // Initialize empty queue

    // Enqueue the starting state (assuming start is properly initialized)
    enqueue(&q, start);

    // Example process for tracking the number of moves
    int num_moves = 0;

    // Assuming a maximum depth or number of moves to avoid infinite loops
    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // Check if the goal is reached (this needs your own is_solved function)
        if (is_solved(current_state)) {
            return num_moves;
        }

        // Add possible next states (you'll need to implement num_possible_moves and make_move)
        for (int i = 0; i < num_possible_moves(current_state); i++) {
            struct game_state next_state = make_move(current_state, i);
            enqueue(&q, next_state);
        }

        num_moves++;
    }

    // If the goal is not found
    return -1;
}
