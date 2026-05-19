#include <stdio.h>

int Loans(void)
{
    //int state;
    int p_ID;
    int b_ID;
    int rentalday;
    int yesno;

    while (1){
        while (1){//ID入力
            printf("\nIDを入力して下さい。\n");
            scanf("%d", &p_ID);

            if (p_ID != 1){
                printf("IDが間違っています。\n");
                continue; // 1へ戻る
            }
            while (1){// 2. 本コード入力
                printf("\n本コードを入力して下さい。\n");
                scanf("%d", &b_ID);

                if (b_ID != 0){
                    printf("本コードが間違っています。\n");
                    continue; // 2へ戻る
                }
                while (1){// 3. 期間設定
                    printf("\n貸出期間を設定して下さい。\n");
                    printf("４泊５日[1] または ７泊８日[2]\n");
                    scanf("%d", &rentalday);

                    if (rentalday != 1 && rentalday != 2){
                        printf("入力が不正です。\n");
                        continue; // 3へ戻る
                    }
                    // 4. 確認画面
                    if (rentalday == 1){
                        printf("4泊5日で間違いないですか？ はい[1] いいえ[2]\n");
                    }else{
                        printf("7泊8日で間違いないですか？ はい[1] いいえ[2]\n");
                    }
                    scanf("%d", &yesno);

                    if (yesno == 2){
                        printf("期間設定に戻ります。\n");
                        continue; // 3へ戻る
                    }
                    while (1) {
                        printf("\n支払い方法を選択して下さい。\n");
                        printf("現金[1] または カード[2]\n");
                        scanf("%d", &yesno);

                        if (yesno == 1 || yesno == 2) {// 正しい入力 → 完了画面へ
                            printf("\n貸出が完了しました。\nメインメニューへ戻ります。\n\n");
                            return 0;   // ← Loans() を終了して main() に戻る
                        }
                        // 不正入力
                        printf("入力が不正です。もう一度入力してください。\n");
                    }
                }
            }
        }
    }
    return 0;
}
