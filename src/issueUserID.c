#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define USERS_CSV_PATH "data/users.csv"
#define MAX_LINE_LEN 1024

enum {
	ISSUE_USER_ID_OK = 0,
	ISSUE_USER_ID_ALREADY_EXISTS = 1,
	ISSUE_USER_ID_INVALID_ARGUMENT = -1,
	ISSUE_USER_ID_INVALID_NAME = -2,
	ISSUE_USER_ID_INVALID_PHONE = -3,
	ISSUE_USER_ID_INVALID_EMAIL = -4,
	ISSUE_USER_ID_INVALID_ADDRESS = -5,
	ISSUE_USER_ID_DUPLICATE_USER = -6,
	ISSUE_USER_ID_BUFFER_TOO_SMALL = -7,
	ISSUE_USER_ID_FILE_ERROR = -8
};

static int isNullOrBlank(const char *s)
{
	while (s != NULL && *s != '\0') {
		if (!isspace((unsigned char)*s)) {
			return 0;
		}
		s++;
	}
	return 1;
}

static int hasUnsafeCsvChar(const char *s)
{
	while (*s != '\0') {
		if (*s == ',' || *s == '"' || *s == '\n' || *s == '\r') {
			return 1;
		}
		s++;
	}
	return 0;
}

static int isValidPhone(const char *phone)
{
	int digits = 0;

	while (*phone != '\0') {
		if (isdigit((unsigned char)*phone)) {
			digits++;
		} else if (*phone == '-' || *phone == ' ' || *phone == '(' || *phone == ')') {
			/* Allowed separators. */
		} else {
			return 0;
		}
		phone++;
	}

	return digits >= 10 && digits <= 11;
}

static int isValidEmail(const char *email)
{
	const char *at = strchr(email, '@');
	const char *dot;

	if (at == NULL || at == email || at[1] == '\0') {
		return 0;
	}

	dot = strrchr(at + 1, '.');
	if (dot == NULL || dot == at + 1 || dot[1] == '\0') {
		return 0;
	}

	return 1;
}

static int parseUserNumber(const char *userID)
{
	int value = 0;

	if (userID == NULL || userID[0] != 'U') {
		return -1;
	}

	userID++;
	if (*userID == '\0') {
		return -1;
	}

	while (*userID != '\0') {
		if (!isdigit((unsigned char)*userID)) {
			return -1;
		}
		value = value * 10 + (*userID - '0');
		userID++;
	}

	return value;
}

/*
 * Registers a new library user and issues a unique user ID.
 * Returns ISSUE_USER_ID_OK (0) on success and a negative error code on failure.
 */
