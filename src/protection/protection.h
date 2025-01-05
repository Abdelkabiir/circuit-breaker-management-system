#ifndef PROTECTION_H
#define PROTECTION_H

#include "../breaker/breaker.h"
#include <stdbool.h>

typedef enum {
    PROTECTION_NORMAL,
    PROTECTION_OVERCURRENT,
    PROTECTION_UNDERVOLTAGE,
    PROTECTION_OVERVOLTAGE,
    PROTECTION_OVERTEMP
} ProtectionState;

void protection_init(void);
ProtectionState protection_check(BreakerStatus* status);
bool protection_reset(void);
int protection_get_trip_count(void);
const char* protection_state_to_string(ProtectionState state);

#endif // PROTECTION_H