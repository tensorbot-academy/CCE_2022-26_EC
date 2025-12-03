# 04_OLED STM32 Project (Nucleo‑F303RE, 0.96" I2C OLED 128x64)

This project shows how to drive a 0.96" SSD1306 OLED display (128x64, I2C) using an STM32 Nucleo‑F303RE board and STM32CubeIDE. The OLED is connected via I2C on pins PB8 (SCL) and PB9 (SDA).

---

## 1. STM32CubeIDE Project Setup

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named `04_OLED`.
- Select the `NUCLEO‑F303RE` board (or MCU `STM32F303RE`) from the board list.
- Finish the wizard so the `.ioc` configuration window opens.

---

## 2. CubeMX Configuration (.ioc)

### 2.1 System

- Go to: `System Core → SYS → Debug`
- Set `Debug` to: `Serial Wire`

### 2.2 I2C Configuration

- Go to: `Connectivity → I2C1`
  - Mode: `I2C`
  - I2C Speed Mode: `Fast Mode (400 kHz)`
- In the Pinout view:
  - `PB8` → `I2C1_SCL`
  - `PB9` → `I2C1_SDA`
- In `I2C1 → Parameter Settings`:
  - Addressing Mode: `7‑bit`
  - No Stretch Mode: `Disabled`

### 2.3 Clocks

- Use the default clock configuration, or configure HSI + PLL for a 72 MHz system clock (typical for Nucleo‑F303RE).

### 2.4 Code Generation

- Save the `.ioc` file.
- When prompted, click **Yes** to generate initialization code.

---

## 3. Add the SSD1306 Library

### 3.1 Copy Library Files

In your project directory, create or use this folder:

- `Drivers/SSD1306/`

Copy the SSD1306 library files into that folder (from the library you already have):

- `ssd1306.c`
- `ssd1306.h`
- `ssd1306_fonts.c`
- `ssd1306_fonts.h`
- `ssd1306_conf.h` (or create it from the provided `ssd1306_conf_template.h`)

### 3.2 Configure Include Path

- Right‑click the project → `Properties`
- Go to: `C/C++ Build → Settings`
- Under: `MCU GCC Compiler → Include paths`
  - Add: `Drivers/SSD1306`
- Click **Apply and Close**.

### 3.3 Configure `ssd1306_conf.h`

Open `ssd1306_conf.h` and configure it for STM32F3, I2C, and a 128x64 OLED.

Example (adapt macro names to match your library):

#ifndef SSD1306_CONF_H
#define SSD1306_CONF_H

#define STM32F3
#define SSD1306_USE_I2C

#define SSD1306_I2C_PORT hi2c1
#define SSD1306_I2C_ADDR (0x3C << 1) // Typical I2C address for 0.96" SSD1306

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_INCLUDE_FONT_11x18

#endif /* SSD1306_CONF_H */


---

## 4. Main Code Changes

Open `Core/Src/main.c`.

### 4.1 Includes

Add the SSD1306 includes in the user include section:

/* USER CODE BEGIN Includes /
#include "ssd1306.h"
#include "ssd1306_fonts.h"
/ USER CODE END Includes */

text

### 4.2 OLED Initialization and Text

Inside `main()`, after the generated initialization calls:

insert this block inside `/* USER CODE BEGIN 2 */`:

/* USER CODE BEGIN 2 */
ssd1306_Init();

ssd1306_Fill(Black);
ssd1306_UpdateScreen();

ssd1306_SetCursor(15, 2);
ssd1306_WriteString("TENSORBOT", Font_11x18, White);
ssd1306_SetCursor(2, 24);
ssd1306_WriteString("INNOVATIONS", Font_11x18, White);

ssd1306_UpdateScreen();
/* USER CODE END 2 */


The `while (1)` loop can remain as generated for a static display:

while (1)
{
/* USER CODE BEGIN WHILE */

/* USER CODE END WHILE */

/* USER CODE BEGIN 3 /
// Add dynamic updates here if needed
/ USER CODE END 3 */
}



## 5. Wiring the 0.96" I2C OLED

Connect the OLED module to the Nucleo‑F303RE:

- OLED `VCC` → 3.3 V (or 5 V if your module supports it; check its datasheet)
- OLED `GND` → GND
- OLED `SDA` → PB9 (`I2C1_SDA`)
- OLED `SCL` → PB8 (`I2C1_SCL`)

If your OLED board does not include pull‑up resistors on SDA and SCL, add 4.7 kΩ resistors from SDA to VCC and from SCL to VCC.

---

## 6. Build and Flash

- Click the **Build** (hammer) icon to compile the project.
- Connect the Nucleo‑F303RE to your PC via USB (on‑board ST‑LINK).
- Click **Run** (green play button) to flash the firmware.

---

## 7. Expected Behavior

After flashing and reset, the OLED should:

- Clear the screen (black background).
- Display:
  - `TENSORBOT` on the first line (top, large font).
  - `INNOVATIONS` on the second line below.

The text will remain static until you change the program.

If the display is blank:

- Check the wiring and power to the OLED.
- Make sure I2C1 is configured on PB8/PB9 in Fast Mode (400 kHz).
- Confirm `SSD1306_I2C_ADDR` in `ssd1306_conf.h` matches your OLED module (commonly `0x3C << 1`).
- Optionally add an I2C scanner to verify the OLED appears on the bus.