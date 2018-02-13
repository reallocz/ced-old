#include "core/doccmd.h"

doccmd dc_create(enum doccmd_type type)
{
	doccmd dc = {.flags = 0, .type = type, .cmd = {0}};
	return dc;
}
