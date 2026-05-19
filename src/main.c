#include <stdio.h>
#include "Loans.h"
#include "ID_Register.h"

int main(void){
    int i=1;// ループ制御用の変数
    int total;//合計金額
    int rentalmoney;//料金
    int cash;//現金
    //int ;
    int p_ID;//本コード
    int b_ID;//会員コード
    int rentalday;//レンタル日数
    int num1;// 規約読みましたかの確認
    int num2;// 入力した情報で間違いないかの確認
    int status;//状態保存
    char name[100];//名前
    int tel;//電話番号
    char adress[200];//住所
    char mail[100];//メールアドレス
    //char BookTitle[][100];//データベース内書籍名
    char BookSearch[100]; //入力文字格納場所
    int choice;//選択肢格納場所
    int state;

    while(1){
        printf("\n==================================\n");
        printf("\n           メインメニュー          \n");
        printf("\n==================================\n");
        printf("\n   [1]貸出   [2]検索   [3]ID登録   \n\n");
        scanf("%d",&state);//１，２，4を選択

        if(state == 1){//貸出
           Loans();
        }else if(state == 2){//検索

        }else if(state == 3){//ID登録
            ID_Register();
        }else if(state == 4){//状況確認
            
        }else{//１，２，３,４以外の時
           printf("\n[1],[2],[3],[4]を選択して下さい。\n");
        }
    }
    return 0;
}