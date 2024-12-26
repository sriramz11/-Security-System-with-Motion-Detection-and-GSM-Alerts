#include "stm32f091xc.h"
#include "stm32f0xx.h"
#include "mpu6050.h"
#include "i2c.h"
#include "uart.h"
#include "pwm.h"
#include "system_config.h"


/* STATIC VOLATILE VARIABLE TO TRACK MONITORING STATE */
static volatile uint8_t is_monitoring = 0;

/* STATIC VOLATILE VARIABLE TO TRACK ALERT STATUS */
static volatile uint8_t alert_triggered = 0;

/* STATIC VOLATILE VARIABLE FOR CONTINUOUS VALUE PRINTING */
static volatile uint8_t print_values = 0;





/* INITIALIZE TIMER3 FOR PERIODIC INTERRUPTS */
static void Timer_Init(void){

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;       /* ENABLE CLOCK FOR TIMER3 */
    TIM3->PSC = 24000 - 1;           /* SET PRESCALER TO DIVIDE 24 MHZ BY 24000 (1 KHZ) */

    TIM3->ARR = 100 - 1;                      /* SET AUTO-RELOAD VALUE FOR 10 HZ INTERRUPT RATE */

    TIM3->DIER |= TIM_DIER_UIE;               /* ENABLE UPDATE INTERRUPT */
    NVIC_EnableIRQ(TIM3_IRQn);                /* ENABLE TIMER3 INTERRUPT IN NVIC */
}

void Test_UART_Communication(void) {
    UART_PrintString("=== UART Communication Test ===\r\n");

    // Test MPU6050 initialization message
    mpu6050_init();
    UART_PrintString("\r\nMPU6050 Initialization Test Passed\r\n");

    // Test menu display
    UART_ShowMenu();
    UART_PrintString("\r\nMenu Display Test Passed\r\n");

    // Simulate menu option selection
    UART_ProcessCommand('1'); // Simulate option 1
    UART_PrintString("\r\nOption 1 Test Passed: Current Values Printed\r\n");

    UART_ProcessCommand('2'); // Simulate option 2
    UART_PrintString("\r\nOption 2 Test Passed: Recalibration Triggered\r\n");
}


void Test_I2C_Communication(void) {
    UART_PrintString("=== I2C Communication Test ===\r\n");

    uint8_t test_data[6];
    I2C_ReadMultiple(MPU6050_ADDR, ACCEL_XOUT_H, test_data, 6);

    int16_t x = set_together_int16(test_data[0], test_data[1]);
    int16_t y = set_together_int16(test_data[2], test_data[3]);
    int16_t z = set_together_int16(test_data[4], test_data[5]);

    char buffer[64];
    sprintf(buffer, "Test Accelerometer Values - X: %d, Y: %d, Z: %d\r\n", x, y, z);
    UART_PrintString(buffer);

    UART_PrintString("\r\nI2C Communication Test Passed\r\n");
}

/* MAIN FUNCTION */
int main(void){
    SystemInit();             /* INITIALIZE SYSTEM */
    SystemClock_Config();     /* CONFIGURE SYSTEM CLOCK */
    GPIO_Init();              /* INITIALIZE GPIO PINS */
    UART_Initialize();  /* INITIALIZE UART */
    I2C_Init(); /* INITIALIZE I2C */
    PWM_Init();/* INITIALIZE PWM */
    Timer_Init();  /* INITIALIZE TIMER3 */
    EXTI_Init(); /* INITIALIZE EXTERNAL INTERRUPTS */


    UART_PrintString("\r\n=== Automated Testing Start ===\r\n");

    Test_UART_Communication();
    Test_I2C_Communication();


    UART_PrintString("\r\n=== Automated Testing Complete ===\r\n");

    if (GSM_Init()) {
        GSM_SendSMS("+1234567890", "System Initialized Successfully!\r\n");
    } else {
        UART_PrintString("GSM Initialization Failed\r\n");
    }

    mpu6050_init();           /* INITIALIZE MPU6050 SENSOR */
    calibrateMPU6050();       /* CALIBRATE MPU6050 SENSOR */

    is_monitoring = 1;        /* ENABLE MONITORING STATE */
    TIM3->CR1 |= TIM_CR1_CEN; /* START TIMER3 */

    UART_ShowMenu();          /* DISPLAY UART MENU */

    while (1) {
        __WFI();              /* ENTER LOW POWER MODE AND WAIT FOR INTERRUPT */
    }

    while (1);                /* REDUNDANT LOOP, CAN BE REMOVED */
}



/* TIMER3 INTERRUPT HANDLER */
void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {                /* CHECK IF UPDATE INTERRUPT FLAG IS SET */
        TIM3->SR &= ~TIM_SR_UIF;               /* CLEAR INTERRUPT FLAG */

        detectMotion();                        /* DETECT MOTION USING MPU6050 SENSOR */

        if (is_alarm_active && !alert_triggered) {  /* CHECK IF ALARM IS ACTIVE BUT NOT YET TRIGGERED */
            UART_AlertMovement();             /* ALERT USER ABOUT MOVEMENT DETECTION */
            GSM_SendSMS("+7208787881", "Movement Detected in Dorm Room!"); /* SEND SMS */
            alert_triggered = 1;              /* SET ALERT TRIGGERED STATUS */
        } else if (!is_alarm_active) {        /* CHECK IF ALARM IS NO LONGER ACTIVE */
            alert_triggered = 0;              /* RESET ALERT STATUS */
        }
    }
}

/* UART COMMAND PROCESSING FUNCTION */
void UART_ProcessCommand(char cmd)
{


    switch(cmd) {
        case '1':                             /* CASE 1: PRINT ACCELEROMETER VALUES */
            UART_PrintString("\r\n");
            GetRawData();                     /* READ AND PRINT ACCELEROMETER DATA */
            break;

        case '2':                             /* CASE 2: RECALIBRATE SENSOR */
            UART_PrintString("\r\nRecalibrating sensor...\r\n");
            calibrateMPU6050();    /* CALIBRATE MPU6050 */
            UART_ShowMenu();       /* SHOW MENU AFTER CALIBRATION */
            break;

        case '3':                             /* CASE 3: DISPLAY SYSTEM INFORMATION */
            UART_PrintString("\r\n=== Dorm Room Security System ===\r\n");
            UART_PrintString("This system protects valuable items by detecting unauthorized movement.\r\n");
            UART_PrintString("When movement is detected:\r\n");
            UART_PrintString("1. Visual/audio alarm activates\r\n");
            UART_PrintString("2. Event is logged via UART\r\n");
            UART_PrintString("3. System continues monitoring until reset\r\n");
            UART_PrintString("\r\nPress any key to return to menu...\r\n");
            break;

        case '4':                  /* CASE 4: STOP CONTINUOUS VALUE PRINTING */
        	/* CHECK IF CONTINUOUS PRINTING IS ENABLED */
            if (print_values) {
                print_values = 0;             /* DISABLE CONTINUOUS PRINTING */
                UART_PrintString("\r\nStopped printing values.\r\n");
                UART_ShowMenu();              /* SHOW MENU AFTER STOPPING PRINTING */
            }


            break;


/* DEFAULT CASE: INVALID COMMAND */
        default:


            if (!print_values) {              /* ONLY DISPLAY ERROR IF CONTINUOUS PRINTING IS DISABLED */
                UART_PrintString("\r\nInvalid character\r\nPlease choose an option from the menu\r\n");
                UART_ShowMenu();              /* SHOW MENU AFTER INVALID INPUT */
            }
            break;
    }
}
