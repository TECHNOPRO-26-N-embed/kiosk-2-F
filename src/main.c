#include <stdio.h>
#include "registerLoan.h"
#include "issueUserID.h"

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
            char name[64], phone[32], email[64], address[128], userID[16];
            printf("\n氏名: ");
            scanf("%63s", name);
            printf("電話番号: ");
            scanf("%31s", phone);
            printf("メール: ");
            scanf("%63s", email);
            printf("住所: ");
            scanf("%127s", address);
            int res = issueUserID(name, phone, email, address, userID, sizeof(userID));
            if (res == ISSUE_USER_ID_OK) {
                printf("\nユーザー登録成功: userID=%s\n", userID);
            } else if (res == ISSUE_USER_ID_ALREADY_EXISTS) {
                printf("\n既存ユーザー: userID=%s\n", userID);
            } else if (res == ISSUE_USER_ID_DUPLICATE_USER) {
                printf("\n電話番号またはメールが既存ユーザーと重複しています\n");
            } else {
                printf("\nID登録エラー: %d\n", res);
            }
        } else if (state == 4) { /* 状況確認 */
            printf("\n状況確認機能は未実装です。\n");
        } else {
            printf("\n[1],[2],[3],[4]を選択して下さい。\n");
        }
    }
    return 0;
}