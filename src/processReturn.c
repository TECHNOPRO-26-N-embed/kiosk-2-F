#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 延滞料金の1日あたりの金額
#define RETURN_FEE_PER_DAY 50

// 文字列の末尾から改行や空白を削除する関数
static void trimRight(char *text)
{
    size_t len = strlen(text);
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r' || text[len - 1] == ' ' || text[len - 1] == '\t')) {
        text[len - 1] = '\0';
        len--;
    }
}

// YYYY-MM-DD 形式の日付文字列を解析し、struct tm に変換する
// 正しい日付なら 0 を返し、失敗したら -1 を返す
static int parseDate(const char *dateStr, struct tm *out)
{
    int year, month, day;

    if (dateStr == NULL || strlen(dateStr) != 10) {
        return -1;
    }

    if (sscanf(dateStr, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return -1;
    }

    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) {
        return -1;
    }

    memset(out, 0, sizeof(*out));
    out->tm_year = year - 1900;
    out->tm_mon = month - 1;
    out->tm_mday = day;
    out->tm_hour = 0;
    out->tm_min = 0;
    out->tm_sec = 0;
    out->tm_isdst = -1;

    if (mktime(out) == (time_t)-1) {
        return -1;
    }

    if (out->tm_year != year - 1900 || out->tm_mon != month - 1 || out->tm_mday != day) {
        return -1;
    }

    return 0;
}

// 2つの日付の差を日数で返す
static int daysBetween(const struct tm *start, const struct tm *end)
{
    struct tm copyStart = *start;
    struct tm copyEnd = *end;
    time_t startTime = mktime(&copyStart);
    time_t endTime = mktime(&copyEnd);

    if (startTime == (time_t)-1 || endTime == (time_t)-1) {
        return 0;
    }

    double diffSeconds = difftime(endTime, startTime);
    return (int)(diffSeconds / (60.0 * 60.0 * 24.0));
}

// 返却日と貸出日、貸出期間から延滞料金を計算する
static int calculateOverdueFee(const char *loanDateStr, const char *returnDateStr, int period)
{
    struct tm loanDate;
    struct tm returnDate;

    if (parseDate(loanDateStr, &loanDate) != 0) {
        return -1;
    }
    if (parseDate(returnDateStr, &returnDate) != 0) {
        return -1;
    }

    int elapsedDays = daysBetween(&loanDate, &returnDate);
    int overdueDays = elapsedDays - period;
    if (overdueDays <= 0) {
        return 0;
    }
    return overdueDays * RETURN_FEE_PER_DAY;
}

// CSV 行をカンマで分割して、各フィールドの先頭アドレスを fields に格納する
static int splitCsvFields(char *line, char **fields, int maxFields)
{
    int count = 0;
    char *current = line;

    while (current != NULL && count < maxFields) {
        fields[count++] = current;
        char *comma = strchr(current, ',');
        if (comma == NULL) {
            break;
        }
        *comma = '\0';
        current = comma + 1;
    }
    return count;
}

// 返却処理を行う関数
// 成功した場合は延滞料金を返し、失敗時には負のエラーコードを返す
int processReturn(const char *userID, const char *bookCode, const char *returnDate)
{
    // 引数チェック
    if (userID == NULL || userID[0] == '\0' || bookCode == NULL || bookCode[0] == '\0' || returnDate == NULL || returnDate[0] == '\0') {
        return -4;
    }

    FILE *input = fopen("data/loans.csv", "r");
    if (input == NULL) {
        return -1;
    }

    FILE *output = fopen("data/loans.tmp.csv", "w");
    if (output == NULL) {
        fclose(input);
        return -1;
    }

    char line[512];
    char headerLine[512];
    int hasReturnDateColumn = 0;
    int foundLoan = 0;
    int overdueFee = 0;

    // ヘッダー行を読み込み、返却日列があるか確認する
    if (fgets(headerLine, sizeof(headerLine), input) == NULL) {
        fclose(input);
        fclose(output);
        remove("data/loans.tmp.csv");
        return -1;
    }

    trimRight(headerLine);
    strncpy(line, headerLine, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';

    char *headerFields[8];
    int headerCount = splitCsvFields(line, headerFields, 8);
    if (headerCount >= 6) {
        hasReturnDateColumn = 1;
    }

    fprintf(output, "%s\n", headerLine);

    // 各貸出レコードを確認し、対象の貸出を更新する
    while (fgets(line, sizeof(line), input) != NULL) {
        trimRight(line);
        if (line[0] == '\0') {
            fprintf(output, "\n");
            continue;
        }

        char originalLine[512];
        strncpy(originalLine, line, sizeof(originalLine) - 1);
        originalLine[sizeof(originalLine) - 1] = '\0';

        char *fields[8];
        int fieldCount = splitCsvFields(line, fields, 8);

        if (fieldCount >= 5 && strcmp(fields[0], userID) == 0 && strcmp(fields[1], bookCode) == 0 && (strcmp(fields[4], "loaned") == 0 || strcmp(fields[4], "overdue") == 0) && foundLoan == 0) {
            // 対象レコードを発見
            int period = atoi(fields[2]);
            int fee = calculateOverdueFee(fields[3], returnDate, period);
            if (fee < 0) {
                fclose(input);
                fclose(output);
                remove("data/loans.tmp.csv");
                return -5;
            }
            overdueFee = fee;
            foundLoan = 1;

            // 状態を返却済みへ変更
            fields[4] = "returned";

            // 返却日列があれば返却日をセットする
            if (hasReturnDateColumn) {
                if (fieldCount < 6) {
                    fields[5] = (char *)returnDate;
                    fieldCount = 6;
                } else {
                    fields[5] = (char *)returnDate;
                }
            }

            // 更新した行を書き出す
            if (hasReturnDateColumn) {
                if (fieldCount >= 6) {
                    fprintf(output, "%s,%s,%s,%s,%s,%s\n", fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
                } else {
                    fprintf(output, "%s,%s,%s,%s,%s,%s\n", fields[0], fields[1], fields[2], fields[3], fields[4], returnDate);
                }
            } else {
                fprintf(output, "%s,%s,%s,%s,%s\n", fields[0], fields[1], fields[2], fields[3], fields[4]);
            }
        } else {
            // 対象以外の行はそのまま書き出す
            fprintf(output, "%s\n", originalLine);
        }
    }

    fclose(input);
    fclose(output);

    // 対象の貸出が見つからなかった場合
    if (!foundLoan) {
        remove("data/loans.tmp.csv");
        return -2;
    }

    // 元ファイルを差し替える
    if (remove("data/loans.csv") != 0) {
        remove("data/loans.tmp.csv");
        return -1;
    }

    if (rename("data/loans.tmp.csv", "data/loans.csv") != 0) {
        return -1;
    }

    // 成功したら延滞料金を返す
    return overdueFee;
}
