#pragma once
#include "mytypes.h"
#include "flags.h"

typedef struct strbuf {
	uint flags;
	uint len;	
	uint size;
	char* buf;
} strbuf;


// Create a new strbuf
strbuf sb_create(uint size);
// Create a new strbuf from an existing one
strbuf sb_createfrom_strbuf(strbuf* sb);
// Create a new strbuf from a str
strbuf sb_createfrom_str(const char* str);
// Create a new strbuf from file
strbuf sb_createfrom_file(const char* path);

void sb_destroy(strbuf* sb);

// Increase or decrease size
void sb_resize(strbuf* sb, uint newsize);

const char* sb_getstr(strbuf* sb);

void sb_append_char(strbuf* sb, char c);
void sb_append_str(strbuf* sb, const char* str);

void sb_pprint(strbuf* sb);

void sb_print_string(strbuf* sb);

