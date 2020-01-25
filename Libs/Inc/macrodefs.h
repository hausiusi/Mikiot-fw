/*
 * macrodefs.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#ifndef INC_MACRODEFS_H_
#define INC_MACRODEFS_H_

#define array_count(x) (sizeof(x) /sizeof(x[0]))
#define swap(a, b) {a ^= b; b ^= a; a ^= b;} //swaps values from a to b and vice versa
#define inverse_bit(a, b) {a ^= (1 << b);} //inverses bit value in a register
#define set_bit(a, b) {a |= (1 << b);} //sets bit value to  1 in a register
#define clear_bit(a, b) {a &= ~(1 << b);} //bit value to 1 in a register
#define check_bit(x, y) (1 & ( x >> y)) //checks if bit in a register is set or not

#endif /* INC_MACRODEFS_H_ */
