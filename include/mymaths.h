#pragma once
#include <mytypes.h>

/** Because glibc doesn't */
#define MIN(A, B) (A < B ? A : B)
#define MAX(A, B) (A > B ? A : B)

typedef struct vec2 {
	int x;
	int y;
} vec2;

typedef struct vec3 {
	int x;
	int y;
	int z;
} vec3;

typedef struct vec4 {
	int x;
	int y;
	int z;
	int w;
} vec4;

