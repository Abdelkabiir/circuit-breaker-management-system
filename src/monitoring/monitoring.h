#ifndef MONITOR_H
#define MONITOR_H

#include "../breaker/breaker.h"

typedef struct {
    double min_current;
    double max_current;
    double min_voltage;
    double max_voltage;
    double min_temp;
    double max_temp;
} MonitoringStats;

void monitor_init(void);
void monitor_update(BreakerStatus* status);
MonitoringStats monitor_get_stats(void);
void monitor_print_status(BreakerStatus* status);
void monitor_log_event(const char* event_type, BreakerStatus* status);

#endif // MONITOR_H