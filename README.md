# SPI IMU Data Logger

Firmware project for interfacing an **LSM6DSL inertial measurement unit (IMU)** with an **ATxmega128A1U** microcontroller using the SPI protocol.  
The system acquires accelerometer and gyroscope data, triggered by interrupts, and transmits results via UART for real-time visualization (e.g., SerialPlot).

---

## Features
- **SPI driver** for ATxmega128A1U configured as master, supporting register-level access to the LSM6DSL IMU.
- **IMU initialization** routines:
  - Software reset and configuration of accelerometer and gyroscope registers.
  - Setup of output data rate, full-scale range, and interrupt triggers.
- **Interrupt-driven data acquisition**:
  - External interrupt line signals when new accelerometer or gyroscope data is ready.
  - Global flag variables set inside ISRs for efficient and low-latency data handling.
- **UART transmission** of XYZ accelerometer and gyroscope values for real-time data logging and visualization.
- Modularized code structure:
  - `spi.[ch]` – SPI driver implementation
  - `lsm6dsl.[ch]` – Sensor read/write functions and register definitions
  - `usart.[ch]` – UART communication routines
  - Application files (`lab6_*.c`) demonstrating incremental tasks (basic SPI test, WHO_AM_I check, accelerometer logging, gyroscope logging)

---

## Repository Structure
```text
repo-root/
- spi.c / spi.h          # SPI driver (init, read, write)
- lsm6dsl.c / lsm6dsl.h  # IMU driver (register access, init)
- lsm6dsl_registers.h    # Register map for LSM6DSL
- usart.c / usart.h      # UART driver for serial output
- lab6_2.c               # Basic SPI transmit/receive test
- lab6_3.c               # WHO_AM_I register read
- lab6_extra.c           # Accelerometer logging (interrupt-driven)
- lab6_extra_credit.c    # Gyroscope logging (interrupt-driven)
- README.md
