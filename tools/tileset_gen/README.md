# tileset_gen.py

このスクリプトは、mgcでビットマップ画像を取り扱うためのユーティリティです。
インデックスカラー形式のビットマップ画像をmgc_tileset_t構造体定数に変換し、C言語のソースコードとして生成します。

## タイルセットの作り方について

タイルセットは次の画像のように左上からタイルを並べるようにして作成します。
タイルがビットマップ画像の幅を超える場合は、一段下げて左から並べるようにします。
なお、ビットマップ画像の幅に制限はありません。作成しやすい幅・高さでビットマップ画像を作成できます。



<div style="display: flex;">
  <img src="img/ex_tile_count.bmp" style="width: 30%;">
  <img src="img/ex_tile_width_height.bmp" style="width: 30%;">
</div>


## 使用例

```bash
python3 tileset_gen.py ./img/ex_tile_count.bmp --dir ./generates --width 16 --height 16 --count 11
```

この例では、ビットマップ画像ex_tile_count.bmpから、幅が16px、高さが16pxである11個のタイルを
mgc_tileset_t構造体定数に変換し、./generatesにC言語のソースコードとして保存します。


width, heightオプションにはそれぞれタイルの幅、高さを指定します。これらのデフォルト値はいずれも16pxです。
countオプションには、ビットマップ画像内のタイル数を指定します。countを省略すると、以下の計算式でタイル数が算出されます。

```python
count = (bitmap_width//tile_width)*(bitmap_height//tile_height)
```

また、countオプションに1を指定した場合、タイルの幅および高さは、それぞれ強制的にビットマップ画像の幅、高さに変換されます。
以下の例では、タイルの幅および高さは、それぞれ、ビットマップ画像の幅および高さになります。

```bash
python3 tileset_gen.py ./img/ex_count_1.bmp --dir ./generates --count 1
```

## 注意

 - スクリプトで読み込み可能なビットマップ画像は、インデックスカラー形式のみとなります。
 - インデックス番号0の色は、透明色として取り扱われます。
