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