実装すべき関数一覧と役割
1.searchBook(bookName, bookCode)
書籍名またはコードで書籍情報を検索し、結果を返す

2.registerLoan(userID, bookCode, period)
利用者IDと本コード、貸出期間を受け取り、貸出情報を登録

3.processReturn(userID, bookCode, returnDate)
返却処理を行い、返却日・延滞料金を計算

4.calculateOverdueFee(loanDate, returnDate, period)
返却期限超過時の延滞料金を自動計算

5.issueUserID(name, phone, email, address)
利用者情報を登録し、ID（紙カード）を発行

6.getBookStatus(bookCode)
書籍ごとの貸出/返却/延滞などのステータスを取得

7.excludeBook(bookCode, reason)
破損等の理由で書籍を貸出対象から除外

8.processPayment(userID, amount, method)
支払処理（現金またはクレジットカード）を行う

9.exportHistoryCSV(type)
貸出・返却・支払等の履歴をCSV形式で出力

10.showOperationLog()
システムの操作履歴を表示


1. searchBook(bookName, bookCode)
- アルゴリズム:  bookNameまたはbookCodeで書籍データベースを検索し、一致する書籍情報を返す。
- 入力:  bookName（文字列, NULL可）、bookCode（文字列, NULL可）
- 出力:  書籍情報構造体（見つからない場合はNULLやエラーコード）
- エラー処理:  両方NULLの場合はエラー。該当書籍がない場合もエラー。

2. registerLoan(userID, bookCode, period)
- アルゴリズム:  userIDとbookCodeの存在確認、書籍が貸出可能か確認し、貸出情報を登録する。
- 入力:  userID（文字列）、bookCode（文字列）、period（日数, int）
- 出力:  成功/失敗（boolやステータスコード）
- エラー処理:  userIDやbookCodeが存在しない場合、書籍が貸出中・除外中の場合はエラー。

3. processReturn(userID, bookCode, returnDate)
- アルゴリズム:  貸出履歴から該当データを取得し、返却日を記録、延滞料金を計算する。
- 入力:  userID（文字列）、bookCode（文字列）、returnDate（日付）
- 出力:  延滞料金（int）、成功/失敗
- エラー処理:  該当貸出履歴がない場合や既に返却済みの場合はエラー。

4. calculateOverdueFee(loanDate, returnDate, period)
- アルゴリズム:  返却期限（loanDate+period）を計算し、返却日が期限を超過していれば日数分の料金を計算する。
- 入力:  loanDate（日付）、returnDate（日付）、period（日数, int）
- 出力:  延滞料金（int）
- エラー処理:  日付や期間が不正な場合はエラー。

5. issueUserID(name, phone, email, address)
- アルゴリズム:  入力情報のバリデーション、ユーザーID生成（重複チェック）、利用者情報を登録する。
- 入力:  name, phone, email, address（全て文字列）
- 出力:  userID（文字列）、成功/失敗
- エラー処理:  入力不備や既存ユーザーと重複する場合はエラー。

6. getBookStatus(bookCode)
- アルゴリズム:  bookCodeで書籍情報を取得し、貸出/返却/延滞/除外などの状態を返す。
- 入力:  bookCode（文字列）
- 出力:  ステータス情報構造体
- エラー処理:  bookCodeが不正な場合はエラー。

7. excludeBook(bookCode, reason)
- アルゴリズム:  bookCodeで書籍情報を取得し、除外理由を記録して貸出不可に設定する。
- 入力:  bookCode（文字列）、reason（文字列）
- 出力:  成功/失敗
- エラー処理:  bookCodeが不正、または既に除外済みの場合はエラーや警告。

8. processPayment(userID, amount, method)
- アルゴリズム:  userIDの存在確認、支払方法（現金/クレジット）に応じて処理、支払履歴を記録する。
- 入力:  userID（文字列）、amount（int/float）、method（enum: 現金/クレジット）
- 出力:  成功/失敗
- エラー処理:  userIDや金額が不正、支払失敗（クレジット決済エラー等）の場合はエラー。

9. exportHistoryCSV(type)
- アルゴリズム:  typeに応じて履歴データを抽出し、CSV形式でファイル出力する。
- 入力:  type（enum: 貸出/返却/支払）
- 出力:  ファイルパスまたは出力結果
- エラー処理:  typeが不正、出力失敗の場合はエラー。

10. showOperationLog()
- アルゴリズム:  操作履歴データを取得し、画面やファイル等に表示する。
- 入力:  なし
- 出力:  操作履歴リスト
- エラー処理:  履歴データがない場合は「履歴なし」と表示。


