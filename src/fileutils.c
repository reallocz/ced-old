#include "fileutils.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// TODO return NULL on failure
char* file_readfile(const char* path) {
	FILE* file = fopen(path, "r");
	if(! file) {
		printf("E: %s(path = %s): error opening file.\n", __func__, path);
		return NULL;
	}
	size_t fsize;
	size_t readsize;
	char* buf;

	// get filesize
	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	rewind(file);

	// Malloc and read to buffer
	buf = malloc((fsize + 1) * sizeof(char));
	readsize = fread(buf, sizeof(char), fsize, file);
	buf[fsize] = '\0';
	printf("%s: read %zu/%zu bytes.\n", __func__, readsize, fsize);
	return buf;
}


void file_free(char* buf)
{
	assert(buf);
	free(buf);
}

