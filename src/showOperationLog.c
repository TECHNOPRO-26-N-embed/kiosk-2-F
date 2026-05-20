// printfを使うための標準入出力ライブラリ
#include <stdio.h>
// strcpyなど文字列処理を使うためのライブラリ
#include <string.h>
// 現在時刻を取るためのライブラリ
#include <time.h>

// 数値の結果コードを日本語ラベルに変換する小さな関数
static const char *resultLabel(int result)
{
	// 0は成功
	if (result == 0) {
		// 文字列「成功」を返す
		return "成功";
	}
	// 1は失敗
	if (result == 1) {
		// 文字列「失敗」を返す
		return "失敗";
	}
	// それ以外はエラー扱い
	return "エラー";
}

// 他ファイルで集めた情報を受け取り、CSV形式で1行ログを表示する関数
void showOperationLog(const char *userID,
				  // scanfで入力された内容（他関数で取得済み）
				  const char *scanfInput,
				  // その入力に対して表示したprintf内容（他関数で作成済み）
				  const char *printfOutput,
				  // 本のステータス変更前
				  const char *bookStatusBefore,
				  // 本のステータス変更後
				  const char *bookStatusAfter,
				  // 0=成功, 1=失敗, 2=エラー
				  int resultCode,
				  // エラーのときの詳細メッセージ
				  const char *errorDetail)
{
	/*
	 * この関数は手入力を受け付けず、他ファイルで確定した値をそのまま引用してCSV出力する。
	 * 引用元の想定:
	 * - userID: ID登録/貸出処理で確定した利用者ID
	 * - scanfInput: 各処理で実際にscanfで受け取った入力値
	 * - printfOutput: 上記入力に対応して画面へ出したprintf文言
	 * - bookStatusBefore / bookStatusAfter: 書籍ステータス変更前後の値
	 * - resultCode: 0=成功, 1=失敗, 2=エラー
	 * - errorDetail: resultCodeが2のときのエラー詳細(日本語)
	 */
	// 日時文字列を入れる箱（yyyy/mm/dd/hh:mm:ss は19文字+終端1文字）
	char operationDateTime[20];
	// NULLや空文字だった場合に補正した安全なuserID
	const char *safeUserID;
	// NULLや空文字だった場合に補正した安全なscanf入力
	const char *safeScanfInput;
	// NULLや空文字だった場合に補正した安全なprintf出力
	const char *safePrintfOutput;
	// NULLや空文字だった場合に補正した安全な変更前ステータス
	const char *safeBookStatusBefore;
	// NULLや空文字だった場合に補正した安全な変更後ステータス
	const char *safeBookStatusAfter;
	// NULLや空文字だった場合に補正した安全なエラー詳細
	const char *safeErrorDetail;
	// 現在時刻（数値）
	time_t now;
	// 数値時刻を年月日時分秒へ分解した構造体ポインタ
	struct tm *local;

	// resultCodeが0/1/2以外なら不正値としてエラーに強制する
	if (resultCode != 0 && resultCode != 1 && resultCode != 2) {
		// 結果コードを2（エラー）に丸める
		resultCode = 2;
		// 不正コードだったことをエラー内容に設定
		safeErrorDetail = "操作結果コードが不正です";
	} else {
		// エラー詳細がNULLや空文字なら「未指定」を入れる
		safeErrorDetail = (errorDetail != NULL && errorDetail[0] != '\0') ? errorDetail : "未指定";
	}

	// userIDがNULL/空ならUNKNOWNを使う
	safeUserID = (userID != NULL && userID[0] != '\0') ? userID : "UNKNOWN";
	// scanf入力がNULL/空ならN/Aを使う
	safeScanfInput = (scanfInput != NULL && scanfInput[0] != '\0') ? scanfInput : "N/A";
	// printf出力がNULL/空ならN/Aを使う
	safePrintfOutput = (printfOutput != NULL && printfOutput[0] != '\0') ? printfOutput : "N/A";
	// 変更前ステータスがNULL/空ならN/Aを使う
	safeBookStatusBefore = (bookStatusBefore != NULL && bookStatusBefore[0] != '\0') ? bookStatusBefore : "N/A";
	// 変更後ステータスがNULL/空ならN/Aを使う
	safeBookStatusAfter = (bookStatusAfter != NULL && bookStatusAfter[0] != '\0') ? bookStatusAfter : "N/A";

	/* 操作日時はログ出力時点の現在時刻を本関数内で生成する。 */
	// 現在時刻を取得
	now = time(NULL);
	// 現在時刻をローカル時刻に分解
	local = localtime(&now);
	// 変換失敗時はダミー日時を入れる
	if (local == NULL) {
		// 失敗時に固定の日時文字列を設定
		strcpy(operationDateTime, "0000/00/00/00:00:00");
	} else {
		// 成功時は指定フォーマットで日時文字列を作る
		strftime(operationDateTime, sizeof(operationDateTime), "%Y/%m/%d/%H:%M:%S", local);
	}

	// 見やすさのために空行を1つ出す
	printf("\n");
	// CSVヘッダを表示
	printf("operation_user(userID),operation_datetime(yyyy/mm/dd/hh:mm:ss),operation_history,book_status_change,operation_result\n");

	// resultCodeが2（エラー）の場合は、エラー内容もCSVに含める
	if (resultCode == 2) {
		// エラー詳細付きCSV行を表示
		printf("%s,%s,\"scanf:%s | printf:%s\",\"%s -> %s\",\"%d=%s(エラー内容:%s)\"\n",
			   safeUserID,
			   operationDateTime,
			   safeScanfInput,
			   safePrintfOutput,
			   safeBookStatusBefore,
			   safeBookStatusAfter,
			   resultCode,
			   resultLabel(resultCode),
			   safeErrorDetail);
	} else {
		// 成功/失敗時の通常CSV行を表示
		printf("%s,%s,\"scanf:%s | printf:%s\",\"%s -> %s\",\"%d=%s\"\n",
			   safeUserID,
			   operationDateTime,
			   safeScanfInput,
			   safePrintfOutput,
			   safeBookStatusBefore,
			   safeBookStatusAfter,
			   resultCode,
			   resultLabel(resultCode));
	}
}
