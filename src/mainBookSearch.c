#include <stdio.h>
#include <string.h>


int main(void) {
    
    char BookTitle[][100] = {"anpanman"};   /*データベース内書籍名*/

    char BookSearch[100];   /*入力文字格納場所*/

    int i = 1;     /*ループ制御用変数*/
    int choice;   /*購入選択用変数*/
    

    while (i == 1){
        /* 書籍検索ループ */
        printf("==============================\n");
        printf("書籍検索ページ\n");
        printf("==============================\n");
        printf("終了する場合は「OSHIMAI」を入力してください。\n");
        printf("書籍のタイトルを入力してください(ローマ字・読み): ");
        scanf("%s", &BookSearch);
        printf("入力文字: %s\n", BookSearch);


        if (BookSearch || BookTitle) {
            printf("書籍の候補はこちら：%s \n", BookTitle);
            printf("買いますか？　1:はい　2:いいえ\n");
            scanf("%d", &choice);
            if (choice == 1) {
                printf("購入いただきありがとうございます！\n");
                break;

            } else if (choice == 2) {
                printf("またのご利用をお待ちしております。\n");
            } else {
                printf("無効な選択です。さようなら。\n");
            }
        } else  {
            printf("書籍が見つかりませんでした。\n");
        }
        if (strcmp(BookSearch, "OSHIMAI") == 0) {
            printf("書籍検索を終了します。\n");
            break;
        }
    }
    return 0;
}