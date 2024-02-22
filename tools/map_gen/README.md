# map_gen.py

このスクリプトは、CSVファイルで作成したマップ情報をmgcで取り扱うためのユーティリティです。
CSVファイルで作成したマップ情報をmgc_map_t構造体定数に変換し、C言語のソースコードとして生成します。

## マップ情報ファイル

CSVファイルの最初の行には、フォーマットバージョンを記述します。現在のバージョンは固定で「1」です。
二行目以降のセルには、具体的なマップ情報を記述します。

| VERSION | 1 |   |   |   |
|---------|---|---|---|---|
| 0x81    | 0x81 | 0x81 | 0x81 | 0x81 |
| 0x81    |     |     |     | 0x03 |
| 0x81    |     |     |     | 0x03 |
| 0x81    |     | 0x81 |     | 0x03 |

各セルには1バイトのデータを指定します。データの内容は以下のビットフィールドに従います。

| ビット | 説明                       |
|--------|----------------------------|
| 0-6    | 描画するタイル番号（0x00〜0x7F） |
| 7      | 衝突判定の有効無効（0:無効、1:有効） |


描画するタイル番号はタイルセットのインデックスに対応します。また、衝突判定を有効にすると、
そのセルとスプライトの衝突をmaphitモジュールで検出できるようになります。

## 使用例

```bash
python3 map_gen.py ./map/map_example.csv --dir ./generates --compression none
```

この例では、map_example.csvに記述したマップ情報をmgc_map_t構造体定数に変換し、C言語のソースコードとして./generates
フォルダに保存しています。compressionオプションは生成するマップ配列の圧縮方式を表します。noneを設定すると、CSVファイルに記述した
内容を単純に配列化します。一方、compressionにrunlengthを指定した場合は、マップ情報をrun-lengthで圧縮して配列化します。
同じタイル番号が連続するようなマップ情報の場合、後者のほうがデータサイズが小さくなります。
ただし、解凍処理が発生するため、描画速度は前者と比べて遅くなります。

```bash
python3 map_gen.py ./map/map_example.csv --dir ./generates --compression runlength
```

## 注意事項

 - 空欄セルの値は0とみなされます。
 - タイル番号0は透明タイルとして扱われます。
 - 各セルは大きさが16x16ピクセルのタイルとして扱われます。そのため、タイルセットも16x16ピクセルで作成したものを使用する必要があります。
 