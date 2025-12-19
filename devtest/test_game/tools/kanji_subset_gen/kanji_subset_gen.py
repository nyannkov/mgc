import argparse
import re

BASE_STRING = """abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789
 !"#$%&'()-^\\@[;:],./\\=~|`{+*}<>?_
ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん
ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ
　 、。，．・：；？！〜ー「」（）
ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ
ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ
０１２３４５６７８９
"""

def extract_kanji(text: str) -> str:
    kanji = re.findall(r'[\u4E00-\u9FFF]', text)
    return "".join(sorted(set(kanji)))


def main():
    parser = argparse.ArgumentParser(description="Extract kanji and append to base string.")
    parser.add_argument("-s", "--source", required=True, help="Input file path (UTF-8)")
    parser.add_argument("-d", "--dest", required=True, help="Output file path")
    args = parser.parse_args()

    with open(args.source, "r", encoding="utf-8") as f:
        text = f.read()

    kanji = extract_kanji(text)

    output = BASE_STRING + kanji + "\n"

    with open(args.dest, "w", encoding="utf-8") as f:
        f.write(output)

    print(f"Saved to {args.dest}")

if __name__ == "__main__":
    main()

