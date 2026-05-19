#include <stdio.h>

int main(void)
{
    int state;
    int p_ID;
    int b_ID;
    int rentalday;
    int yesno;

    while (1)
    {
        // 0. メインメニュー
        printf("\n==================================\n");
        printf("           メインメニュー\n");
        printf("==================================\n");
        printf("[1]貸出   [2]検索   [3]ID登録\n");
        scanf("%d", &state);

        switch (state)
        {
        case 1: // 貸出
            // 1. ID入力
            while (1)
            {
                printf("\nIDを入力して下さい。\n");
                scanf("%d", &p_ID);

                if (p_ID != 1)
                {
                    printf("IDが間違っています。\n");
                    continue; // 1へ戻る
                }

                // 2. 本コード入力
                while (1)
                {
                    printf("\n本コードを入力して下さい。\n");
                    scanf("%d", &b_ID);

                    if (b_ID != 0)
                    {
                        printf("本コードが間違っています。\n");
                        continue; // 2へ戻る
                    }

                    // 3. 期間設定
                    while (1)
                    {
                        printf("\n貸出期間を設定して下さい。\n");
                        printf("４泊５日[1] または ７泊８日[2]\n");
                        scanf("%d", &rentalday);

                        if (rentalday != 1 && rentalday != 2)
                        {
                            printf("入力が不正です。\n");
                            continue; // 3へ戻る
                        }

                        // 4. 確認画面
                        if (rentalday == 1)
                            printf("4泊5日で間違いないですか？ はい[1] いいえ[2]\n");
                        else
                            printf("7泊8日で間違いないですか？ はい[1] いいえ[2]\n");

                        scanf("%d", &yesno);

                        if (yesno == 2)
                        {
                            printf("期間設定に戻ります。\n");
                            continue; // 3へ戻る
                        }

                        if (yesno == 1)
                        {
                            // 5. 支払い画面
                            printf("\n支払い方法を選択して下さい。\n");
                            printf("現金[1] または カード[2]\n");
                            scanf("%d", &yesno);

                            // 6. 完了画面
                            printf("\n貸出が完了しました。\nメインメニューへ戻ります。\n\n");
                            goto END_RENTAL; // メインメニューへ戻る
                        }
                    }
                }
            }
        END_RENTAL:
            break;

        case 2:
            printf("検索機能は未実装です。\n");
            break;

        case 3:
            printf("ID登録機能は未実装です。\n");
            break;

        default:
            printf("入力が不正です。\n");
            break;
        }
    }

    return 0;
}
