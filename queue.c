#include "queue.h"
#include "tile_game.h"

// Function to enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    size_t serialized_state = serialize(state);
    insert_at_tail(&q->data, serialized_state);
}

// Function to dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (q->data.head == NULL) {
        struct game_state invalid_state;
        invalid_state.tiles[0][0] = 0;  // Mark as invalid state
        return invalid_state;
    }

    size_t serialized_state = remove_from_head(&q->data);
    return deserialize(serialized_state);
}

// Function to check if the puzzle is solved
bool is_solved(struct game_state state) {
    uint8_t solved[4][4] = {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 },
        { 13, 14, 15, 0 }
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state.tiles[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to calculate the number of possible moves
int num_possible_moves(struct game_state state) {
    int possible_moves = 0;
    if (state.empty_row > 0) possible_moves++;  // Can move up
    if (state.empty_row < 3) possible_moves++;  // Can move down
    if (state.empty_col > 0) possible_moves++;  // Can move left
    if (state.empty_col < 3) possible_moves++;  // Can move right
    return possible_moves;
}

// Function to generate the next game state based on a move
struct game_state make_move(struct game_state state, int move) {
    struct game_state next_state = state;

    switch (move) {
        case 0:
            if (state.empty_row > 0) {
                next_state.tiles[state.empty_row][state.empty_col] = state.tiles[state.empty_row - 1][state.empty_col];
                next_state.tiles[state.empty_row - 1][state.empty_col] = 0;
                next_state.empty_row--;
                next_state.num_steps++;
            }
            break;
        case 1:
            if (state.empty_row < 3) {
                next_state.tiles[state.empty_row][state.empty_col] = state.tiles[state.empty_row + 1][state.empty_col];
                next_state.tiles[state.empty_row + 1][state.empty_col] = 0;
                next_state.empty_row++;
                next_state.num_steps++;
            }
            break;
        case 2:
            if (state.empty_col > 0) {
                next_state.tiles[state.empty_row][state.empty_col] = state.tiles[state.empty_row][state.empty_col - 1];
                next_state.tiles[state.empty_row][state.empty_col - 1] = 0;
                next_state.empty_col--;
                next_state.num_steps++;
            }
            break;
        case 3:
            if (state.empty_col < 3) {
                next_state.tiles[state.empty_row][state.empty_col] = state.tiles[state.empty_row][state.empty_col + 1];
                next_state.tiles[state.empty_row][state.empty_col + 1] = 0;
                next_state.empty_col++;
                next_state.num_steps++;
            }
            break;
    }

    return next_state;
}

// Function to calculate the number of moves needed to solve the puzzle
int number_of_moves(struct game_state start) {
    struct queue q;
    q.data.head = NULL;  // Initialize an empty queue (linked list)
    
    // Enqueue the start state with 0 moves
    enqueue(&q, start);

    // Create a visited set (array) to track visited states
    // We'll use 0 for false and 1 for true
    int visited[1 << 15] = {0}; // We need to track up to 2^15 possible states (15 bits for serialized state)
    
    // Serialize the start state and mark it as visited
    size_t serialized_start = serialize(start);
    visited[serialized_start] = 1;

    while (q.data.head != NULL) {
        // Dequeue the current state
        struct game_state current_state = dequeue(&q);

        // Check if the current state is the goal state (solved puzzle)
        if (is_solved(current_state)) {
            return current_state.num_steps; // Return the number of moves when the goal is found
        }

        // Generate possible moves and enqueue them
        for (int i = 0; i < num_possible_moves(current_state); i++) {
            struct game_state next_state = make_move(current_state, i); // Generate next state

            // Serialize the next state and check if it has already been visited
            size_t serialized_next = serialize(next_state);
            if (visited[serialized_next] == 0) { // If not visited
                // Mark the state as visited
                visited[serialized_next] = 1;

                // Enqueue the new state
                enqueue(&q, next_state);
            }
        }
    }

    // If the goal state is not reachable (shouldn't happen in a solvable puzzle)
    return -1;  // Return -1 indicating failure or unreachable state
}
