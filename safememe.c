#include "safemem.h"
#include <stdio.h>
#include <stdlib.h>

// Node to track allocated memory blocks
typedef struct MemNode {
    void* ptr;
    struct MemNode* next;
} MemNode;

static MemNode* head = NULL;  // Head of the linked list tracking allocations

// Add a new allocation to the tracking list
static void add_node(void* ptr) {
    MemNode* node = (MemNode*)malloc(sizeof(MemNode));
    if (!node) {
        fprintf(stderr, "Fatal error: unable to allocate memory for tracking node\n");
        exit(EXIT_FAILURE);
    }
    node->ptr = ptr;
    node->next = head;
    head = node;
}

// Remove an allocation from the tracking list
static void remove_node(void* ptr) {
    MemNode** current = &head;
    while (*current) {
        if ((*current)->ptr == ptr) {
            MemNode* to_free = *current;
            *current = to_free->next;
            free(to_free);
            return;
        }
        current = &(*current)->next;
    }
}

// Allocate memory with error checking and track the allocation
void* sm_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error: memory allocation failed (size=%zu)\n", size);
        exit(EXIT_FAILURE);
    }
    add_node(ptr);
    return ptr;
}

// Reallocate memory with error checking and update tracking
void* sm_realloc(void* ptr, size_t new_size) {
    if (!ptr) {
        return sm_malloc(new_size);
    }
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        fprintf(stderr, "Error: memory reallocation failed (size=%zu)\n", new_size);
        exit(EXIT_FAILURE);
    }
    if (new_ptr != ptr) {
        remove_node(ptr);
        add_node(new_ptr);
    }
    return new_ptr;
}

// Allocate zero-initialized memory and track the allocation
void* sm_calloc(size_t num, size_t size) {
    void* ptr = calloc(num, size);
    if (!ptr) {
        fprintf(stderr, "Error: memory allocation failed (num=%zu, size=%zu)\n", num, size);
        exit(EXIT_FAILURE);
    }
    add_node(ptr);
    return ptr;
}

// Free memory and remove it from tracking
void sm_free(void* ptr) {
    if (!ptr) return;
    remove_node(ptr);
    free(ptr);
}

// Report any memory leaks (not freed allocations)
void sm_report_leaks(void) {
    MemNode* current = head;
    if (!current) {
        printf("No memory leaks detected.\n");
        return;
    }
    printf("Warning: Memory leaks detected:\n");
    while (current) {
        printf(" - Leaked block at address: %p\n", current->ptr);
        current = current->next;
    }
}
