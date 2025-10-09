# 06_ServoMotor STM32 Project (Bluepill F103C8T6)

This project demonstrates how to control a servo motor using PWM on STM32 (Bluepill F103C8T6) with STM32CubeIDE and STM32CubeProgrammer.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named **06_ServoMotor**.
- Select the **F103C8T6** microcontroller from the part list.

### CubeMX Configuration

- Upon project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the left panel, expand **Timers** and configure:
  - **TIM2 → Clock Source:** Internal Clock
  - **TIM2 Channel 2:** PWM Generation CH2
- In TIM2 Configuration:
  - **Prescaler:** 15
  - **Counter Period:** 9999
- Assign the TIM2_CH2 output pin as per your wiring diagram.
- Save the IOC file. When prompted to generate code, select **Yes**.

### Main Code Addition

- At the top of `main.c`, add below the user PV comment section:

```c
/* USER CODE BEGIN PV */
uint16_t pulse = 250;
/* USER CODE END PV */
```

 /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE END 2 */

- In the `while` loop under **User Code Begin 3**, insert the code below:

```c
// Sweep from 0° to 180°
for(pulse=250; pulse<=1250; pulse+=5)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
    HAL_Delay(15);
}
HAL_Delay(1000);

// Sweep from 180° to 0°
for(pulse=1250; pulse>=250; pulse-=5)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
    HAL_Delay(15);
}
HAL_Delay(5000);
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

- The servo motor should slowly sweep from one end to the other, pause, then sweep back, repeating the cycle automatically.

### Wiring Diagram

- Refer to the servo motor wiring diagram provided in the project folder for all hardware connections.

  - **Typical Servo Motor Connections:**
    - Signal (PWM): Connect to TIM2_CH2 output pin (as per IOC mapping)
    - Power: 5V (check servo specs and Bluepill board limits)
    - Ground: GND (shared with Bluepill board)

## Notes

- Ensure ST-Link drivers are installed on your PC.
- Use a separate regulated 5V supply for the servo if possible, as USB power may not be sufficient.
- Check wiring according to the provided diagram to avoid damage to the servo or microcontroller.