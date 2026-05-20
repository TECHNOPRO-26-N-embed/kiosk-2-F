#include <stdio.h>
#include <string.h>
#include <time.h>
#include "excludeBook.h"

static int writeToday(char *dateOut, size_t dateOutSize)
{
	time_t now = time(NULL);
	struct tm *local = localtime(&now);

	if (local == NULL || dateOut == NULL || dateOutSize < 11) {
		return 0;
	}

	strftime(dateOut, dateOutSize, "%Y-%m-%d", local);
	return 1;
}

static void sanitizeReason(char *reason)
{
	char *p;

	if (reason == NULL) {
		return;
	}

	for (p = reason; *p != '\0'; p++) {
		if (*p == ',') {
			*p = ' ';
		}
	}
}

static int isExcludedAlready(const char *excludedCsvPath, const char *bookCode)
{
	FILE *fp;
	char line[256];

	fp = fopen(excludedCsvPath, "r");
	if (fp == NULL) {
		return 0;
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		char csvBookCode[64];
		char reason[128];
		char date[32];

		if (strncmp(line, "book_code,reason,excluded_date", 30) == 0) {
			continue;
		}

		if (sscanf(line, "%63[^,],%127[^,],%31[^,\r\n]", csvBookCode, reason, date) != 3) {
			continue;
		}

		if (strcmp(csvBookCode, bookCode) == 0) {
			fclose(fp);
			return 1;
		}
	}

	fclose(fp);
	return 0;
}

int excludeBookRecord(const char *excludedCsvPath, const char *loansCsvPath, const char *bookCode, const char *reason)
{
	FILE *excludedFp;
	FILE *loansFp;
	char date[11];

	if (excludedCsvPath == NULL || loansCsvPath == NULL || bookCode == NULL || reason == NULL ||
		excludedCsvPath[0] == '\0' || loansCsvPath[0] == '\0' || bookCode[0] == '\0' || reason[0] == '\0') {
		return EXCLUDE_BOOK_INVALID_ARGUMENT;
	}

	if (isExcludedAlready(excludedCsvPath, bookCode)) {
		return EXCLUDE_BOOK_ALREADY_EXCLUDED;
	}

	if (!writeToday(date, sizeof(date))) {
		return EXCLUDE_BOOK_FILE_ERROR;
	}

	excludedFp = fopen(excludedCsvPath, "a+");
	if (excludedFp == NULL) {
		return EXCLUDE_BOOK_FILE_ERROR;
	}

	if (ftell(excludedFp) == 0) {
		fprintf(excludedFp, "book_code,reason,excluded_date\n");
	}
	fprintf(excludedFp, "%s,%s,%s\n", bookCode, reason, date);
	fclose(excludedFp);

	loansFp = fopen(loansCsvPath, "a+");
	if (loansFp == NULL) {
		return EXCLUDE_BOOK_FILE_ERROR;
	}

	if (ftell(loansFp) == 0) {
		fprintf(loansFp, "user_id,book_code,period_days,loan_date,status\n");
	}
	fprintf(loansFp, "SYSTEM,%s,0,%s,stopped\n", bookCode, date);
	fclose(loansFp);

	return EXCLUDE_BOOK_OK;
}

int excludeBook(void)
{
	char bookCode[64];
	char reason[128];
	int result;

	printf("\n====== 除外処理画面 ======\n");
	printf("| 0 : メインメニューへ戻る\n");
	printf("========================\n");
	printf("|\n");
	printf("| 本コードを入力 : ");

	if (scanf("%63s", bookCode) != 1) {
		printf("| 入力が不正です。メインメニューへ戻ります。\n");
		return 0;
	}

	if (strcmp(bookCode, "0") == 0) {
		printf("| メインメニューへ戻ります。\n");
		return 0;
	}

	printf("| 除外理由を入力 : ");
	if (scanf(" %127[^\n]", reason) != 1) {
		printf("| 理由の入力が不正です。メインメニューへ戻ります。\n");
		return 0;
	}
	sanitizeReason(reason);

	result = excludeBookRecord("data/excluded_books.csv", "data/loans.csv", bookCode, reason);

	if (result == EXCLUDE_BOOK_OK) {
		printf("|\n");
		printf("=== 除外処理が完了しました ===\n");
		printf("| 本コード   : %s\n", bookCode);
		printf("| 除外理由   : %s\n", reason);
		printf("| 状態       : 停止中\n");
		printf("| メインメニューへ戻ります。\n");
		return 0;
	}

	if (result == EXCLUDE_BOOK_ALREADY_EXCLUDED) {
		printf("| この本は既に除外済みです。メインメニューへ戻ります。\n");
		return 0;
	}

	if (result == EXCLUDE_BOOK_INVALID_ARGUMENT) {
		printf("| 入力値が不正です。メインメニューへ戻ります。\n");
		return 0;
	}

	printf("| データファイルの操作に失敗しました。メインメニューへ戻ります。\n");
	return 0;
}
