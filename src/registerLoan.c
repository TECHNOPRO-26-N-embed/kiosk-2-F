#include <stdio.h>
#include <string.h>
#include <time.h>

static void trimRight(char *text);

/* user_input_log.csv の先頭列に ID が存在するかを確認する */
static int isUserIDInFile(const char *inputUserID)
{
	FILE *userFile;
	char line[256];

	userFile = fopen("src/user_input_log.csv", "r");
	if (userFile == NULL) {
		return -1;
	}

	while (fgets(line, sizeof(line), userFile) != NULL) {
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
		if (strcmp(line, inputUserID) == 0) {
			fclose(userFile);
			return 1;
		}
	}

	fclose(userFile);
	return 0;
}

enum {
	REGISTER_LOAN_OK = 0,              /* 正常終了 */
	REGISTER_LOAN_INVALID_ARGUMENT = -1, /* 引数が不正 */
	REGISTER_LOAN_INVALID_PERIOD = -2,   /* 貸出期間の値が不正 */
	REGISTER_LOAN_FILE_ERROR = -3        /* ファイルエラー */
};

/* 入力値を 5 日または 8 日に正規化する */
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

/* 文字列末尾の改行・空白を取り除く */
static void trimRight(char *text)
{
	int len;

	len = (int)strlen(text);
	while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r' || text[len - 1] == ' ' || text[len - 1] == '\t')) {
		text[len - 1] = '\0';
		len--;
	}
}

