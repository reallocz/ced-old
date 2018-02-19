#include "rendering/renderer.h"
#include "document/line.h"
#include "ds/strbuf.h"
#include "glad/glad.h"
#include "flags.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void rdr_render_line(const line* ln)
{
	printf("D: Rendering: line number [%d] :: ", ln->num);
	sb_print_string(&ln->sb);
	printf("\n");
}
