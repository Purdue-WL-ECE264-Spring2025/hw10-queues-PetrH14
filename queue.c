#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include "linked_list.h"
#include <stdint.h>
#include <stdio.h>

// Check if a state has already been visited by checking the visited list
int is_visited(struct linked_list *visited, uint64_t state) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        if (current->value == state) {
            return 1; // State has been visited
        }
        current = current->next;
    }
    return 0; // State has not been visited
}
2. Update the queue.c Code
With the is_visited function in place, the number_of_moves function should work as expected. The modified queue.c should look like this:

c
Copy
#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include "linked_list.h"
#include <stdint.h>
#include <stdio.h>

// Check if the state is solved by comparing the tiles with the target solved state
int is_solved(struct game_state *state) {
    int correct[4][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
        {13, 14, 15,  0} // 0 is the empty space
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state->tiles[row][col] != correct[row][col]) {
                return 0; // Not solved
            }
        }
    }

    return 1; // Solved
}

// Check if a state has already been visited by checking the visited list
int is_visited(struct linked_list *visited, uint64_t state) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        if (current->value == state) {
            return 1; // State has been visited
        }
        current = current->next;
    }
    return 0; // State has not been visited
}

// Number of moves function with state exploration
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    q.data.head = NULL;  // Initialize empty queue

    // Enqueue the starting state
    enqueue(&q, start);

    // Create a set to keep track of visited states (serialized states)
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
