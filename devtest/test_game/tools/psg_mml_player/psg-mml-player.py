import argparse
import subprocess

def load_mml(path: str) -> str:
    mml_parts = []

    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n")

            if "//" in line:
                line = line.split("//", 1)[0]

            if line.strip():
                mml_parts.append(line)

    return "".join(mml_parts)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", required=True, help="MML file")
    args = parser.parse_args()

    mml = load_mml(args.file)

    cmd = [
        "./core/build/psg-mml-player-core",
        mml
    ]

    subprocess.run(cmd, check=True)


if __name__ == "__main__":
    main()

