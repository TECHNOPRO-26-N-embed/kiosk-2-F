#include<stdio.h>

void ID_Register(void){

    int i=1;// ループ制御用の変数
    int ID_num=1;//会員番号の変数    
    int num1;// 規約読みましたかの確認
    int num2;// 入力した情報で間違いないかの確認
    char name[100];//名前  
    int tel;//電話番号
    char adress[200];//住所
    char mail[100];//メールアドレス


    while (i == 1){
        printf("規約を読みましたか？(1:はい 2:いいえ) > \n");
        scanf("%d",&num1);
        i = 2;

        while (i == 2){
            if(num1 == 1){
                printf("氏名を入力してください > \n");
                scanf("%s", name);
                printf("電話番号を入力してください > \n");
                scanf("%d", &tel);
                printf("住所を入力してください > \n");
                scanf("%s", adress);
                printf("メールアドレスを入力してください > \n");
                scanf("%s", mail);
                i = 3;


                while(i == 3){
                    printf("氏名は%s\n", name);
                    printf("電話番号は%d\n", tel);
                    printf("住所は%s\n", adress);
                    printf("メールアドレスは%s\n", mail);
                    printf("でお間違いないでしょうか？(1:はい 2:訂正) > \n");
                    scanf("%d",&num2);
                    i=4;

                    if(num2 == 1){
                        printf("CSVファイルに保存しています。\n");
                        printf("カードを発行しています。\n");
                        printf("あなたの会員番号は%dです。\n", ID_num);
                        printf("カードを発行しました。ご利用ありがとうございました。\n");
                        ID_num++; //会員番号を1つ増やす
                        break;                           


                    }
                    else if(num2 == 2){
                        printf("情報を打ち直してください。\n");
                        i=2;
                        break;
                        
                    }
                    else{
                        printf("1か2を入力してください。\n");
                        i=3;
                        break;
                    }

                }
                

                
            }
            else if(num1 == 2 ){
                printf("規約を読んでください。\n");
                i=1;
                break;
            }
            else{
                printf("1か2を入力してください。\n");
                i=1;
                break;
            }            

        }
        

                
    }
    

}