#include "breaker.h"
#include <stdio.h>
#include <time.h>

static BreakerConfig current_config;
static BreakerStatus current_status;

void breaker_init(BreakerConfig* config) {
    current_config = *config;
    current_status.breaker_state = true;
    current_status.trip_state = false;

    printf("Circuit Breaker Initialized\n");
    printf("Overcurrent threshold: %.2f A\n", config->overcurrent_threshold);
    printf("Voltage thresholds: %.2f - %.2f V\n",
           config->undervoltage_threshold,
           config->overvoltage_threshold);
}

void breaker_update_measurements(BreakerStatus* status) {
    current_status = *status;
}

bool breaker_check_protection(BreakerStatus* status) {
    if (status->current_phase_a > current_config.overcurrent_threshold ||
        status->current_phase_b > current_config.overcurrent_threshold ||
        status->current_phase_c > current_config.overcurrent_threshold) {
        breaker_open();
        status->trip_state = true;
        return true;
    }

    if (status->voltage_phase_a < current_config.undervoltage_threshold ||
        status->voltage_phase_b < current_config.undervoltage_threshold ||
        status->voltage_phase_c < current_config.undervoltage_threshold ||
        status->voltage_phase_a > current_config.overvoltage_threshold ||
        status->voltage_phase_b > current_config.overvoltage_threshold ||
        status->voltage_phase_c > current_config.overvoltage_threshold) {
        breaker_open();
        status->trip_state = true;
        return true;
    }

    if (status->temperature > current_config.max_temperature) {
        breaker_open();
        status->trip_state = true;
        return true;
    }

    return false;
}

void breaker_open(void) {
    current_status.breaker_state = false;
    printf("Breaker OPENED\n");
}

void breaker_close(void) {
    if (!current_status.trip_state) {
        current_status.breaker_state = true;
        printf("Breaker CLOSED\n");
    } else {
        printf("Cannot close: Breaker is tripped\n");
    }
}

BreakerStatus breaker_get_status(void) {
    return current_status;
}
