# 07_ButtonServo STM32 Project (Bluepill F103C8T6)

This project demonstrates how to rotate a servo motor to a particular position when a button is pressed, using STM32CubeIDE and STM32CubeProgrammer on the Bluepill F103C8T6 board.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named **07_ButtonServo**.
- Select the **F103C8T6** microcontroller from the part list.

### CubeMX Configuration

- Upon project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the left panel, expand **Timers** and configure:
  - **TIM2 → Clock Source:** Internal Clock
  - **TIM2 Channel 2:** PWM Generation CH2
  - **Prescaler:** 15
  - **Counter Period:** 9999
- Assign TIM2_CH2 output pin as per your wiring diagram.
- In the Pinout view, set:
  - **GPIO PB3** as GPIO Input (for button).
- Save the IOC file. When prompted to generate code, select **Yes**.

### Main Code Addition

- At the `main.c` file, add user codes mentioned below:

```c
/* USER CODE BEGIN PV */
uint16_t pulse = 250;
/* USER CODE END PV */
```
```c
/* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse); // set initial 0 deg position
/* USER CODE END 2 */
```
- In the `while` loop under **User Code Begin 3**, insert the code below:

```c
/* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */
  while (1)
	  {
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET) // Active-low: pressed
  {
      // Move servo to 90 deg (1.25 ms pulse = 625 @ 2us ticks)
      pulse = 625;
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);

      HAL_Delay(5000); // Hold for 5 seconds

      // Move back to 0 deg (0.5 ms = 250)
      pulse = 250;
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);

      // Wait for button release (debounce)
      while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
      {
          HAL_Delay(10);
      }
  }

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
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

- When the push button connected to PB3 is pressed, the servo motor rotates to 90°, pauses, then returns to 0°.  
- The servo only reacts again when the button is released and pressed once more.

### Wiring Diagram

- Refer to the wiring diagram provided in the project folder for all hardware connections.

  - **Typical Servo Motor Connections:**
    - Signal (PWM): Connect to TIM2_CH2 output pin (as per IOC mapping)
    - Power: 5V (check servo specs and Bluepill board limits)
    - Ground: GND (shared with Bluepill board)
  - **Push button connection:**
    - One terminal to PB3
    - Other terminal to GND (use pull-up resistor if required, or configure internal pull-up in CubeMX)

## Notes

- Ensure ST-Link drivers are installed on your PC.
- Provide adequate power for the servo motor (external 5V supply recommended).
- Check all wiring according to the provided diagram to avoid hardware issues.