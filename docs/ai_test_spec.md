## 単体テスト仕様書テンプレート

| テストID | テスト項目 | 入力値 | 期待結果 | 実施日 | 実施者 | 結果 | 備考 |
|----------|------------|--------|----------|--------|--------|------|------|
| UT-001   | searchBook(書籍名検索) | bookName="吾輩は猫である", bookCode=NULL | 書籍情報が返る |        |        |      |      |
| UT-002   | searchBook(書籍コード検索) | bookName=NULL, bookCode="B001" | 書籍情報が返る |        |        |      |      |
| UT-003   | searchBook(両方NULL) | bookName=NULL, bookCode=NULL | エラー |        |        |      |      |
| UT-004   | registerLoan(正常) | userID="U001", bookCode="B001", period=14 | 成功 |        |        |      |      |
| UT-005   | registerLoan(貸出中) | userID="U001", bookCode="B002", period=14 | エラー |        |        |      |      |
| UT-006   | processReturn(正常) | userID="U001", bookCode="B001", returnDate=2026-05-20 | 延滞料金0, 成功 |        |        |      |      |
| UT-007   | processReturn(延滞) | userID="U001", bookCode="B001", returnDate=2026-06-10 | 延滞料金計算, 成功 |        |        |      |      |
| UT-008   | calculateOverdueFee(正常) | loanDate=2026-05-01, returnDate=2026-05-20, period=14 | 延滞料金=250 |        |        |      | 1日50円×5日超過 |
| UT-009   | issueUserID(正常) | name="山田太郎", phone="090...", email="a@b.com", address="東京都" | userID発行, 成功 |        |        |      |      |
| UT-010   | getBookStatus(正常) | bookCode="B001" | ステータス情報返却 |        |        |      |      |
| UT-011   | excludeBook(正常) | bookCode="B003", reason="破損" | 成功 |        |        |      |      |
| UT-012   | processPayment(現金) | userID="U001", amount=500, method=現金 | 成功 |        |        |      |      |
| UT-013   | processPayment(クレジット) | userID="U001", amount=500, method=クレジット | 成功 |        |        |      |      |
| UT-014   | exportHistoryCSV(貸出) | type=貸出 | ファイル出力成功 |        |        |      |      |
| UT-015   | showOperationLog() | なし | 操作履歴リスト返却 |        |        |      |      |

---

## 結合テスト仕様書テンプレート

| テストID | 結合パターン | 前提条件 | 入力値 | 期待結果 | 実施日 | 実施者 | 結果 | 備考 |
|----------|--------------|----------|--------|----------|--------|--------|------|------|
| IT-001   | 貸出→返却 | 書籍B001が貸出可能 | userID="U001", bookCode="B001", period=14→返却日=2026-05-20 | 貸出成功→返却成功・延滞料金0 |        |        |      |      |
| IT-002   | 貸出→延滞返却 | 書籍B001が貸出可能 | userID="U001", bookCode="B001", period=14→返却日=2026-06-10 | 貸出成功→返却成功・延滞料金計算 |        |        |      |      |
| IT-003   | 利用者登録→貸出 | 新規利用者 | name, phone, email, address→userID取得→貸出 | 利用者登録成功→貸出成功 |        |        |      |      |
| IT-004   | 貸出→支払 | 書籍B001貸出中 | userID="U001", amount=500, method=現金 | 支払成功 |        |        |      |      |
| IT-005   | 除外→貸出不可 | 書籍B003を除外 | bookCode="B003", reason="破損"→貸出 | 除外成功→貸出エラー |        |        |      |      |
| IT-006   | CSV出力 | 貸出履歴あり | type=貸出 | ファイル出力成功 |        |        |      |      |
| IT-007   | 操作ログ表示 | 操作履歴あり | なし | 操作履歴リスト返却 |        |        |      |      |
