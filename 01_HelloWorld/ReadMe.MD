# HelloWorld STM32 Project (Bluepill F103C8T6)

This project demonstrates a simple blinking LED using STM32CubeIDE and STM32CubeProgrammer on the Bluepill F103C8T6 board.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named `HelloWorld`.
- Select the **F103C8T6** microcontroller from the part list.

### CubeMX Configuration

- Upon project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the Pinout view, set **PC13** as GPIO Output.
- Save the IOC file; when prompted to generate code, select **Yes**.

### Main Code Addition

- Open `main.c` and locate the `while` loop under **UserCode3**.
- Insert the following code inside the `while` loop:

```c
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
HAL_Delay(1000);
```

- Save and build the project.

### Flashing the Firmware

- Open **STM32CubeProgrammer**.
- Set **Configuration**:
  - Mode: ST-LINK
  - Port: SWD
  - Shared: Enabled
  - Debug in Low Power Mode: **Ticked**
  - Other settings: Leave as default.
- Select the **ELF** file located in your project's `Core` folder.
- Connect your Bluepill (F103C8T6) to the PC via **ST-Link v2**.
- Click **Download** to flash the program.
- After the upload, click **Disconnect and Reset** on CubeProgrammer.

### Program Verification

- PC13 LED on Bluepill should blink with a 1s ON and 1s OFF rate, verifying successful upload.

***

## Notes

- Ensure ST-Link drivers are installed on your PC.
- If you encounter issues, check the ST-Link connection and CubeIDE settings.

***

This file is intended for getting started with STM32 development on the Bluepill board and for generating a simple blinking LED program.