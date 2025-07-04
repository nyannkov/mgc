# mgc

**mgc** is a lightweight, embedded-friendly library for building small games and gadgets.

Rather than being a full-fledged game engine, **mgc** offers a set of lightweight and modular tools  
designed to simplify graphics, input, and asset handling in embedded environments.

It is designed for resource-constrained systems, with a focus on **deterministic** behavior and **zero heap allocation**.

While the core avoids dynamic memory allocation to remain lightweight and predictable,  
this is intended to support user-side control over memory usage and efficient resource reuse,  
e.g., via `std::vector` or `std::unique_ptr`.


> ⚠️ **Warning:** mgc is currently in active development.  
> Some interfaces and features may change without backward compatibility.  
> We recommend using the latest version and following the repository for updates.

---

## ✨ Key Features

- ⚙️ **Heapless design** – No dynamic memory allocation in core modules
- 💡 **Dual interface** – Provides both **C** and **C++** APIs
- 🧱 **Modular architecture** – Display, input, sound, and other modules are platform-selectable
- 🖼️ **Supports 16x16 pixel unit drawing (cell_draw in mgc_cpp)**
- 🎨 **Indexed color format** using RGB565
- 🔤 **BDF font support** (up to 16px height)
- 🕹️ Designed for **RP2040** and adaptable to other embedded targets
- 📦 **CMake-based build system**

---

## 📁 Project Structure

```
root/
├── src/        # Core library source files
│   ├── mgc/        # C interface (platform-agnostic)
│   └── mgc_cpp/    # C++ wrapper (type-safe, heapless)
├── devtest/    # Experimental code and prototypes for internal development
├── drivers/    # Platform-specific drivers (display, input, sound, etc.)
├── external/   # External libraries and git submodules
├── scripts/    # Initialization and utility scripts, including setup and small tools
├── test/       # Test code
├── tools/      # Asset generator tools such as font and tileset converters
├── CMakeLists.txt
├── LICENSE
└── README.md
```

> When using `mgc_cpp`, you must also include `mgc/`, which contains the core functionality.

### 🧪 `devtest/` – Developer Playground

The `devtest/` directory contains experimental code, temporary tests, and prototypes used during development.  
Unlike `test/`, which is for stable unit tests, this directory is intended for internal debugging and validation  
of features that are still in progress or under consideration.

Contents in this directory may break frequently and are not part of the public API.

---

## 🖼️ Graphics

- **Map tile size**: 16x16 pixels
- **Color format**: RGB565
- **Color mode**: Indexed color

---

## 🔤 Font Support

- **Font format**: BDF (Bitmap Distribution Format)
- **Maximum font height**: 16 pixels

---

## 🚀 Setup

After cloning the repository, change into the project directory and run the setup script to initialize external dependencies:

```bash
cd mgc
git submodule update --init --recursive
python3 scripts/setup_assets.py
```
This script downloads and prepares external assets such as fonts and other resources required for building the project.

### Using Raspberry Pi Pico
If you plan to build for the Raspberry Pi Pico platform, you need to set up the [Pico SDK](https://github.com/raspberrypi/pico-sdk) beforehand.
Please follow the official Pico SDK setup instructions to configure your development environment.

---

## 🧰 Intended Use

- **mgc is not a game engine** — it is a **toolkit** providing reusable components like:
  - Drawable interfaces
  - Framebuffer abstraction
  - Font rendering
  - Simple input models
  - Platform-specific device drivers

- You are encouraged to build your own game architecture by composing these building blocks.

---

## ⚖️ License

mgc is released under the **MIT License**.  
See the [LICENSE](./LICENSE) file for details.

> Auto-generated files such as fonts, maps, and tilesets should comply with the license of their original data sources.

> Some drivers under the `drivers/` directory make use of third-party libraries located in the `external/` directory.  
> These external modules are included as Git submodules and are maintained under their own respective licenses  
> (e.g., MIT, BSD, or GPL). Please refer to each submodule's documentation and license file for more information.