int issueUserID(
	const char *name,
	const char *phone,
	const char *email,
	const char *address,
	char *userID,
	size_t userIDSize)
{
	FILE *fp;
	char line[MAX_LINE_LEN];
	int maxNumber = 0;
	int hasData = 0;

	if (userID != NULL && userIDSize > 0) {
		userID[0] = '\0';
	}

	if (name == NULL || phone == NULL || email == NULL || address == NULL ||
		userID == NULL || userIDSize == 0) {
		return ISSUE_USER_ID_INVALID_ARGUMENT;
	}

	if (isNullOrBlank(name) || hasUnsafeCsvChar(name)) {
		return ISSUE_USER_ID_INVALID_NAME;
	}
	if (isNullOrBlank(phone) || hasUnsafeCsvChar(phone) || !isValidPhone(phone)) {
		return ISSUE_USER_ID_INVALID_PHONE;
	}
	if (isNullOrBlank(email) || hasUnsafeCsvChar(email) || !isValidEmail(email)) {
		return ISSUE_USER_ID_INVALID_EMAIL;
	}
	if (isNullOrBlank(address) || hasUnsafeCsvChar(address)) {
		return ISSUE_USER_ID_INVALID_ADDRESS;
	}

	if (userIDSize < 8) {
		return ISSUE_USER_ID_BUFFER_TOO_SMALL;
	}

	fp = fopen(USERS_CSV_PATH, "a+");
	if (fp == NULL) {
        printf("test2\n");
		return ISSUE_USER_ID_FILE_ERROR;
	}

	rewind(fp);
	while (fgets(line, sizeof(line), fp) != NULL) {
		char *userIDField;
		char *nameField;
		char *phoneField;
		char *emailField;
		char *addressField;
		char *ctx = NULL;
		int parsedNumber;

		if (strncmp(line, "user_id,", 8) == 0) {
			continue;
		}

		hasData = 1;

		userIDField = strtok_s(line, ",\r\n", &ctx);
		nameField = strtok_s(NULL, ",\r\n", &ctx);
		phoneField = strtok_s(NULL, ",\r\n", &ctx);
		emailField = strtok_s(NULL, ",\r\n", &ctx);
		addressField = strtok_s(NULL, ",\r\n", &ctx);

		if (userIDField == NULL || nameField == NULL || phoneField == NULL ||
			emailField == NULL || addressField == NULL) {
			continue;
		}

		if (strcmp(nameField, name) == 0 && strcmp(phoneField, phone) == 0 &&
			strcmp(emailField, email) == 0 && strcmp(addressField, address) == 0) {
			if (snprintf(userID, userIDSize, "%s", userIDField) >= (int)userIDSize) {
				fclose(fp);
				return ISSUE_USER_ID_BUFFER_TOO_SMALL;
			}
			fclose(fp);
			return ISSUE_USER_ID_ALREADY_EXISTS;
		}

		if (strcmp(phoneField, phone) == 0 || strcmp(emailField, email) == 0) {
			fclose(fp);
			return ISSUE_USER_ID_DUPLICATE_USER;
		}

		parsedNumber = parseUserNumber(userIDField);
		if (parsedNumber > maxNumber) {
			maxNumber = parsedNumber;
		}
	}

	if (snprintf(userID, userIDSize, "U%06d", maxNumber + 1) >= (int)userIDSize) {
		fclose(fp);
		return ISSUE_USER_ID_BUFFER_TOO_SMALL;
	}

	fseek(fp, 0, SEEK_END);
	if (!hasData && ftell(fp) == 0) {
		fprintf(fp, "user_id,name,phone,email,address\n");
	}

	if (fprintf(fp, "%s,%s,%s,%s,%s\n", userID, name, phone, email, address) < 0) {
		fclose(fp);
		userID[0] = '\0';
        printf("test1\n");
		return ISSUE_USER_ID_FILE_ERROR;
	}

	fclose(fp);

	// 入力内容をテキストファイルにも保存
	{
		// CSV形式で src/user_input_log.csv に保存
		printf("[DEBUG] Writing to log file: src/user_input_log.csv\n");
		FILE *logfp = fopen("src/user_input_log.csv", "a, ccs=UTF-8");
		if (logfp == NULL) {
			perror("[ERROR] Failed to open log file");
		} else {
			// ファイルが空ならヘッダを書き込む
			fseek(logfp, 0, SEEK_END);
			if (ftell(logfp) == 0) {
				fprintf(logfp, "userID,name,phone,email,address\n");
			}
			fprintf(logfp, "%s,%s,%s,%s,%s\n", userID, name, phone, email, address);
			fclose(logfp);
			printf("[DEBUG] Successfully wrote to log file\n");
		}
	}

	return ISSUE_USER_ID_OK;
}

int main(void) {
	char userID[16];
	int result;
	// テスト用ダミー入力
	const char *name = "ダミー六郎";
	const char *phone = "07033445566";
	const char *email = "dummy.rokurou@example.jp";
	const char *address = "京都府京都市左京区";

	result = issueUserID(name, phone, email, address, userID, sizeof(userID));

	if (result == ISSUE_USER_ID_OK) {
		printf("ユーザー登録成功: userID=%s\n", userID);
	} else if (result == ISSUE_USER_ID_ALREADY_EXISTS) {
		printf("既存ユーザー: userID=%s\n", userID);
	} else if (result == ISSUE_USER_ID_DUPLICATE_USER) {
		printf("電話番号またはメールが既存ユーザーと重複しています\n");
	} else {
		printf("エラー: %d\n", result);
	}

	return 0;
}