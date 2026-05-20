#ifndef ISSUE_USER_ID_H
#define ISSUE_USER_ID_H

#include <stddef.h>

#define ISSUE_USER_ID_OK 0
#define ISSUE_USER_ID_INVALID_ARGUMENT -1
#define ISSUE_USER_ID_INVALID_NAME -2
#define ISSUE_USER_ID_INVALID_PHONE -3
#define ISSUE_USER_ID_INVALID_EMAIL -4
#define ISSUE_USER_ID_INVALID_ADDRESS -5
#define ISSUE_USER_ID_ALREADY_EXISTS -6
#define ISSUE_USER_ID_DUPLICATE_USER -7
#define ISSUE_USER_ID_FILE_ERROR -8
#define ISSUE_USER_ID_BUFFER_TOO_SMALL -9

#define USERS_CSV_PATH "data/users.csv"
#define MAX_LINE_LEN 256

int issueUserID(
    const char *name,
    const char *phone,
    const char *email,
    const char *address,
    char *userID,
    size_t userIDSize
);

#endif // ISSUE_USER_ID_H
