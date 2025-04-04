#include "queue.h"
#include "tile_game.h"

#define MAX_QUEUE_SIZE 1024  // Define a fixed size for the queue
#define MAX_VISITED_STATES (1 << 20)  // Size for visited states (adjust as needed)

// Visited array to track states
int visited[MAX_VISITED_STATES] = {0};  

// Function to initialize the queue (assuming it's already defined in queue.h)
void init_queue(struct queue *q) {
    q->front = 0;
    q->rear = 0;
}

// Function to check if the queue is empty
int is_empty(struct queue *q) {
    return q->front == q->rear;
}

// Function to check if the queue is full
int is_full(struct queue *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// Function to enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    size_t serialized_state = serialize(state);

    // Check if the state has already been visited (prevent loops)
    if (visited[serialized_state] == 1) {
        // State already visited, skipping enqueue.
        return;
    }

    // Mark the state as visited
    visited[serialized_state] = 1;

    // Enqueue the state if there's space in the queue
    if (!is_full(q)) {
        q->data[q->rear] = serialized_state;
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    }
}

// Function to dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (is_empty(q)) {
        // If the queue is empty, return an invalid game state (perhaps a sentinel value)
        struct game_state invalid_state = {0};
        return invalid_state;
    }

    // Dequeue the state and increment the front index
    size_t serialized_state = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;

    // Deserialize the state
    struct game_state state = deserialize(serialized_state);
    return state;
}

// Function to calculate the number of moves needed to solve the puzzle (BFS or heuristic)
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

            // Enqueue the next state if it has not been visited yet
            enqueue(&q, next_state);
        }
    }

    // If the goal state is not reachable (shouldn't happen in a solvable puzzle)
    return -1;  // Return -1 indicating failure or unreachable state
}
