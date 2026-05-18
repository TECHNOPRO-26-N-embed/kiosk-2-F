#include <stdio.h>

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

    while(i == 1){
        printf("\n==================================\n");
        printf("\n           メインメニュー          \n");
        printf("\n==================================\n");
        printf("\n   [1]貸出   [2]検索   [3]ID登録   \n\n");
        scanf("%d",&state);//１，２，３を選択

        if(state == 1){//貸出
            switch(state){
            case 1://ID確認
            printf("\nIDを入力して下さい。\n\n");
            scanf("%d",&p_ID);
            if(p_ID == 1){
                state == 2;
            }else if(p_ID == 0){
                state == 3;
                //printf("ID確認へ戻る\n");
            }
            break;
            case 2://本コードを入力
            printf("\n本コードを入力して下さい。\n\n");
            scanf("%d",&b_ID);
            if(b_ID == 0){
                state <= state + 1;
            }else{
                state <= 2;
            }
            case 3://期間の設定
            printf("貸出期間を設定して下さい。\n");
            printf("４泊５日[1]または７泊８日[2]\n");
            scanf("%d",&rentalday);
            case 4://確認
            printf("〇泊〇日で間違いないですか。\n");
            case 5://支払い
            default:
            printf("3つの項目から選んで下さい。\n");
            break;
        }
        }else if(state == 2){//検索

        }else if(state == 3){//ID登録

        }else{//１，２，３以外の時

        }
    }
    return 0;
}