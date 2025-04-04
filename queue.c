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

// Check if a state has been visited (helps avoid revisiting states)
int is_visited(struct linked_list *visited, uint64_t state) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        if (current->value == state) {
            return 1; // State already visited
        }
        current = current->next;
    }
    return 0; // State not visited
}

// Add a state to the visited list
void add_to_visited(struct linked_list *visited, uint64_t state) {
    insert_at_tail(visited, state);  // Insert serialized state at the tail of the visited list
}

// Free the visited list and its nodes
void free_visited(struct linked_list *visited) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        struct list_node *to_free = current;
        current = current->next;
        free(to_free);  // Free each node
    }
    visited->head = NULL;  // Set head to NULL after freeing all nodes
}

// Free the queue and its nodes
void free_queue(struct queue *q) {
    struct list_node *current = q->data.head;
    while (current != NULL) {
        struct list_node *to_free = current;
        current = current->next;
        free(to_free);  // Free each node
    }
    q->data.head = NULL;  // Set head to NULL after freeing all nodes
}

// Generate possible next moves and enqueue them
void generate_possible_moves(struct game_state *state, struct queue *q, struct linked_list *visited) {
    struct game_state new_state = *state;  // Create a copy of the current state

    // Try all four possible moves (up, down, left, right)
    
    // Move up
    move_up(&new_state);
    if (!is_visited(visited, serialize(new_state))) {
        add_to_visited(visited, serialize(new_state));
        enqueue(q, new_state);
    }

    new_state = *state;  // Reset to the original state

    // Move down
    move_down(&new_state);
    if (!is_visited(visited, serialize(new_state))) {
        add_to_visited(visited, serialize(new_state));
        enqueue(q, new_state);
    }

    new_state = *state;  // Reset to the original state

    // Move left
    move_left(&new_state);
    if (!is_visited(visited, serialize(new_state))) {
        add_to_visited(visited, serialize(new_state));
        enqueue(q, new_state);
    }

    new_state = *state;  // Reset to the original state

    // Move right
    move_right(&new_state);
    if (!is_visited(visited, serialize(new_state))) {
        add_to_visited(visited, serialize(new_state));
        enqueue(q, new_state);
    }
}
// Number of moves function (without is_solved, num_possible_moves, and make_move)
int number_of_moves(struct game_state start) {
    // Initialize the queue
    struct queue q;
    q.data.head = NULL;  // Initialize empty queue

    // Initialize the visited list
    struct linked_list visited;
    visited.head = NULL;

    // Enqueue the starting state
    enqueue(&q, start);
    add_to_visited(&visited, serialize(start));  // Add the start state to the visited list

    // Example process for tracking the number of moves
    int num_moves = 0;

    // While the queue is not empty
    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // If the current state is solved, stop processing
        if (is_solved(&current_state)) {
            // Clean up memory before returning
            free_queue(&q);
            free_visited(&visited);
            return num_moves; // Return the number of moves when solved
        }

        num_moves++; // Increment the move count

        // Generate possible moves and enqueue them
        generate_possible_moves(&current_state, &q, &visited);

        // Print the state for debugging (optional)
        printf("Move %d: ", num_moves);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%d ", current_state.tiles[i][j]);
            }
            printf("\n");
        }
    }

    // Clean up memory before returning
    free_queue(&q);
    free_visited(&visited);

    return num_moves; // Return the number of moves processed
}
