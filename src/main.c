#include <stdio.h>
#include "Loans.h"
#include "ID_Register.h"
#include "BookSearch0.h"


int main(void){
    int state;//各状態管理

    while(1){
        printf("\n==================================\n");
        printf("\n           メインメニュー          \n");
        printf("\n==================================\n");
        printf("\n  [1]貸出  [2]検索  [3]ID登録  [4]状況確認  \n\n");
        scanf("%d",&state);//１，２，4を選択

        if(state == 1){//貸出
           Loans();
        }else if(state == 2){//検索
            BookSearch0();
        }else if(state == 3){//ID登録
            ID_Register();
        }else if(state == 4){//状況確認
            
        }else{//１，２，３,４以外の時
           printf("\n[1],[2],[3],[4]を選択して下さい。\n");
        }
    }
    return 0;
}