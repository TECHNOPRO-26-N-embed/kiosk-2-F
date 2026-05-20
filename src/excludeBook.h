#ifndef EXCLUDE_BOOK_H
#define EXCLUDE_BOOK_H

enum {
	EXCLUDE_BOOK_OK = 0,
	EXCLUDE_BOOK_INVALID_ARGUMENT = -1,
	EXCLUDE_BOOK_ALREADY_EXCLUDED = -2,
	EXCLUDE_BOOK_FILE_ERROR = -3
};

int excludeBookRecord(const char *excludedCsvPath, const char *loansCsvPath, const char *bookCode, const char *reason);
int excludeBook(void);

#endif