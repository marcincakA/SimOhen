#ifndef SOCKETS_SERVER_PASSIVE_SOCKET_H
#define SOCKETS_SERVER_PASSIVE_SOCKET_H

#include <stdbool.h>
#include <pthread.h>
#include "active_socket.h"

typedef struct passive_socket {
    int socket_descriptor;
    _Bool is_listening;
    _Bool is_waiting;
    pthread_mutex_t mutex;
    pthread_cond_t waiting_finished;
} PASSIVE_SOCKET;

void passive_socket_init(struct passive_socket* self);
void passive_socket_destroy(struct passive_socket* self);
_Bool passive_socket_start_listening(struct passive_socket* self, short port);
void passive_socket_stop_listening(struct passive_socket* self);
_Bool passive_socket_is_listening(struct passive_socket* self);
_Bool passive_socket_wait_for_client(struct passive_socket* self, struct active_socket* client_sock);

#endif //SOCKETS_SERVER_PASSIVE_SOCKET_H
