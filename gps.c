#include "gps.h"
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_gps_position(char *buffer, size_t buffer_size) {
    const char *lat = "51.5074";  // Latitudine di Londra
    const char *lon = "-0.1278";   // Longitudine di Londra

    // Assicurati che i dati siano formattati correttamente come stringa JSON
    int written = snprintf(buffer, buffer_size, "{\"latitude\": \"%s\", \"longitude\": \"%s\"}", lat, lon);

    // Debug per vedere cosa viene scritto nel buffer
    printf("Written: %d, Buffer: %s\n", written, buffer);

    if (written < 0 || written >= buffer_size) {
        fprintf(stderr, "Errore nella scrittura del buffer o buffer troppo piccolo\n");
        return -1; // Restituisce -1 in caso di errore
    }

    return 0; // Restituisce 0 in caso di successo
}

// Funzione per servire i dati GPS
enum MHD_Result serve_gps(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    char gps_data[256];

    // Recupera i dati GPS
    int ret = get_gps_position(gps_data, sizeof(gps_data));
    if (ret == -1) {
        const char *error_message = "{\"error\": \"Unable to retrieve GPS data\"}";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_message),
                                                                        (void *)error_message,
                                                                        MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "application/json; charset=utf-8");
        ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Crea la risposta HTTP con i dati GPS
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(gps_data),
                                                                    (void *)gps_data,
                                                                    MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", "application/json");
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}
