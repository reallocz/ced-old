#pragma once
#include <mytypes.h>

/** Because glibc doesn't */
#define MIN(A, B) (A < B ? A : B)
#define MAX(A, B) (A > B ? A : B)

