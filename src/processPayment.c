#include <stdio.h>

int processPayment(int userID, int amount, int method)
{
    if (userID <= 0 || amount <= 0) {
        printf("\n支払いデータが不正です。\n");
        return -1;
    }

    printf("\n===== 支払処理 =====\n");
    printf("利用者ID: %d\n", userID);
    printf("支払金額: %d円\n", amount);

    if (method == 1) {
        printf("支払方法: 現金\n");
        printf("現金を受け取りました。\n");
    } else if (method == 2) {
        printf("支払方法: クレジットカード\n");
        printf("カード決済を実行しています...\n");
    } else {
        printf("支払方法が不正です。\n");
        return -2;
    }

    printf("支払いが完了しました。\n");
    return 0;
}
