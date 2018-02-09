#pragma once
#include "mytypes.h"

/** Standard flags (uint).
 * The first 4 bits are reserved for standard flags, so all custom flags
 * should start from the 5th bit.
 */
static const uint f_init = 1 << 0; /**< Constructed by the standard function*/
static const uint f_dead = 1 << 1; /**< Destroyed by the standard function*/
static const uint f_ffff = 1 << 1; /**<  Reserved*/
static const uint f_fff1 = 1 << 1; /**<  Reserved*/

/** Set a flag */
static inline
void flag_set(uint* dst, uint flag)
{
	*dst |= flag;
}


/** Check if a flag is set.
 * \return 1 if true, 0 if false
 */
static inline
int flag_isset(uint dst, uint flag)
{
	return (dst & flag) == flag;
}

/** Remove a flag. */
static inline
void flag_unset(uint* dst, uint flag)
{
	*dst &= ~flag;
}

