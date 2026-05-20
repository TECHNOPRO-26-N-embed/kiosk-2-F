#include <stdio.h>
#include <string.h>

int registerLoan();

int main(void){

void searchBook(void)
{
	char bookName[128];
	char bookCode[128];
	int choice;

    /* 書籍検索初期ループ */


	while (1) {
		FILE *fp;
		int found = 0;

		printf("==============================\n");
		printf("書籍検索ページ\n");
		printf("==============================\n");
		printf("終了する場合は「OSHIMAI」を入力してください。\n");
		printf("書籍タイトルを入力してください。: ");

        /*入力変数→「bookName」に格納*/
		if (scanf("%127s", bookName) != 1) {
			return;
		}

        /*「OSHIMAI」が入力されたら検索機能を終了*/
		if (strcmp(bookName, "OSHIMAI") == 0) {
			printf("書籍検索を終了します。\n");
			return;
		}

		/* bookcode.txtを開く */
		fp = fopen("src/bookcode.txt", "r");
		if (fp == NULL) {
			fp = fopen("bookcode.txt", "r");
		}

		if (fp == NULL) {
			printf("bookcode.txtを開けませんでした。\n");
			return;
		}

        /* 書籍コードと入力された書籍名を比較 */
		while (fscanf(fp, "%*[^,],%127[^\n]\n", bookCode) == 1) {
			int i = 0;

            /* 書籍コードと入力された書籍名を1文字ずつ比較 */
			while (bookName[i] != '\0' && bookCode[i] != '\0' && bookName[i] == bookCode[i]) {
				i++;
			}

            /* 書籍名が完全に一致した場合、foundを1に設定してループを抜ける。
            貸出機能選択ループへ移行*/
			if (bookName[i] == '\0') {
				found = 1;
				break;
			}
		}

		fclose(fp);

        /* 貸出機能選択ループ */

		if (found) {
			while (1) { 
				printf("ヒットした書籍: %s\n", bookCode);
				printf("貸出機能へ移行しますか？ 1:はい  2:いいえ\n");

				if (scanf("%d", &choice) != 1) {
					scanf("%*s");
					printf("無効な選択です。1か2でご回答ください。\n");
					continue;
				}

				if (choice == 1) {
					registerLoan();
					return;
				}

				if (choice == 2) {
					printf("またのご利用をお待ちしております。\n");
					break;
				}

				printf("無効な選択です。1か2でご回答ください\n");
			}
		} else {
			printf("書籍が見つかりませんでした\n");
		}
	}
}

return 0;
}