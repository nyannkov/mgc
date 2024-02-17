# 開発環境構築メモ
## Ubuntu 22.04 + Raspberry Pi Pico + OpenOCD + J-Link

開発環境構築メモです。Ubuntu 22.04はWindows 10のVirtualBox 7.0上に起動したものを使用しました。

(1) 以下のコマンドを実行します。
```bash
sudo apt install git
sudo apt install pkg-config
```
(2) [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)の
"Chapter 1. Quick Pico Setup"を実行します。

(3) [SEGGER社のページ](https://www.segger.com/downloads/jlink/)から"J-Link Software and Documentation pack"をダウンロードし、以下のコマンドを実行します(インストールファイルはダウンロードしたものを指定してください)。
```bash
sudo apt install ./JLink_Linux_V794i_x86_64.deb
```

(4) [みつきんさんの記事](https://mickey-happygolucky.hatenablog.com/entry/2021/02/24/193703)の「設定ファイル」節の内容で
${PICO_SDK_PATH}/../openocd/tcl/pico-jlink.cfgを作成します。

(5) examplesの[hello_pixelbuffer](examples/pixelbuffer/hello_pixelbuffer)に移動し、以下のコマンドを実行します。成功すれば環境構築完了です。
```bash
make connect
make debug
```
