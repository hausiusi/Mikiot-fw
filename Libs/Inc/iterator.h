/*
 * iterator.h
 *
 *  Created on: May 2, 2021
 *      Author: Zviad
 */

#ifndef INC_ITERATOR_H_
#define INC_ITERATOR_H_

typedef struct _iterator {
    void* first;
    void* last;
    void* current;
    void* (*next)(void* it);
    int (*free)(void* self);
} iterator_t;

#define foreach(item_type, item_decl_name, iterable_ptr)    item_type* item_decl_name; \
                                                            iterator_t* __iterator = iterable_ptr->get_iterator(iterable_ptr); \
                                                            for (item_decl_name = (item_type*)__iterator->first; item_decl_name != NULL ? 1 : __iterator->free(__iterator); item_decl_name = (item_type*)__iterator->next(__iterator))

#endif /* INC_ITERATOR_H_ */
