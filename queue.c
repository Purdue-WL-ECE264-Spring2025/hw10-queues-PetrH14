#include "queue.h"
#include "tile_game.h"

// Function to enqueue a game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    // Serialize the game state into an integer
    size_t serialized_state = serialize(state);

    // Insert the serialized state at the tail of the linked list
    insert_at_tail(&q->data, serialized_state);
}

// Function to dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (q->data.head == NULL) {
        // If the queue is empty, return an invalid game state (perhaps a sentinel value)
        struct game_state invalid_state;
        invalid_state.board = NULL; // Set this to NULL or an invalid state representation
        return invalid_state;
    }

    // Remove the serialized state from the head of the list
    size_t serialized_state = remove_from_head(&q->data);

    // Deserialize the integer back into a game state
    struct game_state state = deserialize(serialized_state);

    return state;
}

// Function to calculate the number of moves needed to solve the puzzle (bfs or some heuristic)
int number_of_moves(struct game_state start) {
    // Assuming this function uses BFS (Breadth-First Search) to determine the shortest path
    // to the solved state. You can implement BFS here, utilizing the queue for traversal.

    struct queue q;
    q.data.head = NULL;  // Initialize an empty queue (linked list)
    
    // Enqueue the start state with 0 moves
    enqueue(&q, start);

    // Create a set or list to track visited states (you can use a hash table or array)
    // This part is pseudo-code to illustrate:
    // initialize_visited_set();

    while (q.data.head != NULL) {
        // Dequeue the current state
        struct game_state current_state = dequeue(&q);

        // Check if the current state is the goal state (solved puzzle)
        if (is_solved(current_state)) {
            return current_state.moves; // Return the number of moves when the goal is found
        }

        // Generate possible moves and enqueue them
        // For each possible move, create a new game state
        for (int i = 0; i < num_possible_moves(current_state); i++) {
            struct game_state next_state = make_move(current_state, i); // Generate next state

            // If this state hasn't been visited, enqueue it
            // if (!visited(next_state)) {
            enqueue(&q, next_state);
            // }

            // Update visited set (add next_state to the visited list)
            // visited_add(next_state);
        }
    }

    // If the goal state is not reachable (shouldn't happen in a solvable puzzle)
    return -1;  // Return -1 indicating failure or unreachable state
}
