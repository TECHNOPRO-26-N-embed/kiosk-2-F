#include <stdio.h>


struct Book {
    int status;// 仮定＜貸出状況＞
};

void keep(struct Book *book, int status) {
    book->status = status;
}
int main() {
    struct Book book;

    int status;


    printf("======状態を入力======\n");
    printf("| 0 : 貸出可能\n| 1 : 貸出中\n| 2 : 延滞中\n| 3 : 停止中\n");
    printf("======================\n");
    printf("|\n");
    printf("|状態を入力 : ");
    scanf("%d", &status);
    printf("|\n");
    // 貸出状況の入力

    keep(&book, status);
    //状態の保存

    //printf("=====================\n");
    printf("===状態を保存しました===\n");
    printf("|\n");
    if (book.status == 0){
        printf("|現在の状態 : 貸出可能\n");
    }else if (book.status == 1){
        printf("|現在の状態 : 貸出中\n");
    }else if (book.status == 2){
        printf("|現在の状態 : 延滞中\n");
    }else if (book.status == 3){
        printf("|現在の状態 : 停止中\n");
    }else{
        printf("|無効な操作\n");
    }
    printf("|\n");

    return 0;
}