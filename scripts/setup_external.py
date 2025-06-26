import zipfile
import urllib.request
import subprocess
from pathlib import Path



FONTS = {
    "k8x12": "https://littlelimit.net/arc/k8x12/k8x12_bdf_2021-05-05.zip",
    "misaki": "https://littlelimit.net/arc/misaki/misaki_bdf_2021-05-05.zip",
}

ASSETS_BASE_DIR = Path(__file__).resolve().parent.parent / "external" / "assets" / "fonts"

def download_and_extract_font(name: str, url: str):
    font_dir = ASSETS_BASE_DIR / name
    zip_path = font_dir / f"{name}.zip"

    font_dir.mkdir(parents=True, exist_ok=True)

    print(f"[{name}] Downloading from {url}...")
    urllib.request.urlretrieve(url, zip_path)
    print(f"[{name}] Download complete.")

    print(f"[{name}] Extracting to {font_dir}...")
    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        zip_ref.extractall(font_dir)
    zip_path.unlink()
    print(f"[{name}] Extraction complete.\n")

def init_submodules():
    print("Initializing git submodules...")
    subprocess.run(["git", "submodule", "init"], check=True)
    subprocess.run(["git", "submodule", "update"], check=True)
    print("Submodules initialized.\n")

def main():
    for name, url in FONTS.items():
        try:
            download_and_extract_font(name, url)
        except Exception as e:
            print(f"[{name}] Error: {e}\n")

    init_submodules()

if __name__ == "__main__":
    main()





