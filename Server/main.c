#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "buffer.h"
#include "char_buffer.h"
#include "active_socket.h"
#include "passive_socket.h"

/*
 * Tu to na serveri budeme ukladať svety na server.
 *
 */

GENERATE_BUFFER(struct point, point)

typedef struct thread_data {
    long long replications_count;
    struct buffer_point buf;
    pthread_mutex_t mutex;
    pthread_cond_t is_full;
    pthread_cond_t is_empty;

    short port; // port
    ACTIVE_SOCKET* my_socket;   // socket
} THREAD_DATA;

void thread_data_init(struct thread_data* data, long long replications_count, int buffer_capacity,
                      short port, ACTIVE_SOCKET* my_socket) {
    data->replications_count = replications_count;
    buffer_point_init(&data->buf, buffer_capacity);
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->is_full, NULL);
    pthread_cond_init(&data->is_empty, NULL);

    data->port = port;
    data->my_socket = my_socket;
}

void thread_data_destroy(struct thread_data* data) {
    data->replications_count = 0;
    buffer_point_destroy(&data->buf);
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->is_full);
    pthread_cond_destroy(&data->is_empty);

    data->port = 0;
    data->my_socket = NULL;
}

void* process_client_data(void* thread_data) {
    struct thread_data * data = thread_data;

    PASSIVE_SOCKET p_socket;
    passive_socket_init(&p_socket);
    passive_socket_start_listening(&p_socket, data->port);

    passive_socket_wait_for_client(&p_socket, data->my_socket);

    passive_socket_stop_listening(&p_socket);
    passive_socket_destroy(&p_socket);

    printf("Klient bol pripojeny!\n");
    active_socket_start_reading(data->my_socket);

    return NULL;
}

void* consume(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;

    return NULL;
}


int main(int argc, char *argv[])
{
    struct thread_data data;
    struct active_socket my_socket;

    pthread_t th_produce;
    pthread_t th_receive;

    active_socket_init(&my_socket);

    pthread_create(&th_produce, NULL, produce, &data);
    pthread_create(&th_receive, NULL, process_client_data, &data);

    consume(&data); // posle data

    pthread_join(th_produce, NULL);
    pthread_join(th_receive, NULL);

    char **av=(char **)malloc(sizeof(char *)*(argc+1)); // create a char *[] big enough to hold argv

    /*
    for(int n=0; n<argc; n++) {
        av[n] = strdup(argv[n]); // Allocates memory, copies string into it, gives you pointer
    }
    av[argc]=NULL;


    for(int n=0; n<argc; n++) {
        free(av[n]);
    }*/

    free(av);
    active_socket_destroy(&my_socket);

    return 0;
}
