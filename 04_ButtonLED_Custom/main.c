while (1)
{
    static uint32_t buttonPressStart = 0;
    static uint8_t buttonWasPressed = 0;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
    {
        if (!buttonWasPressed)
        {
            buttonPressStart = HAL_GetTick();  // Record time when button is first pressed
            buttonWasPressed = 1;
        }
        else
        {
            // Check if button has been held for 5 seconds
            if ((HAL_GetTick() - buttonPressStart) >= 5000)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  // Turn on LED
                HAL_Delay(3000);  // Keep LED on for 3 seconds
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  // Turn off LED

                // Wait until button is released to avoid repeated triggering
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);
                buttonWasPressed = 0;
            }
        }
    }
    else
    {
        buttonWasPressed = 0;  // Reset if button is not pressed
    }
}
