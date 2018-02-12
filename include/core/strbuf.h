#pragma once
#include "mytypes.h"
#include "flags.h"

/** Standard string buffer.
 * This is used rather that char* for strings.
 */
typedef struct strbuf {
	uint flags;
	uint len;		/**< Size of buffer in use. */
	uint size;		/**< Total size of buffer. */
	char* buf;		/**< Buffer. */
} strbuf;


/** Create a new strbuf of size `size`.
 * \param size size of the buffer to be allocated.
 * */
strbuf sb_create(uint size);

/** Create a new strbuf from an existing one.
 * \param sb valid strbuf to be copied.
 */
strbuf sb_createfrom_strbuf(const strbuf* sb);

/** Create a new strbuf from a c-string.
 * The created buffer will be of the exact size of the str
 */
strbuf sb_createfrom_str(const char* str);

/** Create a new strbuf from file */
strbuf sb_createfrom_file(const char* path);

/** Standard strbuf destructor function.
 * The buffer will be free'd and the pointer will be set to NULL.
 */
void sb_destroy(strbuf* sb);

/** Resize the strbuf (both increase or decrease).
 * NOTE: The address of the buffer may be changed.
 */
void sb_resize(strbuf* sb, uint newsize);

/** Get the c-string from the buffer.*/
const char* sb_get_cstr(strbuf* sb);

/** Append a char at the end of the buffer.
 * Resizes buffer if necessary.
 */
void sb_append_char(strbuf* sb, char c);

/** Appends a c-string at the end of the buffer.
 * Resizes buffer if needed.
 */
void sb_append_str(strbuf* sb, const char* str);

/** Return 1 if strbuf contains char */
int sb_contains_char(const strbuf* sb, char c);

/** Count the number of times a char occurs in strbuf */
uint sb_count_char(const strbuf* sb, char c);

/** Split the strbuf on 'sep' delimiter.
 * \param sb strbuf to be split
 * \param sep delimiter
 * \param result pointer to split strbufs. NULL if count == 0
 * \param count number of split strbufs
 */
void sb_spliton_char(const strbuf* sb, char sep, strbuf** result, uint* count);


/** Split the strbuf delimited by '\n'.
 * Equivalent to sb_spliton_char(sb, result, count ,'\n'); */
void sb_splitlines(const strbuf* sb, strbuf** result, uint* count);

//// PRINTING AND DEBUG
// TODO add mode param for different kinds of pprint
/** Pretty print the buffer. */
void sb_pprint(const strbuf* sb);

/** Print out the buffer string.*/
void sb_print_string(const strbuf* sb);

