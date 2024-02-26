# samples/platformer

## 使用モジュール

| モジュール名                        | 説明                              |
|-----------------------------------|-----------------------------------|
| Raspberry Pi Pico                 | マイコン                           |
| MSP2807                           | LCDモジュール                       |
| YMZ294                            | 音源IC(SSG)                       |
| Mini 2-Axis Analog Thumbstick     | Adafruit社製、アナログジョイスティック |

## ピンアサイン

Raspberry Pi Picoのピンアサインに関する詳細は、以下の各ソースコードをご参照ください。

 - LCDモジュール:
   [display_ili9341_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c)

 - ゲームパッド:
   [gamepad_rp2040.c](https://github.com/nyannkov/mgc/blob/main/devices/mgc/gamepad/rp2040/gamepad_rp2040.c)
   
   補足: ゲームパッドのKEY0, KEY1にはタクトスイッチを使用しています。Raspberry Pi Pico側でプルアップを行い、スイッチがONの場合に入力ピンをGNDに接続するように配線しています。

 - 音源IC:
   [sound_ymz294_rp2040.cpp](https://github.com/nyannkov/mgc/blob/main/devices/mgc/sound/psg/ymz294/rp2040/sound_ymz294_rp2040.cpp)
   
   補足: 音源が不要の場合は、省略可能です。注意: YMZ294は5V系のICです。Raspberry Pi Picoの出力ピンとYMZ294の入力ピンを直結して制御することができますが、Raspberry Pi Picoが損傷する恐れがあります。そのため、必要に応じて適切な電圧レベルシフターの使用をご検討ください。

## 注意事項

このサンプルは未完成です。今後サンプルの内容を追加しながらライブラリ自体の機能の追加や修正、削除を行っていく予定です。その点につきましてご注意いただけますと幸いです。

