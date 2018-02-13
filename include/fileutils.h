#pragma once

/** Loads a file in the memory.
 * \return buffer on success. NULL on failure.
 */
char* file_readfile(const char* path);

/** Free the buffer.
 * This function is just for consistency (file_read -> file_free) rather that (file_read -> free)
 */
void file_free(char* buf);
