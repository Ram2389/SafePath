// reporting.c
#include <stdio.h>
#include "reporting.h"
#include "gps.h"

void report_hazard() {
    float lat, lon;
    char description[256];
    
    get_current_location(&lat, &lon);
    
    printf("Enter hazard description: ");
    scanf(" %[^\n]", description);  // Read multi-word input
    
    FILE *file = fopen("data/hazards.csv", "a");
    if (file == NULL) {
        printf("Error opening hazards file!\n");
        return;
    }
    
    fprintf(file, "%.4f,%.4f,%s\n", lat, lon, description);
    fclose(file);
    
    printf("Hazard reported at Latitude=%.4f, Longitude=%.4f\n", lat, lon);
}
