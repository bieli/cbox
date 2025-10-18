#include <stdio.h>
#include <stdlib.h>

#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "cbox.h"
#include "cbox_struct.h"
#include "cbox_debug.h"

// Custom struct: Task
typedef struct {
    char label[64];
    int priority;
} Task;

void* task_clone(const void* data) {
    Task* copy = malloc(sizeof(Task));
    if (copy) memcpy(copy, data, sizeof(Task));
    return copy;
}

void task_destroy(void* data) {
    free(data);
}

void task_print(const void* data) {
    const Task* t = (const Task*)data;
    printf("Task[label: %s, priority: %d]\n", t->label, t->priority);
}

void task_serialize_json(const void* data, char* out, size_t maxlen) {
    const Task* t = (const Task*)data;
    snprintf(out, maxlen,
        "{\"type\":\"Task\",\"label\":\"%s\",\"priority\":%d,\"address\":\"%p\"}",
        t->label, t->priority, data);
}

CBoxTrait CBOX_TASK_TRAIT = {
    .clone = task_clone,
    .destroy = task_destroy,
    .print = task_print,
    .serialize_json = task_serialize_json
};

#define CBOX_WRAP_TASK(t) cbox_new_with_trait(&(t), sizeof(Task), CBOX_STRUCT, &CBOX_TASK_TRAIT)

// Simulated task queue
typedef struct {
    CBox** items;
    size_t count;
    size_t capacity;
} TaskQueue;

TaskQueue* queue_create(size_t capacity) {
    TaskQueue* q = malloc(sizeof(TaskQueue));
    q->items = malloc(capacity * sizeof(CBox*));
    q->count = 0;
    q->capacity = capacity;
    return q;
}

void* string_clone(const void* data) {
    const char* s = (const char*)data;
    char* copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

void string_print(const void* data) {
    printf("Message: %s\n", (const char*)data);
}

void queue_push(TaskQueue* q, CBox* item) {
    if (q->count < q->capacity) {
        q->items[q->count++] = item;
    } else {
        printf("Queue full\n");
        cbox_free(item);
    }
}

void queue_process(TaskQueue* q) {
    for (size_t i = 0; i < q->count; ++i) {
        printf("Processing item %zu:\n", i);
        CBOX_DEBUG_ALL(q->items[i]);
    }
}

void queue_destroy(TaskQueue* q) {
    for (size_t i = 0; i < q->count; ++i)
        cbox_free(q->items[i]);
    free(q->items);
    free(q);
}

int main() {
    TaskQueue* queue = queue_create(5);

    // Dynamically allocated string
    char* msg = malloc(strlen("Hello from CBox!") + 1);
    strcpy(msg, "Hello from CBox!");

    CBoxTrait string_trait = {
        .clone = string_clone,
        .print = string_print,
        .destroy = free,
        .serialize_json = NULL
    };
    CBox* msg_box = cbox_new_with_trait(msg, strlen(msg) + 1, CBOX_STRUCT, &string_trait);
    queue_push(queue, msg_box);

    // Integer task ID
    int id = 101;
    CBox* id_box = CBOX_WRAP_INT(id);
    queue_push(queue, id_box);

    // Custom Task struct
    Task t1 = { "Download file", 2 };
    CBox* task_box = CBOX_WRAP_TASK(t1);
    queue_push(queue, task_box);

    // Process all tasks
    queue_process(queue);

    queue_destroy(queue);
    return 0;
}

