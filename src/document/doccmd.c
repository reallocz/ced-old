#include "document/doccmd.h"

doccmd dc_create(enum doccmd_type type)
{
	doccmd dc = {.flags = 0, .type = type, .data = {0}};
	return dc;
}
