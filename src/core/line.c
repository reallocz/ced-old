#include "core/line.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define L_DEFSBSIZE 8

line ln_create()
{
	line ln = {
		.flags = 0,
		.num = 0,
		.sb = sb_create(L_DEFSBSIZE),
	};
	return ln;
}


void ln_pprint(line* ln)
{
	assert(ln != NULL);
	printf("[[line]]\n");
	printf(" num =%zd\n", ln->num);
	sb_pprint(&ln->sb);
	printf("[[/line]]\n");
}

