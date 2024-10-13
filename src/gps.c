#include "gps.h"
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Funzione per ottenere la posizione GPS (devi implementarla in base alle tue esigenze)
int get_gps_position(char *buffer, size_t buffer_size) {
    const char *lat = "40.7128"; // Latitudine
    const char *lon = "-74.0060"; // Longitudine

    // Formatta i dati in JSON
    int written = snprintf(buffer, buffer_size, "{\"latitude\": %s, \"longitude\": %s}", lat, lon);
    
    return (written > 0 && written < buffer_size) ? 0 : -1; // Restituisce 0 in caso di successo
}

// Funzione per servire la posizione GPS in formato JSON
enum MHD_Result serve_gps(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    char gps_data[128];

    // Recupera i dati GPS
    if (get_gps_position(gps_data, sizeof(gps_data)) == -1) {
        const char *error_message = "{\"error\": \"Unable to retrieve GPS data\"}";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_message), 
                                                                      (void *) error_message, 
                                                                      MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "application/json");
        int ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Crea la risposta HTTP con i dati GPS
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(gps_data), 
                                                                    (void *) gps_data, 
                                                                    MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", "application/json");
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    
    return ret;
}
