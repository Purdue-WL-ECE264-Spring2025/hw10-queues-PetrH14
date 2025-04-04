#include "queue.h"
#include "tile_game.h"  // Including tilegame.h for game-related functions

#define MAX_QUEUE_SIZE 1024

// Queue structure, containing data and tracking front/rear positions
static uint64_t data[MAX_QUEUE_SIZE];
static size_t front = 0;  // To track the front of the queue
static size_t rear = 0;   // To track the rear of the queue

// Initialize the queue
void init_queue(struct queue* q) {
    q->front = 0;
    q->rear = 0;
}

// Check if the queue is empty
int is_empty(struct queue* q) {
    return q->front == q->rear;
}

// Check if the queue is full
int is_full(struct queue* q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// Enqueue a new state into the queue
void enqueue(struct queue* q, struct game_state state) {
    if (is_full(q)) {
        // If the queue is full, return or handle the error
        return;
    }

    // Serialize the game state and store it in the queue
    uint64_t serialized_state = serialize(state);
    data[q->rear] = serialized_state;  // Insert the serialized state at the rear of the queue
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;  // Wrap around if necessary
}

// Dequeue a state from the queue
struct game_state dequeue(struct queue* q) {
    if (is_empty(q)) {
        // If the queue is empty, return an invalid state or handle the error
        struct game_state invalid_state;
        invalid_state.num_steps = -1; // Or some other way to indicate an invalid state
        return invalid_state;
    }

    // Get the serialized state from the front of the queue
    uint64_t serialized_state = data[q->front];
    
    // Deserialize the serialized state to get the game state
    struct game_state state = deserialize(serialized_state);

    // Update the front of the queue
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;  // Wrap around if necessary

    return state;
}

// Number of moves function (example implementation)
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    init_queue(&q);

    // Enqueue the starting state (assuming start is properly initialized)
    enqueue(&q, start);

    // Example process for tracking the number of moves
    int num_moves = 0;

    // Assuming a maximum depth or number of moves to avoid infinite loops
    while (!is_empty(&q)) {
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
