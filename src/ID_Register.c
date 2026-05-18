#include<stdio.h>

void main(void){

    int num;


    printf("規約を読みましたか？(1:はい 2:いいえ) >");
    scanf("%d",&num);

    if(num == 1){
        printf("ありがとうございます。");
    }
    else if(num == 2 ){
        return;
    }
    else{
        printf("1か2を入力してください。");
        return;
    }
    
}