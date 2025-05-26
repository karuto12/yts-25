# 🚀 ESP32 SPIFFS Upload Guide

This guide walks you through the process of uploading files to an ESP32 using SPIFFS, from setup to flashing and reading those files in your Arduino code.

---

## 📦 Prerequisites

### 1. Install Python

- Download Python from: [https://python.org](https://python.org)
- During installation, **check** the box: ✅ _"Add Python to PATH"_

---

### 2. Install `esptool.py`

Open a Command Prompt and run:

```bash
pip install esptool
```

---

### 3. Download `mkspiffs` (Windows)

- Visit: [https://github.com/igrr/mkspiffs/releases](https://github.com/igrr/mkspiffs/releases)
- Download the latest Windows `.zip` file (e.g. `mkspiffs-0.2.3-windows.zip`)
- Extract it and place `mkspiffs.exe` in a convenient folder (e.g. your project directory)

---

### 4. Add `mkspiffs` to Environment Variables

To use `mkspiffs` from any terminal:

- Go to **System Properties** → **Advanced** → **Environment Variables**
- Under "System Variables", find `Path`, click **Edit**, then **Add** the folder path where `mkspiffs.exe` is located

---

### 5. Check ESP32 Partition Table

Make sure your ESP32 board uses a partition table that includes SPIFFS.

- Default SPIFFS start address is usually: `0x290000`
- You can verify this in the `partitions.csv` or sometimes `default.csv` file of your board definitions or Arduino sketch

---

## 🛠️ Create `spiffs.bin`

1. Create a folder named `data` in your project directory and place all files you want to upload inside it.
2. Run the following command in the same directory as `mkspiffs.exe`:

```bash
mkspiffs -c data -b 4096 -p 256 -s <SIZE_IN_HEX> spiffs.bin
```

### Example (for a 1MB SPIFFS partition):

```bash
mkspiffs -c data -b 4096 -p 256 -s 0x100000 spiffs.bin
```

---

## 🔁 Upload `spiffs.bin` to ESP32

Use `esptool.py` to flash the generated file:

```bash
python -m esptool --chip esp32 --port COM5 write_flash 0x290000 spiffs.bin
```

- Replace `COM5` with your actual ESP32 COM port
- Ensure `0x290000` matches your SPIFFS start address from the partition table

---

## 🧪 Arduino Code Example

Here’s how to read your uploaded files from SPIFFS:

- Replace `"/yourfile.txt"` with the actual filename in your `data/` folder


```cpp
#include "SPIFFS.h"

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open("/yourfile.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void loop() {
  // Nothing here
}
```


## ✅ All Done!

You’ve now successfully uploaded files to SPIFFS and accessed them in your code. 🎉  
For troubleshooting, make sure your partition table supports SPIFFS and you're flashing to the correct address.

---
