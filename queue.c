#include "queue.h"
#include "tile_game.h"

// Define the maximum size of the queue (adjust as needed)
#define MAX_QUEUE_SIZE 1024

// Initialize the queue (reset front and rear)
void init_queue(struct queue *q) {
    q->front = 0;
    q->rear = 0;
}

// Check if the queue is empty
int is_empty(struct queue *q) {
    return q->front == q->rear;
}

// Check if the queue is full
int is_full(struct queue *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// Enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    size_t serialized_state = serialize(state);

    // Check if there’s space in the queue
    if (!is_full(q)) {
        q->data[q->rear] = serialized_state;
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;  // Wrap around if necessary
    }
}

// Dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (is_empty(q)) {
        // If the queue is empty, return an invalid game state (perhaps a sentinel value)
        struct game_state invalid_state = {0};
        return invalid_state;
    }

    // Dequeue the state and increment the front index
    size_t serialized_state = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;

    // Deserialize the state and return it
    struct game_state state = deserialize(serialized_state);
    return state;
}

// Function to check if the puzzle is solved (implement the logic as needed)
int is_solved(struct game_state state) {
    int solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}  // 0 represents the empty tile
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state.tiles[i][j] != solved[i][j]) {
                return 0;  // Not solved
            }
        }
    }
    return 1;  // Solved
}

// Function to return the number of possible moves (implement as needed)
int num_possible_moves(struct game_state state) {
    int possible_moves = 0;
    int row = state.empty_row;
    int col = state.empty_col;

    // Check the four directions: up, down, left, right
    if (row > 0) possible_moves++;  // Can move up
    if (row < 3) possible_moves++;  // Can move down
    if (col > 0) possible_moves++;  // Can move left
    if (col < 3) possible_moves++;  // Can move right

    return possible_moves;
}

// Function to make a move and return the new state (implement as needed)
struct game_state make_move(struct game_state state, int move) {
    struct game_state new_state = state;
    int row = state.empty_row;
    int col = state.empty_col;

    // Perform the move based on the input (0 = up, 1 = down, 2 = left, 3 = right)
    switch (move) {
        case 0:  // Move up
            if (row > 0) {
                new_state.tiles[row][col] = state.tiles[row - 1][col];
                new_state.tiles[row - 1][col] = 0;
                new_state.empty_row--;
            }
            break;
        case 1:  // Move down
            if (row < 3) {
                new_state.tiles[row][col] = state.tiles[row + 1][col];
                new_state.tiles[row + 1][col] = 0;
                new_state.empty_row++;
            }
            break;
        case 2:  // Move left
            if (col > 0) {
                new_state.tiles[row][col] = state.tiles[row][col - 1];
                new_state.tiles[row][col - 1] = 0;
                new_state.empty_col--;
            }
            break;
        case 3:  // Move right
            if (col < 3) {
                new_state.tiles[row][col] = state.tiles[row][col + 1];
                new_state.tiles[row][col + 1] = 0;
                new_state.empty_col++;
            }
            break;
    }

    // Increment the step counter
    new_state.num_steps = state.num_steps + 1;

    return new_state;
}

// Function to calculate the number of moves needed to solve the puzzle (BFS)
int number_of_moves(struct game_state start) {
    struct queue q;
    init_queue(&q);  // Initialize the queue

    // Enqueue the start state with 0 moves
    enqueue(&q, start);

    // While there are states in the queue, continue processing
    while (!is_empty(&q)) {
        // Dequeue the current state
        struct game_state current_state = dequeue(&q);

        // Check if the current state is the goal state (solved puzzle)
        if (is_solved(current_state)) {
            return current_state.num_steps;  // Return the number of moves when the goal is found
        }

        // Generate possible moves and enqueue them
        for (int i = 0; i < num_possible_moves(current_state); i++) {
            struct game_state next_state = make_move(current_state, i);  // Generate next state

            // Enqueue the next state
            enqueue(&q, next_state);
        }
    }

    // If the goal state is not reachable (shouldn't happen in a solvable puzzle)
    return -1;  // Return -1 indicating failure or unreachable state
}
