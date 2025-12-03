# 03_ServoMotor STM32 Project (Nucleo-F303RE)

This project demonstrates how to control a servo motor using PWM on STM32 (Bluepill F103C8T6) with STM32CubeIDE and STM32CubeProgrammer.

## Setup Instructions

### STM32CubeIDE Workspace

- Open STM32CubeIDE and select your workspace.
- Create a new STM32 project named **03_ServoMotor**.
- Select the **F303RE** microcontroller from the board list.

### CubeMX Configuration

- Upon project creation, the IOC configuration window will open automatically.
- Navigate to **System Core > SYS > Debug** and select **Serial Wire**.
- In the left panel, expand **Timers** and configure:
  - **TIM2 → Clock Source:** Internal Clock
  - **TIM2 Channel 2:** PWM Generation CH2
- In TIM2 Configuration:
  - **Prescaler:** 71
  - **Counter Period:** 9999
- Assign the TIM2_CH2 output pin as per your wiring diagram.
- Save the IOC file. When prompted to generate code, select **Yes**.

### Main Code Addition

- At the `main.c`, add below the user codes:

```c
/* USER CODE BEGIN PV */
uint16_t pulse = 500;
/* USER CODE END PV */
```
```c
/* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
/* USER CODE END 2 */
```
- In the `while` loop under **User Code Begin 3**, insert the code below:

```c
// Sweep from 0° to 180°
for(pulse=500; pulse<=2500; pulse+=5)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
    HAL_Delay(15);
}
HAL_Delay(1000);

// Sweep from 180° to 0°
for(pulse=2500; pulse>=500; pulse-=5)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
    HAL_Delay(15);
}
HAL_Delay(5000);
```

- Save and build the project.

### Flashing the Firmware

- RUN the Program

### Program Verification

- The servo motor should slowly sweep from one end to the other, pause, then sweep back, repeating the cycle automatically.

### Wiring Diagram

- Refer to the servo motor wiring diagram provided in the project folder for all hardware connections.

  - **Typical Servo Motor Connections:**
    - Signal (PWM): Connect to TIM2_CH2 output pin PA0 (A0)
    - Power: 5V (check servo specs and Bluepill board limits)
    - Ground: GND (shared with Bluepill board)

## Notes

- Ensure ST-Link drivers are installed on your PC.
- Use a separate regulated 5V supply for the servo if possible, as USB power may not be sufficient.
- Check wiring according to the provided diagram to avoid damage to the servo or microcontroller.