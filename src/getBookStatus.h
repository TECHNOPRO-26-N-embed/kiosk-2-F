#ifndef GET_BOOK_STATUS_H
#define GET_BOOK_STATUS_H

#include <stddef.h>

enum {
	GET_BOOK_STATUS_OK = 0,
	GET_BOOK_STATUS_NOT_FOUND = -1,
	GET_BOOK_STATUS_INVALID_ARGUMENT = -2,
	GET_BOOK_STATUS_FILE_ERROR = -3
};

int getBookStatusByCode(const char *csvPath, const char *bookCode, char *statusOut, size_t statusOutSize);
int getBookStatus(void);

#endif