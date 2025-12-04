# 07_UltraSonicSensor – HCSR04 Distance Measurement (Nucleo‑F303RE)

This project measures distance using an HC‑SR04 ultrasonic sensor with an STM32 Nucleo‑F303RE board and STM32CubeIDE.

---

## Project Info

- **Project Name:** `07_UltraSonicSensor`  
- **Description:** Finding distance using HC‑SR04 ultrasonic sensor  
- **Board Used:** Nucleo‑F303RE  

---

## 1. STM32CubeIDE / CubeMX Configuration

### 1.1 System Core

1. Open the `.ioc` configuration.
2. Go to: `System Core → SYS → Debug`
   - Set **Debug** to: `Serial Wire`
3. Go to: `System Core → RCC`
   - Set **High Speed Clock (HSE)** to: `Crystal/Ceramic Resonator`

### 1.2 Clock Configuration

1. Open the **Clock Configuration** tab.
2. Select **HSE** as the clock source.
3. Configure PLL so that the system clock is **72 MHz** (HSE → PLL → 72 MHz).
4. Press **Enter** or apply so CubeMX updates all derived clocks.

### 1.3 Timer Configuration (TIM1)

1. Go to: `Timers → TIM1`
   - **Clock Source:** `Internal Clock`
2. In `TIM1 → Parameter Settings`:
   - Set **Prescaler** to `71`

With APB2 / TIM1 clock at 72 MHz:
- Timer tick time = 72 MHz / (Prescaler + 1) = 72 MHz / 72 = 1 MHz → **1 µs per tick**  
  This makes time‑to‑distance calculations easy.

### 1.4 Pin Configuration

In the **Pinout & Configuration** view:

- **PC0**:
  - Mode: `GPIO_Input`
  - Used as **Echo** pin from HC‑SR04
- **PC1**:
  - Mode: `GPIO_Output`
  - Used as **Trigger** pin to HC‑SR04

After configuring all, click **Save** to regenerate the `main.c` and HAL init code.

---

## 2. Code Changes in `main.c`

After code generation, open `Core/Src/main.c` and add the following.

### 2.1 Global Variables and Pin Definitions

Place this inside the `/* USER CODE BEGIN PV */` section (private variables):

/* USER CODE BEGIN PV */

// Define TRIG and ECHO pins and ports for HC-SR04
#define TRIG_PIN GPIO_PIN_1 // PC1 used as Trigger output
#define TRIG_PORT GPIOC
#define ECHO_PIN GPIO_PIN_0 // PC0 used as Echo input
#define ECHO_PORT GPIOC

// Variables for timing and distance calculation
uint32_t pMillis; // For timeout using HAL_GetTick()
uint32_t Value1 = 0; // Timer value when Echo signal goes HIGH
uint32_t Value2 = 0; // Timer value when Echo signal goes LOW
uint16_t Distance = 0; // Measured distance in centimeters

/* USER CODE END PV */


### 2.2 Timer Start and Trigger Pin Initialization

Inside `main()`, after `HAL_Init();`, `SystemClock_Config();`, `MX_GPIO_Init();` and `MX_TIM1_Init();`, add this in the `/* USER CODE BEGIN 2 */` section:

/* USER CODE BEGIN 2 */

// Start TIM1 base timer so it starts counting
HAL_TIM_Base_Start(&htim1);

// Ensure TRIG pin is initially LOW (sensor idle)
HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

/* USER CODE END 2 */

### 2.3 Main Loop – Distance Measurement

Replace or extend the `while (1)` loop content with the code below, inside `/* USER CODE BEGIN 3 */`:

while (1)
{
// 1) Send 10 µs pulse to TRIG pin to start measurement
HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET); // Set TRIG high
__HAL_TIM_SET_COUNTER(&htim1, 0); // Reset timer counter to 0
while (__HAL_TIM_GET_COUNTER(&htim1) < 10); // Wait 10 µs (10 ticks)
HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET); // Set TRIG low

// 2) Wait for ECHO to go HIGH (rising edge)
pMillis = HAL_GetTick(); // Save current tick count for timeout
while (!(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && (HAL_GetTick() < pMillis + 10))
{
// Wait up to 10 ms for echo to rise, prevents infinite loop if sensor fails
}
Value1 = __HAL_TIM_GET_COUNTER(&htim1); // Timer value at rising edge (start time)

// 3) Wait for ECHO to go LOW (falling edge)
pMillis = HAL_GetTick(); // Refresh timeout reference
while ((HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && (HAL_GetTick() < pMillis + 50))
{
// Wait up to 50 ms for echo to fall, prevents infinite loop for long distances
}
Value2 = __HAL_TIM_GET_COUNTER(&htim1); // Timer value at falling edge (end time)

// 4) Compute distance
// Time difference in µs = Value2 - Value1 (since TIM1 tick = 1 µs)
// Speed of sound ≈ 0.034 cm/µs
// Distance (one way) = (Time * Speed) / 2
Distance = (Value2 - Value1) * 0.034 / 2;

// Here you can add code to use Distance (e.g., send via UART, show on OLED, etc.)
// Small delay if needed between measurements
HAL_Delay(100);
}


---

## 3. HC‑SR04 Wiring

Connect the HC‑SR04 ultrasonic sensor to the Nucleo‑F303RE as follows:

- **VCC** → `5V` (5 V pin on Nucleo; HC-SR04 requires 5 V)
- **GND** → `GND`
- **TRIG** → `PC1` (configured as GPIO Output, TRIG)
- **ECHO** → `PC0` (configured as GPIO Input, ECHO)

> **Note:**  
> HC‑SR04 Echo pin outputs 5 V. Many Nucleo pins are 5 V tolerant, but always check the board’s datasheet or use a simple level shifter / voltage divider for safety if needed.

---

## 4. What the Code Does

1. **Timer Setup (TIM1):**
   - Runs at 1 MHz (1 µs resolution) using prescaler 71 at 72 MHz clock.
2. **Trigger Pulse:**
   - MCU sends a 10 µs HIGH pulse on the TRIG pin (PC1) to start a measurement.
3. **Echo Timing:**
   - Timer is read when ECHO goes HIGH (sound pulse left the sensor).
   - Timer is read again when ECHO goes LOW (echo received).
4. **Distance Calculation:**
   - Time difference (Value2 - Value1) is in microseconds.
   - Distance is computed using:
     - `Distance = (Time_us * 0.034) / 2;` → result in centimeters.
5. **Loop:**
   - The process repeats continuously in the `while (1)` loop with a small delay.

This gives you a continuous distance measurement from the HC‑SR04 sensor using the Nucleo‑F303RE.
