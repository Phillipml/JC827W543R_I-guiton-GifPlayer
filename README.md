cat << 'EOF' > README.md

# JC827W543R_I - GIF Player using SPIFFS

[LinkAliExpress](https://pt.aliexpress.com/item/1005006729377800.html?spm=a2g0o.order_list.order_list_main.27.6ec5caa4tGAXDR&gatewayAdapt=glo2bra)

This is a personal project to demonstrate how to display animated GIFs on the JC827W543R_I 4.3" serial display using an ESP32-S3-N4R8 by Guition. It plays a `.gif` file stored in internal flash memory (SPIFFS) with full support for transparency and smooth animation.

The goal is to help other developers who want to play animated content on embedded displays without needing an SD card. This version runs entirely from SPIFFS, but SD card support is planned.

---

## 📦 Requirements

- Arduino IDE 1.8.x
- ESP32-S3 board
- JC827W543R_I display (NV3041A driver)
- Libraries:
  - [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF)
  - [Arduino_GFX](https://github.com/moononournation/Arduino_GFX)
  - SPIFFS (included with ESP32 board support)

---

## 📁 Folder Structure

\`\`\`
JC827W543R_I-guiton-GifPlayer/
├── JC827W543R_I-guiton-GifPlayer.ino # Main source code
├── device_config.h # Board/display configuration
├── data/
│ └── output.gif # GIF to be displayed (480x272)
\`\`\`

---

## ⚙️ Setup Instructions

1. Open **Arduino IDE 1.8.x**
2. Install the required libraries via Library Manager:
   - AnimatedGIF
   - Arduino_GFX
3. Install the ESP32 board package:

   - Go to **File > Preferences** and add this URL:

     \`\`\`
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     \`\`\`

   - Then go to **Tools > Board > Boards Manager**, search for **ESP32**, and install it.

4. Select the board:

   - **Tools > Board > ESP32S3 Dev Module**
   - (Make sure `Partition Scheme` allows SPIFFS)

5. Upload the sketch normally.

---

## 📂 Uploading the GIF

To upload your `output.gif` to the ESP32’s SPIFFS:

### 🛠 Install ESP32 Sketch Data Upload Tool

- Download from: https://github.com/me-no-dev/arduino-esp32fs-plugin
- Place it in:
  - Windows: `<Arduino IDE install path>/tools/ESP32FS/tool/`
  - Linux/macOS: `~/Arduino/tools/ESP32FS/tool/`

After restarting Arduino IDE, go to:

\`\`\`
Tools > ESP32 Sketch Data Upload
\`\`\`

This will upload the contents of the `data/` folder to SPIFFS.

---

## ▶️ Running the Player

After uploading the sketch and the `output.gif`:

- The screen will clear and automatically start playing the animation.
- If the file is missing or corrupted, an error will appear on the display.

---

## 🖼 Creating Compatible GIFs

Make sure your GIF matches the display resolution: **480x272 pixels**.

### ✅ Recommended settings:

- Max resolution: 480x272
- Color format: 256-color (GIF standard)
- Transparency: Supported
- Frame rate: up to ~15 fps

### 🔧 Using FFmpeg to convert video to GIF:

\`\`\`bash
ffmpeg -i input.mp4 -vf "scale=480:272,fps=15" -loop 0 output.gif
\`\`\`

Then place the `output.gif` into the `data/` folder and re-upload with the data upload tool.

---

## 💾 SD Card Support (To Be Implemented)

The `device_config.h` file already defines pin mappings for SD card usage:

\`\`\`
SD_CS -> IO10
SD_MOSI -> IO11
SD_MISO -> IO13
SD_SCK -> IO12
\`\`\`

SD functionality is not yet active in this version. Planned for a future release:

- Load GIFs from SD card
- Auto-detect between SPIFFS and SD

---

## 🔍 Troubleshooting

- Make sure the display backlight pin `GFX_BL` is set to HIGH
- GIF file must be exactly named `/output.gif`
- Check SPIFFS upload success via Serial Monitor
- Ensure `Partition Scheme` allows SPIFFS (e.g. "Default with 1.5MB app, 1.5MB SPIFFS")

---

## 🤝 Contributing

Feel free to fork or open issues if you’d like to expand the project, e.g., SD card support or audio integration (I2S is partially configured in `device_config.h`).

---

## 🙋 About

This is a **personal project** made to help anyone who wants to use GIF animations on ESP32-based embedded displays, especially those using the NV3041A driver.

Pull requests and suggestions are welcome!

---

**Author:** [Phillip Menezes]  
**License:** MIT
EOF
