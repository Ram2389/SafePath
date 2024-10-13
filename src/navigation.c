// navigation.c
#include <stdio.h>
#include "navigation.h"
#include "gps.h"

void calculate_safe_route() {
    float lat, lon;
    get_current_location(&lat, &lon);
    
    printf("Calculating safe route from current location: Latitude=%.4f, Longitude=%.4f\n", lat, lon);
    // Dummy calculation
    printf("Safe route: Start -> Safe Point -> Destination (stub)\n");
}
