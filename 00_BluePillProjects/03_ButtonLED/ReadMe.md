# ButtonLED STM32 Project (Bluepill F103C8T6)

This project demonstrates how to control an LED using a push button with STM32CubeIDE and STM32CubeProgrammer on the Bluepill F103C8T6 board.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named **ButtonLED**.
- Select the **F103C8T6** microcontroller from the part list.

### CubeMX Configuration

- Upon project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the Pinout view, set:
  - **GPIO Pin A1** as GPIO Output (for LED).
  - **GPIO Pin B1** as GPIO Input (for push button).
- Save the IOC file. When prompted to generate code, select **Yes**.

### Main Code Addition

- Open `main.c` and locate the while loop under **User Code Begin 3**.
- Insert the following code inside the while loop:

```c
if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
}
else
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
}
```

- Save and build the project.

### Flashing the Firmware

- Open STM32CubeProgrammer.
- Set Configuration:
  - Mode: **ST-LINK**
  - Port: **SWD**
  - Shared: **Enabled**
  - Debug in Low Power Mode: **Ticked**
  - Other settings: Leave as default.
- Select the ELF file located in your project's **Core** folder.
- Connect your Bluepill (F103C8T6) to the PC via **ST-Link v2**.
- Click **Download** to flash the program.
- After the upload, click **Disconnect and Reset** on CubeProgrammer.

### Program Verification

- When the push button connected to PB1 is pressed, the LED on PA1 will turn OFF.
- When the push button is not pressed, the LED on PA1 will turn ON.

### Wiring Diagram

- **ST-Link v2 to Bluepill connections:**
  - SWDIO → SWDIO (PA13)
  - SWCLK → SWCLK (PA14)
  - GND → GND
  - 3.3V → 3.3V

- **External LED connection:**
  - Connect the LED anode (+) to GPIO A1 via a current-limiting resistor (220Ω–1kΩ).
  - Connect the LED cathode (–) to GND.

- **Push button connection:**
  - Connect one terminal of the push button to GPIO B1.
  - Connect the other terminal to GND (and use a pull-up resistor if required, or configure internal pull-up in CubeMX).

## Notes

- Ensure ST-Link drivers are installed on your PC.
- If you encounter issues, verify ST-Link connection, CubeIDE IOC settings, and wiring for the LED and button.

***

This file is intended for getting started with STM32 development on the Bluepill board, controlling an LED via a push button input and programming via ST-Link v2.