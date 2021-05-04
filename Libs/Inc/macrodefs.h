/*
 * macrodefs.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#ifndef INC_MACRODEFS_H_
#define INC_MACRODEFS_H_

#define array_count(x) (sizeof(x) /sizeof(x[0]))
#define swap(a, b) {a ^= b; b ^= a; a ^= b;} // Swaps values from a to b and vice versa
#define inverse_bit(a, b) {a ^= (1 << b);} // Inverses bit value in a register
#define set_bit(a, b) {a |= (1 << b);} // Sets bit value to  1 in a register
#define clear_bit(a, b) {a &= ~(1 << b);} // Bit value to 1 in a register
#define check_bit(x, y) (1 & ( x >> y)) // Checks if bit in a register is set or not
#define is_between(x, a, b) (((x) > (a)) && ((x) < (b))) // Checks if x is more than a and less than b
#define is_range(x, a, b) (((x) >= (a)) && ((x) <= (b))) // Checks if x is more or equal a and less or equal than b
#define bcd2bin_8b(bcd)  (((bcd & 0xF0) >> 4) + (bcd & 0x0F))

#endif /* INC_MACRODEFS_H_ */
