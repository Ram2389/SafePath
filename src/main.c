#include <stdio.h>
#include "gps.h"
#include "map.h"
#include "reporting.h"
#include "navigation.h"
#include "notifications.h"

void init_app() {
    printf("Initializing SafePath application...\n");
    // Initialization code for GPS, maps, notifications, etc.
    gps_init();
    map_load();
}

int main() {
    init_app();
    
    int choice;
    while (1) {
        printf("\n-- SafePath Menu --\n");
        printf("1. Report Hazard\n");
        printf("2. View Map\n");
        printf("3. Calculate Safe Route\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                report_hazard();
                break;
            case 2:
                map_show();
                break;
            case 3:
                calculate_safe_route();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
