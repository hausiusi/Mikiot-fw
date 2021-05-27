/*
 * linked_list.c
 *
 *  Created on: May 1, 2021
 *      Author: Zviad
 */

#include <lib_linked_list.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

static ll_iterator_t* _get_iterator(linked_list_t* ll);
static ll_node_t* _iterator_next(ll_iterator_t* it);

static int _iterator_free(void* it);

linked_list_t* ll_init() {
    linked_list_t* ll = ll_malloc(sizeof(linked_list_t));
    if (ll == NULL) {
        return NULL;
    }
    memset(ll, 0, sizeof(linked_list_t));
    ll->get_iterator = (void*) _get_iterator;
    return ll;
}

ll_iterator_t* _get_iterator(linked_list_t* ll) {
    ll_iterator_t* iterator = ll_malloc(sizeof(ll_iterator_t));
    if (iterator == NULL) {
        error_report(24, HeapError);
        return NULL;
    }
    iterator->first = iterator->current = ll->head;
    iterator->last = ll->tail;
    iterator->next = (void*) _iterator_next;
    iterator->free = _iterator_free;
    return iterator;
}

ll_node_t* _iterator_next(ll_iterator_t* it) {
    if (it == NULL) {
        error_report(25, LibError);
        return NULL;
    }
    it->current = ((ll_node_t*) it->current)->next;
    return it->current;
}

bool_t ll_add(linked_list_t* ll, ll_node_t* node) {
    if (node == NULL) {
        error_report(23, LibError);
        return false;
    }
    if (ll->count) {
        node->previous = ll->tail;
        node->next = NULL;
        ll->tail->next = node;
        ll->tail = node;
    } else {
        ll->head = ll->tail = node;
        node->previous = node->next = NULL;
    }
    ll->count++;
    return true;
}

ll_node_t* ll_get_node(void* item, uint32_t deep_cp_len) {
    ll_node_t* node = ll_malloc(sizeof(ll_node_t));
    void* new_item = item;
    if (node == NULL) {
        error_report(21, HeapError);
        return NULL;
    }
    if (deep_cp_len) {
        new_item = ll_malloc(deep_cp_len);
        if (new_item == NULL) {
            error_report(22, HeapError);
            return NULL;
        }
        memcpy(new_item, item, deep_cp_len);
        node->free = ll_free;
    }
    node->item = new_item;
    return node;
}

ll_node_t* ll_find(linked_list_t* ll, void* item) {
    foreach(ll_node_t, node, ll) {
        if (ll->equals) {
            if (ll->equals(node->item, item)) {
                return_from_foreach(node);
            }
        }
    }
    return NULL;
}

bool_t ll_remove(linked_list_t* ll, ll_node_t* node) {
    if (!node) {
        return false;
    }
    node->previous != NULL ? (node->previous->next = node->next) : (ll->head =
                                     node->next);
    node->next != NULL ? (node->next->previous = node->previous) : (ll->tail =
                                 node->previous);

    if (node->free) {
        /* Remove deep copied item */
        node->free(node->item);
    }
    ll_free(node);
    ll->count--;
    return true;
}

void ll_node_destroy(ll_node_t* node) {
    if (node->free) {
        node->free(node->item);
    }
    ll_free(node);
}

void ll_destroy(linked_list_t* ll) {
    ll_node_t* next;
    ll_node_t* current = ll->head;
    uint32_t count = ll->count;
    while (count--) {
        next = current->next;
        ll_node_destroy(current);
        current = next;
    }
    ll_free(ll);
}

static int _iterator_free(void* it) {
    ll_free(it);
    return 0;
}
