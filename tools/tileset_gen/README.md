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

この例では、ビットマップ画像ex_tile_count.bmpを、幅16px、高さ16pxのタイル11個を
mgc_tileset_t構造体定数に変換し、C言語のソースコードとして./generatesに保存します。


width, heightのデフォルト値はいずれも16pxです。また、countを省略すると、以下の計算式の値を
count値として取り扱われます。


```python
count = (bitmap_width//tile_width)*(bitmap_height//tile_height)
```

さらに、countとして1を指定した場合、タイルの幅および高さは強制的にビットマップ画像の幅、高さに変換されます。
以下に例では、タイルの幅および高さはデフォルト値の16pxではなく、ビットマップ画像の幅および高さとなります。

```bash
python3 tileset_gen.py ./img/ex_count_1.bmp --dir ./generates --count 1
```

## 注意

取り扱えるビットマップ画像はインデックスカラー形式のみとなります。
また、mgcでは、インデックス番号0の色は透明色として取り扱われます。
