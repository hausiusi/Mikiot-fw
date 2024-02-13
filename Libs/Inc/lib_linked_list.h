/*
 * linked_list.h
 *
 *  Created on: May 1, 2021
 *      Author: Zviad
 */

#ifndef INC_LIB_LINKED_LIST_H_
#define INC_LIB_LINKED_LIST_H_

#include <inttypes.h>
#include "defines.h"
#include "iterator.h"
#include "mikiot_config.h"

#define ll_malloc   mconf_malloc
#define ll_free     mconf_free

typedef iterator_t ll_iterator_t;

typedef struct _node {
    void* item;
    struct _node* previous;
    struct _node* next;
    void (*free)(void* itm);
} ll_node_t;

typedef struct _linked_list {
    ll_node_t* head;
    ll_node_t* tail;
    uint32_t count;
    ll_iterator_t* (*get_iterator)(void* ll);
    bool_t (*equals)(void* a, void* b);
} linked_list_t;

#ifndef ll_malloc
#define ll_malloc malloc
#endif

#ifndef ll_free
#define ll_free free
#endif

linked_list_t* ll_init();

bool_t ll_add(linked_list_t* ll, ll_node_t* node);

ll_node_t* ll_find(linked_list_t* ll, void* item);

bool_t ll_remove(linked_list_t* ll, ll_node_t* node);

ll_node_t* ll_get_node(void const* item, uint32_t deep_cp_len);

void ll_node_destroy(ll_node_t* node);

void ll_destroy(linked_list_t* ll);

#endif /* INC_LIB_LINKED_LIST_H_ */
