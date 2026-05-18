#include <stdio.h>

int main(void){
    int state;
    int p_ID;
    int b_ID;
    int rental;
    
    printf("\n==================================\n");
    printf("\n           メインメニュー          \n");
    printf("\n==================================\n");
    printf("\n   [1]貸出   [2]検索   [3]ID登録   \n\n");
    scanf("%d",&state);
    

    while(1){
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
            scanf("%d",&rental);
            case 4://確認
            printf("〇泊〇日で間違いないですか。\n");
            case 5://支払い
            default:
            printf("3つの項目から選んで下さい。\n");
            break;
        }
    }

    return 0;
}