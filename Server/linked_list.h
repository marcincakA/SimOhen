#ifndef SOCKETS_SERVER_LINKED_LIST_H
#define SOCKETS_SERVER_LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#define GENERATE_LINKED_LIST(T, suffix) \
\
typedef struct linked_item_##suffix { \
    T data; \
    struct linked_item_##suffix* next; \
} LINKED_ITEM_##suffix; \
\
typedef struct linked_list_##suffix { \
    struct linked_item_##suffix* first; \
    struct linked_item_##suffix* last; \
    size_t size; \
    void (*suffix##_init_copy)(T* data, const T* other); \
    void (*suffix##_copy)(T* data, const T* other); \
    void (*suffix##_destroy)(T* data); \
} LINKED_LIST_##suffix; \
\
static void linked_list_##suffix##_clear(struct linked_list_##suffix* list); \
\
static void linked_list_##suffix##_init( \
        struct linked_list_##suffix* list, \
        void (*suffix##_init_copy)(T* data, const T* other), \
        void (*suffix##_copy)(T* data, const T* other), \
        void (*suffix##_destroy)(T* data) \
    ) { \
    list->first = NULL; \
    list->last = NULL; \
    list->size = 0; \
    list->suffix##_init_copy = suffix##_init_copy; \
    list->suffix##_copy = suffix##_copy; \
    list->suffix##_destroy = suffix##_destroy; \
} \
\
static void linked_list_##suffix##_destroy(struct linked_list_##suffix* list) { \
    linked_list_##suffix##_clear(list); \
    list->suffix##_init_copy = NULL; \
    list->suffix##_copy = NULL; \
    list->suffix##_destroy = NULL; \
} \
\
static void linked_list_##suffix##_add(struct linked_list_##suffix* list, const T* data) { \
    LINKED_ITEM_##suffix* item = calloc(1, sizeof(LINKED_ITEM_##suffix)); \
    list->suffix##_init_copy(&item->data, data); \
    item->next = NULL; \
    if (list->first == NULL) { \
        list->first = item; \
    } \
    else { \
        list->last->next = item; \
    } \
    list->last = item; \
    ++list->size; \
} \
\
static _Bool linked_list_##suffix##_try_remove_first(struct linked_list_##suffix* list, T* data) { \
    if (list->first != NULL) { \
        LINKED_ITEM_##suffix* removed = list->first; \
        list->first = list->first->next; \
        --list->size; \
\
        list->suffix##_copy(data, &removed->data); \
        list->suffix##_destroy(&removed->data); \
        free(removed); \
        return true; \
    } \
    else { \
        return false; \
    } \
} \
\
static void linked_list_##suffix##_clear(struct linked_list_##suffix* list) { \
    LINKED_ITEM_##suffix* cur = list->first; \
    while (cur != NULL) { \
        cur = cur->next; \
        list->suffix##_destroy(&list->first->data); \
        free(list->first); \
        list->first = cur; \
    } \
    list->last = NULL; \
    list->size = 0; \
}

#endif //SOCKETS_SERVER_LINKED_LIST_H
