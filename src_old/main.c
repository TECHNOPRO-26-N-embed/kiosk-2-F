#include <stdio.h>
#include "Loans.h"
#include "ID_Register.h"
#include "BookSearch0.h"

int registerLoan(const char *userID, const char *bookCode, int period);


int main(void){
    int state;//各状態管理
    char userID[64];
    char bookCode[64];
    int period;
    int result;

    while(1){
        printf("\n==========================================\n");
        printf("\n               メインメニュー              \n");
        printf("\n==========================================\n");
        printf("\n  [1]貸出  [2]検索  [3]ID登録  [4]状況確認  \n\n");
        scanf("%d",&state);//１，２，4を選択

        if(state == 1){//貸出
            printf("\n利用者IDを入力してください: ");
            scanf("%63s", userID);

            printf("本コードを入力してください: ");
            scanf("%63s", bookCode);

            printf("貸出期間を入力してください (4泊5日[1 or 5], 7泊8日[2 or 8]): ");
            scanf("%d", &period);

            result = registerLoan(userID, bookCode, period);
            if (result == 0) {
                printf("\n貸出情報を登録しました。\n");
            } else {
                printf("\n貸出登録に失敗しました。入力値を確認してください。\n");
            }
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