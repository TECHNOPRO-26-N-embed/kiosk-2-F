#include <stdio.h>

int main(){
    int rentalMoney = 100;
    int lateMoney = 50;
    int total;

    int paymentMethod;
    int cash = 1000;
    int change;

    total = rentalMoney + lateMoney;

    printf("========================\n");
    printf("支払画面\n");
    printf("========================\n");
    printf("貸出料金 : %d円\n", rentalMoney);
    printf("延滞料金 : %d円\n", lateMoney);
    printf("合計金額 : %d円\n", total);

    printf("支払方法を選択してください\n");
    printf("1. 現金\n");
    printf("2. クレジットカード\n");
    printf("入力 : ");
    scanf("%d", &paymentMethod);

    if(paymentMethod == 1){
        printf("お金を投入してください\n");

        if(cash < total){
            printf("=========================\n");
            printf("金額が不足しています\n");
            printf("=========================\n");
        }else{
            change = cash - total;

            printf("\n決済中...\n");
            
            printf("=========================\n");
            printf("支払い完了\n");
            printf("お釣り : %d円\n", change);
            printf("=========================\n");

        }
    }
    else if(paymentMethod == 2){

        printf("クレジットカードを読み取っています...\n");
        printf("決済中...\n");

        printf("=========================\n");
        printf("支払い完了\n");
        printf("=========================\n");
    }else{
        printf("=========================\n");
        printf("無効な入力です\n");
        printf("=========================\n");
    }

    printf("ご利用ありがとうございました\n");
    return 0;
}