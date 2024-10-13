#ifndef GPS_H
#define GPS_H

#include <microhttpd.h>

// Dichiarazione della funzione per ottenere la posizione GPS
int get_gps_position(char *buffer, size_t buffer_size);

// Dichiarazione della funzione per servire la posizione GPS
enum MHD_Result serve_gps(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls);

#endif // GPS_H
