#ifndef GPS_H
#define GPS_H

#include <microhttpd.h>

// Funzione per servire i dati GPS
enum MHD_Result serve_gps(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls);

// Funzione per ottenere la posizione GPS
int get_gps_position(char *buffer, size_t buffer_size);

#endif // GPS_H
