#include <stdio.h>
#include <string.h>
#include <time.h>

enum {
	REGISTER_LOAN_OK = 0,
	REGISTER_LOAN_INVALID_ARGUMENT = -1,
	REGISTER_LOAN_INVALID_PERIOD = -2,
	REGISTER_LOAN_FILE_ERROR = -3
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

int registerLoan(const char *userID, const char *bookCode, int period)
{
	int normalizedPeriod;
	FILE *fp;
	char loanDate[11];
	time_t now;
	struct tm *local;

	if (userID == NULL || bookCode == NULL || userID[0] == '\0' || bookCode[0] == '\0') {
		return REGISTER_LOAN_INVALID_ARGUMENT;
	}

	normalizedPeriod = normalizePeriod(period);
	if (normalizedPeriod < 0) {
		return REGISTER_LOAN_INVALID_PERIOD;
	}

	now = time(NULL);
	local = localtime(&now);
	if (local == NULL) {
		return REGISTER_LOAN_FILE_ERROR;
	}
	strftime(loanDate, sizeof(loanDate), "%Y-%m-%d", local);

	fp = fopen("data/loans.csv", "a");
	if (fp == NULL) {
		return REGISTER_LOAN_FILE_ERROR;
	}

	if (ftell(fp) == 0) {
		fprintf(fp, "user_id,book_code,period_days,loan_date,status\n");
	}

	fprintf(fp, "%s,%s,%d,%s,loaned\n", userID, bookCode, normalizedPeriod, loanDate);
	fclose(fp);

	return REGISTER_LOAN_OK;
}
