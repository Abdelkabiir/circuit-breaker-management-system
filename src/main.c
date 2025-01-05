#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "breaker/breaker.h"
#include "protection/protection.h"
#include "monitoring/monitoring.h"

void print_usage(void) {
    printf("Usage: ./circuit_breaker [OPTIONS]\n");
    printf("Options:\n");
    printf("  -ia CURRENT   Phase A current (Amperes)\n");
    printf("  -ib CURRENT   Phase B current (Amperes)\n");
    printf("  -ic CURRENT   Phase C current (Amperes)\n");
    printf("  -va VOLTAGE   Phase A voltage (Volts)\n");
    printf("  -vb VOLTAGE   Phase B voltage (Volts)\n");
    printf("  -vc VOLTAGE   Phase C voltage (Volts)\n");
    printf("  -t TEMP       Temperature (Celsius)\n");
    printf("\nExample: ./circuit_breaker -ia 95.0 -va 220.0 -t 65.0\n");
}

int main(int argc, char *argv[]) {
    BreakerConfig config = {
        .overcurrent_threshold = 100.0,
        .undervoltage_threshold = 190.0,
        .overvoltage_threshold = 250.0,
        .max_temperature = 75.0,
        .trip_delay_ms = 50
    };

    const double DEFAULT_CURRENT_A = 95.0;
    const double DEFAULT_CURRENT_B = 94.0;
    const double DEFAULT_CURRENT_C = 96.0;
    const double DEFAULT_VOLTAGE_A = 220.0;
    const double DEFAULT_VOLTAGE_B = 221.0;
    const double DEFAULT_VOLTAGE_C = 219.0;
    const double DEFAULT_TEMP = 65.0;

    BreakerStatus status = {
        .current_phase_a = DEFAULT_CURRENT_A,
        .current_phase_b = DEFAULT_CURRENT_B,
        .current_phase_c = DEFAULT_CURRENT_C,
        .voltage_phase_a = DEFAULT_VOLTAGE_A,
        .voltage_phase_b = DEFAULT_VOLTAGE_B,
        .voltage_phase_c = DEFAULT_VOLTAGE_C,
        .temperature = DEFAULT_TEMP
    };

    breaker_init(&config);
    protection_init();
    monitor_init();

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) {
            printf("Error: Missing value for %s\n", argv[i]);
            print_usage();
            return 1;
        }

        double value = atof(argv[i + 1]);

        if (strcmp(argv[i], "-ia") == 0) status.current_phase_a = value != 0 ? value : DEFAULT_CURRENT_A;
        else if (strcmp(argv[i], "-ib") == 0) status.current_phase_b = value != 0 ? value : DEFAULT_CURRENT_B;
        else if (strcmp(argv[i], "-ic") == 0) status.current_phase_c = value != 0 ? value : DEFAULT_CURRENT_C;
        else if (strcmp(argv[i], "-va") == 0) status.voltage_phase_a = value != 0 ? value : DEFAULT_VOLTAGE_A;
        else if (strcmp(argv[i], "-vb") == 0) status.voltage_phase_b = value != 0 ? value : DEFAULT_VOLTAGE_B;
        else if (strcmp(argv[i], "-vc") == 0) status.voltage_phase_c = value != 0 ? value : DEFAULT_VOLTAGE_C;
        else if (strcmp(argv[i], "-t") == 0) status.temperature = value != 0 ? value : DEFAULT_TEMP;
        else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage();
            return 1;
        }
    }

    breaker_update_measurements(&status);
    monitor_update(&status);

    ProtectionState prot_state = protection_check(&status);
    if (prot_state != PROTECTION_NORMAL) {
        monitor_log_event("PROTECTION_TRIGGERED", &status);
        printf("Protection triggered: %s\n", protection_state_to_string(prot_state));
        printf("Total trips: %d\n", protection_get_trip_count());
    }

    monitor_print_status(&status);
    
    return 0;
}