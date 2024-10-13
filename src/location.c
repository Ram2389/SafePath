#include "location.h"
#include "gps.h"  // Usa get_gps_position da gps.c

// Funzione per ottenere la posizione corrente
void get_current_location(char *buffer, size_t buffer_size) {
    get_gps_position(buffer, buffer_size);  // Richiama get_gps_position per ottenere i dati GPS
}
