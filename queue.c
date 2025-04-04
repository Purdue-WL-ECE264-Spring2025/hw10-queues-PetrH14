
#include "queue.h"
#include "tile_game.h"    // Include tile_game.h for game-related functions
#include <stdlib.h>        // For malloc and free
#include "linked_list.h"   // For struct list_node and linked list functions

// Check if the current state is solved (goal state)
int is_solved(struct game_state *state) {
    uint8_t goal_state[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    // Compare the current state with the goal state
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state->tiles[i][j] != goal_state[i][j]) {
                return 0; // Not solved
            }
        }
    }
    return 1; // Solved
}

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

    // While the queue is not empty
    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // If the current state is solved, stop processing
        if (is_solved(&current_state)) {
            return num_moves; // Return the number of moves when solved
        }

        num_moves++; // Increment the move count

        // Print the state for debugging (optional)
        printf("Move %d: ", num_moves);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%d ", current_state.tiles[i][j]);
            }
            printf("\n");
        }

        // Here you can add the logic to generate possible moves and enqueue them.
        // But for now, it will just stop when it encounters the solved state.
    }

    return num_moves; // Return the number of moves processed
}
