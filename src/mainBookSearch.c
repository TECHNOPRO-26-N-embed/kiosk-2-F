#include <stdio.h>
#include <string.h>



int BookSearch0(void)   /*書籍検索関数の宣言*/{
    
    char BookTitle[][100] = {"anpanman"};   /*データベース内書籍名*/

    char BookSearch[100];   /*入力文字格納場所*/

    int book = 1;     /*ループ制御用変数*/
    int choice;   /*購入選択用変数*/
    

    while (book == 1){
        /* 書籍検索ループ */
        printf("==============================\n");
        printf("書籍検索ページ\n");
        printf("==============================\n");
        printf("終了する場合は「OSHIMAI」を入力してください。\n");
        printf("書籍のタイトルを入力してください(ローマ字・読み): ");
        scanf("%s", &BookSearch);
        printf("入力文字: %s\n\n", BookSearch);


        if (strcmp(BookSearch, "anpanman") == 0) {
            printf("書籍の候補はこちら：%s \n", BookTitle[0]);
            printf("買いますか？　1:はい　2:いいえ\n");
            scanf("%d", &choice);
            if (choice == 1) {
                printf("購入いただきありがとうございます！\n");
                break;/*ここのbreakを貸出機能ループに入るための式に変える*/

            } else if (choice == 2) {
                printf("またのご利用をお待ちしております。\n");
                break;

            } else {
                printf("無効な選択です。数字でご回答ください。\n");
                continue;

            }
        } else if (strcmp(BookSearch, "OSHIMAI") == 0) {
            printf("書籍検索を終了します。\n");
            break;
        } 
        else  {
            printf("書籍が見つかりませんでした。\n");
            printf("文字を正確に入力してください。\n\n\n");
        }
    }
}
