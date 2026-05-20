#include <stdio.h>
#include "registerLoan.h"

int main(void)
{
    int state; /* 各状態管理 */
    int result;

    while (1) {
        printf("\n==========================================\n");
        printf("\n               メインメニュー              \n");
        printf("\n==========================================\n");
        printf("\n  [1]貸出  [2]検索  [3]ID登録  [4]状況確認  \n\n");
        scanf("%d", &state); /* 1,2,3,4 を選択 */

        if (state == 1) { /* 貸出 */
            result = registerLoan(NULL, NULL, 0);
            if (result != 0) {
                printf("\n貸出登録に失敗しました。\n");
            }
        } else if (state == 2) { /* 検索 */
            printf("\n検索機能は未実装です。\n");
        } else if (state == 3) { /* ID登録 */
            printf("\nID登録機能は未実装です。\n");
        } else if (state == 4) { /* 状況確認 */
            printf("\n状況確認機能は未実装です。\n");
        } else {
            printf("\n[1],[2],[3],[4]を選択して下さい。\n");
        }
    }
    return 0;
}