#include "queue.h"
#include "tile_game.h"    // Include tile_game.h for game-related functions
#include <stdlib.h>        // For malloc and free
#include "linked_list.h"   // For struct list_node and linked list functions

// Enqueue a new state into the queue
void enqueue(struct queue *q, struct game_state state) {
    // Serialize the game state to store it in the linked list
    uint64_t serialized_state = serialize(state);

    // Create a new node for the linked list (using struct list_node)
    struct list_node *new_node = (struct list_node *)malloc(sizeof(struct list_node));
    if (new_node == NULL) {
        // Handle memory allocation failure (optional)
        return;
    }
    
    new_node->value = serialized_state;  // Store the serialized state
    new_node->next = NULL;

    // If the queue is empty, the new node will be the first element
    if (q->data.head == NULL) {
        q->data.head = new_node;
    } else {
        // Otherwise, append to the end of the list
        struct list_node *temp = q->data.head;
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
        struct game_state invalid_state = {0}; // Return an invalid state if the queue is empty
        return invalid_state;
    }

    // Remove the first node from the linked list
    struct list_node *node_to_remove = q->data.head;
    q->data.head = node_to_remove->next;

    // Deserialize the serialized state from the node
    struct game_state state = deserialize(node_to_remove->value);
    
    // Free the removed node
    free(node_to_remove);

    return state;
}

// Number of moves function (without is_solved, num_possible_moves, and make_move)
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    q.data.head = NULL;  // Initialize empty queue

    // Enqueue the starting state
    enqueue(&q, start);

    // Example process for tracking the number of moves
    int num_moves = 0;

    // While the queue is not empty (basic example, no solving logic)
    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // Just for demonstration: Print the current state and increment the move count
        // You can replace this with your own logic to process the state.
        printf("Move %d: ", num_moves);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%d ", current_state.tiles[i][j]);
            }
            printf("\n");
        }

        num_moves++; // Just an example of incrementing moves

        // If you need to add more logic here for processing states, you can.
        // For now, this loop just increments the move count and prints the state.
    }

    return num_moves; // Return number of moves processed
}
