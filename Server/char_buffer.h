#ifndef SOCKETS_SERVER_CHAR_BUFFER_H
#define SOCKETS_SERVER_CHAR_BUFFER_H

#include <stdlib.h>

// dynamicka implementacia textu v C <3
typedef struct char_buffer {
    size_t capacity;
    size_t size;
    char* data;
} CHAR_BUFFER;

void char_buffer_init(struct char_buffer* buf);
void char_buffer_init_copy(struct char_buffer* buf, const struct char_buffer* other);
void char_buffer_destroy(struct char_buffer* buf);
void char_buffer_append(struct char_buffer* buf, const char* data, size_t data_size);
void char_buffer_clear(struct char_buffer* buf);
void char_buffer_copy(struct char_buffer* dest, const struct char_buffer* src);


#endif //SOCKETS_SERVER_CHAR_BUFFER_H
