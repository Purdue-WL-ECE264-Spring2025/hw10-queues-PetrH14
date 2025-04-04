#include "queue.h"
#include "tile_game.h"    // Include tile_game.h for game-related functions
#include <stdlib.h>        // For malloc and free
#include "linked_list.h"   // For struct list_node and linked list functions
#include <stdint.h>        // For uint64_t type
#include <stdio.h>         // For printf (for debugging)

// Check if the state already exists in the visited list
int is_visited(struct linked_list *visited, uint64_t state) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        if (current->value == state) {
            return 1;  // State is already visited
        }
        current = current->next;
    }
    return 0;  // State is not visited
}

// Number of moves function with state exploration
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
            if (!is_visited(&visited, serialized_state)) {
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
            if (!is_visited(&visited, serialized_state)) {
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
            if (!is_visited(&visited, serialized_state)) {
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
            if (!is_visited(&visited, serialized_state)) {
                enqueue(&q, next_state);
                insert_at_tail(&visited, serialized_state);  // Add to visited list
            }
        }
    }

    return num_moves; // Return the number of moves processed
} 
