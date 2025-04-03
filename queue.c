#include "queue.h"
#include "tile_game.h"
#define MAX_QUEUE_SIZE 1024  // Maximum size of the queue (adjustable)

// Queue structure
struct queue {
    uint64_t data[MAX_QUEUE_SIZE];  // Array to hold serialized game states
    int front;                      // Index of the front element in the queue
    int rear;                       // Index of the rear element in the queue
    int size;                       // Current number of elements in the queue
};

// Initialize the queue
void init_queue(struct queue *q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

// Check if the queue is empty
int is_empty(struct queue *q) {
    return q->size == 0;
}

// Check if the queue is full
int is_full(struct queue *q) {
    return q->size == MAX_QUEUE_SIZE;
}

// Enqueue function: Adds a game_state to the queue by serializing it
void enqueue(struct queue *q, struct game_state state) {
    // Check if the queue is full
    if (is_full(q)) {
        return;  // Can't enqueue if the queue is full
    }

    // Serialize the game_state to an integer and add it to the rear of the queue
    q->data[q->rear] = serialize(state);
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;  // Move rear to the next position (circular)
    q->size++;
}

// Dequeue function: Removes and returns the first game_state from the queue
struct game_state dequeue(struct queue *q) {
    // Check if the queue is empty
    if (is_empty(q)) {
        return (struct game_state){0};  // Return an empty game state if the queue is empty
    }

    // Get the serialized state from the front of the queue
    uint64_t serialized_state = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;  // Move front to the next position (circular)
    q->size--;

    // Deserialize the serialized state back into a game_state structure
    return deserialize(serialized_state);
}

// Helper function: Perform BFS to find the number of moves required to solve the puzzle
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    init_queue(&q);  // Initialize the queue with no elements

    // Enqueue the starting state
    enqueue(&q, start);

    // Set of visited states (using a simple array for simplicity)
    uint64_t visited[MAX_QUEUE_SIZE] = {0};  // Array to track visited states

    // Perform BFS
    while (!is_empty(&q)) {
        struct game_state current_state = dequeue(&q);  // Dequeue the next state

        // Check if we've reached the goal state (solved puzzle)
        if (is_solved(current_state)) {
            return current_state.num_steps;  // Return the number of steps taken
        }

        // Generate the possible next moves (up, down, left, right)
        struct game_state next_states[4] = {
            current_state,  // Initialize with the current state
        };

        // Make possible moves (assuming we are modifying the current state within each function)
        move_up(&next_states[0]);
        move_down(&next_states[1]);
        move_left(&next_states[2]);
        move_right(&next_states[3]);

        // Enqueue valid and not visited states
        for (int i = 0; i < 4; i++) {
            if (is_new_state(next_states[i], visited)) {
                enqueue(&q, next_states[i]);  // Enqueue the new state
                insert_to_visited(visited, next_states[i]);  // Mark as visited
            }
        }
    }

    // Return -1 if no solution is found (in case the puzzle is unsolvable)
    return -1;
}

// Utility function to check if the state is solved (target configuration)
bool is_solved(struct game_state state) {
    int target[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}  // The empty space (0) in the bottom-right corner
    };

    // Compare the current state to the target state
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state.tiles[row][col] != target[row][col]) {
                return false;  // Return false if any tile is out of place
            }
        }
    }

    return true;  // Return true if the board matches the target state
}

// Check if a state has been visited (by comparing serialized states)
bool is_new_state(struct game_state state, uint64_t *visited) {
    size_t serialized_state = serialize(state);

    // Traverse the visited states array to check if the state is already visited
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (visited[i] == serialized_state) {
            return false;  // Return false if the state is already visited
        }
    }

    return true;  // Return true if the state has not been visited
}
void insert_to_visited(uint64_t *visited, struct game_state state) {
    size_t serialized_state = serialize(state);
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (visited[i] == 0) {  // Find an empty spot in the visited array
            visited[i] = serialized_state;
            break;
        }
    }
}
