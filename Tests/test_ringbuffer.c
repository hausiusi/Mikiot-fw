/*
 * test_ringbuffer.c
 *
 *  Created on: Mar 7, 2021
 *      Author: Zviad
 */
#include "mikiot_config.h"

#if (COMPILE_WITH_UNIT_TESTS == 1)

#include "lib_ringbuffer.h"
#include "test_framework.h"

#pragma pack(push, 1)
typedef struct {
    uint8_t a;
    uint16_t b;
    uint32_t c;
} rb_test_item_t;
#pragma pack(pop)

static rb_test_item_t data[5];
static ringbuffer_t ringbuffer;
static bool_t overlap_function_called = false;

/* @formatter:off */
const rb_test_item_t rb_test_items[10] = {
        {.a = 7, .b = 23000, .c = 65000000 },
        {.a = 8, .b = 23001, .c = 65000011 },
        {.a = 9, .b = 23002, .c = 65000012 },
        {.a = 10, .b = 23003, .c = 65000013 },
        {.a = 11, .b = 23004, .c = 65000014 },
        {.a = 12, .b = 23005, .c = 65000015 },
        {.a = 13, .b = 23006, .c = 65000016 },
        {.a = 14, .b = 23007, .c = 65000017 },
        {.a = 15, .b = 23008, .c = 65000018 },
        {.a = 16, .b = 23009, .c = 65000019 },
};
/* @formatter:on */

static void _enq_item_and_verify(int item_index) {
    tf_write("Test rinbuffer_enq. Item %i\n", item_index);
    bool_t ret = lib_ringbuffer_enq(&ringbuffer,
            (uint8_t*) &rb_test_items[item_index], sizeof(rb_test_item_t));
    tf_equals(ret, true);
}

static void _deq_item_and_verify(int item_index_to_compare) {
    tf_write("Test ringbuffer_deq and compare with item %i\n",
            item_index_to_compare);
    rb_test_item_t item = { 0 };
    bool_t ret = lib_ringbuffer_deq(&ringbuffer, (uint8_t*) &item,
            sizeof(rb_test_item_t));
    tf_equals(ret, true);
    int diff_between_items = memcmp((void*) &item,
            (void*) &rb_test_items[item_index_to_compare],
            sizeof(rb_test_item_t));
    tf_equals(diff_between_items, 0);
}

static void _overlap_detected() {
    overlap_function_called = true;
}

static void _check_overlap_and_reset() {
    tf_equals(overlap_function_called, true);
    overlap_function_called = false;
    tf_equals(check_bit(ringbuffer.status, Overlapped), 1);
}

static void _check_for_no_overlap() {
    tf_not_equals(overlap_function_called, true);
}

/*
 * Test with overlap support on
 */
static void _test_with_overlap() {
    tf_write("Test ringbuffer with overlap support on\n");
    lib_ringbuffer_init(&ringbuffer, (uint8_t*) data, sizeof(rb_test_item_t),
            sizeof(data), true, _overlap_detected);
    _enq_item_and_verify(0);
    _check_for_no_overlap();
    _enq_item_and_verify(1);
    _check_for_no_overlap();
    _enq_item_and_verify(2);
    _check_for_no_overlap();
    _enq_item_and_verify(3);
    _check_for_no_overlap();

    _deq_item_and_verify(0);
    _check_for_no_overlap();

    _enq_item_and_verify(4);
    _check_for_no_overlap();
    _enq_item_and_verify(5);
    _check_for_no_overlap();

    _enq_item_and_verify(6);
    _check_overlap_and_reset();

    // The item[1] is overlapped we have left the next item in the ring
    _deq_item_and_verify(2);

    _enq_item_and_verify(7);
    _check_for_no_overlap();

    _enq_item_and_verify(8);
    _check_overlap_and_reset();
    _enq_item_and_verify(9);
    _check_overlap_and_reset();

    // Two overlaps happened, hence, the item[3] and item[4] must be overlapped
    _deq_item_and_verify(5);
    _deq_item_and_verify(6);
    _deq_item_and_verify(7);
    _deq_item_and_verify(8);
    _deq_item_and_verify(9);
    // During dequeue no overlap must happen
    _check_for_no_overlap();
}

/*
 * Tests without overlap support. This will block ringbuffer indefinitely
 * and it must be freed from another thread or core
 */
static void _test_without_overlap() {
    tf_write("Test ringbuffer without overlap support\n");
    tf_write("NOT IMPLEMENTED\n");
    /*TODO: implementation idea: make any timer able to execute function after
     * the specific (X) time and dequeue the full ringbuffer there. After that check
     * if new item is added and measure with time-base-timer how much time has
     * been passed while thread was blocked and compare to that specific (X) time.
     */
}

extern void test_ringbuffer() {
    _test_with_overlap();
    _test_without_overlap();
}

#endif
