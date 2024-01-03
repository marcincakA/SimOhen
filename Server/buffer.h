#ifndef SERVER_BUFFER_H
#define SERVER_BUFFER_H

#include <stdbool.h>
#include <stdlib.h>

#define GENERATE_BUFFER(type, suffix) \
typedef struct buffer_##suffix { \
    int capacity; \
    int start_index; \
    int size; \
    type* data; \
} BUFFER_##suffix; \
\
static void buffer_##suffix##_init(struct buffer_##suffix* buf, int capacity) { \
    buf->capacity = capacity; \
    buf->start_index = 0; \
    buf->size = 0; \
    buf->data = calloc(capacity, sizeof(type)); \
} \
\
static void buffer_##suffix##_destroy(struct buffer_##suffix* buf) { \
    buf->capacity = 0; \
    buf->start_index = 0; \
    buf->size = 0; \
    free(buf->data); \
    buf->data = NULL; \
} \
\
static _Bool buffer_##suffix##_try_push(struct buffer_##suffix* buf, type data) { \
    if (buf->size < buf->capacity) { \
        int index = (buf->start_index + buf->size) % buf->capacity; \
        buf->data[index] = data; \
        ++buf->size; \
        return true; \
    } \
    else { \
        return false; \
    } \
} \
\
static _Bool buffer_##suffix##_try_pop(struct buffer_##suffix* buf, type* data) { \
    if (buf->size > 0) { \
        *data = buf->data[buf->start_index]; \
        buf->start_index = (buf->start_index + 1) % buf->capacity; \
        --buf->size; \
        return true; \
    } \
    else { \
        return false; \
    } \
}

#endif //SERVER_BUFFER_H
