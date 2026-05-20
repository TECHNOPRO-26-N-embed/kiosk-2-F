#include <stdio.h>
#include <string.h>
#include <time.h>

static void trimRight(char *text);

static int isUserIDInFile(const char *inputUserID)
{
	FILE *userFile;
	char line[256];

	userFile = fopen("src/user_input_log.txt", "r");
	if (userFile == NULL) {
		return -1;
	}

	while (fgets(line, sizeof(line), userFile) != NULL) {
		char *comma;
		trimRight(line);
		if (line[0] == '\0') continue;
		comma = strchr(line, ',');
		if (comma != NULL) *comma = '\0';
		trimRight(line);
		if (strcmp(line, inputUserID) == 0) {
			fclose(userFile);
			return 1;
		}
	}
	fclose(userFile);
	return 0;
}

enum {
	REGISTER_LOAN_OK = 0,//正常終了
	REGISTER_LOAN_INVALID_ARGUMENT = -1,//引数が不正
	REGISTER_LOAN_INVALID_PERIOD = -2,//貸出期間の値が不正
	REGISTER_LOAN_FILE_ERROR = -3//ファイルエラー
};

static int normalizePeriod(int period)
{
	if (period == 1 || period == 5) {
		return 5;
	}
	if (period == 2 || period == 8) {
		return 8;
	}
	return -1;
}

static void trimRight(char *text)
{
	int len;

	len = (int)strlen(text);
	while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r' || text[len - 1] == ' ' || text[len - 1] == '\t')) {
		text[len - 1] = '\0';
		len--;
	}
}

static int isBookCodeInFile(const char *inputBookCode)
{
	FILE *bookFile;
	char line[256];

	bookFile = fopen("src/bookcode.txt", "r");
	if (bookFile == NULL) {
		return -1;
	}

	while (fgets(line, sizeof(line), bookFile) != NULL) {
		char *comma;

		trimRight(line);
		if (line[0] == '\0') {
			continue;
		}

		comma = strchr(line, ',');
		if (comma != NULL) {
			*comma = '\0';
		}

		trimRight(line);
		if (strcmp(line, inputBookCode) == 0) {
			fclose(bookFile);
			return 1;
		}
	}

	fclose(bookFile);
	return 0;
}

int registerLoan(const char *userID, const char *bookCode, int period)
{
	int normalizedPeriod;
	FILE *fp;
	char loanDate[11];
	char inputUserID[64];
	char inputBookCode[64];
	const char *expectedUserID;
	const char *expectedBookCode;
	const char *finalUserID;
	const char *finalBookCode;
	int codeFound;
	int finalPeriod;
	int yesno;
	time_t now;
	struct tm *local;

	expectedUserID = userID;
	expectedBookCode = bookCode;
	finalUserID = userID;
	finalBookCode = bookCode;
	finalPeriod = period;

	while (1) {
		printf("\nIDを入力して下さい。\n");
		scanf("%63s", inputUserID);

		if (inputUserID[0] == '\0') {
			printf("IDが間違っています。\n");
			continue;
		}

		if (expectedUserID != NULL && expectedUserID[0] != '\0' && strcmp(inputUserID, expectedUserID) != 0) {
			printf("IDが間違っています。\n");
			continue;
		}

		int idFound = isUserIDInFile(inputUserID);
		if (idFound < 0) {
			printf("user_input_log.txtの読み込みに失敗しました。\n");
			return REGISTER_LOAN_FILE_ERROR;
		}
		if (idFound == 0) {
			printf("IDが見つかりません。もう一度入力してください。\n");
			continue;
		}

		finalUserID = inputUserID;
		break;
	}

	while (1) {
		printf("\n本コードを入力して下さい。\n");
		scanf("%63s", inputBookCode);

		if (inputBookCode[0] == '\0') {
			printf("本コードが間違っています。\n");
			continue;
		}

		if (expectedBookCode != NULL && expectedBookCode[0] != '\0' && strcmp(inputBookCode, expectedBookCode) != 0) {
			printf("本コードが間違っています。\n");
			continue;
		}

		codeFound = isBookCodeInFile(inputBookCode);
		if (codeFound < 0) {
			printf("bookcode.txtの読み込みに失敗しました。\n");
			return REGISTER_LOAN_FILE_ERROR;
		}
		if (codeFound == 0) {
			printf("本コードが見つかりません。もう一度入力してください。\n");
			continue;
		}

		finalBookCode = inputBookCode;
		break;
	}

	if (finalUserID == NULL || finalUserID[0] == '\0') {
		finalUserID = "UNKNOWN";
	}

	while (1) {
		printf("\n貸出期間を設定して下さい。\n");
		printf("４泊５日[1] または ７泊８日[2]\n");

		if (finalPeriod <= 0) {
			if (scanf("%d", &finalPeriod) != 1) {
				printf("入力が不正です。\n");
				while (getchar() != '\n'); // 入力バッファクリア
				finalPeriod = 0;
				continue;
			}
		}

		normalizedPeriod = normalizePeriod(finalPeriod);
		if (normalizedPeriod < 0) {
			printf("入力が不正です。\n");
			finalPeriod = 0;
			continue;
		}

		if (normalizedPeriod == 5) {
			printf("4泊5日で間違いないですか？ はい[1] いいえ[2]\n");
		} else {
			printf("7泊8日で間違いないですか？ はい[1] いいえ[2]\n");
		}
		scanf("%d", &yesno);

		if (yesno == 2) {
			printf("期間設定に戻ります。\n");
			finalPeriod = 0;
			continue;
		}
		if (yesno != 1) {
			printf("入力が不正です。\n");
			finalPeriod = 0;
			continue;
		}
		break;
	}

	now = time(NULL);
	local = localtime(&now);
	if (local == NULL) {
		printf("貸出登録でエラーが発生しました。\n");
		return REGISTER_LOAN_FILE_ERROR;
	}
	strftime(loanDate, sizeof(loanDate), "%Y-%m-%d", local);

	fp = fopen("data/loans.csv", "a");
	if (fp == NULL) {
		printf("貸出登録でエラーが発生しました。\n");
		return REGISTER_LOAN_FILE_ERROR;
	}

	if (ftell(fp) == 0) {
		fprintf(fp, "user_id,book_code,period_days,loan_date,status\n");
	}

	fprintf(fp, "%s,%s,%d,%s,loaned\n", finalUserID, finalBookCode, normalizedPeriod, loanDate);
	fclose(fp);
	printf("\n貸出が完了しました。\nメインメニューへ戻ります。\n\n");

	return REGISTER_LOAN_OK;
}

int main(void)
{
	return registerLoan(NULL, NULL, 0);
}
