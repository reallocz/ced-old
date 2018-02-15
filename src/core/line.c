#include "core/line.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "flags.h"

#define L_DEFSBSIZE 8

line ln_create()
{
	line ln = {
		.flags = 0,
		.num = 0,
		.sb = sb_create(L_DEFSBSIZE),
	};
	flag_set(&ln.flags, f_init);
	return ln;
}


void ln_destroy(line* ln)
{
	assert(ln);
	if(flag_isset(ln->flags, f_dead)) {
		printf("E: %s: double destroy!\n", __func__);
		return;
	}
	sb_destroy(&ln->sb);
	flag_set(&ln->flags, f_dead);
	ln = NULL;
	return;
}

void ln_pprint(line* ln)
{
	assert(ln != NULL);
	printf("[[line]]\n");
	printf(" num =%d\n", ln->num);
	sb_pprint(&ln->sb);
	printf("[[/line]]\n");
}

