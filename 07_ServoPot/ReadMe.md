# 08_ServoPot STM32 Project (Bluepill F103C8T6)

This project demonstrates how to control a servo motor’s position with a potentiometer (analog input), using STM32CubeIDE and STM32CubeProgrammer on the Bluepill F103C8T6 board.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named **08_ServoPot**.
- Select the **F103C8T6** microcontroller from the part list.

### CubeMX Configuration

- On project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the left panel, expand **Timers** and configure:
  - **TIM2 → Clock Source:** Internal Clock
  - **TIM2 Channel 2:** PWM Generation CH2
  - **Prescaler:** 15
  - **Counter Period:** 9999
- Assign TIM2_CH2 output pin as per your wiring diagram.
- Enable analog input:
  - **Analog > ADC1**: Tick **IN9** (connect potentiometer wiper here)
  - Under ADC Configuration: Enable **Continuous Conversion Mode**
- Save the IOC file. When prompted to generate code, select **Yes**.

### Main Code Addition

- At the top of `main.c`, add below the user PV section:

```c
/* USER CODE BEGIN PV */
uint16_t adcValue = 0;
uint16_t pulse = 250;  // initial pulse value
/* USER CODE END PV */
```

- In **User Code Begin 2** (inside main before the loop), start the PWM:

```c
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE END 2 */
```

- In the `while` loop under **User Code Begin 3**, insert the code below:

```c
/* USER CODE BEGIN 3 */
	  HAL_ADC_Start(&hadc1);
	      if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	      {
	        adcValue = HAL_ADC_GetValue(&hadc1);
	      }
	      HAL_ADC_Stop(&hadc1);

	      pulse = 250 + ((uint32_t)adcValue * 1000) / 4095;  // Map ADC to pulse (250-1250)
	      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);

	      HAL_Delay(20);
  }
  /* USER CODE END 3 */

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

- Rotating the potentiometer will smoothly adjust the servo position in real time.

### Wiring Diagram

- Refer to the diagram provided in the project folder for hardware connections.

  - **Typical Servo Motor Connections:**
    - Signal (PWM): Connect to TIM2_CH2 output pin
    - Power: 5V (according to servo specs and Bluepill limits)
    - GND: Shared with Bluepill board
  - **Potentiometer Connection:**
    - One end → 3.3V
    - Other end → GND
    - Wiper (middle terminal) → ADC IN9 pin

## Notes

- Use a proper regulated supply for servo motor power, if necessary.
- Ensure good connection for potentiometer terminals and ADC pin.
- Adjust mapping parameters for different servo or ADC ranges as needed.