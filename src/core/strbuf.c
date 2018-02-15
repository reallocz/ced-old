#include "core/strbuf.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


// Static functions
// print out diagnostic info
static int _sb_diag(strbuf* sb) __attribute__((unused));

/** Internal wrapper around malloc.
 * NOTE: Exits on failure
 */
static char* _sb_malloc(uint size)
{
	char* data = malloc(size * sizeof(char));
	if(data == NULL) {
		printf("E: %s(size=%d)\n", __func__, size);
		exit(1);
	} else {
		return data;
	}
}


/** Internal wrapper around realloc.
 * NOTE: Exits on failure.
 */
static char* _sb_realloc(char* ptr, uint size)
{
	char* newdata = realloc(ptr, size * sizeof(char));
	if(newdata == NULL) {
		printf("E: %s(ptr=%p, size=%d)\n", __func__, ptr, size);
		exit(1);
	} else {
		return newdata;
	}
}


/** **unsafe** Internal wrapper around strncpy.*/
static void _sb_strncpy(char* dst, const char* src, uint srclen)
{
	assert(dst && src);
	for(uint i = 0; i < srclen; ++i) {
		dst[i] = src[i];
	}
}


/** Copy the src strbuf to dst.
 * dst is resized if necessary
 */
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

// Initializes flags too
strbuf sb_create(uint size)
{
	strbuf sb = {.flags = 0, .len = 0, .size = size, .buf = NULL};
	sb.buf = _sb_malloc(size);
	flag_set(&sb.flags, f_init);
	return sb;
}

strbuf sb_createfrom_strbuf(const strbuf* sb)
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


strbuf sb_createfrom_file(const char* path)
{
	FILE* f = fopen(path, "r");
	if(! f) {
		printf("E: %s(path = %s): error opening file.\n", __func__, path);
		exit(1);
	}
	strbuf sb;
	size_t fsize;
	size_t readsize;

	// get filesize
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);
	sb = sb_create(fsize);
	sb.len = fsize;
	readsize = fread(sb.buf, sizeof(char), sb.len, f);
	printf("%s: read %zu/%zu bytes.\n", __func__, readsize, fsize);
	return sb;
}


char* sb_create_cstr(const strbuf* sb)
{
	char* buf = _sb_malloc(sb->len + 1);
	_sb_strncpy(buf, sb->buf, sb->len);
	buf[sb->len] = '\0';
	return buf;
}

void sb_destroy_cstr(char* buf)
{
	free(buf);
}

void sb_destroy(strbuf* sb)
{
	assert(sb);
	if(flag_isset(sb->flags, f_dead)) {
		printf("E: %s: double destroy.\n", __func__);
		return;
	}
	if(! flag_isset(sb->flags, f_init)) {
		printf("E: %s: uninitialized strbuf.\n", __func__);
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
	assert(sb);
	uint oldsize = sb->size;
	if(newsize == oldsize) {
		return;
	}
	sb->buf = _sb_realloc(sb->buf, newsize);
	sb->size = newsize;
}



void sb_append_char(strbuf* sb, char c)
{
	assert(sb);
	if(sb->len == sb->size) {
		printf("Resizing %d -> %d\n", sb->size, sb->size * 2);
		sb_resize(sb, sb->size * 2);
	}
	sb->buf[sb->len] = c;
	(sb->len)++;
}


void sb_append_str(strbuf* sb, const char* str)
{
	assert(sb && str);

	for(int i = 0 ; ; ++i) {
		char c = str[i];
		if(c == '\0')
			break;
		else
			sb_append_char(sb, c);
	}
}


int sb_contains_char(const strbuf* sb, char c)
{
	return sb_count_char(sb, c) > 0;
}


uint sb_count_char(const strbuf* sb, char c)
{
	assert(sb);
	uint count = 0;
	for(uint i = 0; i < sb->len; ++i) {
		if(sb->buf[i] == c) count++;
	}
	return count;
}


void sb_spliton_char(const strbuf* sb, char sep, strbuf** result, uint* count)
{
	assert(sb);
	uint sepcount = sb_count_char(sb, sep);
	if(sepcount == 0) {
		*count = 0;
		*result = NULL;
		return;
	}

	// number of splits required = sepcount + 1
	strbuf* splits = malloc((sepcount + 1) * sizeof(strbuf));
	assert(splits);

	const char* srcbuf = sb->buf;	// alias
	uint lastpos = 0;				// last poition of seperator
	uint sc = 0;					// initialized split count

	// This loop should run till i == (sb->len-1) *BUT* to handle the case of
	// the last split, we let it run till i == sb->len and add extra checks
	// (check #1 and #2) to prevent buffer overflows.
	for(uint i = 0; i < sb->len + 1; ++i) {
		if(srcbuf[i] == sep && i < sb->len) {	/* check #1*/
			uint len = i - lastpos;
			strbuf* dsb = &splits[sc];
			*dsb = sb_create(len);
			dsb->len = len;
			_sb_strncpy(dsb->buf, &srcbuf[lastpos], len);
			/* Update last position and init count*/
			lastpos = i + 1;
			sc++;
		} else if (i == sb->len) {				/* check #2*/
			/* Last split case */
			uint lastlen = sb->len - lastpos;
			strbuf* dsb = &splits[sc];
			*dsb = sb_create(lastlen);
			dsb->len = lastlen;
			_sb_strncpy(dsb->buf, &srcbuf[lastpos], lastlen);
			/* Update init count */
			sc++;
			break; /* This is the last split so it's safe to break out.*/
		}
	}

	assert(sc == sepcount + 1);

	// assign the results
	*count = sepcount + 1;
	*result = splits;
}

void sb_splitlines(const strbuf* sb, strbuf** result, uint* count)
{
	sb_spliton_char(sb, '\n', result, count);
}


/// DEBUG AND PRINTING

void sb_pprint(const strbuf* sb)
{
	assert(sb);
	printf("[[strbuf]] ");
	printf(" len =%d,", sb->len);
	printf(" size=%d,", sb->size);
	printf(" &buf=%p,", (sb->buf));
	printf(" buf=\"");
	sb_print_string(sb);
	printf("\"");
	printf("[[/strbuf]]\n");
}


void sb_print_string(const strbuf* sb)
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

