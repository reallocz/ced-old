#pragma once
#include "mytypes.h"

/** Standard flags */
static const uint f_init = 1 << 0; // Properly constructed
static const uint f_dead = 1 << 1; // Destroyed

static inline
void flag_set(uint* dst, uint flag)
{
	*dst |= flag;
}


static inline
int flag_isset(uint dst, uint flag)
{
	return (dst & flag) == flag;
}


static inline
void flag_unset(uint* dst, uint flag)
{
	*dst &= ~flag;
}

