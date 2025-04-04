#include "queue.h"
#include "tile_game.h"    // Include tile_game.h for game-related functions
#include <stdlib.h>        // For malloc and free
#include "linked_list.h"   // For struct list_node and linked list functions

extern void move_up(struct game_state *state);
extern void move_down(struct game_state *state);
extern void move_left(struct game_state *state);
extern void move_right(struct game_state *state);

// Helper function to free a list of nodes (renamed to avoid conflict)
void free_node_list(struct list_node *head) {
    while (head != NULL) {
        struct list_node *to_free = head;
        head = head->next;
        free(to_free);
    }
}

// Check if the current state is solved (goal state)
int is_solved(struct game_state *state) {
    uint8_t goal_state[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

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
    uint64_t serialized_state = serialize(state);

    struct list_node *new_node = (struct list_node *)malloc(sizeof(struct list_node));
    if (new_node == NULL) return;  // Handle memory allocation failure
    new_node->value = serialized_state;
    new_node->next = NULL;

    if (q->data.head == NULL) {
        q->data.head = new_node;
    } else {
        struct list_node *temp = q->data.head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Dequeue a state from the queue
struct game_state dequeue(struct queue *q) {
    if (q->data.head == NULL) {
        struct game_state invalid_state = {0}; // Return an invalid state if the queue is empty
        return invalid_state;
    }

    struct list_node *node_to_remove = q->data.head;
    q->data.head = node_to_remove->next;

    struct game_state state = deserialize(node_to_remove->value);
    free(node_to_remove);

    return state;
}

// Check if a state has been visited
int is_visited(struct linked_list *visited, uint64_t state) {
    struct list_node *current = visited->head;
    while (current != NULL) {
        if (current->value == state) {
            return 1;  // State already visited
        }
        current = current->next;
    }
    return 0;  // State not visited
}

// Add a state to the visited list
void add_to_visited(struct linked_list *visited, uint64_t state) {
    insert_at_tail(visited, state);  // Insert serialized state at the tail of the visited list
}

// Free the visited list and its nodes
void free_visited(struct linked_list *visited) {
    free_node_list(visited->head);
    visited->head = NULL;
}

// Free the queue and its nodes
void free_queue(struct queue *q) {
    free_node_list(q->data.head);
    q->data.head = NULL;
}

// Generate possible next moves and enqueue them
void generate_possible_moves(struct game_state *state, struct queue *q, struct linked_list *visited) {
    struct game_state new_state;
    
    void (*moves[])(struct game_state*) = {move_up, move_down, move_left, move_right};

    for (int i = 0; i < 4; i++) {
        new_state = *state;  // Copy the current state
        moves[i](&new_state);

        // Ensure the new state is not visited and is within boundaries
        if (!is_visited(visited, serialize(new_state))) {
            add_to_visited(visited, serialize(new_state));
            enqueue(q, new_state);
        }
    }
}

// Number of moves function (without is_solved, num_possible_moves, and make_move)
int number_of_moves(struct game_state start) {
    struct queue q = {0};
    struct linked_list visited = {0};

    enqueue(&q, start);
    add_to_visited(&visited, serialize(start));

    int num_moves = 0;

    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        if (is_solved(&current_state)) {
            free_queue(&q);
            free_visited(&visited);
            return num_moves;
        }

        num_moves++;
        generate_possible_moves(&current_state, &q, &visited);

        // Debugging: Print current state and the number of visited states
        printf("State %d: ", num_moves);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%d ", current_state.tiles[i][j]);
            }
        }
        printf("\n");

        // Debugging: Print the number of visited states
        int visited_count = 0;
        struct list_node *node = visited.head;
        while (node != NULL) {
            visited_count++;
            node = node->next;
        }
        printf("Visited states: %d\n", visited_count);
    }

    free_queue(&q);
    free_visited(&visited);

    return -1;  // If no solution found, return -1
}
