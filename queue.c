#include "queue.h"
#include "tile_game.h"    // Include tile_game.h for game-related functions
#include <stdlib.h>        // For malloc and free
#include "linked_list.h"   // For struct list_node and linked list functions
#include <stdint.h>        // For uint64_t type
#include <stdio.h>         // For printf (for debugging)

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

    // Create a set to keep track of visited states (serialized states)
    // This will prevent revisiting the same state and causing an infinite loop
    struct linked_list visited;
    visited.head = NULL;

    // Add the serialized starting state to the visited list
    uint64_t serialized_start = serialize(start);
    insert_at_tail(&visited, serialized_start);  // Add to visited list

    // Track the number of moves
    int num_moves = 0;

    // While the queue is not empty
    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // If the current state is solved, return the number of moves
        if (is_solved(&current_state)) {
            return num_moves;
        }

        num_moves++; // Increment the move count

        // Find the position of the empty tile (0)
        int empty_row = -1, empty_col = -1;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (current_state.tiles[i][j] == 0) {
                    empty_row = i;
                    empty_col = j;
                    break;
                }
            }
            if (empty_row != -1) break;
        }

        // Try moving the empty tile in all four possible directions
        struct game_state next_state;

        // Move up
        if (empty_row > 0) {
            next_state = current_state;
            // Swap the empty tile with the tile above it
            next_state.tiles[empty_row][empty_col] = next_state.tiles[empty_row - 1][empty_col];
            next_state.tiles[empty_row - 1][empty_col] = 0;

            uint64_t serialized_state = serialize(next_state);
            if (!contains(visited, serialized_state)) {
                enqueue(&q, next_state);
                insert_at_tail(&visited, serialized_state);  // Add to visited list
            }
        }

        // Move down
        if (empty_row < 3) {
            next_state = current_state;
            // Swap the empty tile with the tile below it
            next_state.tiles[empty_row][empty_col] = next_state.tiles[empty_row + 1][empty_col];
            next_state.tiles[empty_row + 1][empty_col] = 0;

            uint64_t serialized_state = serialize(next_state);
            if (!contains(visited, serialized_state)) {
                enqueue(&q, next_state);
                insert_at_tail(&visited, serialized_state);  // Add to visited list
            }
        }

        // Move left
        if (empty_col > 0) {
            next_state = current_state;
            // Swap the empty tile with the tile to the left
            next_state.tiles[empty_row][empty_col] = next_state.tiles[empty_row][empty_col - 1];
            next_state.tiles[empty_row][empty_col - 1] = 0;

            uint64_t serialized_state = serialize(next_state);
            if (!contains(visited, serialized_state)) {
                enqueue(&q, next_state);
                insert_at_tail(&visited, serialized_state);  // Add to visited list
            }
        }

        // Move right
        if (empty_col < 3) {
            next_state = current_state;
            // Swap the empty tile with the tile to the right
            next_state.tiles[empty_row][empty_col] = next_state.tiles[empty_row][empty_col + 1];
            next_state.tiles[empty_row][empty_col + 1] = 0;

            uint64_t serialized_state = serialize(next_state);
            if (!contains(visited, serialized_state)) {
                enqueue(&q, next_state);
                insert_at_tail(&visited, serialized_state);  // Add to visited list
            }
        }
    }

    return num_moves; // Return the number of moves processed
}
