# **Dorm Room Security System Using MPU6050 and STM32**

## **Overview**
This project implements a **Dorm Room Security System** based on the **STM32F091RC microcontroller** and the **MPU6050 accelerometer**. The system detects unauthorized movements of valuable items and triggers a visual or audio alert through a buzzer. This project uses I2C communication, PWM for buzzer control, and UART for real-time data to the USARt terminal. The solution integrates sesnor calibration, motion detection, and interrupt-based handling.

---

## **Features**
1. **Motion Detection**:
   - Continuously monitors three-axis accelerometer data.
   - Detects movements exceeding calibrated thresholds.
   - Alerts through a buzzer when suspicious activity is detected.
2. **Sensor Calibration**:
   - Dynamically calibrates the accelerometer for accurate motion thresholds.
   - The MCU takes n samples and calculates the range of each axis
   - Adjusts sensitivity to fit specific application needs.
4. **Real-Time Feedback**:
   - Provides real-time accelerometer readings via UART option 1 in the menu.
   - Displays alerts and calibration results also.
     
5. **User Interaction**:
   - Reset button to recalibrate the system.
   - Command-based interaction through UART.
6. **Power Efficiency**:
   - Implements power-saving through interrupt-driven processing and sleep mode.

---

## **Pin Layout**
| **Peripheral** | **STM32 Pin** | **Connected Device**     | **Description**             |
|----------------|---------------|--------------------------|-----------------------------|
| I2C SCL        | PB8           | MPU6050                  | I2C Clock Line              |
| I2C SDA        | PB9           | MPU6050                  | I2C data Line               |
| Buzzer Output  | PA5           | Buzzer                   | PWM Output for Alert        |
| UART TX        | PA2           | Serial Terminal (VCP)  | Transmit Data to Terminal   |
| UART RX        | PA3           | Serial Terminal (VCP)    | Receive Data from Terminal  |
| Reset Button   | PC13          | Push Button              | Recalibration Trigger       |

---NOTE--- JUST CONNECT ST-LINK CABLE FOR USART

## **Modules**

### **1. MPU6050 Sensor**
- **File**: `mpu6050.c`, `mpu6050.h`
- **Description**:
  - Initializes and configures the MPU6050 accelerometer.
  - Reads accelerometer data via I2C.
  - Provides calibration functionality to determine motion thresholds.
  - Handles motion detection by comparing real-time data with calibrated ranges.
- **Key Functions**:
  - `mpu6050_init`: Initializes the sensor.
  - `calibrateMPU6050`: Collects calibration data and sets thresholds.
  - `detectMotion`: Compares real-time accelerometer readings with thresholds.

### **2. I2C Communication**
- **File**: `i2c.c`, `i2c.h`
- **Description**:
  - Configures and manages I2C communication with the MPU6050.
  - Supports multi-byte read and write operations.
- **Key Functions**:
  - `I2C_Init`: Initializes I2C peripheral for communication.
  - `I2C_Write`: Writes a single byte to the sensor register.
  - `I2C_ReadMultiple`: Reads multiple bytes from the sensor register.

### **3. UART Communication**
- **File**: `uart.c`, `uart.h`
- **Description**:
  - Configures UART for serial communication with a terminal.
  - Sends and receives data using interrupt-driven processing.
  - Provides command processing and menu display.
- **Key Functions**:
  - `UART_Initialize`: Initializes UART with interrupt support.
  - `UART_PrintString`: Sends a string to the terminal.
  - `UART_ShowMenu`: Displays the command menu on the terminal.

### **4. PWM for Buzzer Control**
- **File**: `pwm.c`, `pwm.h`
- **Description**:
  - Configures and controls PWM for the buzzer.
  - Generates an alert tone when suspicious movement is detected.
- **Key Functions**:
  - `PWM_Init`: Initializes the PWM peripheral.
  - `PWM_SetTone`: Toggles the buzzer state.
  - `PlayAlertTone`: Plays an alert tone for a fixed duration.

### **5. System Configuration**
- **File**: `system_config.c`, `system_config.h`
- **Description**:
  - Configures the system clock and GPIO peripherals.
  - Initializes external interrupt for the reset button.
- **Key Functions**:
  - `SystemClock_Config`: Sets up the clock at 48 MHz.
  - `GPIO_Init`: Configures GPIO pins for peripherals.
  - `EXTI_Init`: Sets up the external interrupt for the reset button.

---

## **Command List**
The UART interface allows users to interact with the system using the following commands:

| **Command** | **Functionality**                                               |
|-------------|-----------------------------------------------------------------|
| `1`         | Displays real-time accelerometer values for X, Y, and Z axes.  |
| `2`         | Initiates recalibration of the sensor.                         |
| `3`         | Displays project details and usage instructions.               |
| `4`         | Stops continuous monitoring and exits to the main menu.        |

---

## **Testing**
### **Manual Testing**:
1. **Motion Detection**:
   - Simulated unauthorized movement to trigger an alert.
   - Verified buzzer activation and UART alert messages.
2. **Calibration**:
   - Conducted sensor recalibration with varying initial positions.
   - Verified updated threshold ranges via UART output.
3. **UART Interaction**:
   - Tested all command inputs and ensured proper responses.

### **Automated Testing**:
1. **Circular Buffer**:
   - Verified enqueue and dequeue operations.
   - Tested edge cases like buffer overflow and underflow.
2. **Command Processor**:
   - Simulated UART inputs to test all commands and invalid cases.

---

## **How to Build and Run**
1. **Setup**:
   - Clone the repository and open the project in STM32CubeIDE.
   - Connect the Nucleo-64 board to your computer via USB.
   - Do the connections according to the layout.
2. **Build**:
   - Compile the project and run in any mode.
3. **Run**:
   - Flash the firmware onto the Nucleo-64 board using the ST-Link debugger.
   - Open a serial terminal (e.g., PuTTY, Tera Term) with the following settings:
     - **Baud Rate**: 19200(changeable through define in uart.h)
     - **Data Bits**: 8
     - **Parity**: None
     - **Stop Bits**: 1
       
4. **Interaction**:
   - Use the terminal to issue commands and monitor the system's behavior.

---

## **Block Diagram**
![image](https://github.com/user-attachments/assets/4601f1d7-9f60-458b-8737-c18cdb34a96c)


---

## **Future Improvements**
1. **Enhance Sensitivity**:
   - Implement dynamic sensitivity adjustments based on user input.
2. **Additional Alerts**:
   - Add support for SMS or email alerts using Wi-Fi modules.
3. **Battery Operation**:
   - Optimize for low-power operation in battery-powered environments.
   - Can use BMA400 for accelerometer purpose
   - can expand based on gyroscope

---

Let me know if you want any section elaborated or if specific code references need to be included for clarity.







