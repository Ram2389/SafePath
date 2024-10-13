// gps.c
#include <stdio.h>
#include "gps.h"

void gps_init() {
    printf("GPS initialized (stub)\n");
}

void get_current_location(float *lat, float *lon) {
    *lat = 34.0522;  // Simulate coordinates (Los Angeles)
    *lon = -118.2437;
    printf("Simulated GPS location: Latitude=%.4f, Longitude=%.4f\n", *lat, *lon);
}
