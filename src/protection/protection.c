#include "protection.h"
#include <stdio.h>

static ProtectionState current_state = PROTECTION_NORMAL;
static int trip_counter = 0;

void protection_init(void) {
    current_state = PROTECTION_NORMAL;
    trip_counter = 0;
    printf("Protection system initialized\n");
}

ProtectionState protection_check(BreakerStatus* status) {
    ProtectionState previous_state = current_state;

    if (status->current_phase_a > 100.0 ||
        status->current_phase_b > 100.0 ||
        status->current_phase_c > 100.0) {
        current_state = PROTECTION_OVERCURRENT;
        trip_counter++;
        return current_state;
    }

    if (status->voltage_phase_a < 190.0 ||
        status->voltage_phase_b < 190.0 ||
        status->voltage_phase_c < 190.0) {
        current_state = PROTECTION_UNDERVOLTAGE;
        trip_counter++;
        return current_state;
    }

    if (status->voltage_phase_a > 250.0 ||
        status->voltage_phase_b > 250.0 ||
        status->voltage_phase_c > 250.0) {
        current_state = PROTECTION_OVERVOLTAGE;
        trip_counter++;
        return current_state;
    }

    if (status->temperature > 75.0) {
        current_state = PROTECTION_OVERTEMP;
        trip_counter++;
        return current_state;
    }

    current_state = PROTECTION_NORMAL;

    if (previous_state != current_state) {
        printf("Protection state changed: %s -> %s\n",
               protection_state_to_string(previous_state),
               protection_state_to_string(current_state));
    }

    return current_state;
}

bool protection_reset(void) {
    if (current_state == PROTECTION_NORMAL) {
        trip_counter = 0;
        return true;
    }
    return false;
}

int protection_get_trip_count(void) {
    return trip_counter;
}

const char* protection_state_to_string(ProtectionState state) {
    switch(state) {
        case PROTECTION_NORMAL: return "NORMAL";
        case PROTECTION_OVERCURRENT: return "OVERCURRENT";
        case PROTECTION_UNDERVOLTAGE: return "UNDERVOLTAGE";
        case PROTECTION_OVERVOLTAGE: return "OVERVOLTAGE";
        case PROTECTION_OVERTEMP: return "OVERTEMPERATURE";
        default: return "UNKNOWN";
    }
}