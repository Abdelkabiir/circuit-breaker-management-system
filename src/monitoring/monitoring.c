#include "monitoring.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static MonitoringStats stats = {
    .min_current = 999999.0,
    .max_current = 0.0,
    .min_voltage = 999999.0,
    .max_voltage = 0.0,
    .min_temp = 999999.0,
    .max_temp = 0.0
};

void monitor_init(void) {
    printf("Monitoring system initialized\n");
}

void monitor_update(BreakerStatus* status) {
    if (status->current_phase_a < stats.min_current)
        stats.min_current = status->current_phase_a;
    if (status->current_phase_a > stats.max_current)
        stats.max_current = status->current_phase_a;

    if (status->voltage_phase_a < stats.min_voltage)
        stats.min_voltage = status->voltage_phase_a;
    if (status->voltage_phase_a > stats.max_voltage)
        stats.max_voltage = status->voltage_phase_a;

    if (status->temperature < stats.min_temp)
        stats.min_temp = status->temperature;
    if (status->temperature > stats.max_temp)
        stats.max_temp = status->temperature;
}

MonitoringStats monitor_get_stats(void) {
    return stats;
}

void monitor_print_status(BreakerStatus* status) {
    printf("\n=== Current Status ===\n");
    printf("Phase A: %.2fA @ %.2fV\n",
           status->current_phase_a,
           status->voltage_phase_a);
    printf("Phase B: %.2fA @ %.2fV\n",
           status->current_phase_b,
           status->voltage_phase_b);
    printf("Phase C: %.2fA @ %.2fV\n",
           status->current_phase_c,
           status->voltage_phase_c);
    printf("Temperature: %.2f°C\n", status->temperature);
    printf("Breaker State: %s\n",
           status->breaker_state ? "CLOSED" : "OPEN");
    printf("Trip State: %s\n",
           status->trip_state ? "TRIPPED" : "NORMAL");
}

void monitor_log_event(const char* event_type, BreakerStatus* status) {
    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    printf("[%s] %s: I=%.2fA, V=%.2fV, T=%.2f°C\n",
           date, event_type,
           status->current_phase_a,
           status->voltage_phase_a,
           status->temperature);
}
