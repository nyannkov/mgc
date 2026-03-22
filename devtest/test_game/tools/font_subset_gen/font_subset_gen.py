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

def extract_subset(text: str) -> str:
    subset = re.findall(r'[\u0000-\uFFFF]', text)
    return "".join(sorted(set(subset)))


def main():
    parser = argparse.ArgumentParser(description="Extract subset.")
    parser.add_argument("-s", "--source", required=True, action="append", help="Input file path (UTF-8)")
    parser.add_argument("-d", "--dest", required=True, help="Output file path")
    args = parser.parse_args()

    text = BASE_STRING
    for s in args.source:
        with open(s, "r", encoding="utf-8") as f:
            text += f.read()

    subset = extract_subset(text)

    output = subset + "\n"

    with open(args.dest, "w", encoding="utf-8") as f:
        f.write(output)

    print(f"Saved to {args.dest}")

if __name__ == "__main__":
    main()