/* bookID.csv の先頭列に本コードが存在するかを確認する */
static int isBookCodeInFile(const char *inputBookCode)
{
	FILE *bookFile;
	char line[256];

	bookFile = fopen("src/bookID.csv", "r");
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

/* 本コードに対応するタイトルを bookID.csv から取得する */
static int getBookTitleByCode(const char *inputBookCode, char *titleOut, int titleSize)
{
	FILE *bookFile;
	char line[256];

	bookFile = fopen("src/bookID.csv", "r");
	if (bookFile == NULL) {
		return -1;
	}

	while (fgets(line, sizeof(line), bookFile) != NULL) {
		char *comma;
		char *title;

		trimRight(line);
		if (line[0] == '\0') {
			continue;
		}

		comma = strchr(line, ',');
		if (comma == NULL) {
			continue;
		}

		*comma = '\0';
		title = comma + 1;
		trimRight(line);
		trimRight(title);

		if (strcmp(line, inputBookCode) == 0) {
			strncpy(titleOut, title, (size_t)(titleSize - 1));
			titleOut[titleSize - 1] = '\0';
			fclose(bookFile);
			return 1;
		}
	}

	fclose(bookFile);
	return 0;
}

/*
 * 貸出登録のメイン処理:
 * 1) ID確認 2) 本コード複数入力 3) 確認 4) 期間設定 5) loans.csv へ追記
 */
int registerLoan(const char *userID, const char *bookCode, int period)
{
	int normalizedPeriod;
	FILE *fp;
	char loanDate[11];
	char inputUserID[64];
	char inputBookCode[64];
	char loanBookCodes[20][64];
	char loanBookTitles[20][128];
	const char *expectedUserID;
	const char *expectedBookCode;
	const char *finalUserID;
	int codeFound;
	int bookCount;
	int continueInput;
	int confirmBooks;
	int i;
	int finalPeriod;
	int yesno;
	int idFound;
	time_t now;
	struct tm *local;

	expectedUserID = userID;
	expectedBookCode = bookCode;
	finalUserID = userID;
	finalPeriod = period;
	bookCount = 0;

	/* 利用者IDの入力と存在確認 */
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

		idFound = isUserIDInFile(inputUserID);
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

	/* 本コードを最大20冊まで入力 */
	while (1) {
		printf("\n本コードを入力して下さい。\n");
		printf("期間設定に進む場合は 0 を入力して下さい。\n");
		scanf("%63s", inputBookCode);

		if (strcmp(inputBookCode, "0") == 0) {
			if (bookCount == 0) {
				printf("本コードが未入力です。1冊以上入力してください。\n");
				continue;
			}
			goto BOOK_INPUT_DONE;
		}

		if (inputBookCode[0] == '\0') {
			printf("本コードが間違っています。\n");
			continue;
		}

		if (expectedBookCode != NULL && expectedBookCode[0] != '\0' && bookCount == 0 && strcmp(inputBookCode, expectedBookCode) != 0) {
			printf("本コードが間違っています。\n");
			continue;
		}

		codeFound = isBookCodeInFile(inputBookCode);
		if (codeFound < 0) {
			printf("bookID.csvの読み込みに失敗しました。\n");
			return REGISTER_LOAN_FILE_ERROR;
		}
		if (codeFound == 0) {
			printf("本コードが見つかりません。もう一度入力してください。\n");
			continue;
		}

		if (getBookTitleByCode(inputBookCode, loanBookTitles[bookCount], (int)sizeof(loanBookTitles[bookCount])) <= 0) {
			printf("本のタイトル取得に失敗しました。\n");
			continue;
		}

		strcpy(loanBookCodes[bookCount], inputBookCode);
		bookCount++;

		if (bookCount >= 20) {
			printf("これ以上は入力できません。次の工程へ進みます。\n");
			break;
		}

		while (1) {
			printf("続けて本コードを入力しますか？ はい[1] いいえ[2]\n");
			if (scanf("%d", &continueInput) != 1) {
				printf("入力が不正です。\n");
				while (getchar() != '\n');
				continue;
			}

			if (continueInput == 1) {
				break;
			}
			if (continueInput == 2) {
				goto BOOK_INPUT_DONE;
			}

			printf("入力が不正です。\n");
		}
	}

BOOK_INPUT_DONE:

	/* 入力済み本コードの確認 */
	while (1) {
		printf("\n入力された本コードとタイトルを確認して下さい。\n");
		for (i = 0; i < bookCount; i++) {
			printf("%d. %s : %s\n", i + 1, loanBookCodes[i], loanBookTitles[i]);
		}
		printf("この内容でよろしいですか？ はい[1] いいえ[2]\n");

		if (scanf("%d", &confirmBooks) != 1) {
			printf("入力が不正です。\n");
			while (getchar() != '\n');
			continue;
		}

		if (confirmBooks == 1) {
			break;
		}
		if (confirmBooks == 2) {
			printf("本コード入力に戻ります。\n");
			goto REENTER_BOOK_CODES;
		}

		printf("入力が不正です。\n");
	}

	goto AFTER_BOOK_CONFIRM;

REENTER_BOOK_CODES:
	/* 確認で「いいえ」を選んだ場合の再入力 */
	while (1) {
		printf("\n本コードを入力して下さい。\n");
		printf("期間設定に進む場合は 0 を入力して下さい。\n");
		scanf("%63s", inputBookCode);

		if (strcmp(inputBookCode, "0") == 0) {
			if (bookCount == 0) {
				printf("本コードが未入力です。1冊以上入力してください。\n");
				continue;
			}
			break;
		}

		if (inputBookCode[0] == '\0') {
			printf("本コードが間違っています。\n");
			continue;
		}

		codeFound = isBookCodeInFile(inputBookCode);
		if (codeFound < 0) {
			printf("bookID.csvの読み込みに失敗しました。\n");
			return REGISTER_LOAN_FILE_ERROR;
		}
		if (codeFound == 0) {
			printf("本コードが見つかりません。もう一度入力してください。\n");
			continue;
		}

		if (getBookTitleByCode(inputBookCode, loanBookTitles[bookCount], (int)sizeof(loanBookTitles[bookCount])) <= 0) {
			printf("本のタイトル取得に失敗しました。\n");
			continue;
		}

		strcpy(loanBookCodes[bookCount], inputBookCode);
		bookCount++;

		if (bookCount >= 20) {
			printf("これ以上は入力できません。次の工程へ進みます。\n");
			break;
		}
	}

AFTER_BOOK_CONFIRM:

	if (finalUserID == NULL || finalUserID[0] == '\0') {
		finalUserID = "UNKNOWN";
	}

	/* 貸出期間の入力と確認 */
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

	/* 貸出日を取得 */
	now = time(NULL);
	local = localtime(&now);
	if (local == NULL) {
		printf("貸出登録でエラーが発生しました。\n");
		return REGISTER_LOAN_FILE_ERROR;
	}
	strftime(loanDate, sizeof(loanDate), "%Y-%m-%d", local);

	/* 貸出データを CSV に追記 */
	fp = fopen("data/loans.csv", "a");
	if (fp == NULL) {
		printf("貸出登録でエラーが発生しました。\n");
		return REGISTER_LOAN_FILE_ERROR;
	}

	if (ftell(fp) == 0) {
		fprintf(fp, "user_id,book_code,period_days,loan_date,status\n");
	}

	for (i = 0; i < bookCount; i++) {
		fprintf(fp, "%s,%s,%d,%s,loaned\n", finalUserID, loanBookCodes[i], normalizedPeriod, loanDate);
	}

	fclose(fp);
	printf("\n貸出が完了しました。\nメインメニューへ戻ります。\n\n");

	return REGISTER_LOAN_OK;
}

int main(void)
{
	return registerLoan(NULL, NULL, 0);
}
