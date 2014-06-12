/**
 * @file       util.h
 * @brief      Miscellaneous utilities
 * 
 * @author     Laurent Saint-Marcel (lstmarcel@yahoo.fr)
 * @date       2004/07/25
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <asf.h>
#include <stdio.h>
#include <avr/wdt.h>
#include "avr.h"
#include "lcd.h"

/** @brief Boolean */
//typedef uint8_t bool;
//#define FALSE 0
//#define TRUE  1
//#define false FALSE
//#define true  TRUE

/** @brief Pointer on a function */
typedef void (*UtilFnPtr)();

#ifndef NULL
/** @brief NULL pointer = 0 */
#define NULL 0
#endif

/**
   Set \c bit in IO port \c port.
*/
#define sbi(port, bit) (port) |= (1 << (bit))

/**
   Clear \c bit in IO port \c port.
*/
#define cbi(port, bit) (port) &= ~(1 << (bit))

#endif /* __UTIL_H__ */
