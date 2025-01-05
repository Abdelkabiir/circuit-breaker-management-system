# Circuit Breaker Management System

A digital device for the protection, control, and management of a three-phase circuit breaker implemented in C.

## Features
- Real-time monitoring of three-phase currents and voltages
- Temperature monitoring
- Protection system implementation
- Auto-trip functionality
- Status logging and reporting

## Prerequisites
- GCC compiler
- Make build system

## Building the Project
```bash
make
```

## Usage

Run with default values:
```bash
./circuit_breaker
```

Specify measurements:
```bash
./circuit_breaker -ia 95.0 -va 220.0 -t 65.0
```

### Command Line Options
- `-ia` Phase A current (Amperes)
- `-ib` Phase B current (Amperes)
- `-ic` Phase C current (Amperes)
- `-va` Phase A voltage (Volts)
- `-vb` Phase B voltage (Volts)
- `-vc` Phase C voltage (Volts)
- `-t`  Temperature (Celsius)

### Default Values
- Current: 95.0A, 94.0A, 96.0A
- Voltage: 220.0V, 221.0V, 219.0V
- Temperature: 65.0°C

### Protection Thresholds
- Overcurrent: 100.0A
- Undervoltage: 190.0V
- Overvoltage: 250.0V
- Maximum Temperature: 75.0°C

## Project Structure
```
circuit-breaker-system/
├── src/
│   ├── main.c
│   ├── breaker/
│   │   ├── breaker.c
│   │   └── breaker.h
│   ├── monitoring/
│   │   ├── monitoring.c
│   │   └── monitoring.h
│   ├── protection/
│   │   ├── protection.c
│   │   └── protection.h
├── Makefile
└── README.md
```