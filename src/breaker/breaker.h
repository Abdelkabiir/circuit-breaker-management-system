#ifndef BREAKER_H
#define BREAKER_H

#include <stdbool.h>

typedef struct {
    double current_phase_a;
    double current_phase_b;
    double current_phase_c;
    double voltage_phase_a;
    double voltage_phase_b;
    double voltage_phase_c;
    bool breaker_state;     // true = closed, false = open
    bool trip_state;        // true = tripped
    double temperature;
} BreakerStatus;

typedef struct {
    double overcurrent_threshold;
    double undervoltage_threshold;
    double overvoltage_threshold;
    double max_temperature;
    int trip_delay_ms;
} BreakerConfig;

void breaker_init(BreakerConfig* config);
void breaker_update_measurements(BreakerStatus* status);
bool breaker_check_protection(BreakerStatus* status);
void breaker_open(void);
void breaker_close(void);
BreakerStatus breaker_get_status(void);

#endif // BREAKER_H