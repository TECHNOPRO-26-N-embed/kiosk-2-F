#include <stdio.h>
#include <string.h>
#include "getBookStatus.h"
#include "excludeBook.h"

static int readMenuChoice(int *choice)
{
	if (choice == NULL) {
		return 0;
	}
	if (scanf("%d", choice) != 1) {
		return 0;
	}
	return 1;
}

static const char *toJapaneseStatus(const char *status)
{
	if (status == NULL) {
		return "不明";
	}
	if (strcmp(status, "loaned") == 0) {
		return "貸出中";
	}
	if (strcmp(status, "returned") == 0) {
		return "返却済み";
	}
	if (strcmp(status, "overdue") == 0) {
		return "延滞中";
	}
	if (strcmp(status, "stopped") == 0) {
		return "停止中";
	}
	return status;
}

int getBookStatusByCode(const char *csvPath, const char *bookCode, char *statusOut, size_t statusOutSize)
{
	FILE *fp;
	char line[256];
	int found;

	if (csvPath == NULL || bookCode == NULL || statusOut == NULL || statusOutSize == 0 ||
		csvPath[0] == '\0' || bookCode[0] == '\0') {
		return GET_BOOK_STATUS_INVALID_ARGUMENT;
	}

	fp = fopen(csvPath, "r");
	if (fp == NULL) {
		return GET_BOOK_STATUS_FILE_ERROR;
	}

	found = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		char userID[64];
		char csvBookCode[64];
		char period[32];
		char loanDate[32];
		char status[64];

		/* CSVヘッダ行をスキップ */
		if (strncmp(line, "user_id,book_code,period_days,loan_date,status", 46) == 0) {
			continue;
		}

		if (sscanf(line, "%63[^,],%63[^,],%31[^,],%31[^,],%63[^,\r\n]", userID, csvBookCode, period, loanDate, status) != 5) {
			continue;
		}

		if (strcmp(csvBookCode, bookCode) == 0) {
			strncpy(statusOut, status, statusOutSize - 1);
			statusOut[statusOutSize - 1] = '\0';
			found = 1;
		}
	}

	fclose(fp);

	if (!found) {
		return GET_BOOK_STATUS_NOT_FOUND;
	}

	return GET_BOOK_STATUS_OK;
}

int getBookStatus(void)
{
	char bookCode[64];
	char statusCode[64];
	const char *statusLabel;
	int result;

	while (1) {
		printf("\n====== 貸出状態確認 ======\n");
		printf("| 0 : メインメニューへ戻る\n");
		printf("========================\n");
		printf("|\n");
		printf("| 本コードを入力 : ");

		if (scanf("%63s", bookCode) != 1) {
			printf("|\n");
			printf("| 入力が不正です。メインメニューへ戻ります。\n");
			return 0;
		}

		if (strcmp(bookCode, "0") == 0) {
			printf("|\n");
			printf("| メインメニューへ戻ります。\n");
			return 0;
		}

		result = getBookStatusByCode("data/loans.csv", bookCode, statusCode, sizeof(statusCode));

		if (result == GET_BOOK_STATUS_OK) {
			statusLabel = toJapaneseStatus(statusCode);
			printf("|\n");
			printf("=== 状態を確認しました ===\n");
			printf("| 本コード     : %s\n", bookCode);
			printf("| 現在の状態   : %s\n", statusLabel);
			printf("| (状態コード) : %s\n", statusCode);
			printf("|\n");
			printf("| メインメニューへ戻ります。\n");
			return 0;
		}

		if (result == GET_BOOK_STATUS_NOT_FOUND) {
			printf("|\n");
			printf("| 該当する書籍コードの貸出履歴が見つかりません。\n");
			printf("| もう一度入力してください。\n");
			continue;
		}

		if (result == GET_BOOK_STATUS_FILE_ERROR) {
			printf("|\n");
			printf("| 貸出データファイル（data/loans.csv）を開けません。\n");
			printf("| メインメニューへ戻ります。\n");
			return 0;
		}

		printf("|\n");
		printf("| 入力値が不正です。\n");
	}
}

int runStatusExcludeMenu(void)
{
	int choice;

	while (1) {
		printf("\n==========================================\n");
		printf("         状態確認・除外処理メニュー         \n");
		printf("==========================================\n");
		printf("[1] 貸出状態確認\n");
		printf("[2] 除外処理\n");
		printf("[0] 終了\n");
		printf("選択してください: ");

		if (!readMenuChoice(&choice)) {
			printf("入力が不正です。数字で入力してください。\n");
			return 0;
		}

		if (choice == 1) {
			getBookStatus();
			continue;
		}

		if (choice == 2) {
			excludeBook();
			continue;
		}

		if (choice == 0) {
			printf("終了します。\n");
			return 0;
		}

		printf("[0]〜[2]を選択してください。\n");
	}
}

#ifdef STATUS_EXCLUDE_STANDALONE_MAIN
int main(void)
{
	return runStatusExcludeMenu();
}
#endif
