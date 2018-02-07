#include "core/strbuf.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


// Static functions
// print out diagnostic info
static int _sb_diag(strbuf* sb);


static char* _sb_malloc(uint size)
{
	char* data = malloc(size * sizeof(char));
	if(data == NULL) {
		printf("E: %s(size=%zd)\n", __func__, size);
		exit(1);
	} else {
		return data;
	}
}


static char* _sb_realloc(char* ptr, uint size)
{
	char* newdata = realloc(ptr, size * sizeof(char));
	if(newdata == NULL) {
		printf("E: %s(ptr=%x, size=%zd)\n", __func__);
		exit(1);
	} else {
		return newdata;
	}
}


static void _sb_strncpy(char* dst, char* src, uint srclen)
{
	for(uint i = 0; i < srclen; ++i) {
		dst[i] = src[i];
	}
}

static void _sb_copy(strbuf* dst, const strbuf* src)
{
	assert(dst != NULL);
	assert(src != NULL);
	// Check for uninitialized strbufs
	if(! flag_isset(dst->flags, f_init)
		|| ! flag_isset(src->flags, f_init)) {
		printf("E: %s: copying an uninitialized strbuf\n", __func__);
		exit(1);
	}
	// Check for NULL bufs
	if(dst->buf == NULL || src->buf == NULL) {
		printf("E: %s: NULL buf\n", __func__);
		exit(1);
	}

	// Resize dst if necessary
	if(dst->size < src->size)
		sb_resize(dst, src->size);

	// copy buffers
	_sb_strncpy(dst->buf, src->buf, src->size);
	dst->len = src->len;
}

strbuf sb_create(uint size)
{
	strbuf sb = {.flags = 0, .len = 0, .size = size, .buf = NULL};
	sb.buf = _sb_malloc(size);
	flag_set(&sb.flags, f_init);
	return sb;
}

strbuf sb_createfrom_strbuf(strbuf* sb)
{
	assert(sb != NULL);
	if(! flag_isset(sb->flags, f_init)) {
		printf("E: %s: uninitialized strbuf.\n", __func__);
		exit(1);
	} else {
		strbuf ret = sb_create(sb->size);
		_sb_copy(&ret, sb);
		return ret;
	}
}


strbuf sb_createfrom_str(const char* str)
{
	assert(str != NULL);
	uint srclen = strlen(str);
	strbuf sb = sb_create(srclen);
	_sb_strncpy(sb.buf, (char*)str, srclen);
	sb.len = srclen; // len == size
	return sb;
}


const char* sb_getstr(strbuf* sb)
{
	sb->buf[sb->len] = '\0';
	return sb->buf;
}

void sb_destroy(strbuf* sb)
{
	assert(sb != NULL);
	if(! flag_isset(sb->flags, f_init)) {
		printf("E: %s: uninitialized strbuf.\n", __func__);
		return;
	}
	if(flag_isset(sb->flags, f_dead)) {
		printf("E: %s: double destroy.\n", __func__);
		return;
	} else {
		sb->len = 0;
		sb->size = 0;
		free(sb->buf);	
		sb->buf = NULL;
		flag_unset(&sb->flags, f_init);
		flag_set(&sb->flags, f_dead);
		sb = NULL;
		return;
	}
}


void sb_resize(strbuf* sb, uint newsize)
{
	assert(sb != NULL);
	uint oldsize = sb->size;
	if(newsize == oldsize) {
		return;
	} 
	sb->buf = _sb_realloc(sb->buf, newsize);
	sb->size = newsize;
}



void sb_append_char(strbuf* sb, char c)
{
	assert(sb != NULL);
	if(sb->len == sb->size) {
		printf("Resizing %d -> %d\n", sb->size, sb->size * 2);
		sb_resize(sb, sb->size * 2);
	}
	sb->buf[sb->len] = c;
	(sb->len)++;
}


void sb_append_str(strbuf* sb, const char* str)
{
	assert(sb != NULL);
	assert(str != NULL);
	
	for(int i = 0 ; ; ++i) {
		char c = str[i];
		if(c == '\0')
			break;
		else
			sb_append_char(sb, c);
	}
}


void sb_pprint(strbuf* sb)
{
	assert(sb != NULL);
	printf("[[strbuf]] ");
	printf(" len =%zd,", sb->len);
	printf(" size=%zd,", sb->size);
	printf(" &buf=%x,", (sb->buf));
	printf(" buf=\"%s\"", sb_getstr(sb));
	printf("[[/strbuf]]\n");
}


void sb_print_string(strbuf* sb)
{
	for(uint i = 0; i < sb->len; ++i)
		putchar(sb->buf[i]);
}

static int _sb_diag(strbuf* sb)
{
	if(sb == NULL) {
		printf("E: %s: sb is NULL.\n", __func__);
		return -1;
	}

	int ret = 0;
	printf("[[ diag: strbuf ]]\n");
	if (! flag_isset(sb->flags, f_init)) {
		printf(" - sb was not initialized\n");
		ret = -1;
	}
	if (flag_isset(sb->flags, f_dead)) {
		printf(" - sb is destroyed\n");
		ret = -1;
	}
	if (sb->size == 0) {
		printf(" - sb.size = 0.\n");
		ret = -1;
	}
	if (sb->buf == NULL) {
		printf("- sb.buf = NULL.\n");
		ret = -1;
	}
	if (sb->size == 0) {
		printf(" - sb is NULL.\n");
		ret = -1;
	}
	if(ret == 0) {
		printf("OK\n");
	}
	printf("[[/diag]]\n");
	return ret;
}

