#include "active_socket.h"
#include "char_buffer.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define INVALID_SOCKET -1
#define SOCKET_TERMINATE_CHAR '\0'

void active_socket_init(struct active_socket* self) {
    self->socket_descriptor = INVALID_SOCKET;

    self->end_message = ":end";
    self->is_reading = false;
    pthread_mutex_init(&self->mutex_reading, NULL);
    pthread_mutex_init(&self->mutex_writing, NULL);
    linked_list_char_buffer_init(&self->received_data, char_buffer_init_copy, char_buffer_copy, char_buffer_destroy);
    pthread_mutex_init(&self->mutex_received_data, NULL);
}

void active_socket_destroy(struct active_socket* self) {
    if (self->socket_descriptor >= 0) {
        close(self->socket_descriptor);
    }
    self->socket_descriptor = INVALID_SOCKET;

    self->is_reading = false;
    pthread_mutex_destroy(&self->mutex_reading);
    pthread_mutex_destroy(&self->mutex_writing);
    linked_list_char_buffer_destroy(&self->received_data);
    pthread_mutex_destroy(&self->mutex_received_data);
}

void active_socket_stop_reading(struct active_socket* self) {
    pthread_mutex_lock(&self->mutex_reading);
    self->is_reading = false;
    pthread_mutex_unlock(&self->mutex_reading);
}

_Bool active_socket_is_reading(struct active_socket* self) {
    pthread_mutex_lock(&self->mutex_reading);
    bool can_read = self->is_reading;
    pthread_mutex_unlock(&self->mutex_reading);
    return can_read;
}

_Bool active_socket_try_get_read_data(struct active_socket* self, struct char_buffer* output) {
    _Bool result = false;

    if (pthread_mutex_trylock(&self->mutex_received_data) == 0) {
        result = linked_list_char_buffer_try_remove_first(&self->received_data, output);
        pthread_mutex_unlock(&self->mutex_received_data);
    }

    return result;
}

_Bool active_socket_is_end_message(struct active_socket* self, struct char_buffer* message) {
    return message->size == strlen(self->end_message) &&
        strncmp(message->data, self->end_message, message->size) == 0;
}

void active_socket_start_reading(struct active_socket* self) {
#define BUFFER_LENGTH 100

    pthread_mutex_lock(&self->mutex_reading);
    if (self->is_reading) {
        pthread_mutex_unlock(&self->mutex_reading);
        fprintf(stderr, "active_socket is reading. Cannot call start_reading again.\n");
        return;
    }
    self->is_reading = true;
    pthread_mutex_unlock(&self->mutex_reading);

    char buffer[BUFFER_LENGTH];
    CHAR_BUFFER r_buffer;
    char_buffer_init(&r_buffer);

    fd_set sockets;
    FD_ZERO(&sockets);
    struct timeval tv;
    tv.tv_usec = 0;
    while(active_socket_is_reading(self)) {
        bzero(buffer, BUFFER_LENGTH);

        FD_SET(self->socket_descriptor, &sockets);
        tv.tv_sec = 1;
        select(self->socket_descriptor + 1, &sockets, NULL, NULL, &tv);
        if (FD_ISSET(self->socket_descriptor, &sockets)) {
            int read_length = read(self->socket_descriptor, buffer, BUFFER_LENGTH);
            if (read_length > 0) {
                size_t first_i = 0;
                size_t last_i = 0;
                while (last_i < read_length) {
                    while (last_i < read_length && buffer[last_i] != SOCKET_TERMINATE_CHAR) {
                        ++last_i;
                    }
                    size_t count = last_i - first_i;
                    char_buffer_append(&r_buffer, buffer + first_i, count);
                    if (last_i < read_length && buffer[last_i] == SOCKET_TERMINATE_CHAR) {
                        pthread_mutex_lock(&self->mutex_received_data);
                        linked_list_char_buffer_add(&self->received_data, &r_buffer);
                        pthread_mutex_unlock(&self->mutex_received_data);
                        char_buffer_clear(&r_buffer);

                    }
                    last_i = last_i + 1;
                    first_i = last_i;
                }
            } else {
                //perror("read failed.");
                active_socket_stop_reading(self);
                close(self->socket_descriptor);
                self->socket_descriptor = INVALID_SOCKET;
            }
        }
    }

    char_buffer_destroy(&r_buffer);
#undef BUFFER_LENGTH
}

void active_socket_write_data(struct active_socket* self, struct char_buffer* message) {
    pthread_mutex_lock(&self->mutex_writing);
    write(self->socket_descriptor, message->data, message->size);
    write(self->socket_descriptor, SOCKET_TERMINATE_CHAR, sizeof(SOCKET_TERMINATE_CHAR));
    pthread_mutex_unlock(&self->mutex_writing);
}

void active_socket_write_end_message(struct active_socket* self) {
    CHAR_BUFFER r_buf;
    char_buffer_init(&r_buf);
    char_buffer_append(&r_buf, self->end_message, strlen(self->end_message));
    active_socket_write_data(self, &r_buf);
    char_buffer_destroy(&r_buf);
}

#undef INVALID_SOCKET
#undef SOCKET_TERMINATE_CHAR