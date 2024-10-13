#include "gps.h"
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8888

// Funzione per gestire tutte le richieste HTTP
static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    if (strcmp(url, "/gps") == 0) {
        return serve_gps(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
    }

    // Se non è richiesta la GPS, restituisci la pagina principale
    const char *page = "<html><body><h1>La tua mappa</h1>"
                       "<div id='map' style='height: 100vh;'></div>"
                       "<script src='/app.js'></script></body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page), 
                                                                    (void *) page, 
                                                                    MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main()
{
    struct MHD_Daemon *daemon;

    // Avvia il server sulla porta 8888
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, 
                              &handle_request, NULL, MHD_OPTION_END);

    if (NULL == daemon)
        return 1;

    printf("Server in esecuzione su http://localhost:%d\n", PORT);

    // Mantieni il server in esecuzione
    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}
